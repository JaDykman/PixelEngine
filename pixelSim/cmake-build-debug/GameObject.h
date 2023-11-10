#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Graphics.hpp>

class GameObject {
public:
    GameObject(sf::Texture& texture, sf::Vector2<float> position, bool dynamic, bool absPos);

    void update(float deltaTime);

    void draw(sf::RenderWindow& window);

    void freeze();

    void alignToGrid();

    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f velocity; // If object dynamic
    const bool dynamic;
    const bool absPos; // Snap object to grid
    sf::RectangleShape colliderBody; // This is the member you need for your collider
private:

};


#endif // GAMEOBJECT_H