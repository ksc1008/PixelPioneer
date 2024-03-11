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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
GLFWwindow* window = nullptr;

int initiateGL() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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
    //ShaderLoader::getInstance()->getDefaultShader();
    return 0;
}

void test(Chunk& ch) {
    int n = 16;
    auto heightmap = WorldGenerator::createLandscape(n, 4);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k <= heightmap[i][j]; k++) {
                ch.setBlock(0, i, k, j);
            }
        }
        delete[] heightmap[i];
    }
    delete[] heightmap;  
}

void test2(Chunk** chunks, int w) {
    int n = w * 16;
    auto heightmap = WorldGenerator::createLandscape(n, 12);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k <= heightmap[i][j]; k++) {
                chunks[(i / 16) * w + (j / 16)]->setBlock(0, i%16, k, j%16);
            }
        }
        delete[] heightmap[i];
    }
    delete[] heightmap;    
}

void updateChunks(Chunk** chunks, int n) {
    for (int i = 0; i < n; i++)
        chunks[i]->update(0);
}

void renderChunks(Chunk** chunks, int n) {
    for (int i = 0; i < n; i++) {
        chunks[i]->bind();
        chunks[i]->render();
    }
}

int openWindow()
{
    Light light = Light(DIRECTIONAL,glm::vec3(-1,-1,-2),0.6);
    int chunk_count = 16;
    Chunk** chunks = new Chunk*[chunk_count* chunk_count];
    for (int i = 0; i < chunk_count * chunk_count; i++) {
        chunks[i] = new Chunk(i / chunk_count - chunk_count/2, 0, i % chunk_count - chunk_count / 2);
    }
    test2(chunks, chunk_count);
    VoxelTexture* vt = new VoxelTexture(*testManifest());
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    //ShaderLoader::getInstance()->getDefaultShader()->setInt("texture1", 0);
    updateChunks(chunks, chunk_count * chunk_count);
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
        //ShaderLoader::getInstance()->getDefaultShader()->setInt("texArray", 1);
        vt->bindTextures();
        ControlManager::getInstance()->applyCameraToShader();
        renderChunks(chunks, chunk_count * chunk_count);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete[] chunks;

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
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
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
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
