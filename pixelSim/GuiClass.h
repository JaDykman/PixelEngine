//GuiClass.h
#ifndef GUICLASS_H
#define GUICLASS_H

#include <SFML/Graphics.hpp>

// Base class for GUI objects
class GuiObject {
public:
    virtual ~GuiObject() = default;

    // You can add common GUI object methods here if needed
};

// Slider class derived from GuiObject

class Slider : public GuiObject {
public:
    Slider(float x, float y, float width, float height, float minVal, float maxVal, sf::Color color);

    void handleEvent(sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    float getValue() const;

private:
    sf::RectangleShape bar;
    sf::CircleShape handle;
    bool isDragging = false;
    bool isHorizontal;
    float minValue, maxValue, currentValue;
};

#endif // GUICLASS_H
