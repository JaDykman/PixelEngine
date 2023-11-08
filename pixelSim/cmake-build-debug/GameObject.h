#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include "Collider.h"

class GameObject {
public:
    GameObject(sf::Texture& texture, const sf::Vector2f& position, bool dynamic, bool absPos);

    void update(float deltaTime);

    void draw(sf::RenderWindow& window);

    void freeze();

    void alignToGrid();

    Collider& getCollider();

    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f velocity; // If object dynamic
    const bool dynamic;
    const bool absPos; // Snap object to grid
    sf::RectangleShape colliderBody; // This is the member you need for your collider
    Collider collider;
private:

};


#endif // GAMEOBJECT_H