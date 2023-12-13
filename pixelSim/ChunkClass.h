//
// Created by jacob on 12/4/2023.
//

#ifndef CHUNKCLASS_H
#define CHUNKCLASS_H

#include <vector>
#include "cmake-build-debug/GameObject.h"

class ChunkClass {
};


class Chunk : public ChunkClass{
public:
    void updateObjectsWithin(float deltaTime, float gravity, sf::RenderWindow window);
    std::vector<GameObject*> objectsWithin;
    const sf::Vector2f chunkSize();
};

#endif //CHUNKCLASS_H
