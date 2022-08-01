#ifndef ANIMATION_HEADER
#define ANIMATION_HEADER

#include "AnimationAttributes.h"

namespace Maze3D
{
	class Animation :public AnimationAttributes, public DynamicGameObject
	{
	public:

		Animation(const glm::vec3 position, const glm::vec3 rotation, const glm::vec3 scale, string const& path, Shader &shader, vector<Texture> textures = {}) :
			DynamicGameObject(position, rotation, scale, path, shader, textures)
		{
			this->translate = glm::vec3(0.0f, 0.0f, 0.0f);		
		}

		virtual void render(Camera& camera, Shader &shader) override
		{
			shader.use();
			const glm::mat4 projection = camera.GetProjectionMatrix();
			const glm::mat4 view = camera.GetViewMatrix();

			transform = glm::mat4(1.0f);

			transform = transform * physicsMatrix;
			transform = glm::translate(transform, glm::vec3(translate.x, translate.y, translate.z));

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

		virtual void update() override
		{
			btTransform physics_transform;
			body->getMotionState()->getWorldTransform(physics_transform);
			glm::mat4 graphics_transform;
			physics_transform.getOpenGLMatrix(glm::value_ptr(physicsMatrix));
			position = glm::vec3(physicsMatrix[3][0], 0, physicsMatrix[3][2]);

		}

		void addBodyPhysicsBox(const int user_index, BulletWorldController* world_controller)
		{
			this->userIndex = user_index;
			btCollisionShape* shape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
			this->shape = shape;
			bodyPhysicsConfiguration(world_controller);
		}

		void addBodyPhysicsSphere(const int user_index, BulletWorldController* world_controller)
		{
			this->userIndex = user_index;
			btCollisionShape* shape = new btSphereShape(btScalar(1));
			this->shape = shape;
			bodyPhysicsConfiguration(world_controller);
		}

		void addBodyPhysicsCapsule(int userIndex, BulletWorldController* worldController)
		{
			this->userIndex = userIndex;
			btCollisionShape* nshape = new btCapsuleShape(btScalar(1.0f), btScalar(1.0f));
			this->shape = nshape;
			bodyPhysicsConfiguration(worldController);
		}

		void addBodyPhysicsCylinder(int userIndex, BulletWorldController* worldController)
		{
			this->userIndex = userIndex;
			btCollisionShape* nshape = new btCylinderShape(btVector3(1.0f, 1.0f, 1.0f));
			this->shape = nshape;
			bodyPhysicsConfiguration(worldController);
		}

		void addBodyPhysicsCone(int userIndex, BulletWorldController* worldController)
		{
			this->userIndex = userIndex;
			btCollisionShape* nshape = new btConeShape(btScalar(1.0f), btScalar(1.0f));
			this->shape = nshape;
			bodyPhysicsConfiguration(worldController);
		}

		void bodyPhysicsConfiguration(const BulletWorldController* world_controller)
		{
			btTransform transform;
			transform.setIdentity();
			transform.setOrigin(btVector3(position.x, position.y, position.z));
			transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, 1));

			//Calculamos la inercia del modelo
			btVector3 inertia(localInertia.x, localInertia.y, localInertia.z);
			shape->calculateLocalInertia(mass, inertia);

			//Configuramos las propiedades b�sicas de construcci�n del cuerpo

			btDefaultMotionState* state = new btDefaultMotionState(transform);
			btRigidBody::btRigidBodyConstructionInfo info(mass, state, shape, inertia);

			//Establecemos los par�metros que recibidos como par�metro
			body = new btRigidBody(info);
			body->setRestitution(restitution);
			body->setUserIndex(this->userIndex);
			body->setLinearFactor(btVector3(linearFactor.x, linearFactor.y, linearFactor.z));

			//Por defecto, todos los modelos est�n bloqueados en el espacio en X,Z as� como sus ejes de rotaci�n
			body->setAngularFactor(btVector3(angularFactor.x, angularFactor.y, angularFactor.z));
			body->setGravity(btVector3(gravity.x, gravity.y, gravity.z));

			body->setActivationState(DISABLE_DEACTIVATION);

			//A�adimos el cuerpo al mundo din�mico
			world_controller->dynamicsWorld->addRigidBody(body);
		}

		void destroy(BulletWorldController* worldController)
		{
			//Obtenemos todos los componentes de colision del mundo
			btCollisionObjectArray arr = worldController->dynamicsWorld->getCollisionObjectArray();

			//ID del cuerpo que deseamos destruir
			int deleteID;

			//Buscamos entre todos los componentes aquel con el ID que se desea borrar, guardamos su ID y lo eliminamos del mundo
			for (size_t i = 0; i < arr.size(); i++)
			{
				if (arr.at(i)->getUserIndex() == userIndex) deleteID = i;
			}
			worldController->dynamicsWorld->removeCollisionObject(worldController->dynamicsWorld->getCollisionObjectArray()[deleteID]);
		}

		
		
	};
}



#endif