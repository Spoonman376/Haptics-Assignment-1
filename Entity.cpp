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

