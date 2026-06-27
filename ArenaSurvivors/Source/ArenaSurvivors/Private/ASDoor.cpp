#include "ASDoor.h"
#include "ASGameMode.h"
#include "ASBaseCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

AASDoor::AASDoor()
{
    PrimaryActorTick.bCanEverTick = true;

    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    Wall1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall1"));
    Wall1->SetupAttachment(RootComponent);
    Wall1->SetCollisionProfileName(TEXT("BlockAllDynamic"));

    Wall2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall2"));
    Wall2->SetupAttachment(RootComponent);
    Wall2->SetCollisionProfileName(TEXT("BlockAllDynamic"));

    Wall3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall3"));
    Wall3->SetupAttachment(RootComponent);
    Wall3->SetCollisionProfileName(TEXT("BlockAllDynamic"));

    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorMesh->SetupAttachment(RootComponent);
    DoorMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));

    DoorTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorTrigger"));
    DoorTrigger->SetupAttachment(RootComponent);
    DoorTrigger->SetBoxExtent(FVector(100.f, 100.f, 100.f));
    DoorTrigger->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
    DoorTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    ExitTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ExitTrigger"));
    ExitTrigger->SetupAttachment(RootComponent);
    ExitTrigger->SetBoxExtent(FVector(100.f, 100.f, 100.f));
    ExitTrigger->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
    ExitTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    OpenSpeed = 150.f;
    OpenDistance = 400.f;
    bIsOpen = false;
    bGameStarted = false;
    bTriggersEnabled = false;

    bReplicates = true;
}

void AASDoor::BeginPlay()
{
    Super::BeginPlay();

    DoorClosedLocation = DoorMesh->GetComponentLocation();
    DoorTargetLocation = DoorClosedLocation;

    DoorTrigger->OnComponentBeginOverlap.AddDynamic(
        this, &AASDoor::OnDoorTriggerOverlap);

    ExitTrigger->OnComponentBeginOverlap.AddDynamic(
        this, &AASDoor::OnExitTriggerOverlap);

    if (HasAuthority())
    {
        GetWorldTimerManager().SetTimer(
            PlayerCheckTimer,
            this,
            &AASDoor::CheckPlayerCount,
            0.5f,
            true
        );
    }
}

void AASDoor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsOpen)
    {
        FVector CurrentLocation = DoorMesh->GetComponentLocation();
        FVector NewLocation = FMath::VInterpTo(
            CurrentLocation, DoorTargetLocation, DeltaTime, OpenSpeed);
        DoorMesh->SetWorldLocation(NewLocation);
    }
}

void AASDoor::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AASDoor, bIsOpen);
}

void AASDoor::CheckPlayerCount()
{
    if (bTriggersEnabled) return;

    int32 PlayerCount = UGameplayStatics::GetNumPlayerControllers(GetWorld());

    if (PlayerCount >= 2)
    {
        EnableTriggers();
        GetWorldTimerManager().ClearTimer(PlayerCheckTimer);
    }
}

void AASDoor::EnableTriggers()
{
    bTriggersEnabled = true;

    DoorTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ExitTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
            TEXT("Both players joined! Door is now active."));
    }
}

void AASDoor::OnDoorTriggerOverlap(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!HasAuthority()) return;
    if (!bTriggersEnabled) return;

    AASBaseCharacter* Player = Cast<AASBaseCharacter>(OtherActor);
    if (!Player) return;

    if (!bIsOpen)
    {
        bIsOpen = true;
        OnRep_IsOpen();
    }
}

void AASDoor::OnExitTriggerOverlap(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!HasAuthority()) return;
    if (bGameStarted) return;
    if (!bTriggersEnabled) return;

    AASBaseCharacter* Player = Cast<AASBaseCharacter>(OtherActor);
    if (!Player) return;

    PlayersExited.Add(OtherActor);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow,
            FString::Printf(TEXT("Players exited: %d / 2"),
                PlayersExited.Num()));
    }

    if (PlayersExited.Num() >= 2)
    {
        bGameStarted = true;

        AASGameMode* GM = Cast<AASGameMode>(
            UGameplayStatics::GetGameMode(GetWorld()));
        if (GM)
        {
            GM->StartNextWave();
        }

        Destroy();
    }
}

void AASDoor::OnRep_IsOpen()
{
    if (bIsOpen)
    {
        // Kapı sola kayar
        FVector LeftDirection = -DoorMesh->GetRightVector();
        DoorTargetLocation = DoorClosedLocation + (LeftDirection * OpenDistance);
        DoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}