#include <SFML/Graphics.hpp>
#include <cmath>
#include "GuiClass.h"
#include "cmake-build-debug/GameObject.h"
#include <iostream>
#include <memory>

// Constants and globals
const sf::Vector2f pixelSize(12.0f, 12.0f); //TODO this
float gravity = 9.81f;
float deltaTime = 0.015667f;
bool buttonL;
bool buttonR;
GameObject *pSelectedObject = nullptr;

//Vectors
std::vector<GameObject*> objects;
std::vector<std::pair<GameObject*, GameObject*>> vecCollidingPairs;
std::vector<Slider*> GuiObjects;

//TODO Move to class
bool DoRectanglesOverlap(GameObject *obj, GameObject *target) {
    // Calculate the left, right, top, and bottom of each rectangle
    float left1 = obj->px - obj->w / 2.0f;
    float right1 = obj->px + obj->w / 2.0f;
    float top1 = obj->py - obj->h / 2.0f;
    float bottom1 = obj->py + obj->h / 2.0f;

    float left2 = target->px - target->w / 2.0f;
    float right2 = target->px + target->w / 2.0f;
    float top2 = target->py - target->h / 2.0f;
    float bottom2 = target->py + target->h / 2.0f;

    // Check for overlap
    return !(left1 > right2 || right1 < left2 || top1 > bottom2 || bottom1 < top2);
}

void handleMouseButtonPressed(const sf::Event& event);
void handleMouseButtonReleased(const sf::Event& event);
bool doGhostsOverlap(GameObject *obj, GameObject *target);
auto IsPointInRectangle = [](float rectX, float rectY, float width, float height, float px, float py) {
    // Calculate the left, right, top, and bottom of the rectangle
    float left = rectX - width / 2.0f;
    float right = rectX + width / 2.0f;
    float top = rectY - height / 2.0f;
    float bottom = rectY + height / 2.0f;

    // Check if the point is within the bounds of the rectangle
    return (px >= left && px <= right) && (py >= top && py <= bottom);
};
/*bool isGroundOrObjectBelow(GameObject& obj) {
    // Example check for ground or other objects
    // This needs to be tailored to your game's logic
    for (const GameObject other : objects) {
        if (other.id != obj.id) {
            // Adjust these conditions based on your game's collision logic
            if (obj.px < other.px + other.w &&
                obj.px + obj.w > other.px &&
                obj.py < other.py + other.h &&
                obj.h + obj.py > other.py) {
                return true; // There's an object below
            }
        }
    }
    return false; // Nothing below
}*/
bool isInCollidingPairs(GameObject *obj) { // Function to check if an object is in the vecCollidingPairs
    for (const auto& pair : vecCollidingPairs) {
        if (pair.first == obj || pair.second == obj) {
            return true;
        }
    }
    return false;
}

sf::RenderWindow window(sf::VideoMode(900, 500), "Pixel Engine"); // Create the game window

int main()
{
    Slider gravitySlider(0, 0, 10, 100, gravity, -gravity, sf::Color(255, 255, 255, 1));
    GuiObjects.push_back(&gravitySlider);
    Slider timeSlider(10, 0, 10, 100, deltaTime, 0, sf::Color(255, 255, 255, 1));
    GuiObjects.push_back(&timeSlider);

    for (int i = 0; i < 200; ++i) { // Draw some pixels randomly
        float adjustedX = rand() % (int)std::round(window.getSize().x / pixelSize.x) * pixelSize.x;
        float adjustedY = rand() % (int) std::round(window.getSize().y / pixelSize.y) * pixelSize.y;
        GameObject::AddRect(adjustedX, adjustedY, pixelSize.x, pixelSize.y, objects);
    }

    while (window.isOpen()) {
        sf::Event event{};
        vecCollidingPairs.clear(); // Clear colliding objects vector every frame

        while (window.pollEvent(event)) {
            for (auto& guiObj : GuiObjects){
                guiObj->handleEvent(event, window);
            }
            gravity = gravitySlider.getValue();
            deltaTime = timeSlider.getValue();

            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::MouseButtonPressed) handleMouseButtonPressed(event);
            else if (event.type == sf::Event::MouseButtonReleased) handleMouseButtonReleased(event);
        }
        window.clear();
        if (buttonL) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            pSelectedObject->px = mousePos.x;
            pSelectedObject->py = mousePos.y;
        }
        if (buttonR) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            Line line(pSelectedObject->px, pSelectedObject->py, mousePos.x, mousePos.y, window, sf::Color::Blue);
        }


        // Update Physics and Check Collisions
        for (size_t i = 0; i < objects.size(); i++) {
            auto &obj = objects[i];
            obj->c = sf::Color::White;
            obj->updatePhysics(deltaTime, gravity, window);


            for (auto &target : objects) {
                if (obj != target && DoRectanglesOverlap(obj, target)) {
                    // Handle collision
                    vecCollidingPairs.emplace_back(obj, target);
                    obj->handleCollision(target);
                }
            }
            // Ghost Overlap Check
            for (auto &target : objects) {
                if (obj != target && doGhostsOverlap(obj, target)) {
                    obj->c = sf::Color::Red;
                }
            }
        }

        // Snap to Grid and Draw
        for (auto &obj : objects) {
            if (obj->isStationary()) {
                obj->snapGameObjectToGrid(pixelSize.x, pixelSize.y);
            }
            // Draw the object and its ghost
            obj->drawRectangleGameObject(window);
            obj->drawGhost(window);
        }

        // Draw GUI Objects
        for (auto& guiObj : GuiObjects) {
            guiObj->draw(window);
        }

        window.display();
    }
}

void handleMouseButtonPressed(const sf::Event& event){
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    if(event.mouseButton.button == sf::Mouse::Right){
        if (!buttonL) { // Only proceed if left button is not pressed
            pSelectedObject = nullptr;
            for(auto &obj : objects){
                if(IsPointInRectangle(obj->px, obj->py, obj->w, obj->h, mousePos.x, mousePos.y)){
                    buttonR = true;
                    pSelectedObject = obj;
                    break;
                }
            }
        }
    }
    else if(event.mouseButton.button == sf::Mouse::Left){
        if (!buttonR) { // Only proceed if right button is not pressed
            pSelectedObject = nullptr;
            for(auto &obj : objects){
                if(IsPointInRectangle(obj->px, obj->py, obj->w, obj->h, mousePos.x, mousePos.y)){
                    buttonL = true;
                    pSelectedObject = obj;
                    break;
                }
            }
        }
    }
}
void handleMouseButtonReleased(const sf::Event& event){
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    if(event.mouseButton.button == sf::Mouse::Left){
        buttonL = false;
    }
    if(event.mouseButton.button == sf::Mouse::Right) {
        buttonR = false;
        if (pSelectedObject != nullptr) {
            pSelectedObject->vx = 0.5f * ((pSelectedObject->px) - (float)mousePos.x);
            pSelectedObject->vy = 0.5f * ((pSelectedObject->py) - (float)mousePos.y);
        }
    }

    // Only set to nullptr if both buttons are not pressed
    if (!buttonL && !buttonR) {
        pSelectedObject = nullptr;
    }
}

bool doGhostsOverlap(GameObject *obj, GameObject *target){
    float checkWidth = obj->w * 2.99;
    float checkHeight = obj->h * 2.99;
    float left1 = obj->px - checkWidth / 2.f;
    float right1 = obj->px + checkWidth / 2.f;
    float top1 = obj->py - checkHeight / 2.f;
    float  bottom1 = obj->py + checkHeight / 2.f;

    float left2 = target->px - checkWidth / 2.f;
    float right2 = target->px + checkWidth / 2.f;
    float top2 = target->py - checkHeight / 2.f;
    float  bottom2 = target->py + checkHeight / 2.f;
    return !(left1 > right2 || right1 < left2 || top1 > bottom2 || bottom1 < top2);
}
