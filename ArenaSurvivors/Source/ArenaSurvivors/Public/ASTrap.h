#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASBaseCharacter.h"
#include "ASTrap.generated.h"

UCLASS()
class ARENASURVIVORS_API AASTrap : public AActor
{
    GENERATED_BODY()

public:
    AASTrap();

    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UBoxComponent* BoxComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UStaticMeshComponent* MeshComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Trap")
    float DamagePerSecond;

    UPROPERTY(EditDefaultsOnly, Category = "Trap")
    float DamageInterval;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
    TArray<AASBaseCharacter*> OverlappingCharacters;

    FTimerHandle DamageTimer;

    void ApplyTrapDamage();
};