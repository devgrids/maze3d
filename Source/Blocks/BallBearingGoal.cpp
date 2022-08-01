#include "BallBearingGoal.h"
#include "Components/SphereComponent.h"
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"


static float GetRatio(float value, float minimum, float maximum, bool clamp = true)
{
	if (value > maximum && clamp == true)
	{
		return 1.0f;
	}
	else if (value > minimum)
	{
		return (value - minimum) / (maximum - minimum);
	}
	else
	{
		return 0.0f;
	}
}

ABallBearingGoal::ABallBearingGoal()
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorHiddenInGame(false);
}

void ABallBearingGoal::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<ACharacterController>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}


void ABallBearingGoal::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GetCollisionComponent()->SetHiddenInGame(true);

#if WITH_EDITORONLY_DATA
	GetSpriteComponent()->SetHiddenInGame(true);
#endif
}


void ABallBearingGoal::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);

	FVector ourLocation = GetActorLocation();
	float sphereRadius = Cast<USphereComponent>(GetCollisionComponent())->GetScaledSphereRadius();
	float magnetism = Magnetism;

	// static const IConsoleVariable* extraForce = IConsoleManager::Get().FindConsoleVariable(
	// 	TEXT("OurGame.ExtraMagnetism"));
	//
	// if (extraForce != nullptr &&
	// 	extraForce->GetInt() != 0)
	// {
	// 	magnetism *= 4.0f;
	// }

	for (ABallBearing* ballBearing : BallBearings)
	{
		FVector difference = ourLocation - ballBearing->GetActorLocation();
		float distance = difference.Size();
		FVector direction = difference;

		direction.Normalize();

		float ratio = GetRatio(distance, 0.0f, sphereRadius);
		FVector force = (1.0f - ratio) * magnetism * direction;

		ballBearing->BallMesh->AddForce(force);
	}

	if (HasBallBearing())
	{
		Character->bTarget = true;
		// UE_LOG(LogTemp, Warning, TEXT("Win!!!"));
	}
}

void ABallBearingGoal::NotifyActorBeginOverlap(AActor* otherActor)
{
	Super::NotifyActorBeginOverlap(otherActor);

	ABallBearing* ballBearing = Cast<ABallBearing>(otherActor);

	if (ballBearing != nullptr &&
		ballBearing->Magnetized == true)
	{
		BallBearings.AddUnique(ballBearing);
	}
}

void ABallBearingGoal::NotifyActorEndOverlap(AActor* otherActor)
{
	Super::NotifyActorEndOverlap(otherActor);

	ABallBearing* ballBearing = Cast<ABallBearing>(otherActor);

	if (ballBearing != nullptr &&
		ballBearing->Magnetized == true)
	{
		BallBearings.Remove(ballBearing);
	}
}

bool ABallBearingGoal::HasBallBearing() const
{
	FVector ourLocation = GetActorLocation();

	for (const ABallBearing* ballBearing : BallBearings)
	{
		FVector difference = ourLocation - ballBearing->GetActorLocation();
		float distance = difference.Size();

		if (distance < 75.0f)
		{
			return true;
		}
	}
	return false;
}
