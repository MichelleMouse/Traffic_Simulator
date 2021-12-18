#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include "TrafficObject.h"

class Graphics
{
public:
  //Constructor & Destructor

  //Getters & Setters
  void setBgFilename(std::string filename) { _bgFilename = filename; }
  
  void setTrafficObjects(std::vector<std::shared_ptr<TrafficObject>> &trafficObjects)
  {
    _trafficObjects = trafficObjects;
  };

  //Typical behaviour methods
  void simulate();

private:
  //Typical behaviour methods
  void loadBackgroundImg();
  void drawTrafficObjects();

  //Member variables
  std::vector<std::shared_ptr<TrafficObject>> _trafficObjects;
  std::string _bgFilename;
  std::string _windowName;
  std::vector<cv::Mat> _images;
};

#endif
