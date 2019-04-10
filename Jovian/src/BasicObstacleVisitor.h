#ifndef BASIC_OBSTACLE_VISITOR_H__
#define BASIC_OBSTACLE_VISITOR_H__

#include <osg/NodeVisitor>
#include <vector>
#include <string>
#include <boost/regex.hpp>

class CrudeVRObstacle {
public:
	enum ExclusionZone {INSIDE, OUTSIDE}; // Distinguished scene obstacles like cones and cylinders, from world-outer-wall obstacle.

	CrudeVRObstacle() 
		: name("")
		, xy_radius(0)
		, location(0, 0)
		, exclusionZone(INSIDE)
		, z_min(0)
		, z_max(0)
		, node(NULL)
		, d_threshold(0)
		, category_label("")
	{}

	std::string name;
	float xy_radius; // for cyclindrical exclusion area
	osg::Vec2 location; // location in space
	ExclusionZone exclusionZone; // foreground/background distinction
	float z_min, z_max; // for excluding floor
	osg::Node* node; // just in case
	double d_threshold; // precomputed distance threshold for intersection
	std::string category_label; // For individually tuning reinforcement zones
};

struct CollisionResult {
	osg::Vec2 decollidedXY;
	float minDistance; // distance to closest obstacle edge
	float minCenterDistance; // distance to closest obstace center
	const CrudeVRObstacle * collidedObstacle;
};

/**
 * BasicObstacleVisitor class intended to support crude non-bullet collision prevention
 */
class BasicObstacleVisitor : public osg::NodeVisitor
{
public:
	BasicObstacleVisitor();

	// "apply" is used to traverse the scene graph, to identify all scene obstacles
	// @Override
	virtual void apply(osg::Node & node);
	void cullObstacles(); // populate relevant_obstacles with those obstacles that could ever intersect subject; i.e. not _Plane_p_
	virtual CollisionResult collide(osg::Vec2 subject_location) const; // change location to avoid penetration, if necessary

// protected:
	std::vector<CrudeVRObstacle> all_obstacles;
	std::vector<CrudeVRObstacle> relevant_obstacles;
	CrudeVRObstacle subject;

protected:
	boost::regex node_regexp; // ( ".*_p([im]?)(r?)(c?)_?$" );
};

#endif // BASIC_OBSTACLE_VISITOR_H__
