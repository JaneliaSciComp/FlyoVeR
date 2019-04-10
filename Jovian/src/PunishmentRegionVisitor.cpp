#include "PunishmentRegionVisitor.h"

#include <iostream>
PunishmentRegionVisitor::PunishmentRegionVisitor() 
	: baselineValue(0)
	, _reinforcementState(PRIMED)
{
    node_regexp = boost::regex(".+_[picrm]*z([0-9]*)[picrm]*_?$"); // Only "reinforcement zone" scene items, with an "z"
	reinforcementCategories[""] = ReinforcementCategory(""); // seed initial empty-string category
	_reinforcement_timer.start();
}

/* virtual */
CollisionResult PunishmentRegionVisitor::collide(osg::Vec2 subject_location) const {
    float minDistance = 100.00; // upper limit on closest approach, for numerical stability
    float minCenterDistance = 1000.00; // used for reinforcement regions
    const double epsilon = 0.001; // allow animal to penetrate this absolute depth before casting it back to obstacle edge

    // Check each obstacle, one at a time.
    const CrudeVRObstacle * collidedObstacle = NULL;
    for (std::vector<CrudeVRObstacle>::const_iterator
        i = relevant_obstacles.begin();
        i != relevant_obstacles.end(); i++)
    {
        // Compute distance from animal to center of zone
        const CrudeVRObstacle& ob = *i;
        osg::Vec2 dv = subject_location - ob.location; // vector to obstacle center
        double centerDist = dv.length(); // distance to center of obstacle

        // Choose the reinforcement zone with the closest outer radius
        const ReinforcementCategory& rc = reinforcementCategories.find(ob.category_label)->second;
        float outerRadiusPlus = rc.outerRadius + rc.timeoutResetRadius; // Include additional buffer zone in collision check
        float edgeDist = centerDist - outerRadiusPlus; // signed distance to outer edge of zone
        if (edgeDist >= minDistance)   
            continue; // some other zone was closer

        minDistance = std::min(edgeDist, minDistance); // negative value means penetration
        minCenterDistance = centerDist;

        if (edgeDist > 0)
            continue; // we are not actually in this zone

        collidedObstacle = &ob;
    }


	CollisionResult result;
	result.decollidedXY = subject_location;
	result.minDistance = minDistance;
	result.minCenterDistance = minCenterDistance;
	result.collidedObstacle = collidedObstacle;
	return result;

}

int PunishmentRegionVisitor::reinforcementAtXy(double x, double y) const
{
	osg::Vec2 wl2(x, y);
	CollisionResult collision = collide(wl2); // absolute XY location after collision
	// Precompute reinforcement value, so Arduino does not need to calculate much
	float minPunishmentDistance = collision.minCenterDistance; // distance from animal to center of zone

	// Set reinforcment value to a constant, outside of punishment region
	if (collision.collidedObstacle == nullptr) 
	{
		// Animal is definitely outside the reinforcement zone, so restart the timeout
		_reinforcement_timer.start();
		_reinforcementState = PRIMED;
		return baselineValue;
	}

	const std::string& rcLabel = collision.collidedObstacle->category_label;
	const ReinforcementCategory& category = reinforcementCategories.find(rcLabel)->second;
    
       
	// New timed reinforcement July 2016
    if (category.getReinforcementDuration() > 0.0) // time-based reinforcment
    {

        // 1) First identify the animal location relative to the reinforcement zone
        AnimalLocation animalLocation = NO_ZONE;
        if (minPunishmentDistance > (category.outerRadius + category.timeoutResetRadius)) {
            animalLocation = NO_ZONE;
        }
        else if (minPunishmentDistance < (category.innerRadius - category.timeoutResetRadius)) {
            animalLocation = NO_ZONE;
        }
        else if (minPunishmentDistance > category.outerRadius) {
            animalLocation = ZONE_BUFFER;
        }
        else if (minPunishmentDistance < category.innerRadius) {
            animalLocation = ZONE_BUFFER;
        }
        else {
            animalLocation = ZONE_CORE;
        }

		// 2) Next update reinforcement state based on previous state and current location
		if (animalLocation == NO_ZONE) {
			_reinforcement_timer.start();
			_reinforcementState = PRIMED; // animal is outside, and ready to come in for more
		}
		else if ((_reinforcement_timer.elapsed() / 1000.0) > category.getReinforcementDuration()) {
			_reinforcementState = EXHAUSTED; // animal overstayed it's reinforcement duration
		}
		else if ((animalLocation == ZONE_CORE) && (_reinforcementState == PRIMED)) {
            _reinforcement_timer.start();
			_reinforcementState = REINFORCING; // animal begins getting reinforced right now
		}
        else if ((animalLocation == ZONE_BUFFER) && (_reinforcementState == PRIMED)) {
            _reinforcement_timer.start();
        }
		else {} // continue whatever state we had before (continue EXHAUSTED or REINFORCING)

		// 3) Finally, return the reinforcement value
		if (_reinforcementState == REINFORCING)
			return category.innerValue;
		else
			return baselineValue;
	}
	// Non-timed reinforcement below, uses ramped reinforcement over zone
	else { // position-based reinforcement
		double outerRadius = category.outerRadius;
		double innerRadius = category.innerRadius;
		double innerValue = category.innerValue;
		double gammaExponent = category.gammaExponent;
		int reinforcementValue = baselineValue;

		if (minPunishmentDistance < outerRadius) { // Is animal inside the reinforcement zone?
			if (minPunishmentDistance < innerRadius) { // Is animal inside the maximum reinforcement zone?
				reinforcementValue = innerValue;
			}
			else { // Animal is in a zone of intermediate reinforcement
					// How far into the zone is the animal? (range 0-1)
				double relativeDistance = (minPunishmentDistance - innerRadius)
					/ (outerRadius - innerRadius);
				assert(relativeDistance <= 1);
				assert(relativeDistance >= 0);
				// TODO: support other function types, besides gamma power law.
				double floatReinforcementValue =
					baselineValue +
					(innerValue - baselineValue)
					* std::pow(1.0 - relativeDistance, gammaExponent);
				// Convert to nearest integer:
				reinforcementValue = int(std::floor(floatReinforcementValue + 0.5)); // "floor" extends rounding correctness to negative values
			}
		}
		return reinforcementValue;
	}

}

