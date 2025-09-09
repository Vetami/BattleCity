#include "includes/object.hpp"
#include "includes/osphere.hpp"
#include <memory>
#include <vector>

class ObjectManager
{
private:
    float G = 0.000667f;
public:
    std::vector<std::unique_ptr<Object>> objects;  // Используем умные указатели

    // Правильное добавление объектов
    void addObject(std::unique_ptr<Object> object)
    {
        objects.push_back(std::move(object));
    }

    void applyGravitaionForces(float deltaTime)
    {
        // Сбрасываем силы
        for(auto &object : objects)
            object->resetForce();
        
        // Применяем гравитацию
        for(size_t i = 0; i < objects.size(); ++i)
        {
            for(size_t j = i + 1; j < objects.size(); ++j)
            {
                if(objects[i]->id != objects[j]->id)
                {
                    glm::vec3 direction = objects[j]->position - objects[i]->position;
                    float distance = glm::length(direction);
                    if(distance < 0.001f) continue;
                    
                    direction = glm::normalize(direction);
                    // ИСПРАВЛЕНО: правильная формула гравитации F = G * m1 * m2 / r^2
                    float forceMagnitude = G * objects[i]->mass * objects[j]->mass / (distance * distance);
                    
                    // Применяем силу к обоим объектам (3-й закон Ньютона)
                    objects[i]->totalForce += forceMagnitude * direction;
                    objects[j]->totalForce -= forceMagnitude * direction;
                }
            }
        }
        
        checkAllCollisions();
        
        for(auto &object : objects)
            object->update(deltaTime);
    }

    void drawAll(Shader &shader)
    {
        for(auto &object : objects)
        {
            object->edraw(shader);
        }
    }

    void checkAllCollisions()
    {
        for(size_t i = 0; i < objects.size(); ++i)
        {
            for(size_t j = i + 1; j < objects.size(); ++j)
            {
                objects[i]->checkForCollisionTwoObjects(*objects[j]);
            }
        }
    }
};