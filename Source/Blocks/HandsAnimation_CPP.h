// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterController.h"
#include "HandsAnimation_CPP.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKS_API UHandsAnimation_CPP : public UAnimInstance
{
	GENERATED_BODY()

public:
	
	UHandsAnimation_CPP();

	virtual void NativeBeginPlay() override;
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACharacterController* Character;
	
};
