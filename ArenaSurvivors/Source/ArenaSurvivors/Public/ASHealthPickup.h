#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASHealthPickup.generated.h"

UCLASS()
class ARENASURVIVORS_API AASHealthPickup : public AActor
{
    GENERATED_BODY()

public:
    AASHealthPickup();

    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class USphereComponent* SphereComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UStaticMeshComponent* MeshComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Pickup")
    float HealAmount;

    UPROPERTY(EditDefaultsOnly, Category = "Pickup")
    float RespawnTime;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

private:
    bool bIsActive;

    void Respawn();

    FTimerHandle RespawnTimer;
};