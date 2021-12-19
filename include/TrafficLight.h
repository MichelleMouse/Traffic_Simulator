#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <condition_variable>
#include <deque>
#include <mutex>
#include "TrafficObject.h"

//Forward declarations to avoid include cycle
class Vehicle;
class TrafficLight;

//Enum for the different states of the traffic lights
enum TrafficLightPhase
{
  red,
  green,
};

//MessageQueue class
template <class T>
class MessageQueue
{
public:
  void send(T &&msg);
  T receive();

private:
  std::deque<T> _queue;
  std::condition_variable _condition;
  std::mutex _mtx;
};

class TrafficLight : public TrafficObject
{
private:
  //Typical behaviour methods
  void cycleThroughPhases();

  //Private memebers
  std::condition_variable _condition;
  std::mutex _mtx;
  TrafficLightPhase _currentPhase;
  MessageQueue<TrafficLightPhase> _messages;

public:
  //Constructors & Destructors
  TrafficLight();
  ~TrafficLight();

  //Getters & Setters
  TrafficLightPhase getCurrentPhase();

  //Typical behaviour methods
  void waitForGreen();
  void simulate();
};

#endif
