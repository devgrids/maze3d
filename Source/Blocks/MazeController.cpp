// Fill out your copyright notice in the Description page of Project Settings.

#define TOP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

#include "MazeController.h"
#include "Block.h"
#include "BallBearingGoal.h"
#include <queue>

// Sets default values
AMazeController::AMazeController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	srand((unsigned)time(NULL));
}

// Called when the game starts or when spawned
void AMazeController::BeginPlay()
{
	Super::BeginPlay();

	GenerateMaze3D();
}

// Called every frame
void AMazeController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMazeController::GenerateMaze3D()
{
	if (size % 2 == 0) size++;
	board = new char[(size * size)]();

	for (int i = 0; i < size * size; i++) board[i] = '*';
	GenerateMaze(1, 1);
	FindPathBFS();

	float range = 202.1f;

	for (std::size_t y = 0; y < size; ++y)
	{
		for (std::size_t x = 0; x < size; ++x)
		{
			FRotator Rotation(0.0f, 0.0f, 0.0f);

			if (board[Index(x, y)] == '*')
			{
				FVector Location(x * range - size, y * range - size, 0);
				GetWorld()->SpawnActor<ABlock>(block, Location, Rotation);
			}
			else if (board[Index(x, y)] == 'B')
			{
				FVector Location(x * range - size, y * range - size, -27);
				GetWorld()->SpawnActor<ABallBearingGoal>(target, Location, Rotation);
				// GetWorld()->SpawnActor<AEnemy>(enemy, FVector(x * range - size, y * range - size, 50),
				//                                Rotation);
			}
		}
	}
}

void AMazeController::GenerateMaze(int x, int y)
{
	board[Index(x, y)] = ' ';

	int direction[4];
	direction[0] = TOP;
	direction[1] = RIGHT;
	direction[2] = DOWN;
	direction[3] = LEFT;

	for (int i = 0; i < 4; ++i)
	{
		int random = rand() % 4;
		int temp = direction[random];
		direction[random] = direction[i];
		direction[i] = temp;
	}

	for (int i = 0; i < 4; ++i)
	{
		int dx = 0, dy = 0;
		switch (direction[i])
		{
		case TOP: dy = -1;
			break;
		case DOWN: dy = 1;
			break;
		case RIGHT: dx = 1;
			break;
		case LEFT: dx = -1;
			break;
		}

		int x2 = x + (dx << 1);
		int y2 = y + (dy << 1);

		if (CheckBoard(x2, y2) && board[Index(x2, y2)] == '*')
		{
			board[Index(x2 - dx, y2 - dy)] = ' ';
			GenerateMaze(x2, y2);
		}
	}
}

void AMazeController::FindPathBFS()
{
	std::queue<std::pair<int, int>> _queue;
	std::pair<int, int> current;

	_queue.push({1, 1});
	board[Index(1, 1)] = 'A';

	bool* visited = new bool[(size * size)]();
	for (int i = 0; i < size * size; i++) visited[i] = false;

	const int dx[4] = {0, 0, 1, -1};
	const int dy[4] = {1, -1, 0, 0};

	while (!_queue.empty())
	{
		current = _queue.front();
		_queue.pop();

		visited[Index(current.first, current.second)] = true;
		for (int i = 0; i < 4; ++i)
		{
			int nx = dx[i] + current.first;
			int ny = dy[i] + current.second;

			if (nx >= 0 && nx < size && ny >= 0 && ny < size && visited[Index(nx, ny)] == false && board[Index(nx, ny)]
				!= '*')
			{
				_queue.push({nx, ny});
			}
		}
	}

	board[Index(current.first, current.second)] = 'B';
}

bool AMazeController::CheckBoard(int x, int y)
{
	if (x < 0 || x >= size) return false;
	if (y < 0 || y >= size) return false;
	return true;
}

int AMazeController::Index(int x, int y)
{
	return y * size + x;
}
