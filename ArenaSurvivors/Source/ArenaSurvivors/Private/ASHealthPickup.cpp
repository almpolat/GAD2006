#include "ASHealthPickup.h"
#include "ASBaseCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

AASHealthPickup::AASHealthPickup()
{
    PrimaryActorTick.bCanEverTick = false;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    SphereComponent->SetSphereRadius(60.f);
    SphereComponent->SetCollisionProfileName(TEXT("Trigger"));
    RootComponent = SphereComponent;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    HealAmount = 30.f;
    RespawnTime = 10.f;
    bIsActive = true;

    bReplicates = true;
}

void AASHealthPickup::BeginPlay()
{
    Super::BeginPlay();

    SphereComponent->OnComponentBeginOverlap.AddDynamic(
        this, &AASHealthPickup::OnOverlapBegin);
}

void AASHealthPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!HasAuthority()) return;
    if (!bIsActive) return;

    AASBaseCharacter* Character = Cast<AASBaseCharacter>(OtherActor);
    if (!Character) return;

    Character->Health = FMath::Clamp(
        Character->Health + HealAmount,
        0.f,
        Character->MaxHealth
    );

    bIsActive = false;
    SetActorHiddenInGame(true);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green,
            FString::Printf(TEXT("Healed %.0f HP!"), HealAmount));
    }

    GetWorldTimerManager().SetTimer(
        RespawnTimer,
        this,
        &AASHealthPickup::Respawn,
        RespawnTime,
        false
    );
}

void AASHealthPickup::Respawn()
{
    bIsActive = true;
    SetActorHiddenInGame(false);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}