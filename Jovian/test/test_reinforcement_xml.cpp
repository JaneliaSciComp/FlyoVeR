// Test Reinforcement zones to make sure we get accurate reinforcement behavior

#include "Config_Memento.h"
#include <QBuffer>
#include <iostream>

#define BOOST_TEST_MODULE JovianReinforcementZoneTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( xml_config_test )
{
	BOOST_CHECK(true); // make sure unit testing framework is loaded
	// PunishmentRegionVisitor is the behavior class we are testing

    PunishmentRegionVisitor punisher;
    // set up reinforcement zones
    std::map<std::string, ReinforcementCategory> testReinforcements;
    ReinforcementCategory testReinforcement1;
    testReinforcement1.innerRadius = 10;
    testReinforcement1.outerRadius = 30;
    testReinforcement1.innerValue = 100; 
    testReinforcement1.gammaExponent = 1;
    testReinforcement1.loiteringTimeout = 100; 
    testReinforcement1.loiteringRadius = 5; 
    testReinforcement1.timeoutResetRadius = 5;
    testReinforcement1.reinforcementDuration = 100;
    testReinforcements.insert(std::map<std::string, ReinforcementCategory>::value_type("1", testReinforcement1));
    punisher.reinforcementCategories = testReinforcements;
    punisher.baselineValue = 0;

    // set up basic obstacle at origin
    CrudeVRObstacle testObstacle1;
    osg::Vec2 testObLoc1(0.0, 0.0);
    testObstacle1.location = testObLoc1;
    testObstacle1.xy_radius = 5;
    testObstacle1.d_threshold = 30.5;
    testObstacle1.exclusionZone = CrudeVRObstacle::ExclusionZone::OUTSIDE;
    testObstacle1.z_min = -3;
    testObstacle1.z_max = 17;
    testObstacle1.category_label = "1";
    punisher.relevant_obstacles.push_back(testObstacle1);

    // set up a subject location outside the obstacle zone including reinforcement timeout
    osg::Vec2 subjectLoc(40, 0);
    CollisionResult checkCollision = punisher.collide(subjectLoc);

	// Verify that we aren't producing a collision
    BOOST_CHECK_EQUAL(checkCollision.minDistance, 5);
    BOOST_CHECK_EQUAL(checkCollision.minCenterDistance,40);
    BOOST_CHECK(checkCollision.collidedObstacle == NULL);

    // we've moved into the reinforcement zone outer radius, collision produced, but no reinforcements yet
    subjectLoc.set(35, 0);
    checkCollision = punisher.collide(subjectLoc);
    BOOST_CHECK(checkCollision.collidedObstacle != NULL);
    int reinforcementVal = punisher.reinforcementAtXy(35, 0);
    BOOST_CHECK(punisher.getReinforcementState() == PunishmentRegionVisitor::ReinforcementState::PRIMED);
    BOOST_CHECK_EQUAL(reinforcementVal, 0);

    // move subject into outer zone buffer, should have no reinforcement yet
    reinforcementVal = punisher.reinforcementAtXy(32, 0);
    BOOST_CHECK(punisher.getReinforcementState() == PunishmentRegionVisitor::ReinforcementState::PRIMED);
    BOOST_CHECK_EQUAL(reinforcementVal, 0);

    // move subject into outer zone, should start reinforcing
    reinforcementVal = punisher.reinforcementAtXy(30, 0);
    BOOST_CHECK(punisher.getReinforcementState() == PunishmentRegionVisitor::ReinforcementState::REINFORCING);
    BOOST_CHECK_EQUAL(reinforcementVal, testReinforcement1.innerValue);

    // move subject back into outer zone buffer, should still be reinforcing
    reinforcementVal = punisher.reinforcementAtXy(34, 0);
    BOOST_CHECK(punisher.getReinforcementState() == PunishmentRegionVisitor::ReinforcementState::REINFORCING);
    BOOST_CHECK_EQUAL(reinforcementVal, testReinforcement1.innerValue);

    // move subject outside of buffer zone, should still be reinforcing
    reinforcementVal = punisher.reinforcementAtXy(36, 0);
    BOOST_CHECK(punisher.getReinforcementState() == PunishmentRegionVisitor::ReinforcementState::PRIMED);
    BOOST_CHECK_EQUAL(reinforcementVal, 0);

    // move subject into outer zone buffer, no reinforcement yet
    reinforcementVal = punisher.reinforcementAtXy(32, 0);
    BOOST_CHECK(punisher.getReinforcementState() == PunishmentRegionVisitor::ReinforcementState::PRIMED);
    BOOST_CHECK_EQUAL(reinforcementVal, 0);

}
