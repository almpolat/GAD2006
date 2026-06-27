#include "ASTrap.h"
#include "ASBaseCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"

AASTrap::AASTrap()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
    MeshComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    MeshComponent->SetGenerateOverlapEvents(false);

    DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageBox"));
    DamageBox->SetupAttachment(RootComponent);
    DamageBox->SetBoxExtent(FVector(100.f, 100.f, 50.f));
    DamageBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
    DamageBox->SetGenerateOverlapEvents(true);

    DamagePerSecond = 10.f;
    DamageInterval = 0.5f;

    bReplicates = true;
}

void AASTrap::BeginPlay()
{
    Super::BeginPlay();

    DamageBox->OnComponentBeginOverlap.AddDynamic(
        this, &AASTrap::OnOverlapBegin);
    DamageBox->OnComponentEndOverlap.AddDynamic(
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