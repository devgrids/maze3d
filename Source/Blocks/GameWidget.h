// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterController.h"
#include "Blueprint/UserWidget.h"
#include "GameWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKS_API UGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
	ACharacterController* Character;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Life;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* State;

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
