//
// Created by jacob on 11/7/2023.
//

#include <iostream>
#include "Collider.h"

Collider::Collider(sf::RectangleShape& body)
        : body(body) {}

void Collider::update(const sf::Vector2f& position) {
    body.setPosition(position);
}

bool Collider::checkCollision(const Collider& other) const {
    return body.getGlobalBounds().intersects(other.body.getGlobalBounds());
}

void Collider::printDebugInfo() const {
    auto position = body.getPosition();
    auto size = body.getSize();

    std::cout << "Collider Position: (" << position.x << ", " << position.y << ")\n";
    std::cout << "Collider Size: (" << size.x << ", " << size.y << ")\n";
}

bool Collider::contains(const sf::Vector2f& point) const {
    return body.getGlobalBounds().contains(point.x, point.y);
}