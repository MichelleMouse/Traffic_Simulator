#include <iostream>
#include <random>
#include <mutex>
#include "Street.h"
#include "Intersection.h"
#include "Vehicle.h"

Vehicle::Vehicle()
{
  _currStreet = nullptr;
  _posStreet = 0.0;
  _type = ObjectType::objectVehicle;
  _speed = 400; // m/s
}

void Vehicle::setCurrentDestination(std::shared_ptr<Intersection> destination)
{
  //Updates destination
  _currDestination = destination;

  //Resets simulation parameters
  _posStreet = 0.0;
}

//Launches drive function in a thread
void Vehicle::simulate()
{
  _threads.emplace_back(std::thread(&Vehicle::drive, this));
}

//Virtual function which is executed in a thread
void Vehicle::drive()
{
  std::unique_lock<std::mutex> lck(TrafficObject::_mtxCout);

  //Prints id of the current thread
  std::cout << "Vehicle #" << _id << "::drive: thread id = " << std::this_thread::get_id() << std::endl;
  lck.unlock();

  //Initalises variables
  bool hasEnteredIntersection = false;
  double cycleDuration = 1; // duration of a single simulation cycle in ms
  std::chrono::time_point<std::chrono::system_clock> lastUpdate;

  //Init stop watch
  lastUpdate = std::chrono::system_clock::now();
  while (true)
  {
    //Sleeps at every iteration to reduce CPU usage
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    //Computes time difference to stop watch
    long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();

    if (timeSinceLastUpdate >= cycleDuration)
    {
      //Updatse position with a constant velocity motion model
      _posStreet += _speed * timeSinceLastUpdate / 1000;

      //Computes completion rate of current street
      double completion = _posStreet / _currStreet->getLength();

      //Computes current pixel position on street based on driving direction
      std::shared_ptr<Intersection> i1, i2;
      i2 = _currDestination;
      i1 = i2->getID() == _currStreet->getInIntersection()->getID() ? _currStreet->getOutIntersection() : _currStreet->getInIntersection();

      double x1, y1, x2, y2, xv, yv, dx, dy, l;
      i1->getPosition(x1, y1);
      i2->getPosition(x2, y2);
      dx = x2 - x1;
      dy = y2 - y1;
      l = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (x1 - x2));
      xv = x1 + completion * dx; //New position based on line equation in parameter form
      yv = y1 + completion * dy;
      this->setPosition(xv, yv);

      //Checks wether halting position in front of destination has been reached
      if (completion >= 0.9 && !hasEnteredIntersection)
      {
        //Starts a task to request entry to the current intersection
        std::future<void> task = std::async(&Intersection::addVehicleToQueue, _currDestination, get_shared_this());

        //Waits until the entry has been granted
        task.get();

        //Slows down and set intersection flag
        _speed /= 10.0;
        hasEnteredIntersection = true;
      }

      //Checks wether intersection has been crossed
      if (completion >= 1.0 && hasEnteredIntersection)
      {
        //Chooses next street and destination
        std::vector<std::shared_ptr<Street>> streetOptions = _currDestination->queryStreets(_currStreet);
        std::shared_ptr<Street> nextStreet;
        if (streetOptions.size() > 0)
        {
          //Picks one street at random and query intersection to enter this street
          std::random_device rd;
          std::mt19937 eng(rd());
          std::uniform_int_distribution<> distr(0, streetOptions.size() - 1);
          nextStreet = streetOptions.at(distr(eng));
        }
        else
        {
          //This street is a dead-end, so drive back the same way
          nextStreet = _currStreet;
        }

        //Picks the one intersection at which the vehicle is currently not
        std::shared_ptr<Intersection> nextIntersection = nextStreet->getInIntersection()->getID() == _currDestination->getID() ? nextStreet->getOutIntersection() : nextStreet->getInIntersection();

        //Sends signal to intersection that vehicle has left the intersection
        _currDestination->vehicleHasLeft(get_shared_this());

        //Assigns new street and destination
        this->setCurrentDestination(nextIntersection);
        this->setCurrentStreet(nextStreet);

        //Resets speed and intersection flag
        _speed *= 10.0;
        hasEnteredIntersection = false;
      }

      //Resets stop watch for next cycle
      lastUpdate = std::chrono::system_clock::now();
    }
  } //EOF simulation loop
}
