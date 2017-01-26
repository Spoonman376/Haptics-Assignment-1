/*

*/

#include "Entity.hpp"





Entity::Entity(double stiff)
{
	mesh = new chai3d::cMesh();
	stiffness = stiff;
}



Entity::~Entity()
{
  if (mesh != nullptr)
    delete mesh;
}


void Entity::addToWorld(chai3d::cWorld* world)
{
  if (mesh != nullptr) {
    world->addChild(mesh);
  }
}

chai3d::cVector3d Entity::calculateAppliedForce(chai3d::cVector3d cPosition, float cRadius)
{
  return chai3d::cVector3d(0, 0, 0);
}

