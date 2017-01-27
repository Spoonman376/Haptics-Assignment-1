//
//  LineMagnet.hpp
//  
//
//  Created by Erik Spooner on 2017-01-25.
//
//

#ifndef LineMagnet_hpp
#define LineMagnet_hpp

#include <stdio.h>
#include "Magnet.hpp"

class LineMagnet : public Magnet
{
public:
  chai3d::cVector3d pointA;
  chai3d::cVector3d pointB;

  LineMagnet(Entity* e, chai3d::cVector3d a, chai3d::cVector3d b, double pull);
  ~LineMagnet();

  chai3d::cVector3d calculateAppliedForce(chai3d::cVector3d cPosition, double cRadius);
 
private:
  
  chai3d::cVector3d calculateClosestPoint(chai3d::cVector3d cPosition);
};

#endif /* LineMagnet_hpp */
