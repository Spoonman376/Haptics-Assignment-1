/*

*/
#ifndef Entity_hpp
#define Entity_hpp

#include "chai3d.h"
#include <GLFW/glfw3.h>



class Entity
{
public:

	chai3d::cMesh* mesh;
	double stiffness; // N/m

	virtual chai3d::cVector3d calculateAppliedForce(float cPosition, float cRadius);

	Entity(double stiff);
	~Entity();
};

#endif /* Entity_hpp */