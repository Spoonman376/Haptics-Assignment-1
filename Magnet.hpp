//
//  Magnet.hpp
//  
//
//  Created by Erik Spooner on 2017-01-25.
//
//

#ifndef Magnet_hpp
#define Magnet_hpp

#include <stdio.h>
#include "chai3d.h"
#include <GLFW/glfw3.h>
#include "Sphere.hpp"


class Magnet
{
public:
  Entity* form;
  double magneticForce;

  Magnet(Entity*, double);
  ~Magnet();

  virtual chai3d::cVector3d calculateAppliedForce(chai3d::cVector3d cPosition, double cRadius);

private:


};

#endif /* Magnet_hpp */
