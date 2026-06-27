#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "ASDoor.generated.h"

UCLASS()
class ARENASURVIVORS_API AASDoor : public AActor
{
    GENERATED_BODY()

public:
    AASDoor();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UStaticMeshComponent* Wall1;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UStaticMeshComponent* Wall2;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UStaticMeshComponent* Wall3;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UStaticMeshComponent* DoorMesh;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UBoxComponent* DoorTrigger;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UBoxComponent* ExitTrigger;

    UPROPERTY(EditDefaultsOnly, Category = "Door")
    float OpenSpeed;

    UPROPERTY(EditDefaultsOnly, Category = "Door")
    float OpenDistance;

    UPROPERTY(ReplicatedUsing = OnRep_IsOpen, BlueprintReadOnly, Category = "Door")
    bool bIsOpen;

    virtual void GetLifetimeReplicatedProps(
        TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION()
    void OnDoorTriggerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnExitTriggerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

private:
    UFUNCTION()
    void OnRep_IsOpen();

    void CheckPlayerCount();
    void EnableTriggers();

    FVector DoorClosedLocation;
    FVector DoorTargetLocation;

    TSet<AActor*> PlayersExited;
    bool bGameStarted;
    bool bTriggersEnabled;

    FTimerHandle PlayerCheckTimer;
};