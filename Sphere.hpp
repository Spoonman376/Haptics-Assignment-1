#pragma once
#include "Entity.h"

class Sphere : public Entity
{
public:

	chai3d::cVector3d centerPoint;
	double radius;

	chai3d::cVector3d calculateAppliedForce(chai3d::cVector3d cPosition, double cRadius);

	Sphere(chai3d::cVector3d, double r, double s);
	~Sphere();
};

