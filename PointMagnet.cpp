//
//  PointMagnet.cpp
//  
//
//  Created by Erik Spooner on 2017-01-25.
//
//

#include "PointMagnet.hpp"

PointMagnet::PointMagnet(Entity* e, chai3d::cVector3d p, double pull) : Magnet(e, pull)
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

  force.normalize();

  force *= magneticForce / distance;

  return force + Magnet::calculateAppliedForce(cPosition, cRadius);
}







