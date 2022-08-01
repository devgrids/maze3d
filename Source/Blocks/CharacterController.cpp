// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterController.h"
#include "Camera/CameraComponent.h"
#include "Chaos/Private/kDOP.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Enemy.h"

// Sets default values
ACharacterController::ACharacterController()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(Root);

	Hand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hand"));
	Hand->SetupAttachment(Camera);

	// Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	// Weapon->SetupAttachment(Hand);
}

// Called when the game starts or when spawned
void ACharacterController::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACharacterController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveFoward", this, &ACharacterController::MoveFoward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterController::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ACharacterController::RotateYaw);
	PlayerInputComponent->BindAxis("LookUp", this, &ACharacterController::RotatePitch);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacterController::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacterController::StopJumping);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACharacterController::StartShoot);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ACharacterController::StopShoot);
}

void ACharacterController::MoveFoward(float value)
{
	AddMovementInput(GetActorForwardVector(), value);
}

void ACharacterController::MoveRight(float value)
{
	AddMovementInput(GetActorRightVector(), value);
}

void ACharacterController::RotateYaw(float value)
{
	AddControllerYawInput(value * this->RotationSpeed * GetWorld()->GetDeltaSeconds());
}

void ACharacterController::RotatePitch(float value)
{
	AddControllerPitchInput(value * this->RotationSpeed * GetWorld()->GetDeltaSeconds());
}

void ACharacterController::StartShoot()
{
	Firing = true;

	FHitResult hitInfo;
	bool hasHit = GetWorld()->LineTraceSingleByChannel(
		hitInfo,
		Camera->GetComponentLocation(),
		Camera->GetComponentLocation() + Camera->GetForwardVector() * 1000.0f,
		ECC_GameTraceChannel3
	);

	DrawDebugLine(
		GetWorld(),
		Camera->GetComponentLocation(),
		Camera->GetComponentLocation() + Camera->GetForwardVector() * 1000.0f,
		FColor::Blue,
		false,
		3
	);

	if (hasHit && hitInfo.GetActor())
	{
		// hitInfo.GetActor()->Destroy();
		if (Damages.Contains(hitInfo.BoneName))
		{
			float damage = Damages[hitInfo.BoneName];

			AEnemy* enemy = Cast<AEnemy>(hitInfo.GetActor());

			UE_LOG(LogTemp, Warning, TEXT("Vida Enemy: %f"), enemy->Life);
			if (Life > 0)
			{
				enemy->DamageLife(damage);
				if (Life <= 0)
				{
					enemy->Destroy();
					UE_LOG(LogTemp, Warning, TEXT("Win"));
				}
			}
		}
	}
}

void ACharacterController::StopShoot()
{
	Firing = false;
}

void ACharacterController::DamageLife(float damage)
{
	Life -= damage;
}
