//
//  Box.hpp
//  
//
//  Created by Erik Spooner on 2017-01-17.
//
//

#ifndef Box_hpp
#define Box_hpp

#include <stdio.h>
#include "Point.hpp"
#include "Entity.hpp"

enum DirectionEntered
{
  left, right,
  front, back,
  top, bottom,
  notEntered
};

class Box : public Entity
{
public:
  
  // This Point is the left front bottom corner of the box
  chai3d::cVector3d pointLFB;
  
  // This is a vector from the left front bottom corner to the right back top corner
  chai3d::cVector3d pointRBT;
  
  DirectionEntered directionEntered;
  
  Box(double centerPoint, double frontDistance, double topDistance, double rightDistance, double s);
  Box(chai3d::cVector3d a, chai3d::cVector3d b, double s);
  ~Box();

  chai3d::cVector3d calculateAppliedForce(chai3d::cVector3d cursorPosition, double cursorRadius);
  chai3d::cVector3d forceApplied(chai3d::cVector3d cursorPosition, double cursorRadius, chai3d::cVector3d pointLess, chai3d::cVector3d, pointGreater, chai3d::cVector3d normal);
  
private:
  
  DirectionEntered checkIntersection(chai3d::cVector3d cursorPosition, double cursorRadius);
};


#endif /* Box_hpp */
