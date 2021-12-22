
#include <CollisionListener.h>
#include <iostream>
#include <Player.h>

CollisionListener::CollisionListener() {

}

void CollisionListener::BeginContact(b2Contact *contact) {
    WorldObject* objectA = static_cast<WorldObject*> (contact->GetFixtureA()->GetBody()->GetUserData());
    WorldObject* objectB = static_cast<WorldObject*> (contact->GetFixtureB()->GetBody()->GetUserData());

    if (objectA && objectB) {
        processCollision(objectA, objectB);
    }
}

void CollisionListener::EndContact(b2Contact *contact) {
    void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
    WorldObject * o1;
    WorldObject * o2;
    if ( bodyUserData ) {
        o1 = static_cast<WorldObject *>( bodyUserData );
        o1->handleEndCollision();
        o1->setNumContacts(o1->getNumContacts()-1) ;
    }
    bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
    if ( bodyUserData ) {
        o2 = static_cast<WorldObject *>( bodyUserData );
        o2->handleEndCollision();
        o2->setNumContacts(o2->getNumContacts()-1) ;
    }
    o1->setCollisionWith(NULL);
    o2->setCollisionWith(NULL);
}

