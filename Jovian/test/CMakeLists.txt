# Unit tests for Jovian library

# Simple test to ensure that ctest infrastructure works
add_executable(test_testing test_testing.cpp)
add_test(NAME test_testing COMMAND test_testing)

find_package(Boost 1.61.0 COMPONENTS regex unit_test_framework)

include_directories("${Jovian_SOURCE_DIR}/src")
include_directories("${Boost_INCLUDE_DIR}")
include_directories("${Qt5Gui_INCLUDE_DIRS}")

if(MSVC)
	if(${MSVC_VERSION} STRGREATER 1600)
		message("msvc2010 + build fix /FORCE:MULTIPLE")
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /FORCE:MULTIPLE")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /FORCE:MULTIPLE")
	endif()
endif()

add_executable(test_config_xml test_config_xml.cpp)
add_test(NAME test_config_xml COMMAND test_config_xml)
target_link_libraries(test_config_xml 
	Jovian 

	debug ${Boost_REGEX_LIBRARY_DEBUG} optimized ${Boost_REGEX_LIBRARY_RELEASE}
	debug ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_DEBUG} optimized ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_RELEASE}
	debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE}
	debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE}

	Qt5::Widgets
	Qt5::OpenGL
	Qt5::PrintSupport
	
	debug ${OSG_LIBRARY_DEBUG} optimized ${OSG_LIBRARY}
	debug ${OSGVIEWER_LIBRARY_DEBUG} optimized ${OSGVIEWER_LIBRARY}
	debug ${OSGGA_LIBRARY_DEBUG} optimized ${OSGGA_LIBRARY}
	debug ${OSGPARTICLE_LIBRARY_DEBUG} optimized ${OSGPARTICLE_LIBRARY}
	debug ${OSGUTIL_LIBRARY_DEBUG} optimized ${OSGUTIL_LIBRARY}
	debug ${OSGDB_LIBRARY_DEBUG} optimized ${OSGDB_LIBRARY}
	debug ${OSGQT_LIBRARY_DEBUG} optimized ${OSGQT_LIBRARY}
	debug ${OPENTHREADS_LIBRARY_DEBUG} optimized ${OPENTHREADS_LIBRARY}
	
	debug ${BULLET_COLLISION_LIBRARY_DEBUG} optimized ${BULLET_COLLISION_LIBRARY}
	debug ${BULLET_DYNAMICS_LIBRARY_DEBUG} optimized ${BULLET_DYNAMICS_LIBRARY}
	debug ${BULLET_MATH_LIBRARY_DEBUG} optimized ${BULLET_MATH_LIBRARY}
	
	debug ${OSGBCOLLISION_LIBRARY_debug} optimized ${OSGBCOLLISION_LIBRARY}
	debug ${OSGBDYNAMICS_LIBRARY_debug} optimized ${OSGBDYNAMICS_LIBRARY}
)


add_executable(test_reinforcement_xml test_reinforcement_xml.cpp)
add_test(NAME test_reinforcement_xml COMMAND test_reinforcement_xml)
target_link_libraries(test_reinforcement_xml 
	Jovian 

	debug ${Boost_REGEX_LIBRARY_DEBUG} optimized ${Boost_REGEX_LIBRARY_RELEASE}
	debug ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_DEBUG} optimized ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_RELEASE}
	debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE}
	debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE}

	Qt5::Widgets
	Qt5::OpenGL
	Qt5::PrintSupport
	
	debug ${OSG_LIBRARY_DEBUG} optimized ${OSG_LIBRARY}
	debug ${OSGVIEWER_LIBRARY_DEBUG} optimized ${OSGVIEWER_LIBRARY}
	debug ${OSGGA_LIBRARY_DEBUG} optimized ${OSGGA_LIBRARY}
	debug ${OSGPARTICLE_LIBRARY_DEBUG} optimized ${OSGPARTICLE_LIBRARY}
	debug ${OSGUTIL_LIBRARY_DEBUG} optimized ${OSGUTIL_LIBRARY}
	debug ${OSGDB_LIBRARY_DEBUG} optimized ${OSGDB_LIBRARY}
	debug ${OSGQT_LIBRARY_DEBUG} optimized ${OSGQT_LIBRARY}
	debug ${OPENTHREADS_LIBRARY_DEBUG} optimized ${OPENTHREADS_LIBRARY}
	
	debug ${BULLET_COLLISION_LIBRARY_DEBUG} optimized ${BULLET_COLLISION_LIBRARY}
	debug ${BULLET_DYNAMICS_LIBRARY_DEBUG} optimized ${BULLET_DYNAMICS_LIBRARY}
	debug ${BULLET_MATH_LIBRARY_DEBUG} optimized ${BULLET_MATH_LIBRARY}
	
	debug ${OSGBCOLLISION_LIBRARY_debug} optimized ${OSGBCOLLISION_LIBRARY}
	debug ${OSGBDYNAMICS_LIBRARY_debug} optimized ${OSGBDYNAMICS_LIBRARY}
)


