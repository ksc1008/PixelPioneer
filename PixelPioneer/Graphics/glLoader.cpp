#include <iostream>
#include "graphics.h"
#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Voxel/chunk.h"
#include <STB/stb_image.h>
#include "shaderLoader.h"
#include "../Voxel/texture.h"
#include "../Manager/controlManager.h"
#include "../World/world_generator.h"
#include "light.h"
#include "../Voxel/chunkLoader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
GLFWwindow* window = nullptr;
bool lineRendering = false;
bool _ao = true;

int initiateGL() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    stbi_set_flip_vertically_on_load(true);
    return 0;
}

ChunkLoader cl;

int openWindow()
{
    Light light = Light(DIRECTIONAL,glm::vec3(-1,-4,-2),0.9);
    int sz = 16;
    cl.setWorldSize(sz, 1, sz);
    cl.generateLargeChunk(0, 0, 0, sz);
    cl.updateChunks();
    VoxelTexture* vt = new VoxelTexture(*testManifest());
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
    glCullFace(GL_BACK);
    
    glActiveTexture(GL_TEXTURE0);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // input
        // -----

        // render
        // ------
        glClearColor(0.45f, 0.55f, 0.65f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ShaderLoader::getInstance()->getDefaultShader()->use();
        light.addToShader();
        vt->bindTextures();
        ControlManager::getInstance()->applyCameraToShader();
        cl.renderChunks(lineRendering ? GL_FRONT_AND_BACK : GL_FRONT_AND_BACK, lineRendering?GL_LINE: GL_FILL);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    static bool oneTime_1 = false;
    static bool oneTime_2 = false;
    static bool oneTime_3 = false;
    static float time = glfwGetTime();
    float dt = glfwGetTime()- time;
    time = glfwGetTime();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        ControlManager::getInstance()->move(-1, 0, 0, dt);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        ControlManager::getInstance()->move(1, 0, 0, dt);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        ControlManager::getInstance()->move(0, 0, 1, dt);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        ControlManager::getInstance()->move(0, 0, -1, dt);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        ControlManager::getInstance()->move(0, 1, 0, dt);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        ControlManager::getInstance()->move(0, -1, 0, dt);

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        if (!oneTime_1) {
            cl.switchMeshUpdateMode();
            oneTime_1 = true;
        }
    }
    else {
        oneTime_1 = false;
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        if (!oneTime_2) {
            lineRendering = !lineRendering;
            ShaderLoader::getInstance()->getDefaultShader()->setBool("solid", lineRendering);
            oneTime_2 = true;
        }
    }
    else {
        oneTime_2 = false;
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        if (!oneTime_3) {
            _ao = !_ao;
            cl.setAO(_ao);
            oneTime_3 = true;
        }
    }
    else {
        oneTime_3 = false;
    }
}

// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static bool firstMouse = true;
    static float lastX, lastY;
    if (firstMouse) {
        firstMouse = false;
        lastX = xpos; lastY = ypos;
    }

    ControlManager::getInstance()->look(xpos - lastX, lastY - ypos);
    lastX = xpos;
    lastY = ypos;
}
