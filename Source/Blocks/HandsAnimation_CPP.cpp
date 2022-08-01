// Fill out your copyright notice in the Description page of Project Settings.


#include "HandsAnimation_CPP.h"

#include "Kismet/GameplayStatics.h"

UHandsAnimation_CPP::UHandsAnimation_CPP()
{
	
}

void UHandsAnimation_CPP::NativeBeginPlay()
{
	
	Super::NativeBeginPlay();
	
	
}

void UHandsAnimation_CPP::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacterController>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
}

void UHandsAnimation_CPP::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	speed = GetOwningActor()->GetVelocity().Size() / 600.0f;
	// UE_LOG(LogTemp, Warning, TEXT("Speed: %d"), speed);
}
