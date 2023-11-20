// GuiClass.cpp

#include "GuiClass.h"

void GuiObject::handleEvent(sf::Event &event, const sf::RenderWindow &window) {

}

void GuiObject::draw(sf::RenderWindow &window) {

}

auto GuiObject::getValue() const {
    return nullptr;
}


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
void Slider::draw(sf::RenderWindow& window) {
    window.draw(bar);
    window.draw(handle);
}
float Slider::getValue() const {
    return currentValue;
}

Button::Button(float x, float y, float width, float height, std::string text, sf::Color color) {
    // Initialize the sprite
    sprite.setSize(sf::Vector2f(width, height));
    sprite.setPosition(x, y);
    sprite.setFillColor(color); // Adjusted alpha value for visibility
    sprite.setFillColor(color);
}
void Button::handleEvent(sf::Event &event, const sf::RenderWindow &window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (sprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                //TODO button state = true
            }
        }
    }


    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            //TODO button state = false
        }
    }
}
void Button::draw(sf::RenderWindow &window) {
    window.draw(sprite);
}
bool Button::getValue() const {
        return currentValue;
}

Line::Line(float x1, float y1, float x2, float y2, sf::RenderWindow &window, sf::Color color) {
    sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x1, y1), color),
            sf::Vertex(sf::Vector2f(x2, y2), color),
    };
    window.draw(line, 2, sf::Lines); // Draw the line
}