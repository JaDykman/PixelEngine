// GameObject.h
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include <memory>

class GameObject : public sf::Drawable {
public:
    GameObject();
    virtual ~GameObject();

    virtual void update(const sf::Time& delta) = 0;

    // Static method to instantiate a GameObject.
    // Returns a unique_ptr for automatic memory management.
    template <typename GameObjectType, typename... Args>
    static std::unique_ptr<GameObjectType> Instantiate(Args&&... args) {
        return std::make_unique<GameObjectType>(std::forward<Args>(args)...);
    }

protected:
    sf::Vector2f position;
    sf::Sprite sprite;

    // This is the method from sf::Drawable that we must implement
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;

    // Other common attributes and methods for game objects...
};

#endif // GAMEOBJECT_H
