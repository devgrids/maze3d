#ifndef BULLET_WORLD_CONTROLLER_HEADER
#define BULLET_WORLD_CONTROLLER_HEADER

#include <btBulletDynamicsCommon.h>
#include <memory>
#include <vector>

using namespace std;
namespace Maze3D
{
	class BulletWorldController
	{

	public:

		static BulletWorldController* Instance()
		{
			static BulletWorldController instance;
			return &instance;
		}

		btDiscreteDynamicsWorld * dynamicsWorld; //Mundo físico para bullet

		//Parámetros de configuración del mundo Bullet
		btDefaultCollisionConfiguration     collisionConfiguration;
		btCollisionDispatcher               collisionDispatcher;
		btDbvtBroadphase                    overlappingPairCache;
		btSequentialImpulseConstraintSolver constraintSolver;

		//Creamos y configuramos el mundo físico en el constructor
		BulletWorldController() : collisionDispatcher(&collisionConfiguration)
		{
			btDiscreteDynamicsWorld* dynamicsWorld
			(
				new btDiscreteDynamicsWorld
				(
					&collisionDispatcher,
					&overlappingPairCache,
					&constraintSolver,
					&collisionConfiguration
				)
			);

			dynamicsWorld->setGravity(btVector3(0, -9.8f, 0));

			this->dynamicsWorld = dynamicsWorld;
		}

		//Actualizamos la física
		void physics_step(float Hz)
		{
			dynamicsWorld->stepSimulation(1.f / Hz);
		}

		//Comprobación de colisiones, devuelve true si están colisionando dos objetos con las IDs indicadas
		bool check_collisions(int id_1, int id_2)
		{
			int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
			for (int i = 0; i < numManifolds; i++)
			{
				btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
				btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
				btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

				if ((obA->getUserIndex() == id_1 && obB->getUserIndex() == id_2) || (obA->getUserIndex() == id_2 && obB->getUserIndex() == id_1))
				{
					return true;
				}
			}
			return false;
		}


		
	};
}

#endif