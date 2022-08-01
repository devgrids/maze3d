#ifndef ENEMY_HEADER
#define ENEMY_HEADER

namespace Maze3D
{
	class Enemy :public Animation
	{

		float currentSpeed = 0.0f;
		float currentTurnSpeed = 0.0f;

		float _time;
		float startFrame;


	public:

		int health = 5;
		float RUN_SPEED = 3.0f;
		float TURN_SPEED = 160.0f;
		bool isRender = true;

		Enemy(Shader& shader, int index) :
			Animation(glm::vec3(10.0f, -1.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.01f), "assets/animations/bruja/bruja.dae", shader)
		{
			mass = 100.0f;
			addBodyPhysicsCapsule(111200 + index, BulletWorldController::Instance());

			shape->setLocalScaling(btVector3(0.40f, 1.0f, 0.0f));
			addAnimation("Floating.dae");

			currentAnimation = 1;

			this->startFrame = glfwGetTime();
		}

		void updateTimeBall(float currentFrame)
		{
			_time = currentFrame - startFrame;
		}

		void render(Camera& camera, Shader& shader) override
		{
			shader.use();
			glm::mat4 projection = camera.GetProjectionMatrix();
			glm::mat4 view = camera.GetViewMatrix();

			transform = glm::mat4(1.0f);

			transform = glm::translate(transform, glm::vec3(position.x, position.y, position.z));

			transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

			transform = glm::scale(transform, scale);

			shader.setMat4("projection", projection);
			shader.setMat4("view", view);
			shader.setMat4("model", transform);

			//glm::mat4 matr_normals_cube = glm::mat4(glm::transpose(glm::inverse(transform)));

			draw(shader);
		}

		void update(float currentFrame)
		{
			float height = body->getWorldTransform().getOrigin().getY();
			float heightTarget = height > position.y ? height : position.y + 1;

			btTransform target;

			auto _pos = body->getCenterOfMassTransform().getOrigin();
			position.x = _pos.getX();
			position.y = 0;
			position.z = _pos.getZ();

			target.setIdentity();
			target.setOrigin(btVector3(_pos));
			position.y = -0.4;

			btQuaternion quat;
			quat.setEulerZYX(btScalar(glm::radians(rotation.z)), btScalar(glm::radians(rotation.y)), btScalar(glm::radians(rotation.x)));
			target.setRotation(btQuaternion(quat));

			//body->setWorldTransform(target);
			body->setCenterOfMassTransform(target);

			move(currentFrame);
			updateTime(currentFrame);
			updateTimeBall(currentFrame);
		}


		void move(float currentFrame)
		{

			srand(time(NULL));
			int n;
			n = 1 + rand() % 20;

			if (_time >= 3.0f)
			{
				rotation.y = rotation.y + n * 9;
				startFrame = currentFrame;
			}


			btVector3 relativeForce = btVector3(0, 0, 1000);
			btMatrix3x3& boxRot = body->getWorldTransform().getBasis();
			btVector3 correctedForce = boxRot * relativeForce;

			body->applyCentralForce(correctedForce);

		}

	};
}
#endif

