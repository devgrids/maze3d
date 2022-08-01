#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#include "Animation.h"

namespace Maze3D
{
	class Player :public Animation
	{
		BulletWorldController* bulletWorldController;

		float currentSpeed = 0.0f;
		float currentTurnSpeed = 0.0f;

	public:

		float RUN_SPEED = 3.0f;
		float TURN_SPEED = 160.0f;

		int health = 1000;
		
		Player(Shader& shader) :
			Animation(glm::vec3(-10.0f, 0.0f, -10.0f), glm::vec3(0.0f), glm::vec3(0.02f), "assets/animations/vegeta/vegeta.dae", shader)
		{
			bulletWorldController = BulletWorldController::Instance();
			mass = 100.0f;
			addBodyPhysicsCapsule(111299, bulletWorldController);
			
			shape->setLocalScaling(btVector3(0.40f, 1.0f, 0.0f));
			addAnimation("Running.dae");
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

		void update(GLFWwindow* window, float deltaTime, float currentFrame)
		{
			float height = body->getWorldTransform().getOrigin().getY();
			float heightTarget = height > position.y ? height : position.y + 1;

			//position.y = height * 2.0f;

			btTransform target;

			auto _pos = body->getCenterOfMassTransform().getOrigin();
			position.x = _pos.getX();
			position.y = 0;
			position.z = _pos.getZ();

			target.setIdentity();
			target.setOrigin(btVector3(_pos));

			btQuaternion quat;
			quat.setEulerZYX(btScalar(glm::radians(rotation.z)), btScalar(glm::radians(rotation.y)), btScalar(glm::radians(rotation.x)));
			target.setRotation(btQuaternion(quat));

			//body->setWorldTransform(target);
			body->setCenterOfMassTransform(target);

			input(window, deltaTime);
			updateTime(currentFrame);
		}


		void input(GLFWwindow* window, float deltaTime)
		{
			static int direction = 1;
			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			{
				btVector3 relativeForce = btVector3(0, 0, 1000);
				btMatrix3x3& boxRot = body->getWorldTransform().getBasis();
				btVector3 correctedForce = boxRot * relativeForce;

				body->applyCentralForce(correctedForce);
				
				this->currentSpeed = RUN_SPEED;
				currentAnimation = 1;
				direction = -1;
			}
			else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			{
				btVector3 relativeForce = btVector3(0, 0, -1000);
				btMatrix3x3& boxRot = body->getWorldTransform().getBasis();
				btVector3 correctedForce = boxRot * relativeForce;

				body->applyCentralForce(correctedForce);

				this->currentSpeed = -RUN_SPEED;
				currentAnimation = 1;
				direction = 1;
			}
			else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			{
				btVector3 relativeForce = btVector3(0, 1, 10);
				btMatrix3x3& boxRot = body->getWorldTransform().getBasis();
				btVector3 correctedForce = boxRot * relativeForce;

				auto _pos = body->getCenterOfMassTransform().getOrigin();
				glm::vec3 _position = glm::vec3(0.0f);
				_position.x = _pos.getX();
				_position.y = _pos.getY();
				_position.z = _pos.getZ();

				PokebolaController::Instance()->addPokebola(_position, glm::vec3(correctedForce.getX(), correctedForce.getY(), correctedForce.getZ()));
			}
			else
			{
				currentAnimation = 0;
				this->currentSpeed = 0.0f;
			}

			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			{
				currentAnimation = 1;
				this->currentTurnSpeed = TURN_SPEED;
			}
			else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			{
				currentAnimation = 1;
				this->currentTurnSpeed = -TURN_SPEED;
			}
			else
			{
				//currentAnimation = 0;
				this->currentTurnSpeed = 0.0f;
			}
			move(deltaTime);
		}

		void move(float deltaTime)
		{
			rotation += glm::vec3(0, currentTurnSpeed * deltaTime, 0);
			float distance = currentSpeed * deltaTime;

			float dx = distance * glm::sin(glm::radians(rotation.y));
			float dz = distance * glm::cos(glm::radians(rotation.y));

			//position += glm::vec3(dx, 0, dz);
		}

	};
}
#endif
