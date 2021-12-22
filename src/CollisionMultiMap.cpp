
#include "CollisionMultiMap.h"
#include "Player.h"
#include "Door.h"
#include "Meteor.h"
#include "Springboard.h"
#include "LaserBeam.h"
#include <iostream>
#include <map>
#include <string>
#include <typeinfo>
#include <typeindex>

namespace { // anonymous namespace — the standard way to make function "static"

    void PlayerPlatform(WorldObject* player, WorldObject* platform)
    {
        auto p = static_cast<Player*>(player);
        auto o = static_cast<Platform*>(platform);
        player->setNumContacts(player->getNumContacts() + 1);
        platform->setNumContacts(platform->getNumContacts() + 1);

        // set player platform
        if (player->getPhysicalPosition().y < platform->getPhysicalPosition().y) {
            player->setCollisionWith(platform);
            platform->setCollisionWith(player);
            p->stop();
        }
    }

    void PlatformPlayer(WorldObject* platform, WorldObject* player)
    {
        PlayerPlatform(player, platform);
    }

    void PlatformPlatform(WorldObject* platform, WorldObject* player)
    {
    }

    void PlayerDoor(WorldObject* player, WorldObject* door)
    {
        auto p = static_cast<Player*>(player);
        p->setLevelComplete(true);
        Resources::instance().playSound(LEVELCOMPLETE_SOUND, NO_LOOP, 100);
    }

    void DoorPlayer(WorldObject* door, WorldObject* player)
    {
        PlayerDoor(player, door);
    }

    void DoorPlatform(WorldObject* door, WorldObject* platform)
    {
    }
    void PlatformDoor(WorldObject* platform, WorldObject* door)
    {
        DoorPlatform(door, platform);
    }

    void PlayerMeteor(WorldObject* player, WorldObject* meteor)
    {
        auto p = static_cast<Player*>(player);
        p->setLives(p->getLives() - 1);
        p->setAlive(false);
        Resources::instance().playSound(DEAD_SOUND, NO_LOOP, 100);
    }
    
    void MeteorPlayer(WorldObject* meteor, WorldObject* player)
    {
        PlayerMeteor(player, meteor);
    }

    void SpringboardPlayer(WorldObject* springboard, WorldObject* player)
    {
        PlayerMeteor(player, springboard);
    }

    void PlayerSpringboard(WorldObject* player, WorldObject* springboard)
    {
        auto p = static_cast<Player*>(player);
        //player->getPhysicalBody()->SetLinearVelocity(b2Vec2(-player->getPhysicalBody()->GetLinearVelocity().x , -player->getPhysicalBody()->GetLinearVelocity().x * 2));
        //player->getPhysicalBody()->SetLinearVelocity(b2Vec2(0, player->getPhysicalBody()->GetLinearVelocity().x * 2));
        float impulse = player->getPhysicalBody()->GetMass() * -20.5;
        player->getPhysicalBody()->ApplyLinearImpulse(b2Vec2(-player->getPhysicalBody()->GetLinearVelocity().x, impulse), player->getPhysicalBody()->GetWorldCenter(), true);

        //m_onGround = false;
    }


    void PlayerLaserBeam(WorldObject* player, WorldObject* beam)
    {
        auto p = static_cast<Player*>(player);
        p->setLives(p->getLives() - 1);
        p->setAlive(false);
        Resources::instance().playSound(DEAD_SOUND, NO_LOOP, 100);
    }

    void LaserBeamPlayer(WorldObject* beam, WorldObject* player)
    {
        PlayerLaserBeam(player, beam);
    }

    void PlatformLaserBeam(WorldObject* platform, WorldObject* beam)
    {
        auto p = static_cast<LaserBeam*>(beam);
        std::cout << "COLLISION PlatformLaserBeam " << std::endl;
    }

    void LaserBeamPlatform(WorldObject* beam, WorldObject* platform)
    {
        PlatformLaserBeam(platform, beam);
    }

    // init the hit map
    using HitFunctionPtr = void (*)(WorldObject*, WorldObject*);
    using Key = std::pair<std::type_index, std::type_index>;
    using HitMap = std::map<Key, HitFunctionPtr>;

    HitMap initializeCollisionMap()
    {
        HitMap phm;

        phm[Key(typeid(Player), typeid(Platform))] = &PlayerPlatform;
        phm[Key(typeid(Platform), typeid(Player))] = &PlatformPlayer;

        phm[Key(typeid(Platform), typeid(Platform))] = &PlatformPlatform;

        phm[Key(typeid(Player), typeid(Door))] = &PlayerDoor;
        phm[Key(typeid(Door), typeid(Player))] = &DoorPlayer;

        phm[Key(typeid(Door), typeid(Platform))] = &DoorPlatform;
        phm[Key(typeid(Platform), typeid(Door))] = &PlatformDoor;

        phm[Key(typeid(Player), typeid(Meteor))] = &PlayerMeteor;
        phm[Key(typeid(Meteor), typeid(Player))] = &MeteorPlayer;

        phm[Key(typeid(Springboard), typeid(Player))] = &SpringboardPlayer;
        phm[Key(typeid(Player), typeid(Springboard))] = &PlayerSpringboard;

        phm[Key(typeid(LaserBeam), typeid(Player))] = &LaserBeamPlayer;
        phm[Key(typeid(Player), typeid(LaserBeam))] = &PlayerLaserBeam;

        phm[Key(typeid(LaserBeam), typeid(Platform))] = &LaserBeamPlatform;
        phm[Key(typeid(Platform), typeid(LaserBeam))] = &PlatformLaserBeam;


        return phm;
    }

    HitFunctionPtr lookup(const std::type_index& class1, const std::type_index& class2)
    {
        static HitMap collisionMap = initializeCollisionMap();
        auto mapEntry = collisionMap.find(std::make_pair(class1, class2));
        if (mapEntry == collisionMap.end())
        {
            return nullptr;
        }
        return mapEntry->second;
    }

}

void processCollision(WorldObject* object1, WorldObject* object2)
{
    auto phf = lookup(typeid(*object1), typeid(*object2));
    if (!phf)
    {
        return;
        //throw UnknownCollision(object1, object2);
    }
    phf(object1, object2);
}
