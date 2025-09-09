#pragma once
#include "includes/object.hpp"
#include <vector>
class Sphere : public Object
{
private:
public:
    float radius;
    unsigned int sectors;
    unsigned int stacks;

    Sphere(glm::vec3 position, glm::vec3 velocity, float ms) : radius(1.0f), sectors(18), stacks(36)
    {
        this->position = position;
        this->velocity = velocity;
        mass = ms;
        generateVertices();
        initializeBuffers();
    }

    Sphere(glm::vec3 position, glm::vec3 velocity, float ms, float R, float G, float B) : radius(10000.0f), sectors(18), stacks(36)
    {
        this->position = position;
        this->velocity = velocity;
        mass = ms;
        this->R = R;
        this->G = G;
        this->B = B;
        generateVertices();
        initializeBuffers();
    }
    Sphere(glm::vec3 position, glm::vec3 velocity, float ms, float R, float G, float B, float radius) : sectors(18), stacks(36)
    {
        this->position = position;
        this->velocity = velocity;
        mass = ms;
        this->R = R;
        this->G = G;
        this->B = B;
        this->radius = radius;
        generateVertices();
        initializeBuffers();
    }

    void generateVertices() override
    {
        vertices.clear();
        indices.clear();
        for (int i = 0; i <= stacks; ++i) {
            float stackAngle = M_PI / 2 - i * M_PI / stacks;
            float xy = this->radius * cosf(stackAngle);
            float z = this->radius * sinf(stackAngle);
            
            for (int j = 0; j <= sectors; ++j) {
                float sectorAngle = j * 2 * M_PI / sectors;
                float x = xy * cosf(sectorAngle);
                float y = xy * sinf(sectorAngle);
                
                // Добавляем вершины через push_back()
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
                // Добавляем цвета
                vertices.push_back(this->R);
                vertices.push_back(this->G);
                vertices.push_back(this->B);
            }
        }
        for (int i = 0; i < stacks; ++i) {
            int k1 = i * (sectors + 1);
            int k2 = k1 + sectors + 1;
            
            for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
                if (i != 0) {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }
                
                if (i != (stacks - 1)) {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }
    }
    void checkForCollisionTwoObjects(Object &other) override
    {
        Sphere *otherSphere = dynamic_cast<Sphere *>(&other);
        if(!otherSphere) return;

        float minDistance = radius + otherSphere->radius;
        glm::vec3 direction = otherSphere->position - position;
        float distance = glm::length(direction);
        
        if(distance <= minDistance && distance > 0.0f)
        {
            // Нормаль столкновения
            glm::vec3 normal = glm::normalize(direction);
            
            // Разрешение пересечения - отталкиваем объекты
            float overlap = minDistance - distance;
            position -= normal * overlap * 0.5f;
            other.position += normal * overlap * 0.5f;

            // Правильный расчет импульсов (упругое столкновение)
            glm::vec3 relativeVelocity = velocity - other.velocity;
            float velocityAlongNormal = glm::dot(relativeVelocity, normal);
            
            // Если объекты удаляются друг от друга - пропускаем
            if (velocityAlongNormal > 0) return;

            // Коэффициент упругости (0.0 - неупругое, 1.0 - абсолютно упругое)
            float restitution = 0.8f;
            
            // Импульс столкновения
            float impulseScalar = -(1.0f + restitution) * velocityAlongNormal;
            impulseScalar /= (1.0f / mass + 1.0f / other.mass);
            
            glm::vec3 impulse = impulseScalar * normal;
            
            // Применяем импульс к обоим объектам
            velocity -= impulse / mass;
            other.velocity += impulse / other.mass;
        }
    }

    ~Sphere() {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &EBO);
    }
};