// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWidget.h"

#include "CharacterController.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

bool UGameWidget::Initialize()
{
	return Super::Initialize();
}

void UGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Character = Cast<ACharacterController>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	State->SetText(FText::FromString(""));

	// AGameModeBase* mode = GetWorld()->GetAuthGameMode();
	// if (mode != nullptr)
	// {
	// 	GameMode = Cast<ABlocksGameMode>(mode);
	// }
}

void UGameWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// FString::SanitizeFloat()

	FString health = FString::SanitizeFloat(Character->Life);
	
	if (Character->Life <= 0)
	{
		Character->Life = 0;
		State->SetText(FText::FromString("Game Over"));
	}

	FText message = FText::FromString("Life: " + health);
	Life->SetText(message);

	if (Character->bTarget)
	{
		State->SetText(FText::FromString("Win"));
	}
}
