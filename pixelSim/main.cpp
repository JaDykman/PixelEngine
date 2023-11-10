#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include "cmake-build-debug/GameObject.h"

// Constants and globals
const sf::Vector2f pixelSize(12.0f, 12.0f);
const sf::Time coolDown = sf::milliseconds(100);
const float gravity = 9.81f;
float deltaTime = 0.0005667f;
bool isDrawingR = false;
bool isDrawingL = false;
std::vector<std::vector<GameObject*>> tiles;

// Function declarations
void updatePixelPosInTiles(GameObject*& gameObject, sf::Vector2f newPosition);
void checkBelowAndApplyGravity(GameObject* obj);

// Helper functions
void handleMouseButtonPressed(const sf::Event& event);
void handleMouseButtonReleased(const sf::Event& event);
void createGameObjectAtMousePosition(const sf::RenderWindow& window, bool dynamic);
void drawGameObjects(sf::RenderWindow& window);

sf::RenderWindow window(sf::VideoMode(900, 500), "Pixel Engine");

// Create a 1x1 pixel white texture
sf::Image whitePixel;
// Create the texture for your GameObject from the sf::Image
sf::Texture objectTexture;

int main()
{

    const int Screen_Width = window.getSize().x;
    const int Screen_Height = window.getSize().y;
    const int numTilesX = Screen_Width / static_cast<int>(pixelSize.x);
    const int numTilesY = Screen_Height / static_cast<int>(pixelSize.y);

    // Resize the tiles grid to fit the window
    tiles.resize(numTilesY, std::vector<GameObject*>(numTilesX, nullptr));

    //Load pixel texture
    whitePixel.create(pixelSize.x, pixelSize.y, sf::Color::White);
    objectTexture.loadFromImage(whitePixel);


    //Create a clock for drag and draw
    sf::Clock dragClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::MouseButtonPressed) handleMouseButtonPressed(event);
            else if (event.type == sf::Event::MouseButtonReleased) handleMouseButtonReleased(event);
        }

        // Logic for drawing on right click
        if (isDrawingR && dragClock.getElapsedTime() >= coolDown) {
            createGameObjectAtMousePosition(window, true);
            dragClock.restart();
        }

        // Logic for drawing on left click
        if (isDrawingL && dragClock.getElapsedTime() >= coolDown) {
            createGameObjectAtMousePosition(window, false);
            dragClock.restart();
        }
        window.clear();

        // Draw game objects
        for (auto& row : tiles) {
            for (auto& obj : row) {
                if (obj) {
                    if(obj->dynamic){
                        checkBelowAndApplyGravity(obj);
                    }
                    obj->update(deltaTime);
                    obj->draw(window);
                    window.draw(obj->sprite);
                }
            }
        }
        //Draw shit between these two
        window.display();
    }

}


const int numTilesX = window.getSize().x / pixelSize.x;
const int numTilesY = window.getSize().y / pixelSize.y;

void updatePixelPosInTiles(GameObject*& gameObject, sf::Vector2f newPosition) {
    //TODO fix objects not being cleared when they move

    // Translate old position to tile indices
    int oldTileX = static_cast<int>(gameObject->position.x / pixelSize.x);
    int oldTileY = static_cast<int>(gameObject->position.y / pixelSize.y);

    // Ensure the old position indices are within the grid bounds
    if (oldTileX >= 0 && oldTileX < numTilesX && oldTileY >= 0 && oldTileY < numTilesY) {
        // Remove the gameObject from its old position in the grid
        tiles[oldTileY][oldTileX] = nullptr;
    }

    // Translate new position to tile indices
    int newTileX = static_cast<int>(newPosition.x / pixelSize.x);
    int newTileY = static_cast<int>(newPosition.y / pixelSize.y);

    // Ensure the new position indices are within the grid bounds
    if (newTileX >= 0 && newTileX < numTilesX && newTileY >= 0 && newTileY < numTilesY) {
        // Move the gameObject to its new position in the grid
        tiles[newTileY][newTileX] = gameObject;
        // Update the gameObject's stored position
        gameObject->position = newPosition;
    }
}

void checkBelowAndApplyGravity(GameObject* obj) {

    // Calculate the tile indices
    int tileX = static_cast<int>(obj->position.x / pixelSize.x);
    int tileY = static_cast<int>(obj->position.y / pixelSize.y);

    // Apply the velocity to the GameObject's position to predict where it will be
    sf::Vector2f predictedPosition = obj->position + obj->velocity * deltaTime;

    // Convert predicted position to tile indices
    int predictedTileX = static_cast<int>(predictedPosition.x / pixelSize.x);
    int predictedTileY = static_cast<int>(predictedPosition.y / pixelSize.y);

    // Check bounds to make sure we're not going to go below the bottom edge
    if(predictedTileY + 1 < numTilesY) {
        // Check the tile below the current GameObject
        if(tiles[predictedTileY + 1][predictedTileX] == nullptr) {
            // If the tile below is empty, apply gravity
            obj->velocity.y += gravity * deltaTime;
        } else {
            // If there is something in the tile below, stop the GameObject's vertical movement
            obj->velocity.y = 0;
            // Adjust position to sit on top of the tile below
            obj->position.y = tileY * pixelSize.y;
        }
    } else {
        // If we are at the bottom edge, stop the GameObject's vertical movement
        obj->velocity.y = 0;
    }

    // Update the GameObject's position with the new velocity
    obj->position += obj->velocity * deltaTime;
    updatePixelPosInTiles(obj, obj->position);
    // Additional bounds check for left and right sides
    if (predictedTileX < 0 || predictedTileX >= numTilesX) {
        obj->velocity.x = 0; // Stop the object if it's going out of bounds on the sides
    }
}
void handleMouseButtonPressed(const sf::Event& event){
    createGameObjectAtMousePosition(window, true);
}
void handleMouseButtonReleased(const sf::Event& event){

}
void createGameObjectAtMousePosition(const sf::RenderWindow& window, bool dynamic){
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    // Convert pixel position to tile indices, ensuring integer division
    int tileX = pixelPos.x / pixelSize.x;
    int tileY = pixelPos.y / pixelSize.y;

    // Ensure we don't go out of bounds
    if (tileX >= 0 && tileX < numTilesX && tileY >= 0 && tileY < numTilesY) {
        // Properly deallocate the previous GameObject if it exists
        delete tiles[tileY][tileX];  // If you later switch to smart pointers, remove this line

        // Create a new GameObject aligned to the top-left corner of the tile
        sf::Vector2f objectPos(tileX * pixelSize.x, tileY * pixelSize.y);
        tiles[tileY][tileX] = new GameObject(objectTexture, objectPos, true, true);
    }
}
