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
    return forceApplied(cursorPosition, cursorRadius, wall);
  }
  else {
    return chai3d::cVector3d(0,0,0);
  }
}


double Box::intersectsWithWall(chai3d::cVector3d cursorPosition, double cursorRadius, Wall wall)
{
  std::vector<int> hAndV(2);

  // We reorinentate so that we treat h, v, and n as the indices of the horizontal, vertical, and normal axies respectively  
  int n;
  int h;
  int v;

  for (int i = 0; i < 2; ++i) {
    if (wall.normal(i) != 0) {
      n = i;
    }
    else {
      hAndV.push_back(i);
    }
  }

  h = hAndV[0];
  v = hAndV[1];

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

  distanceIn = cursorPosition(n) - distanceIn * wall.normal(n) - wall.pointLesser(n);

  return distanceIn;
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
    pointGreater = pointLFB + chai3d::cVector3d(0, pointRBT(1), pointRBT(2));
    normal = chai3d::cVector3d(-1, 0, 0);
    break;

  case right:
    pointLesser = pointLFB + chai3d::cVector3d(pointRBT(0), 0, 0);
    pointGreater = pointLFB + pointRBT;
    normal = chai3d::cVector3d(1, 0, 0);
    break;

  case front:
    pointLesser = pointLFB;
    pointGreater = pointLFB + chai3d::cVector3d(pointRBT(0), 0, pointRBT(2));
    normal = chai3d::cVector3d(0, -1, 0);
    break;

  case back:
    pointLesser = pointLFB + chai3d::cVector3d(0, pointRBT(1), 0);
    pointGreater = pointLFB + pointRBT;
    normal = chai3d::cVector3d(0, 1, 0);
    break;

  case bottom:
    pointLesser = pointLFB;
    pointGreater = pointLFB + chai3d::cVector3d(pointRBT(0), pointRBT(1), 0);
    normal = chai3d::cVector3d(0, 0, -1);
    break;

  case top:
    pointLesser = pointLFB + chai3d::cVector3d(0, 0, pointRBT(2));
    pointGreater = pointLFB + pointRBT;
    normal = chai3d::cVector3d(0, 0, 1);
    break;
  }
}




















