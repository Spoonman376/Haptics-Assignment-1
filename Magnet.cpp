//
//  Magnet.cpp
//  
//
//  Created by Erik Spooner on 2017-01-25.
//
//

#include "Magnet.hpp"


Magnet::Magnet(Entity* f, double mF)
{
  form = f;
  magneticForce = mF;
}

Magnet::~Magnet()
{
}

chai3d::cVector3d Magnet::calculateAppliedForce(chai3d::cVector3d cPosition, double cRadius)
{
  if (form != nullptr) {
    return  form->calculateAppliedForce(cPosition, cRadius);
  }
  else {
    return chai3d::cVector3d(0, 0, 0);
  }
}