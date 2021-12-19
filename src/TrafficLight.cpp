#include <iostream>
#include <random>
#include "TrafficLight.h"

/*        IMPLEMENTATION OF CLASS MESSAGEQUEUE        */
//Pulls new messages from the queue and retrns the object
template <typename T>
T MessageQueue<T>::receive()
{
  std::unique_lock<std::mutex> uLock(_mtx);
  _condition.wait(uLock, [this] {
    return !_queue.empty();
  });

  T msg = std::move(_queue.back());
  _queue.pop_back();

  return msg;
}

//Adds a new message to the queue and sends a notification
template <typename T>
void MessageQueue<T>::send(T &&msg)
{
  std::lock_guard<std::mutex> gLock(_mtx);

  //Adds vector to queue
  _queue.push_back(std::move(msg));
  _condition.notify_one();  //Notifies client after pushing new Vehicle into vector
}

/*        IMPLEMENTATION OF CLASS TRAFFICLIGHT        */
TrafficLight::TrafficLight()
{
  _currentPhase = TrafficLightPhase::red;
}

//Infinite loop to wait for the green light
void TrafficLight::waitForGreen()
{
  // std::lock_guard<std::mutex> uLock(_mtx);
  while(true)
  {
    if(_messages.receive() == TrafficLightPhase::green) { return; }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));  //Reduces the load in the procesor
  }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
  return _currentPhase;
}

//Starts the private method cycleThroughPhases in a thread
void TrafficLight::simulate()
{
  _threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

//Measures the time between two loop cycles and toggles the current phase of the traffic light
void TrafficLight::cycleThroughPhases()
{
  std::unique_lock<std::mutex> uLock(_mtx);
  auto cycleStart = std::chrono::system_clock::now();  //Start time for the loop
  int cycleDuration = rand() % 3000 + 4000;       //Random duration between 4000 and 6000 milliseconds

  while(true)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));  //Reduces the load in the procesor

    long diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - cycleStart).count();

    if(diff >= cycleDuration)
    {
      //Toggles the current phase
      if(_currentPhase == TrafficLightPhase::green)
      {
        _currentPhase = TrafficLightPhase::red;
      } else {
        _currentPhase = TrafficLightPhase::green;
      }
      _messages.send(std::move(_currentPhase));

      cycleStart = std::chrono::system_clock::now();   //Updates the start time of the cycle
      cycleDuration = rand() % 3000 + 4000;       //Gets a new random time
    }
  }
}
