#include "ASBaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

AASBaseCharacter::AASBaseCharacter()
{
    MaxHealth = 100.f;
    Health = MaxHealth;
    bIsDead = false;
}

void AASBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
    Health = MaxHealth;
}

void AASBaseCharacter::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AASBaseCharacter, Health);
    DOREPLIFETIME(AASBaseCharacter, bIsDead);
}

void AASBaseCharacter::TakeDamage_AS(float DamageAmount)
{
    if (!HasAuthority()) return;
    if (bIsDead) return;

    Health = FMath::Clamp(Health - DamageAmount, 0.f, MaxHealth);

    if (Health <= 0.f)
    {
        Die();
    }
}

void AASBaseCharacter::Die()
{
    if (!HasAuthority()) return;
    if (bIsDead) return;

    bIsDead = true;

    // Blueprint'teki OnDeath event'ini tetikle
    OnDeath();
}

float AASBaseCharacter::GetHealthPercent() const
{
    if (MaxHealth <= 0.f) return 1.f;
    return FMath::Clamp(Health / MaxHealth, 0.f, 1.f);
}

void AASBaseCharacter::OnRep_Health()
{
    // Client tarafýnda can deðiþtiðinde HUD güncellenecek
    // Ýlerleyen fazda PlayerController üzerinden yapýlacak
}