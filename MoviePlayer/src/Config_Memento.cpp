
#include <iostream>
using std::cout;
using std::endl;
#include <utility> // for make_pair

#include <Config_Memento.h>
#include <Console>
#include <Globals.h>

// Initialization
Config_Memento::Config_Memento(): _initialized( false ), _major_version( 2 ), _minor_version( 6 )
{
}
	
Config_Memento::Config_Memento( Console const* console ): _initialized( false ), _major_version( 2 ), _minor_version( 6 )
{
	initialize( console );
}
	
// Duplication
// Element Change
void
Config_Memento::initialize( Console const* console )
{
	_numCameras = console->numDisplaysSpinBox->value();
	_num_displays = console->numDisplaysSpinBox->value();
	_starting_display = console->startingDisplaySpinBox->value();
		
	_retries = console->retries();
	from_string<int>( _port, console->port(), std::dec );
	_host = console->hostname();
	_initialized = true;
}

// Basic operations
void
Config_Memento::load( std::ifstream& config )
{
	char v[256];
	char ch;
	config >> v;
	std::string str( v );
	int pos = str.find( "." );
	from_string<int>( _major, str.substr( 0, pos ), std::dec );
	from_string<int>( _minor, str.substr( pos + 1 ), std::dec );
	
	if ( _major == 1 && _minor == 0 )
	{
		cout << "Can only process version 1.1 or later configuration files." << endl;
		return;
	}

	if ( !( _major <= _major_version || ( _major_version == _major && _minor_version <= _minor ) ) )
	{
		cout << "Can only process version 1.1 or later configuration files." << endl;
		return;
	}

	if ( _major == 1 )
		read_v1( config );
	else if ( _major == 2 )
		read_v2( config );
}

void
Config_Memento::save( std::ofstream& config )
{
	config << _major_version << "." << _minor_version << endl;
	config << _numCameras << "," << _num_displays << "," << _starting_display << endl;
	config << _gangCameras << endl;
	config << _xCenterDistorts << endl;

	for ( int i = 0; i < _numCameras; i++ )
	{
		config << _frame[ i ].first << "," << _frame[ i ].second;
		config << "," << _distort[ i ].first << "," << _distort[i].second << "," << _distort[i].third;
		config << "," << _center[ i ].first << "," << _center[i].second << endl;
	}
	config << _retries << "," << _port << "," << _host << endl;

	config << _gangMotion << endl;
	config << _frame_packing << endl;

	for ( int i = 0; i < _numCameras; i++ )
	{
		config << _graph[i].size() << "," << _graph[i][0].size() << endl;
		for ( int j = 0; j < _graph[i].size(); j++ )
		{
			for ( int k = 0; k < _graph[i][0].size(); k++ )
				config << "(" << _graph[i][j][k].first << "," <<  _graph[i][j][k].second << "),";
			
			config << endl;
		}
	}
	
	config << _ambient.name().toStdString() << "," << _diffuse.name().toStdString() 
	       << "," << _power << "," << _background.name().toStdString()
	       << endl;

	for ( int i = 0; i < _numCameras; i++ )
		config << _global_brightness[i].name().toStdString() << ",";
	config << endl;
		
	config << _calib_x<< "," << _gain_x << endl;
	config << _calib_y << "," << _gain_y << endl;
	config << _calib_z << "," << _gain_z << endl;

	config << _enable_vel_smoothing << "," << _vel_smoothing_interval << endl;
	config << _enable_input_smoothing << "," << _input_interval << endl;
	config << _enable_intermediary_smoothing << "," << _intermediary_interval << endl;
	config << _enable_output_smoothing << "," << _output_interval << endl;
	config << _ball_radius << endl;
	config << _crossbar_enabled << "," << _crossbar_width << endl;
	config << _minimum_velocity_thresold << endl;
	config << _restrict_vertical_motion << endl;
	config << _port_name << endl;
	config << _output_rate << endl;
	config << _rotatedCameras << "," << _fov << "," << _fov_offset << endl;

	config << _frame_mode << "," 
	       << _indicator_size << ","
	       << _border_size << ","
	       << _screen_select << ","
	       << _indicator_horizontal_pos << ","
	       << _indicator_vertical_pos << endl;

	config << _shader_choice << endl;
	
	config << _enable_threshold_turning << ","
	       << _min_threshold_scale << ","
	       << _max_threshold_scale << ","
	       << _current_threshold_scale << endl;
	
	config <<  _plot_x.size() << ", ";
	for( int i = 0; i < _plot_x.size(); i++ )
		config << "(" << _plot_x[i] << "," <<  _plot_y[i] << "),";
			
	config << endl;

	config <<  _plot_vel_x.size() << ", ";
	for( int i = 0; i < _plot_vel_x.size(); i++ )
		config << "(" << _plot_vel_x[i] << "," <<  _plot_vel_y[i] << "),";
			
	config << endl;
}

// Implementation
void
Config_Memento::read_v1( std::ifstream& config )
{
	char ch;
	int pos;
	
	config >> _numCameras;

	config >> _gangCameras;

	if ( _minor >= 3 )
		config >> _xCenterDistorts;
			
	_center.resize( _numCameras );
	_distort.resize( _numCameras );
	_frame.resize( _numCameras );
	_global_brightness.resize( _numCameras );
	
	for ( int i = 0; i < _numCameras; i++ )
	{
		int lower, upper;
		bool checked;
		int focalLength_y, focalLength_x;

		config >> lower >> ch >> upper;
				
		_frame[ i ].first = lower;
		_frame[ i ].second = upper;

		if ( _minor == 1 )
		{
			config >> ch >> checked >> ch >> focalLength_y;
			focalLength_x = focalLength_y;
		}
		else
			config >> ch >> checked >> ch >> focalLength_y >> ch >> focalLength_x;

		_distort[ i ].first = checked;
		_distort[ i ].second = focalLength_y;
		if ( _minor == 1 )
			_distort[ i ].third = focalLength_y;
		else
			_distort[ i ].third = focalLength_x;
				
		config >> ch >> lower >> ch >> upper;
		_center[ i ].first = lower;
		_center[ i ].second = upper;
	}

	config >> _retries >> ch >> _port >> ch >> _host;

	if ( _minor >= 9 )
		config >> _gangMotion;
	
   // GraphWidget
	if ( _minor >= 6 )
	{
		for ( int i = 0; i < _numCameras; i++ )
		{
			char ch;
			int r, c;
			GraphPositions g;
			
			config >> r >> ch >> c;
			g.resize( r );
			
			for ( int i = 0; i < r; i++ )
			{
				qreal x, y;
				g[i].resize( c );
				
				for ( int j = 0; j < c; j++ )
				{
					config >> ch >> x >> ch >> y >> ch >> ch;

					g[i][j] = std::make_pair( x, y );
				}
			}
			
			_graph.push_back( g );
		}
	}
		
	if ( _minor >= 4 )
	{
		char s[256];
		std::string sss;

		// Lighting Controls
		config >> s;
		std::string ss( s );
		pos = ss.find( "," );
		sss = ss.substr( 0, pos );
		_ambient = QColor( sss.c_str() );

		ss = ss.substr( pos + 1 );
		pos = ss.find( "," );
		sss = ss.substr( 0, pos );
		_diffuse = QColor( sss.c_str() );

		ss = ss.substr( pos + 1 );
		pos = ss.find( "," );
		sss = ss.substr( 0, pos );
		from_string<int>( _power, sss, std::dec );

		ss = ss.substr( pos + 1 );
		pos = ss.find( "," );
		sss = ss.substr( 0, pos );
		_background = QColor( sss.c_str() );
			
		if ( _minor == 4 )
		{
			ss = ss.substr( pos + 1 );
			pos = ss.find( "," );
			sss = ss.substr( 0, pos );
			QColor color = QColor( sss.c_str() );
			for ( int i = 0; i < _numCameras; i++ )
				_global_brightness[ i ] = color;
		}
		else
		{
			config >> s;
			ss = std::string( s );
			for ( int i = 0; i < _numCameras; i++ )
			{
				pos = ss.find( "," );
				sss = ss.substr( 0, pos );
				_global_brightness[ i ] = QColor( sss.c_str() );
				ss = ss.substr( pos + 1 );
			}
		}
			
		// Calibration Parameters
		config >> s;
		ss = std::string( s );
		pos = ss.find( "," );
		_calib_x = ss.substr( 0, pos );
		sss = ss.substr( pos+1 );
		from_string<float>( _gain_x, sss, std::dec );

		config >> s;
		ss = std::string( s );
		pos = ss.find( "," );
		_calib_y = ss.substr( 0, pos );
		sss = ss.substr( pos+1 );
		from_string<float>( _gain_y, sss, std::dec );

		config >> s;
		ss = std::string( s );
		pos = ss.find( "," );
		_calib_z = ss.substr( 0, pos );
		sss = ss.substr( pos+1 );
		from_string<float>( _gain_z, sss, std::dec );

	}

	if ( _minor >= 5 )
	{
		double value;
		int iVal;
			
		if ( _minor < 8 )
		{
			// Used to be maxAnimalVelocity
			config >> value;
			
			// Used to be pathThreshold
			config >> value;
		}
		
		if ( _minor >= 7 )
			config >> _ball_radius;
		else
		{
			config >> iVal;
			_ball_radius = iVal;
		}

		config >> _port_name;

		config >> _rotatedCameras >> ch >> _fov >> ch >> _fov_offset;
			
		std::string sss;

		char s[256];
		config >> s;
		std::string ss( s );
		pos = ss.find( "," );
		sss = ss.substr( 0, pos );
		from_string<int>( _frame_mode, sss, std::dec );

		ss = ss.substr( pos + 1 );
		pos = ss.find( "," );
		sss = ss.substr( 0, pos );
		from_string<int>( _indicator_size, sss, std::dec );

		ss = ss.substr( pos + 1 );
		pos = ss.find( "," );
		sss = ss.substr( 0, pos );
		from_string<int>( _border_size, sss, std::dec );

		ss = ss.substr( pos + 1 );
		pos = ss.find( "," );
		sss = ss.substr( 0, pos );
		from_string<int>( _screen_select, sss, std::dec );

		ss = ss.substr( pos + 1 );
		pos = ss.find( "," );
		sss = ss.substr( 0, pos );
		from_string<int>( _indicator_horizontal_pos, sss, std::dec );

		ss = ss.substr( pos + 1 );
		pos = ss.find( "," );
		sss = ss.substr( 0, pos );
		from_string<int>( _indicator_vertical_pos, sss, std::dec );
	}
}

void
Config_Memento::read_v2( std::ifstream& config )
{
	char ch;
	int pos;
	
	config >> _numCameras >> ch >> _num_displays >> ch >> _starting_display;
	config >> _gangCameras;
	config >> _xCenterDistorts;
			
	_center.resize( _numCameras );
	_distort.resize( _numCameras );
	_frame.resize( _numCameras );
	_global_brightness.resize( _numCameras );
	
	for ( int i = 0; i < _numCameras; i++ )
	{
		int lower, upper;
		bool checked;
		int focalLength_y, focalLength_x;

		config >> lower >> ch >> upper;
				
		_frame[ i ].first = lower;
		_frame[ i ].second = upper;

		config >> ch >> checked >> ch >> focalLength_y >> ch >> focalLength_x;

		_distort[ i ].first = checked;
		_distort[ i ].second = focalLength_y;
		_distort[ i ].third = focalLength_x;
				
		config >> ch >> lower >> ch >> upper;
		_center[ i ].first = lower;
		_center[ i ].second = upper;
	}

	config >> _retries >> ch >> _port >> ch >> _host;

	config >> _gangMotion;
	config >> _frame_packing;

	// GraphWidget
	for ( int i = 0; i < _numCameras; i++ )
	{
		char ch;
		int r, c;
		GraphPositions g;
			
		config >> r >> ch >> c;
		g.resize( r );
			
		for ( int i = 0; i < r; i++ )
		{
			qreal x, y;
			g[i].resize( c );
				
			for ( int j = 0; j < c; j++ )
			{
				config >> ch >> x >> ch >> y >> ch >> ch;

				g[i][j] = std::make_pair( x, y );
			}
		}
			
		_graph.push_back( g );
	}
		
	char s[256];
	std::string sss;

	// Lighting Controls
	config >> s;
	std::string ss( s );
	pos = ss.find( "," );
	sss = ss.substr( 0, pos );
	_ambient = QColor( sss.c_str() );

	ss = ss.substr( pos + 1 );
	pos = ss.find( "," );
	sss = ss.substr( 0, pos );
	_diffuse = QColor( sss.c_str() );

	ss = ss.substr( pos + 1 );
	pos = ss.find( "," );
	sss = ss.substr( 0, pos );
	from_string<int>( _power, sss, std::dec );

	ss = ss.substr( pos + 1 );
	pos = ss.find( "," );
	sss = ss.substr( 0, pos );
	_background = QColor( sss.c_str() );
			
	config >> s;
	ss = std::string( s );
	for ( int i = 0; i < _numCameras; i++ )
	{
		pos = ss.find( "," );
		sss = ss.substr( 0, pos );
		_global_brightness[ i ] = QColor( sss.c_str() );
		ss = ss.substr( pos + 1 );
	}
			
	// Calibration Parameters
	config >> s;
	ss = std::string( s );
	pos = ss.find( "," );
	_calib_x = ss.substr( 0, pos );
	sss = ss.substr( pos+1 );
	from_string<float>( _gain_x, sss, std::dec );

	config >> s;
	ss = std::string( s );
	pos = ss.find( "," );
	_calib_y = ss.substr( 0, pos );
	sss = ss.substr( pos+1 );
	from_string<float>( _gain_y, sss, std::dec );

	config >> s;
	ss = std::string( s );
	pos = ss.find( "," );
	_calib_z = ss.substr( 0, pos );
	sss = ss.substr( pos+1 );
	from_string<float>( _gain_z, sss, std::dec );

	double value;
	int iVal;

	config >> _enable_vel_smoothing >> ch >>_vel_smoothing_interval;
	if ( _minor >= 5 )
	{
		config >> _enable_input_smoothing >> ch >> _input_interval;
		if ( _minor >= 6 )
			config >> _enable_intermediary_smoothing >> ch >> _intermediary_interval;
		else
		{
			_enable_intermediary_smoothing = false;
			_intermediary_interval = 32;
		}
		
		config >> _enable_output_smoothing >> ch >> _output_interval;
	}
	else
	{
		_enable_input_smoothing = _enable_output_smoothing = false;
		_input_interval = _output_interval = 32;
	}
	
	if ( _minor >= 4 )
	{
		config >> _ball_radius;
		config >> _crossbar_enabled >> ch >> _crossbar_width;
		config >> _minimum_velocity_thresold;
		config >> _restrict_vertical_motion;
	}
	else
	{
		if ( _minor >= 3 )
		{
			config >> _minimum_velocity_thresold;
		}
		else
		{
			// Set sensible defaults
			_minimum_velocity_thresold = 1.e-6;
		}
		config >> _ball_radius;
		_crossbar_width = 3.0f;
		_crossbar_enabled = false;
		_restrict_vertical_motion = false;
	}
	
	config >> _port_name;
	config >> _output_rate;
	config >> _rotatedCameras >> ch >> _fov >> ch >> _fov_offset;
			
	config >> s;
	ss = std::string( s );
	pos = ss.find( "," );
	sss = ss.substr( 0, pos );
	from_string<int>( _frame_mode, sss, std::dec );

	ss = ss.substr( pos + 1 );
	pos = ss.find( "," );
	sss = ss.substr( 0, pos );
	from_string<int>( _indicator_size, sss, std::dec );

	ss = ss.substr( pos + 1 );
	pos = ss.find( "," );
	sss = ss.substr( 0, pos );
	from_string<int>( _border_size, sss, std::dec );

	ss = ss.substr( pos + 1 );
	pos = ss.find( "," );
	sss = ss.substr( 0, pos );
	from_string<int>( _screen_select, sss, std::dec );

	ss = ss.substr( pos + 1 );
	pos = ss.find( "," );
	sss = ss.substr( 0, pos );
	from_string<int>( _indicator_horizontal_pos, sss, std::dec );

	ss = ss.substr( pos + 1 );
	pos = ss.find( "," );
	sss = ss.substr( 0, pos );
	from_string<int>( _indicator_vertical_pos, sss, std::dec );

	if ( _minor >= 1 )
	{
		config >> _shader_choice;
	}

	if ( _minor >= 2 )
	{
		int r;
		
		if ( _minor >= 5 )
			config >> _enable_threshold_turning >> ch
			       >> _min_threshold_scale >> ch
			       >> _max_threshold_scale >> ch
			       >> _current_threshold_scale;
		else
		{
			_enable_threshold_turning = false;
			_min_threshold_scale = 0.9;
			_max_threshold_scale = 1.0;
			_current_threshold_scale = 0.95;
		}

		// Ratio plot widget
		config >> r >> ch;
		_plot_x.resize( r );
		_plot_y.resize( r );
			
		for ( int i = 0; i < r; i++ )
		{
			double x, y;
			config >> ch >> x >> ch >> y >> ch >> ch;

			_plot_x[i] = x;
			_plot_y[i] = y;
		}

		// velocity plot widget
		config >> r >> ch;
		_plot_vel_x.resize( r );
		_plot_vel_y.resize( r );
			
		for ( int i = 0; i < r; i++ )
		{
			double x, y;
			config >> ch >> x >> ch >> y >> ch >> ch;

			_plot_vel_x[i] = x;
			_plot_vel_y[i] = y;
		}

	}
}

