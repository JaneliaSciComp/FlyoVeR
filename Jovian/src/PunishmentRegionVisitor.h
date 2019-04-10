#ifndef PUNISHMENT_REGION_VISITOR_H__
#define PUNISHMENT_REGION_VISITOR_H__

#include "BasicObstacleVisitor.h"
#include "api/IConfig.h"
#include <QElapsedTimer>
#include <map>
#include <string>

class ReinforcementCategory : public IZone
{
private:
	void init(const std::string& categoryName) {
		this->m_categoryName = categoryName;
		innerRadius = 0;
		outerRadius = 10;
		innerValue = 100;
		gammaExponent = 1;
		loiteringTimeout = 100;
		loiteringRadius = 5;
		// New positive reinforcement parameters June 2016
		timeoutResetRadius = 0.0; // relative radius offset
		reinforcementDuration = 0;
		// bMaintainReinforcementTimeoutAfterExit = false;
	}

public:
	ReinforcementCategory(const std::string& categoryName)
	{
		init(categoryName);
	}

	// default constructor required because ReinforcementCategory is used in a map<>
	ReinforcementCategory() {
		init("");
	}

	// IZone interface
	virtual double getReinforcementDuration() const override {return reinforcementDuration;}
	virtual void setReinforcementDuration(double d) override {reinforcementDuration = d;}

	std::string m_categoryName;
	double innerRadius; // reinforcement is constant maxValue within this radius
	double outerRadius; // reinforcement is constant baseline value outside this radius
	int innerValue; // value at inner radius
	double gammaExponent;
	double loiteringTimeout; // seconds of loitering, before being teleported to spawn point
	double loiteringRadius; // distance threshold for what is considered loitering
	//
	double timeoutResetRadius;
	double reinforcementDuration;
};

/**
 * PunishmentRegionVisitor class intended to support crude non-bullet punishment region identification
 */
class PunishmentRegionVisitor : public BasicObstacleVisitor
{
private:
	typedef BasicObstacleVisitor super;

public:
	enum ReinforcementState {
		PRIMED, // Initial state: Animal is ready to get reinforcement as soon as it enters the right zone
		REINFORCING, // Animal is actively getting reinforcement
		EXHAUSTED // Reinforcement timed out, but animal has not yet returned to the refresh area
	};
	enum AnimalLocation {
		NO_ZONE, // beyond either the inner or outer boundary of the reinforcment zone
		ZONE_BUFFER, // between the ZONE_CORE and the NO_ZONE areas, within a strip of width timeoutResetRadius
		ZONE_CORE // the inner sanctum of the reinforcement zone. Reinforcement can be activated here
	};

    PunishmentRegionVisitor();
	virtual CollisionResult collide(osg::Vec2 subject_location) const;
	int reinforcementAtXy(double x, double y) const;

	int baselineValue; // baseline reinforcement value outside of all regions

	std::map<std::string, ReinforcementCategory> reinforcementCategories;

	ReinforcementState getReinforcementState() { return _reinforcementState;}

private:
	mutable QElapsedTimer _reinforcement_timer;
	mutable ReinforcementState _reinforcementState;
};

#endif // PUNISHMENT_REGION_VISITOR_H__
