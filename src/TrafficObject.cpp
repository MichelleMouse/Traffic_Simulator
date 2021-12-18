#include <iostream>
#include <chrono>
#include "TrafficObject.h"

//Init static variables
int TrafficObject::_idCnt = 0;
std::mutex TrafficObject::_mtxCout;

void TrafficObject::setPosition(double x, double y)
{
  _posX = x;
  _posY = y;
}

void TrafficObject::getPosition(double &x, double &y)
{
  x = _posX;
  y = _posY;
}

TrafficObject::TrafficObject()
{
  _type = ObjectType::noObject;
  _id = _idCnt++;
}

TrafficObject::~TrafficObject()
{
  //Sets a thread barrier before object is destroyed
  std::for_each(_threads.begin(), _threads.end(), [](std::thread &thread){
    thread.join();
  });
}
