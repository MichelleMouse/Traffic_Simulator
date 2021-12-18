#ifndef TRAFFICOBJECT_H
#define TRAFFICOBJECT_H

#include <mutex>
#include <vector>
#include <thread>

enum ObjectType
{
  noObject,
  objectVehicle,
  objectIntersection,
  objectStreet,
};

class TrafficObject
{
public:
  //Constructor & Destructor
  TrafficObject();
  ~TrafficObject();

  //Getters & Setters
  int getID() { return _id; }
  void setPosition(double x, double y);
  void getPosition(double &x, double &y);
  ObjectType getType() { return _type; }

  //Typical behaviour methods
  virtual void simulate(){};

protected:
  ObjectType _type;                   //Identifies the class type
  int _id;                            //Every traffic object has its own unique id
  double _posX, _posY;                //Vehicle position in pixels
  std::vector<std::thread> _threads;  //Holds all threads that have been launched within this object
  static std::mutex _mtxCout;         //Static mutex to protect standard-out
  static std::mutex _mtx;             //Static mutex shared by all traffic objects for protecting

private:
  static int _idCnt;                  //Global variable for counting object ids
};

#endif
