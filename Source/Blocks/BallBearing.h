// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallBearing.generated.h"

UCLASS()
class BLOCKS_API ABallBearing : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABallBearing();

	// The static mesh that represents the ball bearing.
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BallMesh = nullptr;

	// Is the ball bearing attractive to magnets?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BallBearing)
	bool Magnetized = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Receive notification of a collision contact and record that we're in contact with something.
	virtual void NotifyHit(UPrimitiveComponent* myComponent, AActor* other, UPrimitiveComponent* otherComp,
	                       bool selfMoved, FVector hitLocation, FVector hitNormal, FVector normalImpulse,
	                       const FHitResult& hitResult) override;

	// Is the ball bearing in contact with any other geometry?
	bool InContact = false;
};
