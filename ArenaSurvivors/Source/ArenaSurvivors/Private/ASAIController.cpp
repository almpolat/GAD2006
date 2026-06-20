#include "ASAIController.h"
#include "ASEnemyBase.h"
#include "ASBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

AASAIController::AASAIController()
{
    PrimaryActorTick.bCanEverTick = true;
    AttackRange = 150.f;
    ChaseRange = 1500.f;
    CurrentTarget = nullptr;
}

void AASAIController::BeginPlay()
{
    Super::BeginPlay();
}

void AASAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    GetWorldTimerManager().SetTimer(
        AITickTimer,
        this,
        &AASAIController::UpdateAI,
        0.2f,
        true
    );
}

void AASAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

AActor* AASAIController::FindNearestPlayer()
{
    TArray<AActor*> Players;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), Players);

    AActor* Nearest = nullptr;
    float MinDist = FLT_MAX;

    APawn* MyPawn = GetPawn();
    if (!MyPawn) return nullptr;

    for (AActor* Actor : Players)
    {
        AASEnemyBase* AsEnemy = Cast<AASEnemyBase>(Actor);
        if (AsEnemy) continue;

        float Dist = FVector::Dist(MyPawn->GetActorLocation(), Actor->GetActorLocation());
        if (Dist < MinDist)
        {
            MinDist = Dist;
            Nearest = Actor;
        }
    }

    return Nearest;
}

void AASAIController::MoveToTarget(AActor* Target)
{
    if (!Target) return;
    MoveToActor(Target, AttackRange * 0.8f);
}

void AASAIController::UpdateAI()
{
    CurrentTarget = FindNearestPlayer();

    if (!CurrentTarget) return;

    APawn* MyPawn = GetPawn();
    if (!MyPawn) return;

    float DistToTarget = FVector::Dist(
        MyPawn->GetActorLocation(),
        CurrentTarget->GetActorLocation()
    );

    if (DistToTarget <= ChaseRange)
    {
        MoveToTarget(CurrentTarget);
    }
    else
    {
        StopMovement();
    }
}