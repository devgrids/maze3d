#ifndef POKEBOLA_HEADER
#define POKEBOLA_HEADER

namespace Maze3D
{
	class Pokebola :public Model
	{

	public:
		float time;
		float startFrame;

		Pokebola(glm::vec3 position, int index) :
			Model(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), "assets/objects/pokebola/pokebola.obj")
		{
			this->shapeScalar = glm::vec3(0.2f);
			this->mass = 1.0f;
			//this->angularFactor[0] = this->angularFactor[1] = this->angularFactor[2] = 1.0f;
			this->shape_current = 1;
			this->startFrame = glfwGetTime();
			this->addBodyPhysicsSphere(index);
		}

		~Pokebola() {}

		void updateTime(float currentFrame)
		{
			time = currentFrame - startFrame;
		}

	};
}
#endif