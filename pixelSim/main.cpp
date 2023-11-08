#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "cmake-build-debug/GameObject.h"

//Pixel Size
const sf::Vector2<float> pixelSize = sf::Vector2<float>(12,12);
const sf::Time coolDown = sf::milliseconds(100); // Cool down for drag and drawing
float deltaTime = 0.005667f; // DeltaTime (adjust as needed)
const float gravity = 9.81f; // Example gravity value (adjust as needed)


//List of all pixels
std::vector<GameObject> pixels;



int main()
{
    //Create the window
    sf::RenderWindow window(sf::VideoMode(900, 500), "Pixel Engine");

    // Create a 1x1 pixel white texture
    sf::Image whitePixel;
    whitePixel.create(pixelSize.x, pixelSize.y, sf::Color::White);

    // Save the texture to a file (optional)
    whitePixel.saveToFile("object_texture.png");

    // Load the texture for your GameObject (replace "object_texture.png" with your file path)
    sf::Texture objectTexture;
    if (!objectTexture.loadFromFile("object_texture.png")) {
        // Handle texture loading error
        return 1;
    }
    bool isDrawingR = false;
    bool isDrawingL = false;

    //Create a cool down for drag and draw
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Right) {
                        isDrawingR = true;
                    }
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        isDrawingL = true;
                    }
                    break;
                case sf::Event::MouseButtonReleased:
                    if (event.mouseButton.button == sf::Mouse::Right) {
                        isDrawingR = false;

                    }
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        isDrawingL = false;

                    }
                    break;

                default:
                    break;
            }
        }

        //Draw Objects
        if (isDrawingR && clock.getElapsedTime() >= coolDown) {
            // Get mouse position
            sf::Vector2i clickPosition = sf::Mouse::getPosition(window);
            // Place a pixel at the current mouse position
            GameObject gameObject(objectTexture, sf::Vector2f(clickPosition.x, clickPosition.y), false, true);
            //Add 'gameObject' to 'pixels' vector
            pixels.push_back(gameObject);

            clock.restart();
        }
        if (isDrawingL && clock.getElapsedTime() >= coolDown) {
            // Get mouse position
            sf::Vector2i clickPosition = sf::Mouse::getPosition(window);
            // Place a pixel at the current mouse position
            GameObject gameObject(objectTexture, sf::Vector2f(clickPosition.x, clickPosition.y), true, true);
            //Add 'gameObject' to 'pixels'
            pixels.push_back(gameObject);

            clock.restart();
        }

        window.clear();


        // for every 'pixel' in 'pixels'
        for (GameObject& pixel : pixels) {
            // Apply gravity if the space directly below the current pixel is not occupied
            if (pixel.position.y <= window.getSize().y - pixelSize.y ) {
                // Apply gravity only if the pixel is above the bottom of the window
                pixel.velocity += sf::Vector2f(0, gravity * deltaTime);
            }else{
                pixel.velocity = sf::Vector2f(0, 0);
            }

            pixel.update(deltaTime);
            pixel.draw(window);

        }
        window.display();
    }

}




