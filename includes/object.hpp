#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "includes/shader.hpp"


class Object
{
protected:
    GLuint VAO, VBO, EBO;
public:
    float mass;
    std::vector <float> vertices;
    std::vector <unsigned int> indices;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 totalForce;
    static int nextId;
    int id;
    float R = 1.0f, G = 0.0f, B = 0.0f;
    Object() 
    {
        acceleration = glm::vec3(1.0f);
        totalForce = glm::vec3(1.0f);
        id = nextId++;
    }
    void resetForce()
    {
        totalForce = glm::vec3(0.0f);
    }
    void updateVelocity(float deltaTime)
    {
        acceleration = totalForce / mass;
        velocity += acceleration * deltaTime;
    }
    void updatePosition(float deltaTime)
    {
        position += velocity * deltaTime;
    }
    virtual void generateVertices()
    {
        //...Определение в классе-наследнике
    }
    virtual void checkForCollisionTwoObjects(Object &other)
    {

    }
    void update(float deltaTime)
    {
        updateVelocity(deltaTime);
        updatePosition(deltaTime);
    }
    void draw(Shader &shader)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        shader.setMat4("model", model);
        glUniform3f(glGetUniformLocation(shader.ID, "objectColor"), R, G, B);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size()/6);;
    }
    virtual void edraw(Shader &shader)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        shader.setMat4("model", model);
        glUniform3f(glGetUniformLocation(shader.ID, "objectColor"), R, G, B);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
    void initializeBuffers()
    {
        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    }
    ~Object()
    {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &VAO);
        glDeleteBuffers(1, &EBO);
    }
};
int Object::nextId = 0;
