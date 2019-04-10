// Test Config_Memento class for saving and restoring experimental session parameters

#include "Config_Memento.h"
#include <QBuffer>

#define BOOST_TEST_MODULE JovianXmlConfigTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( xml_config_test )
{
	BOOST_CHECK(true); // make sure unit testing framework is loaded

	// Config_Memento is the configuration state class we are testing here

	// Create two Config_Mementos, each with different parameter values
	Config_Memento config1, config2;
	config1.setFogEnabled(true);
	config2.setFogEnabled(false);

	// Verify that the parameter settings took correctly
	BOOST_CHECK(config1.isFogEnabled());
	BOOST_CHECK( ! config2.isFogEnabled());

	// Test xml read/write
	// Write config to xml buffers
	QBuffer xmlBuffer1;
	QBuffer xmlBuffer2;
	xmlBuffer1.open(QBuffer::ReadWrite);
	xmlBuffer2.open(QBuffer::ReadWrite);
	config1.save_xml(xmlBuffer1);
	config2.save_xml(xmlBuffer2);

	xmlBuffer1.reset();
	xmlBuffer2.reset();

	// Before swapping, verify everything is still the way we left it.
	BOOST_CHECK(config1.isFogEnabled());
	BOOST_CHECK(!config2.isFogEnabled());

	// Swap values by reading from one anothers' xmlbuffers
	config1.load_xml(xmlBuffer2);
	config2.load_xml(xmlBuffer1);

	// Verify that the values were swapped by this process
	BOOST_CHECK(!config1.isFogEnabled());
	BOOST_CHECK(config2.isFogEnabled());

	// TODO: all the parameters, not just fog enabled
}
