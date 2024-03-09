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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
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

    stbi_set_flip_vertically_on_load(true);
    //ShaderLoader::getInstance()->getDefaultShader();
    return 0;
}

int openWindow()
{
    Chunk* ch = new Chunk();
    ch->setBlock(0, 0, 0, 1);
    ch->setBlock(1, 0, 0, 1);
    ch->setBlock(1, 0, 0, 2);
    ch->setBlock(1, 0, 1, 2);
    ch->setBlock(1, 2, 0, 0);
    VoxelTexture* vt = new VoxelTexture(*testManifest());
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    //ShaderLoader::getInstance()->getDefaultShader()->setInt("texture1", 0);
    ch->update(0);
    glActiveTexture(GL_TEXTURE0);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // input
        // -----

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ShaderLoader::getInstance()->getDefaultShader()->use();
        ShaderLoader::getInstance()->getDefaultShader()->setViewTransform(glm::lookAt(glm::vec3(5, 4, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
        ShaderLoader::getInstance()->getDefaultShader()->setProjTransform(
            glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f));
        //ShaderLoader::getInstance()->getDefaultShader()->setInt("texArray", 1);
        ch->bind();
        vt->bindTextures();
        ch->render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete ch;

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}