#pragma once

#include "CoreMinimal.h"
#include "ASBaseCharacter.h"
#include "ASAvatar.generated.h"

UCLASS()
class ARENASURVIVORS_API AASAvatar : public AASBaseCharacter
{
    GENERATED_BODY()

public:
    AASAvatar();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(
        class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class UCameraComponent* Camera;

    UPROPERTY(EditDefaultsOnly, Category = "Dash")
    float DashDistance;

    UPROPERTY(EditDefaultsOnly, Category = "Dash")
    float DashCooldown;

    UPROPERTY(BlueprintReadOnly, Category = "Dash")
    bool bCanDash;

    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    float MeleeDamage;

    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    float MeleeRange;

    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    bool bIsAttacking;

    UPROPERTY(ReplicatedUsing = OnRep_CostumeRowName, BlueprintReadOnly, Category = "Costume")
    FName ReplicatedCostumeRowName;

    UFUNCTION()
    void OnRep_CostumeRowName();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
    class UWidgetComponent* NameTagWidget;

    void MoveForward(float Value);

    UFUNCTION(BlueprintCallable, Category = "Dash")
    void Dash();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void MeleeAttack();

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_Dash(FVector Direction);

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_MeleeAttack();

    UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Networking")
    void Server_SetRotation(FRotator NewRotation);

    UFUNCTION(Server, Reliable)
    void Server_SetPlayerInfo(const FString& Nickname, int32 InCostumeIndex, FName InCostumeRowName);

    UFUNCTION(BlueprintImplementableEvent, Category = "Costume")
    void ApplyCostumeFromPlayerState();

protected:
    FTimerHandle DashCooldownTimer;

    void ResetDash();
    void PerformMeleeTrace();

    virtual void GetLifetimeReplicatedProps(
        TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};