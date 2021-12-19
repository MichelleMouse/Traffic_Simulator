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

// FP.3 Define a class „MessageQueue“ which has the public methods send and receive.
// Send should take an rvalue reference of type TrafficLightPhase whereas receive should return this type.
// Also, the class should define an std::dequeue called _queue, which stores objects of type TrafficLightPhase.
// Also, there should be an std::condition_variable as well as an std::mutex as private members.

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
  // FP.4b : create a private member of type MessageQueue for messages of type TrafficLightPhase
    // and use it within the infinite loop to push each new TrafficLightPhase into it by calling
    // send in conjunction with move semantics.

  std::condition_variable _condition;
  std::mutex _mtx;
  TrafficLightPhase _currentPhase;
  MessageQueue<TrafficLightPhase> _messages;

  void cycleThroughPhases();

public:
  //Constructors & Destructors
  TrafficLight();

  //Getters & Setters
  TrafficLightPhase getCurrentPhase();

  //Typical behaviour methods
  void waitForGreen();
  void simulate();
};

#endif
