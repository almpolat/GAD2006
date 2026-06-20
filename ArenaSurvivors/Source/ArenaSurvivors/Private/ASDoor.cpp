#include "ASDoor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

AASDoor::AASDoor()
{
    PrimaryActorTick.bCanEverTick = true;

    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    RootComponent = DoorMesh;

    InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
    InteractionBox->SetupAttachment(RootComponent);
    InteractionBox->SetBoxExtent(FVector(150.f, 150.f, 100.f));
    InteractionBox->SetCollisionProfileName(TEXT("Trigger"));

    OpenAngle = 90.f;
    OpenSpeed = 2.f;
    bIsOpen = false;

    bReplicates = true;
}

void AASDoor::BeginPlay()
{
    Super::BeginPlay();

    ClosedRotation = GetActorRotation();
    TargetRotation = ClosedRotation;

    InteractionBox->OnComponentBeginOverlap.AddDynamic(
        this, &AASDoor::OnOverlapBegin);
}

void AASDoor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FRotator CurrentRotation = GetActorRotation();
    FRotator NewRotation = FMath::RInterpTo(
        CurrentRotation, TargetRotation, DeltaTime, OpenSpeed);
    SetActorRotation(NewRotation);
}

void AASDoor::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AASDoor, bIsOpen);
}

void AASDoor::Interact()
{
    if (!HasAuthority()) return;

    bIsOpen = !bIsOpen;
    OnRep_IsOpen();
}

void AASDoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Interact();
}

void AASDoor::OnRep_IsOpen()
{
    if (bIsOpen)
    {
        TargetRotation = ClosedRotation + FRotator(0.f, OpenAngle, 0.f);
    }
    else
    {
        TargetRotation = ClosedRotation;
    }
}