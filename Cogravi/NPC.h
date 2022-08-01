#ifndef NPC_HEADER
#define NPC_HEADER

namespace Maze3D
{
	class NPC
	{

	public:

		AnimationController* animationController;

		glm::mat4* transform;
		glm::vec3* position;
		glm::vec3* rotation;
		glm::vec3* scale;

		int index;
		int cantidad;

		float currentSpeed = 0.0f;
		float currentTurnSpeed = 0.0f;

		float RUN_SPEED = 2.5f;
		float TURN_SPEED = 160.0f;

		NPC(int index, int cantidad, float radius = 55.0f, float offset = 35.0f)
		{
			this->index = index;
			this->cantidad = cantidad;

			transform = new glm::mat4[cantidad];
			position = new glm::vec3[cantidad];
			rotation = new glm::vec3[cantidad];
			scale = new glm::vec3[cantidad];

			srand(time(NULL));

			for (unsigned int i = 0; i < cantidad; i++)
			{
				glm::mat4 model = glm::mat4(1.0f);

				float angle = (float)i / (float)cantidad * 360.0f;
				float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;

				position[i].x = sin(angle) * radius + displacement;
				displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
				position[i].y = 0.0f;
				displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
				position[i].z = cos(angle) * radius + displacement;

				model = glm::translate(model, position[i]);

				float s = (rand() % 6) / 1000.0f + 0.009f;

				scale[i] = glm::vec3(s);
				model = glm::scale(model, scale[i]);

				rotation[i].y = (rand() % 360);
				model = glm::rotate(model, glm::radians(rotation[i].y), glm::vec3(0.0f, 1.0f, 0.0f));

				transform[i] = model;
			}

			animationController = AnimationController::Instance();
		}		

		void move(int i, float deltaTime)
		{
			srand(time(NULL));
			int indexR = rand() % 100;

			this->currentSpeed = RUN_SPEED;

			if (indexR == 1)
			{
				this->currentTurnSpeed = TURN_SPEED;
				animationController->changeAnimationInstance(this->index, 0);
			}
			else if (indexR == 50)
			{
				animationController->changeAnimationInstance(this->index, 0);
				this->currentTurnSpeed = -TURN_SPEED;
			}
			else
			{
				//animationController->changeAnimationInstance(this->index, 1);
				this->currentTurnSpeed = 0;
			}

			rotation[i] += glm::vec3(0, currentTurnSpeed * deltaTime, 0);

			if (position[i].x < -88.0f || position[i].x > 88.0f || position[i].z < -88.0f || position[i].z > 88.0f)
			{
				rotation[i].y = rotation[i].y + 180.f;
			}

			float distance = currentSpeed * deltaTime;

			float dx = distance * glm::sin(glm::radians(rotation[i].y));
			float dz = distance * glm::cos(glm::radians(rotation[i].y));

			position[i] += glm::vec3(dx, 0, dz);

			transform[i] = glm::mat4(1.0f);

			transform[i] = glm::translate(transform[i], position[i]);
			transform[i] = glm::rotate(transform[i], glm::radians(rotation[i].y), glm::vec3(0.0f, 1.0f, 0.0f));
			transform[i] = glm::scale(transform[i], scale[i]);	

			animationController->animationsInstance[this->index]->configureInstance(transform);

		}

	};
}
#endif
