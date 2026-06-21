#include "ASEnemyBase.h"
#include "ASAIController.h"
#include "ASBaseCharacter.h"
#include "ASPlayerState.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

AASEnemyBase::AASEnemyBase()
{
    AttackDamage = 10.f;
    AttackCooldown = 1.5f;
    ScoreValue = 100;
    bCanAttack = true;

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = AASAIController::StaticClass();

    // Düþman hýzýný düþür
    GetCharacterMovement()->MaxWalkSpeed = 200.f;
}

void AASEnemyBase::BeginPlay()
{
    Super::BeginPlay();
}

void AASEnemyBase::PerformAttack()
{
    if (!bCanAttack || bIsDead) return;

    bCanAttack = false;

    AASAIController* AIC = Cast<AASAIController>(GetController());
    if (AIC && AIC->CurrentTarget)
    {
        AASBaseCharacter* Target = Cast<AASBaseCharacter>(AIC->CurrentTarget);
        if (Target)
        {
            Target->TakeDamage_AS(AttackDamage);
        }
    }

    GetWorldTimerManager().SetTimer(
        AttackCooldownTimer,
        this,
        &AASEnemyBase::ResetAttack,
        AttackCooldown,
        false
    );
}

void AASEnemyBase::ResetAttack()
{
    bCanAttack = true;
}

void AASEnemyBase::Die()
{
    Super::Die();

    TArray<AActor*> Controllers;
    UGameplayStatics::GetAllActorsOfClass(
        GetWorld(),
        APlayerController::StaticClass(),
        Controllers
    );

    for (AActor* ControllerActor : Controllers)
    {
        APlayerController* PC = Cast<APlayerController>(ControllerActor);
        if (PC)
        {
            AASPlayerState* PS = PC->GetPlayerState<AASPlayerState>();
            if (PS)
            {
                PS->AddKill();
            }
        }
    }

    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);

    FTimerHandle DestroyTimer;
    GetWorldTimerManager().SetTimer(DestroyTimer, [this]()
        {
            Destroy();
        }, 2.f, false);
}