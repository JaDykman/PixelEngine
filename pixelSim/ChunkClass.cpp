//
// Created by jacob on 12/4/2023.
//

#include "ChunkClass.h"

void Chunk::updateObjectsWithin(float deltaTime, float gravity, sf::RenderWindow window) {
    for(GameObject* obj : objectsWithin){
        obj->updatePhysics(deltaTime, gravity, window);
    }
}
