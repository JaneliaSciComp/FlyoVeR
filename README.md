# FlyoVeR

FlyoVeR is a virtual reality tool for displaying virtual environments to drosophila for behaviorial and other scientific experiments.  It includes three subprojects.

  - FlyOver provides a wrapper around Jovian as well as cmake build targets for versioning and creating installers.
  - Jovian provides the core functionality for managing the virtual reality environment, including driving the display, updating the position of the animal in the virtual reality world, and sending out reinforcement triggers to external behavior control through a serial interface.
  - Remote Data Server provides an interface to translate camera capture information into location and heading data that is fed into Jovian.


## Using Software

The easiest way to use the software is run the FlyOver and RemoteDataServer binaries located [HERE](https://github.com/JaneliaSciComp/FlyoVeR/tree/master/binaries).  These have been compiled for x64 architecture and tested on Windows 7.  

## Building Software

If you would like to modify the software, follow the README files in each subproject for more details about building the software.  It is highly recommended that you use Visual Studio 2015 or more recent as your development environment, since cmake will produce a Visual Studio Solution to simplify the build process. Dependencies for this project include:
  - Boost 1.58
  - Collada DOM
  - OpenSceneGraph
  - OSGBullet
  - OSGWorks
  - Qt5
  - Bullet Physics

## License 

[Janelia Open Source License](https://www.janelia.org/open-science/software-licensing)
