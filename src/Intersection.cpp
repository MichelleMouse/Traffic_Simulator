#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <random>

#include "Street.h"
#include "Intersection.h"
#include "Vehicle.h"

int WaitingVehicles::getSize()
{
  std::unique_lock<std::mutex> lck(mtx);
  return _vehicles.size();
}

void WaitingVehicles::pushBack(std::shared_ptr<Vehicle> vehicle, std::promise<void> &&promise)
{
  std::unique_lock<std::mutex> lck(mtx);
  _vehicles.push_back(vehicle);
  _promises.push_back(std::move(promise));
}

void WaitingVehicles::permitEntryToFirstInQueue()
{
  std::unique_lock<std::mutex> lck(mtx);

  auto firstpromise = _promises.begin();
  auto firstvehicle = _vehicles.begin();

  firstpromise->set_value();

  _promises.erase(firstpromise);
  _vehicles.erase(firstvehicle);
}

/*    IMPLEMENTS CLASS INTERSECTION    */
Intersection::Intersection()
{
  _type = ObjectType::objectIntersection;
  _isBlocked = false;
}

void Intersection::addStreet(std::shared_ptr<Street> street)
{
  _streets.push_back(street);
}

std::vector<std::shared_ptr<Street>> Intersection::queryStreets(std::shared_ptr<Street> incoming)
{
  //Stores all outgoing streets in a vector ...
  std::vector<std::shared_ptr<Street>> outgoings;
  for (auto it : _streets)
  {
    if (incoming->getID() != it->getID()) //... except the street making the inquiry
    {
      outgoings.push_back(it);
    }
  }

  return outgoings;
}

void Intersection::addVehicleToQueue(std::shared_ptr<Vehicle> vehicle)
{
  std::unique_lock<std::mutex> lck(TrafficObject::_mtxCout);

  std::cout << "Intersection #" << _id << "::addVehicleToQueue: thread id " << std::this_thread::get_id() << std::endl;
  lck.unlock();

  //Adds vehicle to the waiting line
  std::promise<void> prms;
  std::future<void> ftr = prms.get_future();
  _waitingVehicles.pushBack(vehicle, std::move(prms));

  ftr.wait();
  lck.lock();
  std::cout << "Intersection #" << _id << ": Vehicle #" << vehicle->getID() << " is granted entry" << std::endl;

  lck.unlock();

  //BLock the exeuction until the traffic light turn green
  if(_trafficLight.getCurrentPhase() == TrafficLightPhase::red)
  {
    lck.lock();
    std::cout << "Waiting for green light..." << std::endl;
    lck.unlock();
    _trafficLight.waitForGreen();
  }
}

//Unblocks the queue processing
void Intersection::vehicleHasLeft(std::shared_ptr<Vehicle> vehicle)
{
  std::cout << "Intersection #" << _id << ": Vehicle #" << vehicle->getID() << " has left" << std::endl;

  this->setIsBlocked(false);
}

void Intersection::setIsBlocked(bool isBlocked)
{
  _isBlocked = isBlocked;

  //Prints the state of the intersection
  std::cout << "Intersection #" << _id << " isBlocked = " << isBlocked << std::endl;
}

//Virtual function which is executed in thread
void Intersection::simulate()
{
  //Starts the traffic light simulation
  _trafficLight.simulate();

  //Launches vehicle queue processing in a thread
  _threads.emplace_back(std::thread(&Intersection::processVehicleQueue, this));
}

void Intersection::processVehicleQueue()
{
  //Prints the id of the current thread
  std::cout << "Intersection #" << _id << "::processVehicleQueue: thread id = " << std::this_thread::get_id() << std::endl;

  //Continuously process the vehicle queue
  while(true)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));  //Reduces CPU usage by sleeping at every iteration

    //Only proceed when at least one vehicle is waiting in the queue
    if(_waitingVehicles.getSize() > 0 && !_isBlocked)
    {
      this->setIsBlocked(true);    //Sets intersection to blocked, to prevent other vehicles from entering
      _waitingVehicles.permitEntryToFirstInQueue();   //Permits entry to the first vehcile in the queue (FIFO)
    }
  }
}

bool Intersection::trafficLightIsGreen()
{
  return _trafficLight.getCurrentPhase() == TrafficLightPhase::green;
}
