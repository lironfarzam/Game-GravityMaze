
#include "GameObjectFactory.h"

std::shared_ptr<WorldObject> GameObjectFactory::create(const std::string& name, b2World& world, float x, float y, float w, float h) {
    auto it = GameObjectFactory::getMap().find(name);
    if (it == GameObjectFactory::getMap().end())
        return nullptr;
    return it->second(world,x,y,w,h);
}

bool GameObjectFactory::registerit(const std::string& name, pFnc f) {
    GameObjectFactory::getMap().emplace(name, f);
    return true;
}
