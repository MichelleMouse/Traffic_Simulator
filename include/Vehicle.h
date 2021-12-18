#ifndef VEHICLE_H
#define VEHICLE_H

#include "TrafficObject.h"

//Forward declarations to avoid include cycle
class Street;
class Intersection;

class Vehicle : public TrafficObject, public std::enable_shared_from_this<Vehicle>
{
public:
  //Constructor & Destructor
  Vehicle();

  //Getters & Setters
  void setCurrentStreet(std::shared_ptr<Street> street) { _currStreet = street; };
  void setCurrentDestination(std::shared_ptr<Intersection> destination);

  //Typical behaviour methods
  void simulate();

  //Miscellaneous
  std::shared_ptr<Vehicle> get_shared_this() { return shared_from_this(); }

private:
  //Typical behaviour methods
  void drive();

  std::shared_ptr<Street> _currStreet;            //Street on which the vehicle is currently on
  std::shared_ptr<Intersection> _currDestination; //Destination to which the vehicle is currently driving
  double _posStreet;                              //Position on current street
  double _speed;                                  //Ego speed in m/s
};

#endif
