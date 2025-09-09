#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <includes/stb_image.h>

#include <glm/glm.hpp>


#include <filesystem>
#include <includes/shader.hpp>
#include <includes/camera.hpp>

#include <iostream>
#include "includes/omanager.hpp"
#include "includes/osphere.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 100.0f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	
float lastFrame = 0.0f;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }




    
    glEnable(GL_DEPTH_TEST);

    
    Shader ourShader("shaders/vShader.vs", "shaders/fShader.fs");
    

    ObjectManager OManager;

    Sphere sphere1(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.01f, 0.0f, 0.0f), 10.0f, 1.0f, 0.0f, 0.0f);
    Sphere sphere2(glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.01f, 0.1f, 0.0f), 3000.0f, 0.0f, 0.0f, 1.0f);

    OManager.addObject(std::make_unique<Sphere>(
        glm::vec3(0.0f, 0.0f, 20000.0f), 
        glm::vec3(-1.0f, 0.0f, 0.0f), 
        10000.0f, 0.7f, 0.7f, 0.7f, 1737.4f
    ));

    OManager.addObject(std::make_unique<Sphere>(
        glm::vec3(0.0f, 0.0f,   0.0f), 
        glm::vec3(1.0f, 0.0f, 0.0f), 
        810000.0f, 0.519f, 1.0f, 0.652f, 6366.0f
    ));

    OManager.addObject(std::make_unique<Sphere>(
        glm::vec3(-20.0f, 0.0f,   -200000.0f), 
        glm::vec3(1.0f, 0.0f, 1.0f), 
        8100000000.0f, 1.0f, 0.832f, 0.0f, 6366.0f
    ));
    OManager.addObject(std::make_unique<Sphere>(
        glm::vec3(20.0f, 0.0f,   200000.0f), 
        glm::vec3(-1.0f, 0.0f, -1.0f), 
        8100000000.0f, 1.0f, 0.832f, 0.0f, 6366.0f
    ));


    ourShader.use();





    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.101f, 0.101f, 0.101f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        ourShader.use();
        glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 1000000000.0f);
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", camera.GetViewMatrix());

        OManager.applyGravitaionForces(100.0f);
        OManager.drawAll(ourShader);

        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}