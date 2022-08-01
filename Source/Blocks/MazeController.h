// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Block.h"
#include "BallBearingGoal.h"
#include "Enemy.h"
#include "GameFramework/Actor.h"
#include "MazeController.generated.h"

UCLASS()
class BLOCKS_API AMazeController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMazeController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void GenerateMaze3D();
	void GenerateMaze(int x, int y);
	void FindPathBFS();
	bool CheckBoard(int x, int y);
	int Index(int x, int y);

private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"), Category="Maze")
	TSubclassOf<ABlock> block;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"), Category="Maze")
	TSubclassOf<ABallBearingGoal> target;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"), Category="Maze")
	TSubclassOf<AEnemy> enemy;

	UPROPERTY(EditAnywhere, Category="Maze")
	int size = 13;

	char* board;
	int nBlocks = 0;
};
