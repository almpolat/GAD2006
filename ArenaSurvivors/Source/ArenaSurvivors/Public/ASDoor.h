#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
    class UStaticMeshComponent* DoorMesh;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UBoxComponent* InteractionBox;

    UPROPERTY(EditDefaultsOnly, Category = "Door")
    float OpenAngle;

    UPROPERTY(EditDefaultsOnly, Category = "Door")
    float OpenSpeed;

    UPROPERTY(ReplicatedUsing = OnRep_IsOpen, BlueprintReadOnly, Category = "Door")
    bool bIsOpen;

    UFUNCTION(BlueprintCallable, Category = "Door")
    void Interact();

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    virtual void GetLifetimeReplicatedProps(
        TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    UFUNCTION()
    void OnRep_IsOpen();

    FRotator ClosedRotation;
    FRotator TargetRotation;
};