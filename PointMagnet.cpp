//
//  PointMagnet.cpp
//  
//
//  Created by Erik Spooner on 2017-01-25.
//
//

#include "PointMagnet.hpp"

PointMagnet::PointMagnet(Sphere* e, chai3d::cVector3d p, double pull) : Magnet(e, pull)
{
  position = p;
}

PointMagnet::~PointMagnet()
{
}



chai3d::cVector3d PointMagnet::calculateAppliedForce(chai3d::cVector3d cPosition, double cRadius)
{
  chai3d::cVector3d force = position - cPosition;
  double distance = force.length();

  if (distance < 0.01) {
    distance = 0.01;
  }

  force.normalize();

  force *= magneticForce/ distance;

  return force + dynamic_cast<Sphere*>(form)->calculateAppliedForce(cPosition, cRadius);
}







