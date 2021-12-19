# Traffic_Simulator
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) [![macOS](https://svgshare.com/i/ZjP.svg)](https://svgshare.com/i/ZjP.svg) [![Atom](https://badgen.net/badge/icon/atom?icon=atom&label)](https://atom.io)

Welcome to the fourth project of Udacity's C++ Nanodegree!

In this project, I finalised the ongoing lesson project "Traffic Simulator". As the final project, I have implemented a message queue class and traffic lights in each intersection to make the simulator more realistic.

<p align="center"><img src="data/traffic_simulation.gif" width="400" height="200" alt="Image of the Traffic Simulator"/></p>

## Environment
👉 macOS 12.1 Monterey with Intel Core i7 chip  
👉 Homebrew 3.3.5  
👉 cMake 3.21.3  
👉 GNU Make 3.81  
👉 C++ 17  
👉 Apple clang version 13.0.0  
👉 OpenCV 4.1.0  

## Compiling and Running
### Compile
Create a `build` folder and change to that directory
```
mkdir build && cd build
```

Within the build directory run
```
cmake ..
make
```

### Running
The executable is within the `build` directory. You can run it as follows
```
./traffic_simulation
```
