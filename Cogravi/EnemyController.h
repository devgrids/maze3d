#ifndef ENEMY_CONTROLLER_HEADER
#define ENEMY_CONTROLLER_HEADER

#include "Enemy.h"

namespace Maze3D {

    class EnemyController
    {
        BulletWorldController* bulletWorldController;

    public:

        static EnemyController* Instance()
        {
            static EnemyController instance;
            return &instance;
        }

        vector<Enemy*> enemies;

        EnemyController()
        {
            bulletWorldController = BulletWorldController::Instance();
        }

        void addEnemy(Shader* shaderAnimation)
        {
            enemies.push_back(new Enemy(*shaderAnimation, enemies.size()));
        }

        void update(float currentFrame)
        {
            for (Enemy*& enemy : enemies)
                if (enemy->isRender)
                    enemy->update(currentFrame);
        }

        int count()
        {
            int c = 0;
            for (Enemy*& enemy : enemies)
                if (enemy->isRender) c++;
            return c;
        }

        void render(Camera& camera, Shader& shader, float currentFrame)
        {
            for (Enemy*& enemy : enemies)
            {
                if (enemy->isRender)
                {
                    enemy->updateTime(currentFrame);
                    enemy->render(camera, shader);
                }
            }
        }

        void remove(Enemy*& enemy, BulletWorldController* worldController)
        {
            if (!enemies.empty())
            {
                enemy->destroy(worldController);
                enemies.erase(enemies.begin() + (enemy->userIndex - 111200));

                for (int i = 0; i < enemies.size(); i++)
                {
                    enemies[i]->userIndex = i;
                    enemies[i]->body->setUserIndex(i);
                }
                enemy = NULL;
                delete enemy;
            }
        }
    };
}
#endif