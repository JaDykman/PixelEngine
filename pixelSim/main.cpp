#include <SFML/Graphics.hpp>
#include <cmath>

// Constants and globals
const sf::Vector2f pixelSize(20.0f, 20.0f); //TODO this
const sf::Time coolDown = sf::milliseconds(100); //TODO drag and draw
const float gravity = 9.81f;
float deltaTime = 0.005667f;
bool buttonL;
bool buttonR;
struct GameObject{
    float px; float py;
    float vx; float vy;
    float ax; float ay;
    float w; float h;
    int id;
};

std::vector<GameObject> objects;
std::vector<std::pair<GameObject*, GameObject*>> vecCollidingPairs;
GameObject *pSelectedObject = nullptr;
void AddRect(float x, float y, float w = pixelSize.x, float h = pixelSize.y){
    GameObject b;
    b.px = x; b.py = y;
    b.w = w; b.h = h;
    b.vx = 0; b.vy = 0;
    b.ax = 0; b.ay = 0;
    b.id = objects.size();
    objects.emplace_back(b);
}

// Functions
void drawRectangleGameObject(GameObject& rect); // Draw a rect
void drawLineBetween(float x1, float y1, float x2, float y2, sf::Color); // Draw a line between two points
void snapGameObjectToGrid(GameObject& obj, float gridSize); //TODO Make this only happen if the object is not moving

// Helper Functions
void handleMouseButtonPressed(const sf::Event& event);
void handleMouseButtonReleased(const sf::Event& event);
bool DoRectanglesOverlap(float px1, float py1, float width1, float height1, float px2, float py2, float width2, float height2) {
    // Calculate the left, right, top, and bottom of each rectangle
    float left1 = px1 - width1 / 2.0f;
    float right1 = px1 + width1 / 2.0f;
    float top1 = py1 - height1 / 2.0f;
    float bottom1 = py1 + height1 / 2.0f;

    float left2 = px2 - width2 / 2.0f;
    float right2 = px2 + width2 / 2.0f;
    float top2 = py2 - height2 / 2.0f;
    float bottom2 = py2 + height2 / 2.0f;

    // Check for overlap
    return !(left1 > right2 || right1 < left2 || top1 > bottom2 || bottom1 < top2);
}

auto IsPointInRectangle = [](float rectX, float rectY, float width, float height, float px, float py) {
    // Calculate the left, right, top, and bottom of the rectangle
    float left = rectX - width / 2.0f;
    float right = rectX + width / 2.0f;
    float top = rectY - height / 2.0f;
    float bottom = rectY + height / 2.0f;

    // Check if the point is within the bounds of the rectangle
    return (px >= left && px <= right) && (py >= top && py <= bottom);
};
sf::RenderWindow window(sf::VideoMode(900, 500), "Pixel Engine"); // Create the game window


int main()
{
    for (int i = 0; i < 10; ++i) { // Draw some pixels randomly
        AddRect(rand() % window.getSize().x, rand() % window.getSize().y, 20);
    }
    while (window.isOpen()) {
        sf::Event event{};
        vecCollidingPairs.clear(); // Clear colliding objects vector every frame



        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::MouseButtonPressed) handleMouseButtonPressed(event);
            else if (event.type == sf::Event::MouseButtonReleased) handleMouseButtonReleased(event);
        }
        window.clear();

        if(buttonL){
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            pSelectedObject->px = mousePos.x;
            pSelectedObject->py = mousePos.y;
        }
        if(buttonR){
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            drawLineBetween(pSelectedObject->px, pSelectedObject->py, mousePos.x, mousePos.y, sf::Color::Cyan);
        }
        for(auto &ball : objects){
            ball.ax = -ball.vx * 0.3f;
            ball.ay = -ball.vy * 0.3f;
            ball.vx += ball.ax * deltaTime;
            ball.vy += ball.ay * deltaTime;
            ball.px += ball.vx * deltaTime;
            ball.py += ball.vy * deltaTime;

            if(std::abs(ball.vx * ball.vx + ball.vy * ball.vy) <0.01f){ // If the velocity is < 0.01f set it to 0
                ball.vx = 0;
                ball.vy = 0;
            }

        }
        for (auto &obj : objects) {
            for (auto &target : objects) {
                if (&obj != &target) { // Check if they are different objects
                    if (DoRectanglesOverlap(obj.px, obj.py, obj.w, obj.h,
                                            target.px, target.py, target.w, target.h)) { // Collision
                        vecCollidingPairs.emplace_back(&obj, &target);

                        // Calculate overlap in both axes
                        float overlapX, overlapY;

                        if (obj.px < target.px)
                            overlapX = (obj.px + obj.w) - target.px;
                        else
                            overlapX = (target.px + target.w) - obj.px;

                        if (obj.py < target.py)
                            overlapY = (obj.py + obj.h) - target.py;
                        else
                            overlapY = (target.py + target.h) - obj.py;

                        // Displace the rectangles based on the axis of minimum overlap
                        if (overlapX < overlapY) {
                            float displacementX = overlapX * 0.5f;
                            obj.px -= displacementX * (obj.px < target.px ? 1 : -1);
                            target.px += displacementX * (obj.px < target.px ? 1 : -1);
                        } else {
                            float displacementY = overlapY * 0.5f;
                            obj.py -= displacementY * (obj.py < target.py ? 1 : -1);
                            target.py += displacementY * (obj.py < target.py ? 1 : -1);
                        }

                    }
                }
            }
        }
        for(auto c :vecCollidingPairs){
            GameObject* obj1 = c.first;
            GameObject* obj2 = c.second;
            drawLineBetween(obj1->px, obj1->py, obj2->px, obj2->py, sf::Color::Red);
        }
        for(auto &ball : objects){
            drawRectangleGameObject(ball); //Draw each ball
        }
        window.display();
    }

}

void handleMouseButtonPressed(const sf::Event& event){
    if(event.mouseButton.button == sf::Mouse::Right){
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        pSelectedObject = nullptr;
        for(auto &obj : objects){
            if(IsPointInRectangle(obj.px, obj.py, obj.w, obj.h, mousePos.x, mousePos.y)){
                buttonR = true;
                pSelectedObject = &obj;
                break;
            }
        }
    }
    if(event.mouseButton.button == sf::Mouse::Left){
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        pSelectedObject = nullptr;
        for(auto &obj : objects){
            if(IsPointInRectangle(obj.px, obj.py, obj.w, obj.h, mousePos.x, mousePos.y)){
                buttonL = true;
                pSelectedObject = &obj;
                break;
            }
        }
    }
}
void handleMouseButtonReleased(const sf::Event& event){
    if(event.mouseButton.button == sf::Mouse::Left){
        buttonL = false;
        pSelectedObject = nullptr;
    }
    if(event.mouseButton.button == sf::Mouse::Right) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        if (pSelectedObject != nullptr) {
            buttonR = false;
            pSelectedObject->vx = 0.5f * ((pSelectedObject->px) - (float)mousePos.x);
            pSelectedObject->vy = 0.5f * ((pSelectedObject->py) - (float)mousePos.y);
        }

        pSelectedObject = nullptr;
    }
}
void drawRectangleGameObject(GameObject& rect){
    sf::RectangleShape rectangle;
    // Set the position considering px and py as the center
    rectangle.setPosition(rect.px - rect.w / 2.0f, rect.py - rect.h / 2.0f);
    rectangle.setSize(sf::Vector2f(rect.w, rect.h));
    window.draw(rectangle);
}

void drawLineBetween(float x1, float y1, float x2, float y2, sf::Color color){
    sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x1, y1), color),
            sf::Vertex(sf::Vector2f(x2, y2), color),
    };
    window.draw(line, 2, sf::Lines); // Draw the line
}
void snapGameObjectToGrid(GameObject& obj, float gridSize){
    // Adjust position to align to a grid
    float adjustedX = std::floor(obj.px / gridSize) * gridSize;
    float adjustedY = std::floor(obj.py / gridSize) * gridSize;
    obj.px = adjustedX;
    obj.py = adjustedY;
}

