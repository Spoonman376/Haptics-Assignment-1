//
//  Box.cpp
//  
//
//  Created by Erik Spooner on 2017-01-17.
//
//

#include "Box.hpp"


chai3d::cVector3d Box::calculateAppliedForce(chai3d::cVector3d cursorPosition, float cursorRadius)
{
  
  DirectionEntered direction = checkIntersection(cursorPosition, cursorRadius)
  
  if (directionEntered == notEntered || direction == notEntered) {
    directionEntered = direction;
  }
  
  chai3d::cVector3d pointLess;
  chai3d::cVector3d pointGreater;
  chai3d::cVector3d normal;

  // Based on which wall that the cursor has entered the box
  switch (directionEntered) {
    case left:
      pointLess = pointLFB;
      pointGreater = pointLFB + chai3d::cVector3d(0,pointRBT(1),pointRBT(2));
      normal = chai3d::cVector3d(-1,0,0);
      break;

    case right:
      pointLess = pointLFB + chai3d::cVector3d(pointRBT(0),0,0);
      pointGreater = pointLFB + pointRBT;
      normal = chai3d::cVector3d(1,0,0);
      break;

    case front:
      pointLess = pointLFB;
      pointGreater = pointLFB + chai3d::cVector3d(pointRBT(0), 0, pointRBT(2));
      normal = chai3d::cVector3d(0,-1,0);
      break;
      
    case back:
      pointLess = pointLFB + chai3d::cVector3d(0,pointRBT(1),0);
      pointGreater = pointLFB + pointRBT;
      normal = chai3d::cVector3d(0,1,0);
      break;

    case bottom:
      pointLess = pointLFB;
      pointGreater = pointLFB + chai3d::cVector3d(pointRBT(0), pointRBT(1), 0);
      normal = chai3d::cVector3d(0,0,-1);
      break;

    case top:
      pointLess = pointLFB + chai3d::cVector3d(0,0,pointRBT(2));
      pointGreater = pointLFB + pointRBT;
      normal = chai3d::cVector3d(0,0,1);
      break;

      // The cursor has not interested the box so no force is imparted
    default:
      return chai3d::cVector3d(0,0,0);
      break;
  }
  
  return forceApplied(cursorPosition, cursorRadius, pointLess, pointGreater, normal);
  
}

/* We know that the box and the cursor intersect with eachother so now calculate the force applied to the cursor
 *
 */
chai3d::cVector3d Box::forceApplied(chai3d::cVector3d cursorPosition, double cursorRadius, chai3d::cVector3d pointLess, chai3d::cVector3d, pointGreater, chai3d::cVector3d normal)
{
  
  vector<int> hAndV (2);

  int n;
  
  for (int i = 0; i < 2; ++i) {
    if (normal(i) != 0) {
      n = i;
    }
    
    hAndV.add(i);
  }

  int h = hAndV[0];
  int v = hAndV[1];
  
  bool below = cursorPosition(v) < pointLess(v);
  bool above = cursorPosition(v) > pointGreater(v);

  bool leftOf = cursorPosition(h) < pointLess(h);
  bool rightOf = cursorPosition(h) > pointGreater(h);

  double distLH = pointLess(h) - cursorPosition(h);
  double distLV = pointLess(v) - cursorPosition(v);
  double distGH = pointGreater(h) - cursorPosition(h);
  double distGV = pointGreater(v) - cursorPosition(v);
  double distanceIn = 0;

  // To the left and below
  if(below && leftOf) {
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
  else if(!below && !above && leftOf) {
    distanceIn = sqrt(cursorRadius * cursorRadius - distLH * distLH);
  }
  // In the middle
  else if(!below && !above && !leftOf && !rightOf) {
    distanceIn = cursorRadius;
  }
  // To the right and in the middle
  else if(!below && !above && rightOf) {
    distanceIn = sqrt(cursorRadius * cursorRadius - distGH * distGH);
  }
  // To the left and above
  else if(above && leftOf) {
    distanceIn = sqrt(cursorRadius * cursorRadius - distLH * distLH - distGV * distGV);
  }
  // In the middle and above
  else if(above && !leftOf && !rightOf) {
    distanceIn = sqrt(cursorRadius * cursorRadius - distGV * distGV);
  }
  // To the right and above
  else if(above && rightOf) {
    distanceIn = sqrt(cursorRadius * cursorRadius - distGH * distGH - distGV * distGV);
  }

  
  // This might be wrong ---------------------------------------------------
  // Fixe the distance in
  distanceIn =  cursorPosition(n) - distanceIn * normal(n) - pointLess(n)
  
  return normal * distanceIn * stiffness;
}


Box::Box(Point a, Point b, float s) : Entity(s)
{
  pointA = a;
  pointB = b;
}




















