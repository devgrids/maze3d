// Fill out your copyright notice in the Description page of Project Settings.


#include "Block.h"

// Sets default values
ABlock::ABlock()
{
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;

		FConstructorStatics()
			: PlaneMesh(TEXT("StaticMesh'/Game/Maze3D/Models/block/block.block'"))
		{
		}
	};

	static FConstructorStatics ConstructorStatics;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	Mesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	Mesh->SetRelativeLocation(FVector(0.f,0.f,0.f));
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECollisionResponse::ECR_Ignore);
	Mesh->SetupAttachment(GetRootComponent());
	
}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
