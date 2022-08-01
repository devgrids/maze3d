#ifndef I_APPLICATION_HEADER
#define I_APPLICATION_HEADER

const int WIDTH = 1280;
const int HEIGHT = 800;

#include <functional>
#include <GL/glew.h> 

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <irrKlang.h>
using namespace irrklang;

#include <iostream>
using namespace std;

//#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Shader.h"
#include "Util.h"
#include "BulletWorldController.h"
#include "Camera.h"

#include "Terrain.h"
#include "Skybox.h"
#include "DebugDrawer.h"
#include "AnimationController.h"
#include "ModelController.h"
#include "EnemyController.h"
#include "PokebolaController.h"
#include "PlayerController.h"
#include "Player.h"
#include "InputProcessor.h"
#include "Lighting.h"
#include "NPC.h"
#include "Text.h"
#include "Maze.h"

using namespace Maze3D;

namespace Maze3D
{
	class IApplication
	{
	public:

        GLFWwindow* window;

        bool isEngine = true;
        bool isVr = false;

        ImVec2 mouse;
        int countE;
        int state = 1;

        float deltaTime;
        float lastFrame;

        GLuint framebufferEngine;
        GLuint textureEngine;

        GLuint textureTablet;

        ImVec2 displayRender;
        ImVec2 display;

        Camera* camera;
        vector<GLuint> textureFloors;

        vector<GLuint> textureSkyboxs;
        vector<GLuint> textureObjects;

        vector<GLuint> texturesImGui;

        Terrain* terrain;
        Skybox* skybox;

        BulletWorldController* bulletWorldController;
        GameObject* maze;

        Maze* _maze;
        Text* text;

        Util* util;
        InputProcessor* input;

        ModelController* modelController;
        AnimationController* animationController;

        PokebolaController* pokebolaController;
        EnemyController* enemyController;

        Player* player;

        Shader* shaderModel;
        Shader* shaderAnimation;

        Shader* shaderInstance;
        Shader* shaderInstanceDynamic;

        Lighting* luz;
        Shader* shaderSol;

        NPC* NPC1;
        NPC* NPC2;

        ISoundEngine* SoundEngine;

        bool isDrawWireframe = true;
        bool isDrawAABB = false;
        bool isWireframe = false;
        bool isTree = true;
        bool isNPC = false;

        //VR
        int WIDTH_VR = 1920;
        int HEIGHT_VR = 986;

        std::function<void(int width, int height)> resize;
        std::function<void(int key, int scancode, int action, int mods)> keyboard;
        std::function<void(double xpos, double ypos)> mousePosition;
        std::function<void(double xoffset, double yoffset)> offset;
        std::function<void(int button, int action, int mods)> mouseButton;

	};
}
#endif