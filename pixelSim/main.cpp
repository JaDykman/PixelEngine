#include <SFML/Graphics.hpp>
#include <cmath>

// Constants and globals
const sf::Vector2f pixelSize(12.0f, 12.0f); //TODO this
const float gravity = 9.81f;
float deltaTime = 0.005667f;
bool buttonL;
bool buttonR;
struct GameObject{
    float px; float py;
    float vx; float vy;
    float ax; float ay;
    float w; float h;
    sf::Color c;
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
void drawRectangleGameObject(GameObject& rect, sf::Color color); // Draw a rect
void drawLineBetween(float x1, float y1, float x2, float y2, sf::Color); // Draw a line between two points
void snapGameObjectToGrid(GameObject& obj); //TODO Make this only happen if the object is not moving

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
bool doGhostsOverlap(float px, float py, float checkWidth, float checkHeight, float targetX, float targetY);
auto IsPointInRectangle = [](float rectX, float rectY, float width, float height, float px, float py) {
    // Calculate the left, right, top, and bottom of the rectangle
    float left = rectX - width / 2.0f;
    float right = rectX + width / 2.0f;
    float top = rectY - height / 2.0f;
    float bottom = rectY + height / 2.0f;

    // Check if the point is within the bounds of the rectangle
    return (px >= left && px <= right) && (py >= top && py <= bottom);
};
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
    for (int i = 0; i < 50; ++i) { // Draw some pixels randomly
        float adjustedX = rand() % (int)std::round(window.getSize().x / pixelSize.x) * pixelSize.x;
        float adjustedY = rand() % (int) std::round(window.getSize().y / pixelSize.y) * pixelSize.y;
        AddRect(adjustedX, adjustedY);
    }
    for(auto obj : objects){
        snapGameObjectToGrid(obj);
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
        if (buttonL) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            pSelectedObject->px = mousePos.x;
            pSelectedObject->py = mousePos.y;
        }
        if (buttonR) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            drawLineBetween(pSelectedObject->px, pSelectedObject->py, mousePos.x, mousePos.y, sf::Color::Cyan);
        }
        // Physics
        for (auto &obj : objects) {
            obj.ax = -obj.vx * 0.3f;
            if (obj.py < window.getSize().y - pixelSize.y) {
                obj.ay = gravity - obj.vy * 0.3f; // Include gravity
            }else{
                // Stop the object if it's at the bottom of the window
                obj.vy = 0;
                obj.py = window.getSize().y - pixelSize.y/2; // Adjust position to sit on the bottom
            }
            obj.vx += obj.ax * deltaTime;
            obj.vy += obj.ay * deltaTime;
            obj.px += obj.vx * deltaTime;
            obj.py += obj.vy * deltaTime;

            if(std::abs(obj.vx * obj.vx + obj.vy * obj.vy) <0.001f){ // If the velocity is < 0.01f set it to 0
                obj.vx = 0;
                obj.vy = 0;
            }

        }
        for (auto &obj : objects) {
            for (auto &target : objects) {
                if (&obj != &target) { // Check if they are different objects
                    if (DoRectanglesOverlap(obj.px, obj.py, obj.w, obj.h,target.px, target.py, target.w, target.h)) { // Collision
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
                    if(doGhostsOverlap(obj.px, obj.py, obj.w *3, obj.h *3,target.px, target.py)){
                        obj.c = sf::Color::Red;
                    }
                    else{
                        obj.c = sf::Color::White;
                    }
                }
            }
        }
        for (auto &obj : objects) {
            obj.c = sf::Color::White;  // Reset color to white at the start

            for (auto &target : objects) {
                if (&obj != &target) { // Check if they are different objects
                    if (doGhostsOverlap(obj.px, obj.py, obj.w * 2.99, obj.h * 2.99, target.px, target.py)) {
                        obj.c = sf::Color::Red;  // Set color to red if overlapping
                        break;  // Break out of the loop after finding the first overlap
                    }
                }
            }
        }
        for (auto &obj : objects) {
            sf::RectangleShape ghost;
            float scaleFactor = 3.0f;
            float newWidth = obj.w * scaleFactor;
            float newHeight = obj.h * scaleFactor;
            float offsetX = obj.w * (scaleFactor) / 2.0f;
            float offsetY = obj.h * (scaleFactor) / 2.0f;
            ghost.setPosition(obj.px - offsetX, obj.py - offsetY);
            ghost.setSize(sf::Vector2f(newWidth, newHeight));
            ghost.setFillColor(sf::Color(200, 200, 200, 20));


            // Draw the ghost
            window.draw(ghost);
            drawRectangleGameObject(obj, obj.c); //Draw each
        }

        for (auto c :vecCollidingPairs){
            GameObject* obj1 = c.first;
            GameObject* obj2 = c.second;
            drawLineBetween(obj1->px, obj1->py, obj2->px, obj2->py, sf::Color::Red);
        }
        for (GameObject& obj : objects) {
            // Check if the object is not moving
            if (obj.vx == 0 && obj.vy == 0) {
                snapGameObjectToGrid(obj);
            }
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
void drawRectangleGameObject(GameObject& rect, sf::Color color){
    sf::RectangleShape rectangle;
    // Set the position considering px and py as the center
    rectangle.setPosition(rect.px - rect.w / 2.0f, rect.py - rect.h / 2.0f);
    rectangle.setSize(sf::Vector2f(rect.w, rect.h));
    rectangle.setFillColor(color);
    window.draw(rectangle);
}
void drawLineBetween(float x1, float y1, float x2, float y2, sf::Color color){
    sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x1, y1), color),
            sf::Vertex(sf::Vector2f(x2, y2), color),
    };
    window.draw(line, 2, sf::Lines); // Draw the line
}
void snapGameObjectToGrid(GameObject& obj) {
    // Adjust position to align to the nearest grid spot
    float adjustedX = std::round(obj.px / pixelSize.x) * pixelSize.x;
    float adjustedY = std::round(obj.py / pixelSize.y) * pixelSize.y;
    obj.px = adjustedX;
    obj.py = adjustedY;

}
bool doGhostsOverlap(float px, float py, float checkWidth, float checkHeight, float targetX, float targetY){
    float left1 = px - checkWidth / 2.f;
    float right1 = px + checkWidth / 2.f;
    float top1 = py - checkHeight / 2.f;
    float  bottom1 = py + checkHeight / 2.f;

    float left2 = targetX - checkWidth / 2.f;
    float right2 = targetX + checkWidth / 2.f;
    float top2 = targetY - checkHeight / 2.f;
    float  bottom2 = targetY + checkHeight / 2.f;
    return !(left1 > right2 || right1 < left2 || top1 > bottom2 || bottom1 < top2);
}
