/* -*-c++-*- */

#ifndef CONFIG_MEMENTO_H
#define CONFIG_MEMENTO_H

#include <fstream>
#include <vector>

#include <QtGui/QColor>
#include <QIODevice>

#include <Globals.h> // for from_string, triple, GraphPositions

class Console;

#include "api/IConfig.h"
#include "PunishmentRegionVisitor.h"

/**
 * Config_Memento.
 **/

// New per-camera members should go here, to maybe reduce the number 
// of std::vector members in Config_Memento
class Config_Memento_Per_Camera {
public:
	Config_Memento_Per_Camera() 
		: _screen_width(10)
		, _screen_height(10)
	{}

	int _screen_width;
	int _screen_height;
};

class Config_Memento : public IConfig
{
	friend std::ostream& operator<<( std::ostream& os, Config_Memento const& f );
	
public:
	
	// Initialization
	Config_Memento();
	Config_Memento( Console const* console );
	
	// Duplication
	// Destruction
	~Config_Memento() {}
	
	// Access
	bool initialized() const { return _initialized; }
	int major_version() const { return _major_version; }
	int minor_version() const { return _minor_version; }
	
	// Measurement
	// Comparison
	// Status report
	// Status setting
	// Cursor movement
	// Element change
	void initialize( Console const* console );
	
	// Removal
	// Resizing
	// Transformation
	// Conversion
	// Basic operations
	void load( std::ifstream& config );
	void save( std::ofstream& config );
	void save_xml( QIODevice& output );
	bool load_xml( QIODevice& input );
	
	// Miscellaneous
	// Obsolete
	// Inapplicable
	
	// IConfig interface
	virtual bool isFogEnabled() const override { return _fog_enabled; }
	virtual void setFogEnabled(bool f) override { _fog_enabled = f; }

protected:
	
	void read_v1( std::ifstream& config );
	void read_v2( std::ifstream& config );

private:
	friend class Console;
	friend class Scene_Model;

	int const _major_version;
	int const _minor_version;
	bool _initialized;
	int _minor, _major;
	int _numCameras, _num_displays, _starting_display;
	bool _gangCameras;
	bool _xCenterDistorts;
	std::vector< std::pair< int, int > > _frame;
	std::vector< triple< bool, int, int > > _distort;
	std::vector< std::pair< int, int > > _center;
	int _retries, _port;
	std::string _host;
	bool _gangMotion;
	bool _frame_packing;
	std::vector< GraphPositions > _graph;
	QColor _ambient, _diffuse, _background;
	int _power;
	std::vector< QColor > _global_brightness;
	int _shader_choice;
	bool _fog_enabled;
	bool _fog_radial;
	int _fog_mode;
	QColor _fog_color;
	float _fog_density;
	int _fog_start, _fog_end;

	std::string _calib_x, _calib_y, _calib_z;
	float _gain_x, _gain_y, _gain_z;
	bool _enable_vel_smoothing, _enable_input_smoothing;
	bool _enable_intermediary_smoothing, _enable_output_smoothing;
	int _vel_smoothing_interval, _input_interval, _intermediary_interval, _output_interval;
	float _ball_radius, _minimum_velocity_thresold, _crossbar_width;
	bool _crossbar_enabled, _restrict_vertical_motion;
	std::string _port_name;
	int _output_rate;
	bool _rotatedCameras;
	float _fov;
	float _fov_offset;
	int _frame_mode, _indicator_size, _border_size, _screen_select;
	int _indicator_horizontal_pos, _indicator_vertical_pos;
	bool _use_custom_matrix;
	bool _edit_per_camera;
	std::vector<Frustum> _frustum;
    std::vector<ScreenGeometry> _screen_geometry;
	bool _enable_threshold_turning;
	float _min_threshold_scale, _max_threshold_scale, _current_threshold_scale;
	std::vector< double > _plot_x, _plot_y;
	std::vector< double > _plot_vel_x, _plot_vel_y;

	// std::vector< Config_Memento_Per_Camera > _per_camera;
	float _screen_width;
	float _screen_height;
	// Reinforcement Zones
	int _zone_baseline_value;
	std::map< std::string, ReinforcementCategory > reinforcementCategories;

	// Move the animal to the respawn point, if it loiters too close to an obstacle for too long
	float _respawn_radius; // millimeters
	float _respawn_timeout; // seconds
	
};	// end of class Config_Memento


#endif // CONFIG_MEMENTO_H

