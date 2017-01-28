/*

*/

#ifndef Box_hpp
#define Box_hpp

#include <stdio.h>
#include "Entity.hpp"


enum DirectionEntered
{
  left = 0, right = 1,
  front = 2, back = 3,
  bottom = 4, top = 5,
  notEntered = 6
};

struct Wall
{
public:
  chai3d::cVector3d pointLesser;
  chai3d::cVector3d pointGreater;
  chai3d::cVector3d normal;

  int n;
  int h;
  int v;

  Wall(chai3d::cVector3d, chai3d::cVector3d, DirectionEntered);
};

class Box : public Entity
{
public:
  
  // This Point is the left front bottom corner of the box
  chai3d::cVector3d pointLFB;
  
  // This is a vector from the left front bottom corner to the right back top corner
  chai3d::cVector3d pointRBT;
  
  DirectionEntered directionEntered = notEntered;
  
  Box(chai3d::cVector3d centerPoint, double xDistance, double yDistance, double zDistance, double s);
  Box(chai3d::cVector3d a, chai3d::cVector3d b, double s);
  ~Box();

  chai3d::cVector3d calculateAppliedForce(chai3d::cVector3d cursorPosition, double cursorRadius);
private:
  
  chai3d::cVector3d forceApplied(chai3d::cVector3d cursorPosition, double cursorRadius, Wall wall);
  DirectionEntered checkIntersection(chai3d::cVector3d cursorPosition, double cursorRadius);
  double intersectsWithWall(chai3d::cVector3d cursorPosition, double cursorRadius, Wall wall);
};


#endif /* Box_hpp */
