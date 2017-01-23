/*

*/

#include "Entity.hpp"



Entity::Entity(double stiff)
{
	mesh = new chai3d::cMesh();
	stiffness = stiff;
}

chai3d::cVector3d Entity::calculateAppliedForce(float cPosition, float cRadius)
{
	return chai3d::cVector3d();
}


Entity::~Entity()
{

}
