/*

*/

#include "Box.hpp"

chai3d::cVector3d Box::calculateAppliedForce(chai3d::cVector3d cursorPosition, double cursorRadius)
{
	DirectionEntered direction = checkIntersection(cursorPosition, cursorRadius);
  
  if (directionEntered == notEntered || direction == notEntered) {
    directionEntered = direction;
  }
  
  // Based on which wall that the cursor has entered the box set the lesser point, greater point and the normal
  if (directionEntered != notEntered) {
    Wall wall = Wall(pointLFB, pointRBT, directionEntered);
    return forceApplied(cursorPosition, cursorRadius, wall);
  }
  else {
    return chai3d::cVector3d(0,0,0);
  }
}


double Box::intersectsWithWall(chai3d::cVector3d cursorPosition, double cursorRadius, Wall wall)
{
  std::vector<int> hAndV = std::vector<int>();

  // We reorinentate so that we treat h, v, and n as the indices of the horizontal, vertical, and normal axies respectively  
  int n = wall.n;
  int h = wall.h;
  int v = wall.v;


  // fin out if the center of the cursor is above or below the wall on the vertical axis
  bool below = cursorPosition(v) < wall.pointLesser(v);
  bool above = cursorPosition(v) > wall.pointGreater(v);

  // find out if the center of the cursor is to the left or the right of the wall on the horizontal axis
  bool leftOf = cursorPosition(h) < wall.pointLesser(h);
  bool rightOf = cursorPosition(h) > wall.pointGreater(h);

  // Find the horizontal and vertical distances from the lesser point to the cursor position
  double distLH = wall.pointLesser(h) - cursorPosition(h);
  double distLV = wall.pointLesser(v) - cursorPosition(v);

  // Find the horizontal and vertical distances from the greater point to the cursor position
  double distGH = wall.pointGreater(h) - cursorPosition(h);
  double distGV = wall.pointGreater(v) - cursorPosition(v);

  double distanceIn = 0;

  // To the left and below
  if (leftOf && below && cursorRadius * cursorRadius - distLH * distLH - distLV * distLV > 0) {
    distanceIn = sqrt(cursorRadius * cursorRadius - distLH * distLH - distLV * distLV);
  }
  // In the middle and below
  else if (!leftOf && !rightOf && below && cursorRadius * cursorRadius - distLV * distLV > 0) {
    distanceIn = sqrt(cursorRadius * cursorRadius - distLV * distLV);
  }
  // To the right and below
  else if (rightOf && below && cursorRadius * cursorRadius - distGH * distGH - distLV * distLV > 0) {
    distanceIn = sqrt(cursorRadius * cursorRadius - distGH * distGH - distLV * distLV);
  }
  // To the left and in the middle
  else if (leftOf && !below && !above && cursorRadius * cursorRadius - distLH * distLH > 0) {
    distanceIn = sqrt(cursorRadius * cursorRadius - distLH * distLH);
  }
  // In the middle
  else if (!leftOf && !rightOf && !below && !above) {
    distanceIn = cursorRadius;
  }
  // To the right and in the middle
  else if (rightOf && !below && !above && cursorRadius * cursorRadius - distGH * distGH > 0) {
    distanceIn = sqrt(cursorRadius * cursorRadius - distGH * distGH);
  }
  // To the left and above
  else if (leftOf && above && cursorRadius * cursorRadius - distLH * distLH - distGV * distGV > 0) {
    distanceIn = sqrt(cursorRadius * cursorRadius - distLH * distLH - distGV * distGV);
  }
  // In the middle and above
  else if (!leftOf && !rightOf && above && cursorRadius * cursorRadius - distGV * distGV > 0) {
    distanceIn = sqrt(cursorRadius * cursorRadius - distGV * distGV);
  }
  // To the right and above
  else if (rightOf && above && cursorRadius * cursorRadius - distGH * distGH - distGV * distGV > 0) {
    distanceIn = sqrt(cursorRadius * cursorRadius - distGH * distGH - distGV * distGV);
  }

  distanceIn = (cursorPosition(n) - distanceIn * wall.normal(n) - wall.pointLesser(n)) * (-wall.normal(n));

  return distanceIn;
}

DirectionEntered Box::checkIntersection(chai3d::cVector3d cursorPosition, double cursorRadius)
{

  DirectionEntered direction = notEntered;

  double x = pointRBT(0) / 2;
  double y = pointRBT(1) / 2;
  double z = pointRBT(2) / 2;

  std::vector<chai3d::cVector3d> wallCenterPoints(6);

  // Left and right walls
  wallCenterPoints[0] = pointLFB + chai3d::cVector3d(0, y, z);
  wallCenterPoints[1] = pointLFB + chai3d::cVector3d(2 * x, y, z);

  // front and back walls
  wallCenterPoints[2] = pointLFB + chai3d::cVector3d(x, 0, z);
  wallCenterPoints[3] = pointLFB + chai3d::cVector3d(x, 2 * y, z);

  // bottom and top walls
  wallCenterPoints[4] = pointLFB + chai3d::cVector3d(x, y, 0);
  wallCenterPoints[5] = pointLFB + chai3d::cVector3d(x, y, 2 * z);

  double distance = FLT_MAX;

  for (int i = 0; i < 6; ++i) {
    chai3d::cVector3d d = wallCenterPoints[i] - cursorPosition;

    if (d.length() < distance) {
      direction = DirectionEntered(i);
      distance = d.length();
    }
  }

  Wall wall = Wall(pointLFB, pointRBT, direction);
  if (intersectsWithWall(cursorPosition, cursorRadius, wall) > 0) {
    return direction;
  }

  return DirectionEntered::notEntered;
}

/* We know that the box and the cursor intersect with eachother so now calculate the force applied to the cursor
 * We know which wall the the cursor entered the box from 
 * We treat the wall as a plane where the normal is the direction that the wall applies force in.
 * 
 * The sparate x,y,z values for pointLesser < pointGreater
 */
chai3d::cVector3d Box::forceApplied(chai3d::cVector3d cursorPosition, double cursorRadius, Wall wall)
{
  double distance = intersectsWithWall(cursorPosition, cursorRadius, wall);

  if (distance > 0) {
    return wall.normal * distance * stiffness;
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

  chai3d::cVector3d cp =  a + (b / 2);
  double x = b(0);
  double y = b(1);
  double z = b(2);

  chai3d::cCreateBox(mesh, x, y, z, cp, chai3d::cIdentity3d(), chai3d::cColorf(1,1,1,1));
}

//
Box::Box(chai3d::cVector3d cp, double x, double y, double z, double s) : Entity(s)
{
  pointLFB = cp - chai3d::cVector3d(x, y, z);
  pointRBT = chai3d::cVector3d(x, y, z);

  chai3d::cCreateBox(mesh, x/2, y/2, z/2, cp, chai3d::cIdentity3d(), chai3d::cColorf(1, 1, 1, 1));
}


Box::~Box()
{
}


Wall::Wall(chai3d::cVector3d pointLFB, chai3d::cVector3d pointRBT, DirectionEntered d)
{
  switch (d) {
  case left:
    pointLesser = pointLFB;
    pointGreater = pointLFB + chai3d::cVector3d(0, pointRBT(1), pointRBT(2));
    normal = chai3d::cVector3d(-1, 0, 0);
    n = 0;
    h = 1;
    v = 2;
    break;

  case right:
    pointLesser = pointLFB + chai3d::cVector3d(pointRBT(0), 0, 0);
    pointGreater = pointLFB + pointRBT;
    normal = chai3d::cVector3d(1, 0, 0);
    n = 0;
    h = 1;
    v = 2;
    break;

  case front:
    pointLesser = pointLFB;
    pointGreater = pointLFB + chai3d::cVector3d(pointRBT(0), 0, pointRBT(2));
    normal = chai3d::cVector3d(0, -1, 0);
    n = 1;
    h = 0;
    v = 2;
    break;

  case back:
    pointLesser = pointLFB + chai3d::cVector3d(0, pointRBT(1), 0);
    pointGreater = pointLFB + pointRBT;
    normal = chai3d::cVector3d(0, 1, 0);
    n = 1;
    h = 0;
    v = 2;
    break;

  case bottom:
    pointLesser = pointLFB;
    pointGreater = pointLFB + chai3d::cVector3d(pointRBT(0), pointRBT(1), 0);
    normal = chai3d::cVector3d(0, 0, -1);
    n = 2;
    h = 0;
    v = 1;
    break;

  case top:
    pointLesser = pointLFB + chai3d::cVector3d(0, 0, pointRBT(2));
    pointGreater = pointLFB + pointRBT;
    normal = chai3d::cVector3d(0, 0, 1);
    n = 2;
    h = 0;
    v = 1;
    break;
  }
}




















