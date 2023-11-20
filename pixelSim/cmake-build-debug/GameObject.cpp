//
// Created by jacob on 11/20/2023.
//

#include <cmath>
#include <SFML/Graphics/RectangleShape.hpp>
#include "GameObject.h"
/* TODO
 Move the Physics Handling
 Move Collision Handling
 Move drawGhost
 Move Event Handling
*/

void GameObject::snapGameObjectToGrid(float gridX, float gridY) {
    // Adjust position to align to the nearest grid spot
    float adjustedX = std::round(this->px / gridX) * gridY;
    float adjustedY = std::round(this->py / gridX) * gridY;
    this->px = adjustedX;
    this->py = adjustedY;

}
void GameObject::drawRectangleGameObject(sf::RenderWindow& window){
    sf::RectangleShape rectangle;
    // Set the position considering px and py as the center
    rectangle.setPosition(this->px - this->w / 2.0f, this->py - this->h / 2.0f);
    rectangle.setSize(sf::Vector2f(this->w, this->h));
    rectangle.setFillColor(this->c);
    window.draw(rectangle);
}
void GameObject::AddRect(float x, float y, float w , float h, std::vector<GameObject*> &objects){
    auto* b = new GameObject;
    b->px = x; b->py = y;
    b->w = w; b->h = h;
    b->vx = 0; b->vy = 0;
    b->ax = 0; b->ay = 0;
    b->id = objects.size();
    objects.emplace_back(b);

}
void GameObject::drawGhost(sf::RenderWindow &window) {
    sf::RectangleShape ghost;
    float scaleFactor = 3.0f;
    float newWidth = this->w * scaleFactor;
    float newHeight = this->h * scaleFactor;
    float offsetX = this->w * (scaleFactor) / 2.0f;
    float offsetY = this->h * (scaleFactor) / 2.0f;
    ghost.setPosition(this->px - offsetX, this->py - offsetY);
    ghost.setSize(sf::Vector2f(newWidth, newHeight));
    sf::Color color = this->c;
    color.a = 10;
    ghost.setFillColor(color);
    window.draw(ghost);

}
void GameObject::handleCollision(GameObject* target) {
// Calculate overlap in both axes
    float overlapX, overlapY;

    if (this->px < target->px)
        overlapX = (this->px + this->w) - target->px;
    else
        overlapX = (target->px + target->w) - this->px;

    if (this->py < target->py)
        overlapY = (this->py + this->h) - target->py;
    else
        overlapY = (target->py + target->h) - this->py;

    // Displace the rectangles based on the axis of minimum overlap
    if (overlapX < overlapY) {
        float displacementX = overlapX * 0.5f;
        this->px -= displacementX * (this->px < target->px ? 1 : -1);
        target->px += displacementX * (this->px < target->px ? 1 : -1);
    } else {
        float displacementY = overlapY * 0.5f;
        this->py -= displacementY * (this->py < target->py ? 1 : -1);
        target->py += displacementY * (this->py < target->py ? 1 : -1);
    }
}
void GameObject::updatePhysics(float deltaTime, float gravity, sf::RenderWindow& window) {

    this->ax = -this->vx * 0.3f;
    // Check if the object is at the bottom and moving downwards
    if (this->py >= window.getSize().y - this->h/2 && this->vy > 0) {
        // Stop the object if it's at the bottom of the window
        this->vy = 0;
        this->py = window.getSize().y - this->h/2; // Adjust position to sit on the bottom
    } else {
        // Apply gravity and air resistance if not on the ground or moving upwards
        this->ay = gravity - this->vy * 0.3f;
    }
    this->vx += this->ax * deltaTime;
    this->vy += this->ay * deltaTime;
    this->px += this->vx * deltaTime;
    this->py += this->vy * deltaTime;

    if(std::abs(this->vx * this->vx + this->vy * this->vy) <0.01f){ // If the velocity is < 0.01f set it to 0
        this->vx = 0;
        this->vy = 0;
    }
}
bool GameObject::isStationary() const {
    if(this->vx ==0 && this->vy == 0){
        return true;
    }else{
        return false;
    }
}
