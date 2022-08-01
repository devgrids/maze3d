// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimation_CPP.h"

UEnemyAnimation_CPP::UEnemyAnimation_CPP()
{
}

void UEnemyAnimation_CPP::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UEnemyAnimation_CPP::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UEnemyAnimation_CPP::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	speed = GetOwningActor()->GetVelocity().Size() / 250.0f;
}
