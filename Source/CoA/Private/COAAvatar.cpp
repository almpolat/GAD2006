#include "COAAvatar.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

ACOAAvatar::ACOAAvatar()
{
    PrimaryActorTick.bCanEverTick = true;

    mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    mSpringArm->SetupAttachment(RootComponent);
    mSpringArm->TargetArmLength = 300.0f;
    mSpringArm->bUsePawnControlRotation = true;

    mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    mCamera->SetupAttachment(mSpringArm, USpringArmComponent::SocketName);
    mCamera->bUsePawnControlRotation = false;
}