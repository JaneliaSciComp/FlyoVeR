/* -*-c++-*- */

#ifndef CONFIG_MEMENTO_H
#define CONFIG_MEMENTO_H

#include <fstream>
#include <vector>

#include <QtGui/QColor>

#include <Globals.h> // for from_string, triple, GraphPositions

class Console;

/**
 * Config_Memento.
 **/

class Config_Memento
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
	
	// Miscellaneous
	// Obsolete
	// Inapplicable
	
protected:
	
	void read_v1( std::ifstream& config );
	void read_v2( std::ifstream& config );

private:
	friend class Console;

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
	int _fov;
	float _fov_offset;
	int _frame_mode, _indicator_size, _border_size, _screen_select;
	int _indicator_horizontal_pos, _indicator_vertical_pos;
	bool _enable_threshold_turning;
	float _min_threshold_scale, _max_threshold_scale, _current_threshold_scale;
	std::vector< double > _plot_x, _plot_y;
	std::vector< double > _plot_vel_x, _plot_vel_y;
	
};	// end of class Config_Memento


#endif // CONFIG_MEMENTO_H

