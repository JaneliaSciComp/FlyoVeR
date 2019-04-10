
#ifndef HUD_H
#define HUD_H

#include <osg/Camera>
#include <osg/Geometry>
#include <osg/Switch>

osg::Camera* createHUD( osg::Switch* switch_node, int numCameras, 
                        int left, int right, int top, int bottom,
                        osg::Vec2 lower_left, int size, int offset );

#endif
