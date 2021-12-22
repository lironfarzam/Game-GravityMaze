#pragma once

#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include <string>
#include "WorldObject.h"

// a factory to create game objects. Each object class must initialize
// itself to register to the factory
class GameObjectFactory {
public:
    using pFnc = std::shared_ptr<WorldObject>(*)(b2World& world, float x, float y, float w, float h);
    static std::shared_ptr<WorldObject> create(const std::string& name, b2World& world, float x, float y, float w, float h);
    static bool registerit(const std::string& name, pFnc);
private:
    static auto& getMap() {
        static std::map<std::string, pFnc> map;
        return map;
    }
};
