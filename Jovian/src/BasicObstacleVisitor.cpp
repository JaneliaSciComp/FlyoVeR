#include "BasicObstacleVisitor.h"

#include <osg/Geode>
#include <osg/BoundingBox>
#include <osg/ComputeBoundsVisitor>

#include <iostream>

using namespace std;

BasicObstacleVisitor::BasicObstacleVisitor() 
	: osg::NodeVisitor(
			osg::NodeVisitor::TRAVERSE_ALL_CHILDREN, 
			osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
	, node_regexp(".*_p([im]?)(r?)(c?)_?$")
{}


/* virtual */
void BasicObstacleVisitor::apply(osg::Node & node) 
{

	// std::cerr << "COV Visited Node: " << node.getName() << std::endl;
	osg::ComputeBoundsVisitor cbv;
	osg::BoundingBox &bb(cbv.getBoundingBox());
	node.accept(cbv);

	float center_x = 0.5 * (bb.xMax() + bb.xMin());
	float extent_x = bb.xMax() - bb.xMin();
	float center_y = 0.5 * (bb.yMax() + bb.yMin());
	float extent_y = bb.yMax() - bb.yMin();

	// measure vertices at animal height, and compute radius, center, in X, Y, and Z
	// std::cerr << center_x << ", " << center_y << ", " << center_z << std::endl;
	// std::cerr << extent_x << ", " << extent_y << ", " << extent_z << std::endl;

	float xy_radius = 0.5 * std::max(extent_x, extent_y);
	// std::cerr << node.getName();
	// std::cerr << " ; center = ( " << center_x << ", " << center_y << ", " << center_z << " )";
	// cerr << " ; radius = " << xy_radius << ", " << 0.5 * extent_z;
	// cerr << endl;

	// Populate obstacle object
	CrudeVRObstacle obstacle;
	obstacle.name = node.getName();
	obstacle.xy_radius = xy_radius;
	obstacle.location = osg::Vec2(center_x, center_y);
	obstacle.z_min = bb.zMin();
	obstacle.z_max = bb.zMax();
	obstacle.node = &node;
	obstacle.exclusionZone = CrudeVRObstacle::INSIDE;
	std::string name = node.getName();
	// boost::regex physics_regexp( ".*_p([im]?)(r?)(c?)_?$" );
	boost::smatch what;
	boost::regex convex_regexp( ".+_[pirm]*c[pirm]*_?$" );
	bool matched = boost::regex_match( name, what, convex_regexp );
	// Assume non-convex objects ("c") have exterior exclusion, like outer walls of arena
	if (matched) {
		// std::cout << "Concave!" << std::endl;
		obstacle.exclusionZone = CrudeVRObstacle::OUTSIDE;
	}
	else {
		// std::cout << "Convex!" << std::endl;
	}

	matched = boost::regex_match( name, what, node_regexp );
	if (node.getName() == "_camera_block_pm_") {
		subject = obstacle;
	}
	// Ignore all nodes that are not "physics" objects;
	// i.e. that lack "..._p..." in their name
	else if ( matched ) 
	{
		std::cout << "Using regexp " << node_regexp.str() << std::endl;
		std::cout << "Item ***matched*** " << name << std::endl;
		std::cout << "At location (" << center_x << ", " << center_y << ")" << std::endl;

		// Capture reinforcement category, if any
		if (what.size() == 2) { // currently true for zone regex...
			std::string zone_label = what[1];
			std::cout << "zone label = '" << zone_label << "'" << std::endl;
			obstacle.category_label = zone_label;
		}

		all_obstacles.push_back(obstacle);
	}
	else
	{
		// std::cout << "Skipping item " << name << std::endl;
	}

	
	traverse(node); // keep going down the tree
}


// populate relevant_obstacles with those obstacles that could ever intersect subject; i.e. not _Plane_p_
void BasicObstacleVisitor::cullObstacles() 
{
	relevant_obstacles.clear();

	for ( std::vector<CrudeVRObstacle>::iterator i = all_obstacles.begin();
		i != all_obstacles.end(); i++) 
	{
		CrudeVRObstacle& ob = *i;
		// Ignore obstacles that do not overlap subject in Z
		if (ob.z_min >= subject.z_max) {
			std::cout << "Ignoring obstacle " << ob.name << " for being too high up" << std::endl;
			continue;
		}
		if (ob.z_max <= subject.z_min) {
			std::cout << "Ignoring obstacle " << ob.name << " for being too low down" << std::endl;
			continue;
		}
		// Precompute distance cutoff
		double d = subject.xy_radius + ob.xy_radius;
		ob.d_threshold = d;
		std::cout << "Obstacle " << ob.name << " threshold distance = " << d << std::endl;
		relevant_obstacles.push_back(ob);
	}
}

// Compute a legal place closest to a potentially illegal place.
CollisionResult BasicObstacleVisitor::collide(osg::Vec2 subject_location) const
{
	osg::Vec2 current_location = subject_location;
	float minDistance = 100.00; // upper limit on closest approach, for numerical stability
	float minCenterDistance = 1000.00; // used for reinforcement regions
	const double epsilon = 0.001; // allow animal to penetrate this absolute depth before casting it back to obstacle edge

	// Check each obstacle, one at a time.
	const CrudeVRObstacle * collidedObstacle = NULL;
	for ( std::vector<CrudeVRObstacle>::const_iterator 
		i = relevant_obstacles.begin();
		i != relevant_obstacles.end(); i++) 
	{
		const CrudeVRObstacle& ob = *i;
		osg::Vec2 dv = current_location - ob.location; // vector to obstacle center
		double centerDist = dv.length(); // distance to center of obstacle
		if (centerDist < minCenterDistance)
			minCenterDistance = centerDist;
		float edgeDist = centerDist - ob.d_threshold; // signed distance to edge of obstacle
		if (ob.exclusionZone == ob.OUTSIDE)
			edgeDist = -edgeDist; // reverse sense of penetration for concave outer obstacle, like arena wall
		minDistance = std::min(edgeDist, minDistance); // negative value means penetration
		if (edgeDist > -epsilon)
			continue; // not a (deep) penetration
		// cerr << "subject penetrated obstacle " << ob.name << endl;
		// cerr << "obstacle loc = (" << ob.location.x() << ", " << ob.location.y() << ") r = " << ob.xy_radius << endl;
		// cerr << "player loc = (" << current_location.x() << ", " << current_location.y() << ")" << endl;
		// If we get this far, this is a pentration event.
		// Find the closest point on the penetration radius
		if (centerDist > 0) { // avoid divide by zero
			double scale = ob.d_threshold / centerDist;
			current_location = ob.location + dv * scale;
			// cerr << "scale = " << scale << endl;
			// cerr << "ob.d_threshold = " << ob.d_threshold << endl;
		}
		else {
			// some dumb convention for rare case when subject is at exact center of obstacle
			current_location = ob.location + osg::Vec2(ob.d_threshold, 0);
		}
		collidedObstacle = &ob;
		// cerr << "after collision (" << current_location.x() << ", " << current_location.y() << ")" << endl;
	}

	CollisionResult result;
	result.decollidedXY = current_location;
	result.minDistance = minDistance;
	result.minCenterDistance = minCenterDistance;
	result.collidedObstacle = collidedObstacle;
	return result;
}
