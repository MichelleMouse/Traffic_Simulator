#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <vector>
#include <future>
#include <memory>
#include <mutex>
#include "TrafficObject.h"
#include "TrafficLight.h"

//Forward declarations to avoid include cycle
class Street;
class Vehicle;

//Auxiliary class to queue and dequeue waiting vehicles in a thread-safe manner
class WaitingVehicles
{
public:
  //Getters & Setters
  int getSize();

  //Typical behaviour methods
  void pushBack(std::shared_ptr<Vehicle> vehicle, std::promise<void> &&promise);
  void permitEntryToFirstInQueue();

private:
  std::vector< std::shared_ptr<Vehicle> > _vehicles;  //List of all vehicles waiting to enter the intersection
  std::vector< std::promise<void> > _promises;        //List of associated promises
  std::mutex mtx;
};

class Intersection : public TrafficObject
{
public:
  //Constructor & Destructor
  Intersection();

  //Getters & Setters
  void setIsBlocked(bool isBlocked);

  //Typical behaviour methods
  void addVehicleToQueue(std::shared_ptr<Vehicle> vehicle);
  void addStreet(std::shared_ptr<Street> street);
  std::vector<std::shared_ptr<Street>> queryStreets(std::shared_ptr<Street> incoming); //Returns pointer to current list of all outgoing streets
  void simulate();
  void vehicleHasLeft(std::shared_ptr<Vehicle> vehicle);
  bool trafficLightIsGreen();

private:
  //Typical behaviour methods
  void processVehicleQueue();

  //Private members
  std::vector<std::shared_ptr<Street>> _streets;   //List of all streets connected to this intersection
  WaitingVehicles _waitingVehicles;   //List of all vehicles and their associated promises waiting to enter the intersection
  bool _isBlocked;    //Flag indicating wether the intersection is blocked by a vehicle
  TrafficLight _trafficLight;
};

#endif
