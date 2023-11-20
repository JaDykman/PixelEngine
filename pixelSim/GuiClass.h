//GuiClass.h
#ifndef GUICLASS_H
#define GUICLASS_H

#include <SFML/Graphics.hpp>

// Base class for GUI objects
class GuiObject {
public:
    virtual ~GuiObject() = default;

    // You can add common GUI object methods here if needed
    void handleEvent(sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    auto getValue() const;
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

class Button : public GuiObject{
public:
    Button(float x, float y, float width, float height, std::string text, sf::Color color);

    void handleEvent(sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    bool getValue() const;

private:
    sf::RectangleShape sprite;
    sf::String text;
    bool currentValue;
};

class Line : public GuiObject{
public:
    Line(float x1, float y1, float x2, float y2, sf::RenderWindow &window, sf::Color color);
    float x1 = x1, y1 = y1, x2 = x2, y2 = y2;
    sf::Color color = color;
    sf::RenderWindow& window = window;

};
#endif // GUICLASS_H
