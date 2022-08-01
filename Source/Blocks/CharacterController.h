// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterController.generated.h"

UCLASS()
class BLOCKS_API ACharacterController : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveFoward(float value);
	void MoveRight(float value);
	void RotateYaw(float value);
	void RotatePitch(float value);

	void StartShoot();
	void StopShoot();

public:
	UFUNCTION(BlueprintCallable)
	void DamageLife(float damage);

	UPROPERTY()
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* Hand;

	UPROPERTY(EditAnywhere)
	TMap<FName, float> Damages;

	// UPROPERTY(VisibleAnywhere)
	// class USkeletalMeshComponent* Weapon;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// TSubclassOf<class AEnemy> Enemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Life = 100.0f;

	UPROPERTY(EditAnywhere)
	float RotationSpeed = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Firing = false;

	// Ball in state end
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTarget = false;
};
