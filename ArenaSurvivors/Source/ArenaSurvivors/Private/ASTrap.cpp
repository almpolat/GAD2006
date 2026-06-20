#include "ASTrap.h"
#include "ASBaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

AASTrap::AASTrap()
{
    PrimaryActorTick.bCanEverTick = false;

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    BoxComponent->SetBoxExtent(FVector(100.f, 100.f, 20.f));
    BoxComponent->SetCollisionProfileName(TEXT("Trigger"));
    RootComponent = BoxComponent;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    DamagePerSecond = 10.f;
    DamageInterval = 0.5f;

    bReplicates = true;
}

void AASTrap::BeginPlay()
{
    Super::BeginPlay();

    BoxComponent->OnComponentBeginOverlap.AddDynamic(
        this, &AASTrap::OnOverlapBegin);
    BoxComponent->OnComponentEndOverlap.AddDynamic(
        this, &AASTrap::OnOverlapEnd);
}

void AASTrap::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!HasAuthority()) return;

    AASBaseCharacter* Character = Cast<AASBaseCharacter>(OtherActor);
    if (!Character) return;

    OverlappingCharacters.AddUnique(Character);

    if (!GetWorldTimerManager().IsTimerActive(DamageTimer))
    {
        GetWorldTimerManager().SetTimer(
            DamageTimer,
            this,
            &AASTrap::ApplyTrapDamage,
            DamageInterval,
            true
        );
    }
}

void AASTrap::OnOverlapEnd(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    AASBaseCharacter* Character = Cast<AASBaseCharacter>(OtherActor);
    if (Character)
    {
        OverlappingCharacters.Remove(Character);
    }

    if (OverlappingCharacters.Num() == 0)
    {
        GetWorldTimerManager().ClearTimer(DamageTimer);
    }
}

void AASTrap::ApplyTrapDamage()
{
    for (AASBaseCharacter* Character : OverlappingCharacters)
    {
        if (Character && !Character->bIsDead)
        {
            Character->TakeDamage_AS(DamagePerSecond * DamageInterval);
        }
    }
}