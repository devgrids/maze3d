#ifndef APPLICATION_HEADER
#define APPLICATION_HEADER

#include "IApplication.h"

namespace Maze3D
{
    class Application :public IApplication
    {
        
    public:

        static Application* Instance()
        {
            static Application instance;
            return &instance;
        }

        Application()
        {

            if (glfwInit() == GL_FALSE) return;

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
            //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            window = glfwCreateWindow(WIDTH, HEIGHT, "Laberinto 3D", NULL, NULL);
            if (!window) return;

            //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            glfwMakeContextCurrent(window);

            if (glewInit()) return;

            glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
                auto w = (Application*)glfwGetWindowUserPointer(window); if (w->resize) w->resize(width, height);
            });

            glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
                auto w = (Application*)glfwGetWindowUserPointer(window); if (w->keyboard) w->keyboard(key, scancode, action, mods);
            });

            glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
                auto w = (Application*)glfwGetWindowUserPointer(window); if (w->mousePosition) w->mousePosition(xpos, ypos);
            });

            glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
                auto w = (Application*)glfwGetWindowUserPointer(window); if (w->offset) w->offset(xoffset, yoffset);
            });

            glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
                auto w = (Application*)glfwGetWindowUserPointer(window); if (w->mouseButton) w->mouseButton(button, action, mods);
            });

            glfwSetWindowUserPointer(window, this);
            initEvents();

            glfwSwapInterval(0);
        }

        ~Application()
        {
            glfwMakeContextCurrent(window);
            glfwDestroyWindow(window);
            glfwTerminate();
            SoundEngine->drop();
        }

        void initEvents()
        {
            keyboard = [&](int key, int sancode, int action, int mods)
            {
                if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                {
                    constexpr float force = 30.0f;
                    const glm::vec3 impulse = camera->Front * force;
                    pokebolaController->addPokebola(camera->Position + camera->Front, impulse); 
                }

                if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && (state == 2 || state == 3))
                {
                    glfwSetWindowShouldClose(window, true);
                }

            };

            resize = [&](int width, int height)
            {
                if (isEngine)
                    glViewport(0, 0, WIDTH, HEIGHT);
                else if (isVr)
                    glViewport(0, 0, width, height);
            };

            mousePosition = [&](double xpos, double ypos)
            {
                input->processMouse(xpos, ypos);              
            };

            offset = [&](float xoffset, float yoffset)
            {
                camera->distancePlayer(yoffset); 
                camera->distanceUpPlayer(yoffset);
            };

            mouseButton = [&](int button, int action, int mods)
            {
                if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
                {
                    if (!input->mouseCursorDisabled)
                        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                    else
                        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

                    input->mouseCursorDisabled = !input->mouseCursorDisabled;

                    if (input->mouseCursorDisabled)
                        input->firstMouse = true;
                }
            };
        }

        void addModels()
        {
            vector<Texture> texturesLoad;
           
            int sizeTree = 50;

            glm::mat4* modelMatrices;
            modelMatrices = new glm::mat4[sizeTree];
            srand(time(NULL));
            float radius = 55.0f;
            float offset = 15.0f;
            for (unsigned int i = 0; i < sizeTree; i++)
            {
                glm::mat4 model = glm::mat4(1.0f);

                float angle = (float)i / (float)sizeTree * 360.0f;
                float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
                float x = sin(angle) * radius + displacement;
                displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
                float y = 0.0f;
                displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
                float z = cos(angle) * radius + displacement;
                model = glm::translate(model, glm::vec3(x, y, z));

                float scale = (rand() % 20) / 1000.0f + 0.05f;
                model = glm::scale(model, glm::vec3(scale));

                float rotAngle = (rand() % 360);
                model = glm::rotate(model, rotAngle, glm::vec3(0.0f, 0.6f, 0.0f));

                modelMatrices[i] = model;
            }

            modelController->addModelInstance(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.f, 1.f, 1.f), "Oak Tree/oakTree.obj", sizeTree, modelMatrices);
        
            _maze = new Maze(13);
            int nCount = _maze->GetBlockCount();
            glm::mat4* matrix = _maze->GetMatrix();

            maze = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.f, 1.f, 1.f), "assets/objects/block/block.obj", {}, nCount, matrix);
            text = new Text("Arial.ttf");
        }

        void addAnimations()
        {
            animationController->addAnimationInstance(glm::vec3(0.0f, 0.0f, 40.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.04f, 0.04f, 0.04f), "character/character.dae", *shaderInstanceDynamic, NPC1->cantidad, NPC1->transform);           
            animationController->addAnimationInstance(glm::vec3(0.0f, 0.0f, 40.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.04f, 0.04f, 0.04f), "bruja/bruja.dae", *shaderInstanceDynamic, NPC2->cantidad, NPC2->transform);
        }

        void loadTexturesImGui()
        {
            texturesImGui.push_back(Util::loadTexture("assets\\textures\\imgui\\luz.png"));
        }

        void initScene()
        {
            util = Util::Instance();
            bulletWorldController = BulletWorldController::Instance();

            terrain = new Terrain("grass-green.png", glm::vec3(100, -0.01f, 100), 50.0f);
            skybox = new Skybox("bosque", "png");

            modelController = ModelController::Instance();
            animationController = AnimationController::Instance();
            enemyController = EnemyController::Instance();

            luz = Lighting::Instance();

            pokebolaController = PokebolaController::Instance();
            loadTexturesImGui();
  
            shaderModel = util->myShaders[ShaderType::MODEL_STATIC];
            shaderAnimation = util->myShaders[ShaderType::MODEL_DYNAMIC];
            shaderInstance = util->myShaders[ShaderType::INSTANCE_STATIC];
            shaderInstanceDynamic = util->myShaders[ShaderType::INSTANCE_DYNAMIC];
            shaderSol = util->myShaders[ShaderType::SOL];

            player = new Player(*shaderAnimation);
            camera = new Camera(player->body);
            input = new InputProcessor(window, camera);

            NPC1 = new NPC(0, 10);
            NPC2 = new NPC(0, 100);
            SoundEngine = createIrrKlangDevice();

            addModels();
            addAnimations();

            enemyController->addEnemy(shaderAnimation);
            enemyController->addEnemy(shaderAnimation);

            countE = enemyController->enemies.size();
        }

        void initImgui()
        {
            const char* glsl_version = "#version 460";
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       
            io.ConfigFlags |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;

            ImGui::StyleColorsDark();

            ImGuiStyle& style = ImGui::GetStyle();
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                style.WindowRounding = 0.0f;
                style.Colors[ImGuiCol_WindowBg].w = 1.0f;
            }

            ImGuiWindowFlags window_flags = 0;
            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init(glsl_version);
        }

        void initRender()
        {
            glEnable(GL_DEPTH_TEST);
            initImgui();
            initScene();
            util->vincularFrambuffer(framebufferEngine, textureEngine);
        }

        void renderGame()
        {
            glm::mat4 ProjectionMatrix = camera->GetProjectionMatrix();
            glm::mat4 ViewMatrix = camera->GetViewMatrix();

            bulletWorldController->physics_step(ImGui::GetIO().Framerate);

            camera->update();
            modelController->update();
            animationController->update();
            enemyController->update(lastFrame);

            player->update(window, deltaTime, lastFrame);
            input->processInput(deltaTime);

            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            skybox->render(*camera, luz->isLightDirectional ? luz->luzDireccional.ambient : glm::vec3(1));
            terrain->render(*camera, luz->isLightDirectional ? luz->luzDireccional.ambient : glm::vec3(1));
            modelController->render(*camera, *shaderModel);
            maze->renderInstance(*camera, *shaderInstance);
            if (isTree)
                modelController->renderInstance(*camera, *shaderInstance);
            animationController->render(*camera, *shaderAnimation, lastFrame);
            if (isNPC)
                animationController->renderInstance(*camera, *shaderInstanceDynamic, lastFrame);
            pokebolaController->render(*camera, *shaderModel);
            enemyController->render(*camera, *shaderAnimation, lastFrame);
            player->render(*camera, *shaderAnimation);

            int countB = 6 - pokebolaController->pokebolas.size() * 1;
            
            text->RenderText("VIDA: " + to_string(player->health/10), 80.0f, 750.0f, 1.0f, glm::vec3{ 1.0f, 1.0f, 1.0f });
            text->RenderText("BOLAS: " + to_string(countB) , 450.0f, 750.0f, 1.0f, glm::vec3{ 1.0f, 1.0f, 1.0f });
            text->RenderText("ENEMIGOS: " + to_string(countE) , 850.0f, 750.0f, 1.0f, glm::vec3{ 1.0f, 1.0f, 1.0f });

            if (state == 2)
            {
                text->RenderText("GANASTE, PRESIONA LA TECLA P PARA SALIR :) ", 250.0f, 350.0f, 1.0f, glm::vec3{ 1.0f, 1.0f, 1.0f });
            }

            if (state == 3)
            {
                text->RenderText("PERDISTE, PRESIONA LA TECLA P PARA SALIR :( ", 250.0f, 350.0f, 1.0f, glm::vec3{ 1.0f, 1.0f, 1.0f });
            }

        }

        void renderEngine()
        {         
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            glm::mat4 ProjectionMatrix = camera->GetProjectionMatrix();
            glm::mat4 ViewMatrix = camera->GetViewMatrix();

            bulletWorldController->physics_step(ImGui::GetIO().Framerate);

            camera->update();
            modelController->update();
            animationController->update();
            input->processInput(deltaTime);       

            DockSpace();

            glBindFramebuffer(GL_FRAMEBUFFER, framebufferEngine);
            {
                glClearColor(0, 0, 0, 0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                skybox->render(*camera, luz->isLightDirectional ? luz->luzDireccional.ambient : glm::vec3(1));
                terrain->render(*camera, luz->isLightDirectional ? luz->luzDireccional.ambient : glm::vec3(1));
                modelController->render(*camera, *shaderModel);
                maze->renderInstance(*camera, *shaderInstance);
                if (isTree)
                    modelController->renderInstance(*camera, *shaderInstance);
                animationController->render(*camera, *shaderAnimation, lastFrame);

                if (isNPC)
                    animationController->renderInstance(*camera, *shaderInstanceDynamic, lastFrame);
                pokebolaController->render(*camera, *shaderModel);

                lightingImGui();

            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            settingsImGui();
            renderImGui();

            mazeImGui();

            // Rendering
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            ImGuiIO& io = ImGui::GetIO(); (void)io;

            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }

        }

        int render3D()
        {
            initRender();

            while (!glfwWindowShouldClose(window))
            {
                static string lastSound = "";
                float currentFrame;
                currentFrame = glfwGetTime();

                deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;

                if (pokebolaController->update(lastFrame))
                {
                    countE = enemyController->count();
                    if (countE == 0)
                    {
                        state = 2;
                    }
                }

                for (Enemy*& enemy : enemyController->enemies)
                {
                    if (bulletWorldController->check_collisions(111299, enemy->userIndex))
                    {
                        player->health--;

                        if (player->health < 0)
                        {
                            player->health = 0;
                            state = 3;
                        }
                    }
                }

                for (int i = 0; i < NPC1->cantidad; i++)
                {
                    NPC1->move(i, deltaTime);
                }

                if (isEngine)
                    renderEngine();
                else if (isVr)
                    renderGame();

                glfwSwapBuffers(window);
                glfwPollEvents();
            }
            return 0;
        }

        void settingsImGui()
        {
            ImGui::Begin("Settings ");
            const char* items[] = { "Classic", "Light", "Dark" };
            static int item_current = 2;
            if (ImGui::Combo("Editor Theme", &item_current, items, IM_ARRAYSIZE(items)))
            {
                switch (item_current)
                {
                case 0:
                    ImGui::StyleColorsClassic();
                    break;
                case 1:
                    ImGui::StyleColorsLight();
                    break;
                case 2:
                    ImGui::StyleColorsDark();
                    break;
                default:
                    break;
                }
            }

            ImGui::Separator();
            const char* GPU = (const char*)glGetString(GL_RENDERER);
            ImGui::Text("OpenGL Version: %s", (const char*)glGetString(GL_VERSION));
            ImGui::Text("GLSL Version: %s", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
            ImGui::Text("Vendor: %s", (const char*)glGetString(GL_VENDOR));
            ImGui::Text("GPU: %s", GPU);
            ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
            ImGui::Text("%.2f ms", 1000.0f / ImGui::GetIO().Framerate);
            ImGui::Separator();

            if (ImGui::Checkbox("Model Wireframe", &isWireframe))
            {
                if (isWireframe)
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                else
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            ImGui::Separator();

            ImGui::Checkbox("Tree", &isTree);
            ImGui::Checkbox("NPC", &isNPC);

            ImGui::Separator();

            if (ImGui::Button("Run Game"))
            {
                camera->mode = CameraType::THIRD_PERSON;
                isVr = true;
                isEngine = false;
                glViewport(0, 0, WIDTH, HEIGHT);
            }
            ImGui::SameLine(240);

            ImGui::End();
        }

        void mazeImGui()
        {
            static int T = 13;

            ImGui::Begin("Maze");
            {
                ImGui::InputInt("Size", &T, 2);

                if (ImGui::Button("Generate Maze"))
                {
                    _maze->DeleteCollider();
                    if (T > 12 && T < 52) {
                        _maze->CreateMaze2D(T);
                        int nCount = _maze->GetBlockCount();
                        glm::mat4* matrix = _maze->GetMatrix();
                        maze = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.f, 1.f, 1.f), "assets/objects/block/block.obj", {}, nCount, matrix);
                    }
                }
            }

            ImGui::Separator();

            ImGui::End();
        }

        void renderImGui()
        {
            ImGui::Begin("Render", NULL);
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            displayRender = ImGui::GetWindowSize();
            display = ImGui::GetCursorScreenPos();

            drawList->AddImage(
                (void*)textureEngine, display,
                ImVec2(display.x + displayRender.x, display.y + displayRender.y), ImVec2(0, 1), ImVec2(1, 0)
            );

            ImVec2 mousePosition = ImGui::GetMousePos();
            ImVec2 sceenPosition = ImGui::GetCursorScreenPos();
            ImVec2 scroll = ImVec2(ImGui::GetScrollX(), ImGui::GetScrollY());

            this->mouse = ImVec2(mousePosition.x - sceenPosition.x - scroll.x, mousePosition.y - sceenPosition.y - scroll.y);

            ImGui::End();
        }

        void lightingImGui()
        {
            ImGui::Begin("Lighting", NULL);

            ImGui::Checkbox("Directional light", &luz->isLightDirectional);
            ImGui::Checkbox("Point light", &luz->isLightPoint);
            ImGui::Checkbox("Spotlight", &luz->isLightSpot);

            if (luz->isLightDirectional)
            {
                ImGui::Text("Directional light");

                ImGui::DragFloat3("Direction", glm::value_ptr(luz->luzDireccional.direction), 0.001f, 0.0f, 1.0f);
                ImGui::ColorEdit3("Ambient", glm::value_ptr(luz->luzDireccional.ambient));
                ImGui::ColorEdit3("Difusse", glm::value_ptr(luz->luzDireccional.diffuse));
                ImGui::ColorEdit3("Specular", glm::value_ptr(luz->luzDireccional.specular));

                ImGui::Separator();
            }

            if (luz->isLightPoint)
            {
                for (int i = 0; i < luz->sizeSol(); i++)
                {
                    ImGui::Text("Point light %d", i + 1);

                    ImGui::DragFloat3((to_string(i + 1) + ".Position").c_str(), glm::value_ptr(luz->getSol(i)->position), 0.05f);
                    ImGui::ColorEdit3((to_string(i + 1) + ".Ambient").c_str(), glm::value_ptr(luz->luzPuntual[i].ambient));
                    ImGui::ColorEdit3((to_string(i + 1) + ".Difusse").c_str(), glm::value_ptr(luz->luzPuntual[i].diffuse));
                    ImGui::ColorEdit3((to_string(i + 1) + ".Specular").c_str(), glm::value_ptr(luz->luzPuntual[i].specular));
                    ImGui::DragFloat((to_string(i + 1) + ".Constant").c_str(), &luz->luzPuntual[i].constant, 0.001f, 0.0f, 1.0f);
                    ImGui::DragFloat((to_string(i + 1) + ".Linear").c_str(), &luz->luzPuntual[i].linear, 0.001f, 0.0f, 1.0f);
                    ImGui::DragFloat((to_string(i + 1) + ".Quadratic").c_str(), &luz->luzPuntual[i].quadratic, 0.001f, 0.0f, 1.0f);

                    if (ImGui::Button((to_string(i + 1) + ".Eliminar").c_str()))
                    {
                        luz->deleteSol(i);
                        break;
                    }

                    ImGui::Separator();
                }
                if (ImGui::ImageButton((void*)texturesImGui[0], ImVec2(50, 50)))
                {
                    luz->addSol(camera->Position + camera->Front, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(3.50f, 3.50f, 3.50f));
                }
            }

            if (luz->isLightSpot)
            {
                ImGui::Text("Spotlight");

                ImGui::DragFloat3("Position F", glm::value_ptr(camera->Position), 0.05f);
                ImGui::DragFloat3("Direction F", glm::value_ptr(camera->Front), 0.05f);
                ImGui::ColorEdit3("Ambient F", glm::value_ptr(luz->luzFocal.ambient));
                ImGui::ColorEdit3("Difusse F", glm::value_ptr(luz->luzFocal.diffuse));
                ImGui::ColorEdit3("Specular F", glm::value_ptr(luz->luzFocal.specular));
                ImGui::DragFloat("Constant F", &luz->luzFocal.constant, 0.001f, 0.0f, 256.0f);
                ImGui::DragFloat("Linear F", &luz->luzFocal.linear, 0.001f, 0.0f, 256.0f);
                ImGui::DragFloat("Quadratic F", &luz->luzFocal.quadratic, 0.001f, 0.0f, 256.0f);
                ImGui::DragFloat("OuterCutOff F", &luz->luzFocal.outerCutOff, 0.05f, 0.0f, 256.0f);
                ImGui::DragFloat("CutOff F", &luz->luzFocal.cutOff, 0.05f, 0.0f, luz->luzFocal.outerCutOff);
            }

            luz->loadShader(*camera, *shaderAnimation, *shaderSol);
            luz->loadShader(*camera, *shaderModel, *shaderSol);
            luz->loadShader(*camera, *shaderInstance, *shaderSol);
            luz->loadShader(*camera, *shaderInstanceDynamic, *shaderSol);

            ImGui::End();
        }

        void DockSpace()
        {
            static bool opt_fullscreen = true;
            static bool opt_padding = false;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            if (opt_fullscreen)
            {
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->GetWorkPos());
                ImGui::SetNextWindowSize(viewport->GetWorkSize());
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }
            else
            {
                dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
            }

            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                window_flags |= ImGuiWindowFlags_NoBackground;

            if (!opt_padding)
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace Demo", NULL, window_flags);
            if (!opt_padding)
                ImGui::PopStyleVar();

            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            // DockSpace
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }
            else
            {
                ImGuiIO& io = ImGui::GetIO();
                ImGui::Text("ERROR: Docking is not enabled! See Demo > Configuration.");
                ImGui::Text("Set io.ConfigFlags |= ImGuiConfigFlags_DockingEnable in your code, or ");
                ImGui::SameLine(0.0f, 0.0f);
                if (ImGui::SmallButton("click here"))
                    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            }

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Options"))
                {
                    //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                    ImGui::MenuItem("Padding", NULL, &opt_padding);
                    //ImGui::Separator();
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }
            ImGui::End();
        }

        

    };

}
#endif