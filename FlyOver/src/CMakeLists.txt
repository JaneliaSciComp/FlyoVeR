SET(TARGET_SRC 
    main.cpp
)

INCLUDE_DIRECTORIES( 
  ${Jovian_SOURCE_DIR}/src 
  ${OSG_INCLUDE_DIR} 
  ${Boost_INCLUDE_DIRS}
)

IF (CMAKE_BUILD_TYPE MATCHES "Debug")
  SET(TARGET_EXTERNAL_LIBRARIES   
    ${JOVIAN_LIBRARIES_DEBUG}
    ${OSGBULLET_LIBRARIES_DEBUG}
    ${OSGWORKS_LIBRARIES_DEBUG}
    ${OPENSCENEGRAPH_LIBRARIES_DEBUG}
    ${BULLET_LIBRARIES_DEBUG}
  )
else()
  SET(TARGET_EXTERNAL_LIBRARIES   
    ${JOVIAN_LIBRARIES}
    ${OSGBULLET_LIBRARIES}
    ${OSGWORKS_LIBRARIES}
    ${OPENSCENEGRAPH_LIBRARIES}
    ${BULLET_LIBRARIES}
  )
endif()

if(DEFINED ENV{QT_BIN_DIR})
   set(QT_BIN_DIR "$ENV{QT_BIN_DIR}")
else()
   get_filename_component(QT_BIN_DIR "${QT_QMAKE_EXECUTABLE}" PATH)
endif()

if( QT4_FOUND )
  # message("Using Qt4")
  ADD_DEFINITIONS(-DUSE_QT4)
  INCLUDE_DIRECTORIES( 
    ${QT_INCLUDE_DIR} 
    ${QT_QTCORE_INCLUDE_DIR} 
    ${QT_QTGUI_INCLUDE_DIR} 
    ${QT_QTOPENGL_INCLUDE_DIR} 
    ${QT_WIDGETS_INCLUDE_DIR}
  )
  list(APPEND TARGET_EXTERNAL_LIBRARIES
    ${QT_QTCORE_LIBRARY} 
    ${QT_QTGUI_LIBRARY} 
    ${QT_QTOPENGL_LIBRARY}
  )
  file(GLOB QT_SHARED_LIBS 
    "${QT_BIN_DIR}/QtCore*.dll" 
    "${QT_BIN_DIR}/QtGui*.dll"
    "${QT_BIN_DIR}/QtOpenGL*.dll"
  )
else()
  # message("Using Qt5")
  INCLUDE_DIRECTORIES( 
    ${Qt5Widgets_INCLUDE_DIRS} 
    ${Qt5Gui_INCLUDE_DIRS} 
    ${Qt5OpenGL_INCLUDE_DIRS} 
    ${Qt5PrintSupport_INCLUDE_DIRS}
  )
  list(APPEND TARGET_EXTERNAL_LIBRARIES
    ${Qt5Core_LIBRARIES} 
    ${Qt5Widgets_LIBRARIES} 
    ${Qt5Gui_LIBRARIES} 
    ${Qt5OpenGL_LIBRARIES}
    ${Qt5PrintSupport_LIBRARIES}
  )
  file(GLOB QT_SHARED_LIBS 
    "${QT_BIN_DIR}/Qt5Widgets.dll" 
    "${QT_BIN_DIR}/Qt5Core.dll" 
    "${QT_BIN_DIR}/Qt5Gui.dll"
    "${QT_BIN_DIR}/Qt5OpenGL.dll"
  )
  if(MSVC)
    # Special rule for platforms/qwindows.dll
    install(PROGRAMS "${QT_BIN_DIR}/../plugins/platforms/qwindows.dll" DESTINATION bin/platforms)
  endif()
endif()

# message(${QT_SHARED_LIBS})

#### end var setup  ###
# INCLUDE(OsgMacroUtils)
# SETUP_EXAMPLE(FlyOver)

# Build rules
add_executable(FlyOver main.cpp)
TARGET_LINK_LIBRARIES(FlyOver
   ${Boost_LIBRARIES} 
   ${CARBON_LIBRARY} 
   ${COCOA_LIBRARY} 
   ${OPENGL_gl_LIBRARY}
   ${TARGET_EXTERNAL_LIBRARIES}
)

# Install rules
install(TARGETS FlyOver DESTINATION bin)
# We need to find all requied DLL files for the cpack installer
# Install required shared libraries
# OSG Bullet
file(GLOB OSGBULLET_SHARED_LIBS 
  "${OSGBULLET_INCLUDE_DIR}/../bin/osgb*.dll")
# Open Scene Graph
file(GLOB OSG_SHARED_LIBS 
  "${OSG_INCLUDE_DIR}/../bin/osg*-osg*.dll"
  "${OSG_INCLUDE_DIR}/../bin/ot*.dll"
  # "${OSG_INCLUDE_DIR}/../bin/*/osgdb*.dll" # plugins
  )
# OSG Works
file(GLOB OSGWORKS_SHARED_LIBS 
  "${OSGWORKS_INCLUDE_DIR}/../bin/osg*.dll"
  )
# QT - was handled in earlier Qt4/Qt5 stanza
# zlib1.dll etc.
find_path(GNUNWIN32_BIN_DIR
  zlib1.dll
  PATHS
    "C:/Program Files (x86)/GnuWin32/bin"
  )
# TODO - I'm too lazy to figure out right now which gnu libraries we actually need.
file(GLOB GNUWIN32_SHARED_LIBS "${GNUNWIN32_BIN_DIR}/*.dll")

find_path(COLLADA_BUILD_DIR
  collada-dom2.4-sp-vc100-mt.dll
  PATHS
    "C:/Program Files (x86)/collada-dom2.4vs2015x86/bin"
  )
file(GLOB COLLADA_SHARED_LIBS "${COLLADA_BUILD_DIR}/*.dll")

# Boost shared libraries needed by collada-dom
set(BOOST_SHARED_LIBS "")
set(BOOST_DLL_DIR "${BOOST_ROOT}/lib") 
foreach(BOOST_LIB ${Boost_SYSTEM_LIBRARY_RELEASE} ${Boost_FILESYSTEM_LIBRARY_RELEASE})
  # Construct DLL name from LIB name
  if (MSVC)
    get_filename_component(BOOST_LIB_ROOT ${BOOST_LIB} NAME_WE)
    set(SHLIB_NAME ${BOOST_LIB_ROOT}.dll)
    string(REGEX REPLACE "^lib" "" SHLIB_NAME ${SHLIB_NAME})
    set(SHLIB_NAME "${BOOST_DLL_DIR}/${SHLIB_NAME}")
    #message(${SHLIB_NAME})
    list(APPEND BOOST_SHARED_LIBS ${SHLIB_NAME})
  endif()
endforeach()

# libxml2 needed by collada-dom
find_file(LIBXML2_DLL
  libxml2.dll
  PATHS "C:/Users/brunsc/git/libxml2/_build_x64_static_release_mt/bin"
)
message(${LIBXML2_DLL})

foreach(LIB ${OSGBULLET_SHARED_LIBS} ${OSG_SHARED_LIBS} ${OSGWORKS_SHARED_LIBS} 
    ${GNUWIN32_SHARED_LIBS} ${QT_SHARED_LIBS} ${COLLADA_SHARED_LIBS} ${LIBXML2_DLL}
    ${BOOST_SHARED_LIBS})
  install(PROGRAMS "${LIB}" DESTINATION bin)
endforeach()

# Install plugins in a separate folder
file(GLOB OSG_PLUGIN_FOLDERS "${OSG_INCLUDE_DIR}/../bin/osgPlugins*")
foreach(PLUGIN_FOLDER ${OSG_PLUGIN_FOLDERS})
  install(DIRECTORY ${PLUGIN_FOLDER}
    DESTINATION bin
    USE_SOURCE_PERMISSIONS
    FILES_MATCHING PATTERN "*.dll"
    )
endforeach()

# Create releasable installer using CPACK
set(CPACK_PACKAGE_NAME FlyOver)
set(CPACK_PACKAGE_FILE_NAME "InstallFlyOver-${FLYOVER_VERSION}")
set(CPACK_PACKAGE_EXECUTABLES "FlyOver" "FlyOver")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "FlyOver is a closed-loop software virtual reality environment for insects.")
set(CPACK_PACKAGE_VENDOR "Howard Hughes Medical Institute Janelia Research Campus")
set(CPACK_PACKAGE_VERSION "${FLYOVER_VERSION}")
set(CPACK_PACKAGE_CONTACT "brunsc at janelia.hhmi.org")
if (WIN32)
  set(CPACK_GENERATOR NSIS)
  # Install Visual C++ redistributable we used to build all these libraries
  if (MSVC14)
    install(PROGRAMS "${CMAKE_CURRENT_SOURCE_DIR}/../PlatformSpecifics/Windows/msvc2015/vcredist_x86.exe" DESTINATION .)
    set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "ExecWait '\\\"$INSTDIR\\\\vcredist_x86.exe\\\" /passive /norestart' ")
  endif()
endif (WIN32)

# include(CPack) must always be last!
include(CPack)
