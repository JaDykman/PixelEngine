//
// Created by jacob on 11/20/2023.
//

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H


#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class GameObject {

public:
    void snapGameObjectToGrid(float gridX, float gridY);
    void drawRectangleGameObject(sf::RenderWindow& window);
    void drawGhost(sf::RenderWindow& window);
    void handleCollision(GameObject* target);
    void updatePhysics(float deltaTime, float gravity, sf::RenderWindow& window);
    static void AddRect(float x, float y, float w, float h, std::vector<GameObject *> &objects);
    bool isStationary() const;

    float px; float py;
    float vx; float vy;
    float ax; float ay;
    float w; float h;
    unsigned long id;

    sf::Color c;
private:
};


#endif //GAMEOBJECT_H
