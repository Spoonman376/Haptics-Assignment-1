/*

*/
#ifndef Sphere_hpp
#define Sphere_hpp

#include "Entity.hpp"

class Sphere : public Entity
{
public:

	chai3d::cVector3d centerPoint;
	double radius;

	chai3d::cVector3d calculateAppliedForce(chai3d::cVector3d cPosition, double cRadius);

	Sphere(chai3d::cVector3d, double r, double s);
	~Sphere();
};

#endif /* Sphere_hpp */