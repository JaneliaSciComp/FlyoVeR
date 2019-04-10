Build instructions for Jovian project

This document assumes that we are building under Windows (XP or 7) and using Visual Studio 2010 (VS10).

If building on windows
 - Go to this page: http://www.openscenegraph.org/projects/osg/wiki/Downloads/Dependencies
 - Download the jpeg, tiff, and png dependancies listed for windows on the bottom of the page
   - Download the " Complete package, except sources" setup file
 - Install each package
 - Control Panel > System > Advanced Setting > Environment Variables
 - Edit system environment variable: PATH. Append to the list of values "C:\Program Files\GnuWin32\bin"

**************************
IMPORTANT!!  IMPORTANT!!

If this is a new install, you can safely skip this section and proceed to #1

If you are upgrading an old package, you need to remove both the old build directory and the installation directory.
If you don't then most likely your build will fail.

**************************

1. Install CMake
  - Download latest from http://www.cmake.org/cmake/resources/software.html. 
  - Choose the version labeled "Windows (Win32 Installer)"
  - Install

2. Install Qt
  - Download from http://qt.nokia.com/downloads/windows-cpp-vs2010
  - Follow installation directions
 - Control Panel > System > Advanced Setting > Environment Variable
 - Edit the System PATH environment variable
 - Add the Qt bin directory (typically C:\Qt\<version>\bin)

3. Build Boost
 - Download from http://sourceforge.net/projects/boost/files/boost/1.49.0/
 - Unpack boost
 - Open a command prompt and change to the directory where you unpacked boost
 - run ".\boostrap.bat"
 - run ".\b2.exe --prefix="C:\Program Files\Boost_1.49" --build-type=complete install

4. Build Collada
 - Download from http://sourceforge.net/projects/collada-dom/files/OldFiles/. Select "collada_dom-2.2-src.tgz"
 - Unpack Collada
 - Navigate to collada-dom-2.2 > dom > projects > vc10-1.4
 	- Double click the dom Visual Studio Solution icon to launch VS
	- The toolbar should say 'Debug'; if not select it in the drop down menu
	- Left Click "Solution 'dom' in the Solution Explorer; Then right click and select
	  "Build Solution"
	- When Done, Change Debug to Release and repeat last the step
 - Set the COLLADA_DIR environment variable
	- Control Panel > System > Advanced Setting > Environment Variables
- Create a new System environment variable: COLLADA_DIR. Set the value to the path where collada-dom was unpacked.
 - Edit the System PATH environment variable
 - Add the Collada lib and debug directories (%COLLADA_DIR%\dom\build\vc10-1.4;%COLLADA_DIR%\dom\build\vc10-1.4-d)

5. Build OpenSceneGraph
 - If needed, install 7-zip (WinZip is unable to extract these files)
	- Using 7-zip, it's a two step process, first to uncompress, the second to extract.
 - Unpack the OpenSceneGraph-Data from the external_libraries directory in Jovian
 - Unpack OpenSceneGraph from the same directory
 - Run CMake
	- Set "Browse Source" to the unpacked OpenSceneGraph directory
	- Set "Browse Build" to a new directory OpenSceneGraph under build
	- Press Configure
	- When done, Select "Build_OSG_Examples"
	- Select Advanced
	  - Search for JPEG.
	  - Set JPEG_INCLUDE to C:\Program Files\GnuWin32\include
	  - Set JPEG_LIBRARY to C:\Program Files\GnuWin32\lib\jpeg.lib
	  - Search TIFF
	  - Set TIFF_INCLUDE to C:\Program Files\GnuWin32\include
	  - Set TIFF_LIBRARY to C:\Program Files\GnuWin32\lib\libtiff.lib
	- Configure twice more then Generate
 - Goto the OpenSceneGraph build directory
	- Double click the OpenSceneGraph Visual Studio Solution icon to launch VS
	- The toolbar should say 'Debug'; if not select it in the drop down menu
	- Left Click "ALL_BUILD" in the Solution Explorer; Then right click and select Build
	- When Done, Scroll the Solution Explorer until you see INSTALL, then select, 
	  Right Click and build.
	- When Done, Change Debug to Release and repeat last two steps
 - Set the OSG_DIR environment variable
	- Control Panel > System > Advanced Setting > Environment Variables
	- Create a new user environment variable: OSG_DIR. Set the value to the path
	  where OpenSceneGraph was installed (On Windows typically, "C:\Program Files",
	  on Linux and Mac "/usr/local").
 - Quit Visual Studio
 - Control Panel > System > Advanced Setting > Environment Variable
 - Edit the System PATH environment variable
 - Add the OpenSceneGraph bin directory (typically C:\Program File\OpenSceneGraph\bin)
 - Add User Environment variable OSG_FILE_PATH to the unpacked OpenSceneGraph-Data directory

6. Build Bullet
 - If needed, install 7-zip (WinZip is unable to extract these files)
	- Using 7-zip, it's a two step process, first to uncompress, the second to extract.
 - Unpack bullet-<version> from the external_libraries directory in Jovian
 - Run CMake
	- Set "Browse Source" to the unpacked bullet-<version>
           directory ( the bullet source may have extra cruft such as
           revision number in the archive file. That information will
           not show up in the directory name, e.g., if the archive is
           "bullet-2.79-rev2440.tgz", then the directory will be
           "bullet-2.79"). 
	- Set "Browse Build" to a new directory bullet under build
	- Press Configure
	- When done, Select "INSTALL_EXTRA_LIBS" and "INSTALL_LIBS"
	- Configure twice more, then Generate
 - Goto the bullet build directory
	- Double click the bullet Visual Studio Solution icon to launch VS
	- The toolbar should say 'Debug'; if not select it in the drop down menu
	- Left Click "ALL_BUILD" in the Solution Explorer; Then right click and select Build
	- When Done, Scroll the Solution Explorer until you see INSTALL, then select, 
	  Right Click and build.
	- When Done, Change Debug to Release and repeat last two steps
 - Quit Visual Studio

7. Build osgWorks
 - If needed, install 7-zip (WinZip is unable to extract these files)
	- Using 7-zip, it's a two step process, first to uncompress, the second to extract.
 - Unpack osgworks from the external_libraries directory in Jovian
 - Run CMake
	- Set "Browse Source" to the unpacked osgworks directory
	- Set "Browse Build" to a new directory osgworks under build
	- Configure twice, then Generate
 - Goto the bullet build directory
	- Double click the bullet Visual Studio Solution icon to launch VS
	- The toolbar should say 'Debug'; if not select it in the drop down menu
	- Left Click "ALL_BUILD" in the Solution Explorer; Then right click and select Build
	- When Done, Scroll the Solution Explorer until you see INSTALL, then select, 
	  Right Click and build.
	- When Done, Change Debug to Release and repeat last two steps
 - Quit Visual Studio
 - Control Panel > System > Advanced Setting > Environment Variable
 - Edit the System PATH environment variable
 - Add the osgWorks bin directory (typically C:\Program File\osgWorks\bin)

8. Build osgBullet
 - If needed, install 7-zip (WinZip is unable to extract these files)
	- Using 7-zip, it's a two step process, first to uncompress, the second to extract.
 - Unpack osgbullet from the external_libraries directory in Jovian
 - Run CMake
	- Set "Browse Source" to the unpacked osgbullet directory
	- Set "Browse Build" to a new directory osgbullet under build
	- Configure
	- In all likely hood, CMake will barf an error message about not finding BULLET_INCLUDE_DIR. Check Advanced. Then set BULLET_INCLUDE_DIR to the bullet include in the install (C:\Program Files\BULLET_PHYSICS\include\bullet)
	- Check for any missing Bullet Debug libraries and set those missing manually
	- Configure twice, then Generate
 - Goto the bullet build directory
	- Double click the bullet Visual Studio Solution icon to launch VS
	- The toolbar should say 'Debug'; if not select it in the drop down menu
	- Left Click "ALL_BUILD" in the Solution Explorer; Then right click and select Build
	- When Done, Scroll the Solution Explorer until you see INSTALL, then select, 
	  Right Click and build.
	- When Done, Change Debug to Release and repeat last two steps
 - Quit Visual Studio
 - Control Panel > System > Advanced Setting > Environment Variable
 - Edit the System PATH environment variable
 - Add the osgBullet bin directory (typically C:\Program File\osgBullet\bin)

9. Build Jovian
 - Create a Jovian directory under build
 - Run CMake
	- Set "Browse Source" to the Jovian src directory
	- Set "Browse Build" to a new directory Jovian under build/Jovian
	- Press Configure
	- Most likely, it will give you an error about not finding Boost_INCLUDE_DIR
	- Set Boost_ROOT to C:/Program Files/Boost_1.49
	- Configure twice more then Generate
 - Goto the Jovian build directory
	- Double click the Jovian Visual Studio Solution icon to launch VS
	- The toolbar should say 'Debug'; if not select it in the drop down menu
	- Left Click "Solution Jovian in the Solution Explorer; Then right click and select Build

