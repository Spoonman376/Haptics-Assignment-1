#pragma once
#include "chai3d.h"
#include <GLFW/glfw3.h>

enum Type
{
	solid, magnetic
};


class Entity
{
public:

	chai3d::cMesh* mesh;
	double stiffness; // N/m

	Type type;

	virtual chai3d::cVector3d calculateAppliedForce(float cPosition, float cRadius);

	Entity(double stiff);
	~Entity();
};

