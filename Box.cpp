/*

*/

#include "Box.hpp"

chai3d::cVector3d Box::calculateAppliedForce(chai3d::cVector3d cursorPosition, double cursorRadius)
{
	DirectionEntered direction = checkIntersection(cursorPosition, cursorRadius);
  
  if (directionEntered == notEntered || direction == notEntered) {
    directionEntered = direction;
  }
  
  // The sparate x, y, z values for pointLesser < pointGreater
  chai3d::cVector3d pointLesser;
  chai3d::cVector3d pointGreater;
  chai3d::cVector3d normal;

  // Based on which wall that the cursor has entered the box set the lesser point, greater point and the normal
  if (directionEntered != notEntered) {
    Wall wall = Wall(pointLFB, pointRBT, directionEntered);
    return forceApplied(cursorPosition, cursorRadius, wall.pointLesser, wall.pointGreater, wall.normal);
  }
  else {
    return chai3d::cVector3d(0,0,0);
  }
}

// Need to implement ----------------------------------------------------------------------------
DirectionEntered Box::checkIntersection(chai3d::cVector3d cursorPosition, double cursorRadius)
{
  
  DirectionEntered direction;
  
  chai3d::cVector3d left, right, front, back, top, bottom;
  
  double x = pointRBT(0)/2;
  double y = pointRBT(1)/2;
  double z = pointRBT(2)/2;
  
  std::vector<chai3d::cVector3d> wallCenterPoints (6);
  
  // Left and right walls
  wallCenterPoints.push_back(pointLFB + chai3d::cVector3d(0, y, z));
  wallCenterPoints.push_back(pointLFB + chai3d::cVector3d(2 * x, y, z));
  
  // front and back walls
  wallCenterPoints.push_back(pointLFB + chai3d::cVector3d(x, 0, z));
  wallCenterPoints.push_back(pointLFB + chai3d::cVector3d(x, 2 * y, z));

  // bottom and top walls
  wallCenterPoints.push_back(pointLFB + chai3d::cVector3d(x, y, 0));
  wallCenterPoints.push_back(pointLFB + chai3d::cVector3d(x, y, 2 * z));
  
  double distance = MAX_FLOAT;
  
  for (int i = 0; i < 6, ++i) {
    chai3d::cVector3d d = wallCenterPoints(i) - cursorPosition;
    
    if (d.length() < distance) {
      direction = i;
      distance = d.length();
    }
  }
  
  Wall wall = Wall(pointLFB, pointRBT, directionEntered);
  
  

  return DirectionEntered::notEntered;
}


bool



/* We know that the box and the cursor intersect with eachother so now calculate the force applied to the cursor
 * We know which wall the the cursor entered the box from 
 * We treat the wall as a plane where the normal is the direction that the wall applies force in.
 * 
 * The sparate x,y,z values for pointLesser < pointGreater
 */
chai3d::cVector3d Box::forceApplied(chai3d::cVector3d cursorPosition, double cursorRadius, chai3d::cVector3d pointLesser, chai3d::cVector3d pointGreater, chai3d::cVector3d normal)
{
  
  std::vector<int> hAndV (2);

  // We reorinentate so that we treat h, v, and n as the indices of the horizontal, vertical, and normal axies respectively  
  int n;
  int h;
  int v;
  
  for (int i = 0; i < 2; ++i) {
    if (normal(i) != 0) {
      n = i;
    }
    else {
      hAndV.push_back(i);
    }
  }

  h = hAndV[0];
  v = hAndV[1];
  
  // fin out if the center of the cursor is above or below the wall on the vertical axis
  bool below = cursorPosition(v) < pointLesser(v);
  bool above = cursorPosition(v) > pointGreater(v);

  // find out if the center of the cursor is to the left or the right of the wall on the horizontal axis
  bool leftOf = cursorPosition(h) < pointLesser(h);
  bool rightOf = cursorPosition(h) > pointGreater(h);

  // Find the horizontal and vertical distances from the lesser point to the cursor position
  double distLH = pointLesser(h) - cursorPosition(h);
  double distLV = pointLesser(v) - cursorPosition(v);

  // Find the horizontal and vertical distances from the greater point to the cursor position
  double distGH = pointGreater(h) - cursorPosition(h);
  double distGV = pointGreater(v) - cursorPosition(v);

  // 
  double distanceIn = 0;

  // To the left and below
  if(leftOf && below) {
    distanceIn = sqrt(cursorRadius * cursorRadius - distLH * distLH - distLV * distLV);
  }
  // In the middle and below
  else if(!leftOf && !rightOf && below) {
    distanceIn = sqrt(cursorRadius * cursorRadius - distLV * distLV);
  }
  // To the right and below
  else if(rightOf && below) {
    distanceIn = sqrt(cursorRadius * cursorRadius - distGH * distGH - distLV * distLV);
  }
  // To the left and in the middle
  else if(leftOf && !below && !above) {
    distanceIn = sqrt(cursorRadius * cursorRadius - distLH * distLH);
  }
  // In the middle
  else if(!leftOf && !rightOf && !below && !above) {
    distanceIn = cursorRadius;
  }
  // To the right and in the middle
  else if(rightOf && !below && !above) {
    distanceIn = sqrt(cursorRadius * cursorRadius - distGH * distGH);
  }
  // To the left and above
  else if(leftOf && above) {
    distanceIn = sqrt(cursorRadius * cursorRadius - distLH * distLH - distGV * distGV);
  }
  // In the middle and above
  else if(!leftOf && !rightOf && above) {
    distanceIn = sqrt(cursorRadius * cursorRadius - distGV * distGV);
  }
  // To the right and above
  else if(rightOf && above) {
    distanceIn = sqrt(cursorRadius * cursorRadius - distGH * distGH - distGV * distGV);
  }

  
  // This might be wrong ---------------------------------------------------
  // Fix the distance in
  distanceIn = cursorPosition(n) - distanceIn * normal(n) - pointLesser(n);
  
  if (distanceIn > 0) {
    return normal * distanceIn * stiffness;
  }
  else {
    return chai3d::cVector3d(0,0,0);
  }
  
}

//
Box::Box(chai3d::cVector3d a, chai3d::cVector3d b, double s) : Entity(s)
{
  pointLFB = a;
  pointRBT = b;

  chai3d::cVector3d cp = a + (b / 2);
  double x = b(0);
  double y = b(1);
  double z = b(2);

  chai3d::cCreateBox(mesh, x, y, z, pointLFB, chai3d::cIdentity3d(), chai3d::cColorf(1,1,0,1));
}

//
Box::Box(chai3d::cVector3d cp, double x, double y, double z, double s) : Entity(s)
{
  pointLFB = cp - chai3d::cVector3d(x, y, z);
  pointRBT = cp + chai3d::cVector3d(x, y, z);

  chai3d::cCreateBox(mesh, x, y, z, cp, chai3d::cIdentity3d(), chai3d::cColorf(0, 0, 0, 0));
}


Wall::Wall(chai3d::cVector3d pointLFB, chai3d::cVector3d pointRBT, DirectionEntered d)
{
  switch (d) {
    case left:
      pointLesser = pointLFB;
      pointGreater = pointLFB + chai3d::cVector3d(0,pointRBT(1),pointRBT(2));
      normal = chai3d::cVector3d(-1,0,0);
      break;
      
    case right:
      pointLesser = pointLFB + chai3d::cVector3d(pointRBT(0),0,0);
      pointGreater = pointLFB + pointRBT;
      normal = chai3d::cVector3d(1,0,0);
      break;
      
    case front:
      pointLesser = pointLFB;
      pointGreater = pointLFB + chai3d::cVector3d(pointRBT(0), 0, pointRBT(2));
      normal = chai3d::cVector3d(0,-1,0);
      break;
      
    case back:
      pointLesser = pointLFB + chai3d::cVector3d(0,pointRBT(1),0);
      pointGreater = pointLFB + pointRBT;
      normal = chai3d::cVector3d(0,1,0);
      break;
      
    case bottom:
      pointLesser = pointLFB;
      pointGreater = pointLFB + chai3d::cVector3d(pointRBT(0), pointRBT(1), 0);
      normal = chai3d::cVector3d(0,0,-1);
      break;
      
    case top:
      pointLesser = pointLFB + chai3d::cVector3d(0,0,pointRBT(2));
      pointGreater = pointLFB + pointRBT;
      normal = chai3d::cVector3d(0,0,1);
      break;
}




















