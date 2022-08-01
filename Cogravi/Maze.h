#ifndef MAZE_HEADER
#define MAZE_HEADER

#include <queue>

namespace Maze3D
{

#define TOP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

	class Maze
	{
	private:

		unsigned int n;
		char* board;

		unsigned int nBlocks;
		glm::mat4* matrix;

		BulletWorldController* worldController = BulletWorldController::Instance();
		vector<btRigidBody*> _rigidbody;

	public:

		Maze(const unsigned int& n)
		{
			srand((unsigned)time(NULL));
			CreateMaze2D(n);
		}

		glm::mat4* GetMatrix()
		{
			return matrix;
		}

		int GetBlockCount()
		{
			return nBlocks;
		}

		void DeleteCollider()
		{
			int n = CountBlocks();
			for (int c = 0; c < n; c++)
			{
				worldController->dynamicsWorld->removeRigidBody(_rigidbody[c]);
			}
			_rigidbody.clear();
		}

	private:

		~Maze()
		{
			board = NULL;
			delete board;
			delete this;
		}

		void Initialize()
		{
			for (int i = 0; i < n * n; i++)
			{
				board[i] = '*';
			}
		}

		int Index(int x, int y)
		{
			return y * n + x;
		}

		int CheckBoard(int x, int y)
		{
			if (x < 0 || x >= n) return false;
			if (y < 0 || y >= n) return false;
			return true;
		}

		void Draw()
		{
			for (int y = 0; y < n; ++y)
			{
				for (int x = 0; x < n; ++x)
				{
					std::cout << board[Index(x, y)];
				}
				std::cout << "\n";
			}
		}

		int CountBlocks()
		{
			int count = 0;
			for (int y = 0; y < n; ++y)
			{
				for (int x = 0; x < n; ++x)
				{
					if (board[Index(x, y)] == '*') count++;
				}
			}
			return count;
		}

		void GenerateMaze(int x, int y)
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
				case TOP: dy = -1; break;
				case DOWN: dy = 1; break;
				case RIGHT: dx = 1; break;
				case LEFT: dx = -1; break;
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

		void GenerateMatrix()
		{
			int i = 0;
			for (int y = 0; y < n; ++y)
			{
				for (int x = 0; x < n; ++x)
				{
					if (board[Index(x, y)] == '*')
					{
						glm::mat4 model = glm::mat4(1.0f);
						glm::vec3 position = glm::vec3(x * 2.1f - n, 1.0f, y * 2.1f - n);
						model = glm::translate(model, position);
						model = glm::scale(model, glm::vec3(1.0f));
						model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
						model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
						model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

						AddCollider(position, i);

						matrix[i++] = model;
					}
				}
			}
		}

		

		void AddCollider(glm::vec3 position, int i)
		{
			btRigidBody* body;

			auto shape = new btBoxShape(btVector3(1, 1, 1));

			btTransform transform;
			transform.setIdentity();
			transform.setOrigin(btVector3(position.x, position.y, position.z));
			transform.setRotation(btQuaternion(0, 0, 0, 1));

			btVector3 inertia(1, 1, 1);
			shape->calculateLocalInertia(1000, inertia);

			btDefaultMotionState* state = new btDefaultMotionState(transform);
			btRigidBody::btRigidBodyConstructionInfo info(1000, state, shape, inertia);

			body = new btRigidBody(info);
			body->setRestitution(0);
			body->setUserIndex(1024 + i);
			body->setLinearFactor(btVector3(0, 0, 0));

			body->setAngularFactor(btVector3(0, 0, 0));
			body->setGravity(btVector3(0, -9.81, 0));

			body->setActivationState(DISABLE_DEACTIVATION);

			worldController->dynamicsWorld->addRigidBody(body);

			_rigidbody.push_back(body);

		}

		void FindPathBFS()
		{
			std::queue<std::pair<int, int>> _queue;
			std::pair<int, int> current;

			_queue.push({ 1,1 });
			board[Index(1, 1)] = 'A';

			bool* visited = new bool[(n * n)]();
			for (int i = 0; i < n * n; i++) visited[i] = false;

			const int dx[4] = { 0, 0, 1, -1 };
			const int dy[4] = { 1, -1, 0, 0 };

			while (!_queue.empty())
			{
				current = _queue.front();
				_queue.pop();

				visited[Index(current.first, current.second)] = true;
				for (int i = 0; i < 4; ++i)
				{
					int nx = dx[i] + current.first;
					int ny = dy[i] + current.second;

					if (nx >= 0 && nx < n && ny >= 0 && ny < n && visited[Index(nx, ny)] == false && board[Index(nx, ny)] != '*')
					{
						_queue.push({ nx, ny });
					}
				}
			}

			board[Index(current.first, current.second)] = 'B';

		}

		public:

		void CreateMaze2D(int n)
		{
			if (n % 2 == 0) n++;
			this->n = n;

			delete board;
			board = NULL;
			board = new char[(n * n)]();

			Initialize();
			GenerateMaze(1, 1);
			FindPathBFS();
			Draw();

			this->nBlocks = CountBlocks();

			delete matrix;
			matrix = NULL;
			matrix = new glm::mat4[nBlocks];

			GenerateMatrix();

		}


	};
}
#endif