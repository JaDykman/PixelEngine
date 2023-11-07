#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include "cmake-build-debug/GameObject.h"

//Pixel Size
const sf::Vector2<float> pixelSize = sf::Vector2<float>(12,12);
const float m_gravity = .1f;

//List of all pixels
std::vector<GameObject> pixels;

class WaterObject : public GameObject {
public:
    WaterObject(float xPos, float yPos, sf::Vector2<float> vel);
    // Other members and methods specific to WaterObject

    // Concrete implementation of the draw method
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        // Drawing logic for WaterObject
    }

    // Concrete implementation of the update method
    void update(const sf::Time& delta) override {
        // Update logic for WaterObject
        xPos += vel.x * delta.asSeconds();
        yPos += vel.y * delta.asSeconds();
        // Adjust position to align to a pixel grid if necessary
        float adjustedX = std::floor(xPos / pixelSize.x) * pixelSize.x;
        float adjustedY = std::floor(yPos / pixelSize.y) * pixelSize.y;
        shape.setPosition(adjustedX, adjustedY);
    }
private:
    float xPos, yPos;
    sf::Vector2f vel; // Velocity
    sf::Vector2f pixelSize; // Size of the pixel grid
    sf::RectangleShape shape; // Graphical representation
};

WaterObject::WaterObject(float x, float y, sf::Vector2<float> vel) {

}


int main()
{
    //Create the window
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML Application");

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Check if the left mouse button was pressed
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                // Get mouse position
                sf::Vector2i clickPosition = sf::Mouse::getPosition(window);
                // Place a pixel at the current mouse position
                GameObject::Instantiate<WaterObject>(clickPosition.x, clickPosition.y, sf::Vector2f(0,m_gravity));
                //createPixel(clickPosition.x, clickPosition.y);
            }
            // Check if the right mouse button was pressed
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
                // Get mouse position
                sf::Vector2i clickPosition = sf::Mouse::getPosition(window);
                // Delete the pixel at the current mouse position
                //deletePixel(clickPosition.x, clickPosition.y);
            }

        }

        window.clear();

        // for every 'pixel' in 'pixels'
        for (GameObject& pixel : pixels) {
            // Update each pixel with gravity and then draw it
            //pixel.update(sf::Vector2f (0, m_gravity));
            //window.draw(pixel);
        }

        window.display();
    }
}

