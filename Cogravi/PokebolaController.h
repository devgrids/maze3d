#ifndef POKEBOLA_CONTROLLER_HEADER
#define POKEBOLA_CONTROLLER_HEADER

#include "Pokebola.h"

namespace Maze3D 
{

    class PokebolaController
    {

    public:

        vector<Pokebola*>pokebolas;

        EnemyController* enemyController;
        BulletWorldController* bulletWorldController;

        int index;
        float secondsDelete = 10.0f;

        static PokebolaController* Instance()
        {
            static PokebolaController instance;
            return &instance;
        }

        PokebolaController()
        {
            enemyController = EnemyController::Instance();
            bulletWorldController = BulletWorldController::Instance();
        }

        bool update(float currentFrame)
        {
            bool isCollision = false;
            Enemy* enemySelect = nullptr;
            for (Pokebola*& pokebola : pokebolas)
            {
                pokebola->update();
                pokebola->updateTime(currentFrame);

                for (Enemy*& enemy : enemyController->enemies)
                {
                    if (bulletWorldController->check_collisions(enemy->userIndex, pokebola->userIndex))
                    {
                        
                        enemy->isRender = false;
                        enemy->destroy(bulletWorldController);
                        isCollision = true;
                        break;
                    }
                }

                if (pokebola->time >= secondsDelete)
                {
                    removePokebola(pokebola);
                }
            }  
            return isCollision;
        }

        void render(Camera& camera, Shader& shader)
        {
            for (Pokebola*& pokebola : pokebolas)
                pokebola->render(camera, shader);        
        }

        void addPokebola(glm::vec3 position, glm::vec3 impulse)
        {
            if (pokebolas.size() > 5) return;

            Pokebola* pokebola = new Pokebola(position, pokebolas.size() + 100);

            pokebola->body->setRestitution(0.5f);
            pokebola->body->setFriction(0.3f);
            pokebola->body->setRollingFriction(0.3f);
            pokebola->body->setAngularFactor(btVector3(1.0f, 1.0f, 1.0f));
            pokebola->body->applyCentralImpulse(btVector3(impulse.x, impulse.y, impulse.z));

            pokebolas.push_back(pokebola);
        }

        void removePokebola(Pokebola*& pokebola)
        {
            if (!pokebolas.empty())
            {
                pokebola->destroy();
                pokebolas.erase(pokebolas.begin() + (pokebola->userIndex - 100));

                for (int i = 0; i < pokebolas.size(); i++)
                {
                    pokebolas[i]->userIndex = i + 100;
                    pokebolas[i]->body->setUserIndex(i + 100);
                }
            }
        }


    };
}
#endif