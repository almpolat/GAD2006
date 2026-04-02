// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "COABaseCharacter.h"
#include "COAAvatar.generated.h"

/**
 * 
 */
UCLASS()
class COA_API ACOAAvatar : public ACOABaseCharacter
{
	GENERATED_BODY()
public:
    ACOAAvatar();

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* mSpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* mCamera;
	
};
