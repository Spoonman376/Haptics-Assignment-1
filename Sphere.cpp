/*
  
 */

#include "Sphere.hpp"

Sphere::Sphere(chai3d::cVector3d position, double r, double s) : Entity(s)
{
	centerPoint = position;
	radius = r;
	chai3d::cCreateSphere(mesh, radius, 32, 32, position, chai3d::cIdentity3d(), chai3d::cColorf(1,1,1,1));
}

chai3d::cVector3d Sphere::calculateAppliedForce(chai3d::cVector3d cPosition, double cRadius)
{
	chai3d::cVector3d force = chai3d::cVector3d(0, 0, 0);

	chai3d::cVector3d distance = cPosition - centerPoint;

	if (distance.length() < (radius + cRadius)) {
		force = distance;
		force.normalize();

		force = force * stiffness * (radius + cRadius - distance.length());
	}
	
	return force;
}

Sphere::~Sphere()
{
}
