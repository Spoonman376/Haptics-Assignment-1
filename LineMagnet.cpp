//
//  LineMagnet.cpp
//  
//
//  Created by Erik Spooner on 2017-01-25.
//
//

#include "LineMagnet.hpp"


LineMagnet::LineMagnet(Entity* e, chai3d::cVector3d a, chai3d::cVector3d b, double pull) : Magnet(e, pull)
{
  pointA = a;
  pointB = b;
}

LineMagnet::~LineMagnet()
{

}


chai3d::cVector3d LineMagnet::calculateAppliedForce(chai3d::cVector3d cPosition, double cRadius)
{

  chai3d::cVector3d closestPoint = calculateClosestPoint(cPosition);

  chai3d::cVector3d force = closestPoint - cPosition;
  double distance = force.length();
  
  force.normalize();
  force *= magneticForce / distance;

  if (form != nullptr) {
    return force + dynamic_cast<Box*>(form)->calculateAppliedForce(cPosition, cRadius);
  }

  return force;


}

chai3d::cVector3d LineMagnet::calculateClosestPoint(chai3d::cVector3d cPosition)
{
  chai3d::cVector3d AB = (pointB - pointA);
  double d = AB * (cPosition - pointA) / AB.lengthsq();
  
  if (d < 0) {
    return pointA;
  }
  if (d > 1) {
    return pointB;
  }

  return pointA + d * AB;
}






