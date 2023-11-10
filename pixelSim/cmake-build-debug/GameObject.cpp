// GameObject.cpp
#include <cmath>
#include "GameObject.h"

GameObject::GameObject(sf::Texture& texture, sf::Vector2<float> position, bool dynamic, bool absPos)
        : sprite(texture), position(position), dynamic(dynamic), absPos(absPos), velocity(sf::Vector2f (0.f,0.f)){


    sprite.setPosition(position);

    colliderBody.setSize(sf::Vector2f(sprite.getLocalBounds().width, sprite.getLocalBounds().height));
    colliderBody.setPosition(position);
    // You may also want to make the colliderBody invisible if it is just for collision detection
    colliderBody.setFillColor(sf::Color::Transparent);
}



// Update method
void GameObject::update(float deltaTime) {
    // Implement object-specific logic here
    // For example, update position based on velocity and gravity
    // Handle any other behavior or animations

    if(dynamic){
        // Gravity Physics
        position += velocity * deltaTime;
        if(absPos){
            alignToGrid();
        } else {
            sprite.setPosition(position);
        }
    }
}

// Draw method
void GameObject::draw(sf::RenderWindow& window) {
    if(absPos){
        // Adjust position to align to a pixel grid if necessary
        alignToGrid();
    }
    window.draw(sprite);
}

void GameObject::freeze() {
    velocity = sf::Vector2f (0,0);
}

void GameObject::alignToGrid() {
    // Adjust position to align to a pixel grid if necessary
    float adjustedX = std::floor(position.x / sprite.getTexture()->getSize().x) * sprite.getTexture()->getSize().x;
    float adjustedY = std::floor(position.y / sprite.getTexture()->getSize().y) * sprite.getTexture()->getSize().y;
    sprite.setPosition(adjustedX, adjustedY);
}

