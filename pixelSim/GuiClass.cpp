// GuiClass.cpp

#include "GuiClass.h"

// Implementing Slider's constructor
Slider::Slider(float x, float y, float width, float height, float minVal, float maxVal, sf::Color color)
        : isHorizontal(width > height), minValue(minVal), maxValue(maxVal), currentValue(minVal) {
    // Initialize the slider bar
    bar.setSize(sf::Vector2f(width, height));
    bar.setPosition(x, y);
    bar.setFillColor(color); // Adjusted alpha value for visibility

    // Initialize the slider handle
    float handleSize = isHorizontal ? height : width;
    handle.setRadius(handleSize / 2);
    if (isHorizontal) {
        handle.setPosition(x, y + (height / 2) - handle.getRadius());
    } else {
        handle.setPosition(x + (width / 2) - handle.getRadius(), y);
    }
    handle.setFillColor(sf::Color::Red);
}

// Implementing Slider's handleEvent method
void Slider::handleEvent(sf::Event& event, const sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (handle.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                isDragging = true;
            }
        }
    }

    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            isDragging = false;
        }
    }

    if (event.type == sf::Event::MouseMoved) {
        if (isDragging) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (isHorizontal) {
                float newPos = std::max(static_cast<float>(mousePos.x), bar.getPosition().x);
                newPos = std::min(newPos, bar.getPosition().x + bar.getSize().x - handle.getRadius() * 2);
                handle.setPosition(newPos, handle.getPosition().y);

                // Update current value for horizontal slider
                currentValue = minValue + (newPos - bar.getPosition().x) / bar.getSize().x * (maxValue - minValue);
            } else {
                float newPos = std::max(static_cast<float>(mousePos.y), bar.getPosition().y);
                newPos = std::min(newPos, bar.getPosition().y + bar.getSize().y - handle.getRadius() * 2);
                handle.setPosition(handle.getPosition().x, newPos);

                // Update current value for vertical slider
                currentValue = minValue + (newPos - bar.getPosition().y) / bar.getSize().y * (maxValue - minValue);
            }
        }
    }
}

// Implementing Slider's draw method
void Slider::draw(sf::RenderWindow& window) {
    window.draw(bar);
    window.draw(handle);
}

// Implementing Slider's getValue method
float Slider::getValue() const {
    return currentValue;
}
