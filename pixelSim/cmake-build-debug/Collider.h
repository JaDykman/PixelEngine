//
// Created by jacob on 11/7/2023.
//

#ifndef COLLIDER_H
#define COLLIDER_H

#include <SFML/Graphics.hpp>

class Collider {
public:
    Collider(sf::RectangleShape& body);

    void update(const sf::Vector2f& position);
    bool checkCollision(const Collider& other) const;
    void printDebugInfo() const;
    bool contains(const sf::Vector2f &point) const;
private:
    sf::RectangleShape& body;

};

#endif // COLLIDER_H

