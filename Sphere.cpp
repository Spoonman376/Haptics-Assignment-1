#include "Sphere.h"



Sphere::Sphere(chai3d::cVector3d position, double r, double s) : Entity(s)
{
	centerPoint = position;
	radius = r;
	chai3d::cCreateSphere(mesh, radius, 32, 32, position, chai3d::cIdentity3d(), chai3d::cColorf(1,1,1,1));
}

chai3d::cVector3d Sphere::calculateAppliedForce(chai3d::cVector3d cPosition, double cRadius)
{
	chai3d::cVector3d force1 = chai3d::cVector3d(0, 0, 0);
	chai3d::cVector3d force2 = chai3d::cVector3d(0, 0, 0);

	chai3d::cVector3d distance = cPosition - centerPoint;

	if (distance.length() < (radius + cRadius)) {
		force1 = distance;
		force1.normalize();

		force1 = force1 * stiffness * (radius + cRadius - distance.length());
	}
	
	if (type == magnetic)
	{
		if (distance.length() < (2 * radius + cRadius)) {
			force2 = distance;
			force2.normalize();

			force2 += force2 * -300 * (distance.length() - 2 * radius + cRadius);
		}
	}


	return force1 + force2;
}

Sphere::~Sphere()
{
}
