Build instructions for FlyOver project

This document assumes that we are building under Windows (7) and using Visual Studio 2015 (VS14).

 - Make sure you have already built the Jovian project before starting this build

 1. Set up CMake Options
 - Point Cmake to FlyOver project folder and press Configure button.
 - Set the following values (if they are not set)
     - JOVIAN_BUILD_DIR (<Jovian>/build)
     - JOVIAN_INCLUDE_DIR (<Jovian>/src)
     - JOVIAN_ROOT (<Jovian>)
     - JOVIAN_SOURCE_DIR (<Jovian>)
 - Press Configure again.  Then Press Generate to create the Visual Studio Project Solution files.
 - Click on the FlyOver solution file in the FlyOver build directory.
 - Make sure you have the environment variable QT_BIN_DIR set to the binary directory of your QT installation.

 Perform the following actions for both Debug and Release Targets
 - Right-click on ALL_BUILD in the Solution Explorer window and select Build 
 - Right-click on INSTALL in the Solution Explorer window and select Build
 - The code should be deployed to C:\Program Files (x86)\FlyOver\bin by default.
 - Clicking on FlyOver.exe should start the FlyOver console.

 NOTE: If there is an issue with missing Qt DLL libraries when starting up the console, copy all the Qt DLLs into the FlyOver/bin directory.
