// Fill out your copyright notice in the Description page of Project Settings.


#include "BallBearing.h"

#include "Particles/ParticleSystem.h"

// Sets default values
ABallBearing::ABallBearing()
{
	// Set this pawn to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	BallMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
	BallMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECollisionResponse::ECR_Ignore);
	BallMesh->SetSimulatePhysics(true);
	BallMesh->SetupAttachment(GetRootComponent());
	// SetRootComponent(BallMesh);
}

// Called when the game starts or when spawned
void ABallBearing::BeginPlay()
{
	Super::BeginPlay();
	BallMesh->SetLinearDamping(0.5f);
	BallMesh->SetAngularDamping(0.5f);
}

// Called every frame
void ABallBearing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	InContact = false;
}

void ABallBearing::NotifyHit(UPrimitiveComponent* myComponent, AActor* other, UPrimitiveComponent* otherComp,
                             bool selfMoved, FVector hitLocation, FVector hitNormal, FVector normalImpulse,
                             const FHitResult& hitResult)
{
	Super::NotifyHit(myComponent, other, otherComp, selfMoved, hitLocation, hitNormal, normalImpulse, hitResult);
	InContact = true;
}
