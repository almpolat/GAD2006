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

    // Kamera
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class UCameraComponent* Camera;

    // Dash
    UPROPERTY(EditDefaultsOnly, Category = "Dash")
    float DashDistance;

    UPROPERTY(EditDefaultsOnly, Category = "Dash")
    float DashCooldown;

    UPROPERTY(BlueprintReadOnly, Category = "Dash")
    bool bCanDash;

    // Melee saldýrý
    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    float MeleeDamage;

    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    float MeleeRange;

    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    bool bIsAttacking;

    // Input fonksiyonlarý
    void MoveForward(float Value);

    UFUNCTION(BlueprintCallable, Category = "Dash")
    void Dash();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void MeleeAttack();

    // Server RPC'ler
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_Dash(FVector Direction);

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_MeleeAttack();

protected:
    FTimerHandle DashCooldownTimer;

    void ResetDash();
    void PerformMeleeTrace();
};