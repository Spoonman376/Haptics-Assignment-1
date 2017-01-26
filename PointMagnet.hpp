//
//  PointMagnet.hpp
//  
//
//  Created by Erik Spooner on 2017-01-25.
//
//

#ifndef PointMagnet_hpp
#define PointMagnet_hpp

#include <stdio.h>
#include "Magnet.hpp"

class PointMagnet : public Magnet
{
public:
  
  chai3d::cVector3d position;

  PointMagnet(Entity* e, chai3d::cVector3d p, double pull);
  ~PointMagnet();

  chai3d::cVector3d calculateAppliedForce(chai3d::cVector3d cPosition, double cRadius);

};

#endif /* PointMagnet_hpp */
