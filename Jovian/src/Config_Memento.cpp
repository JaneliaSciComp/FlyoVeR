
#include <iostream>
using std::cout;
using std::endl;
#include <utility> // for make_pair

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include <Config_Memento.h>
#include <Console.h>

#define CONFIG_MEMENTO_MAJOR_VERSION 2
#define CONFIG_MEMENTO_MINOR_VERSION 8

// Initialization
Config_Memento::Config_Memento() 
	: _initialized ( false )
	, _major_version ( CONFIG_MEMENTO_MAJOR_VERSION )
	, _minor_version ( CONFIG_MEMENTO_MINOR_VERSION )
	, _fog_mode(0)
	, _numCameras(0)
{
}

Config_Memento::Config_Memento ( Console const* console ) 
	: _initialized ( false )
	, _major_version ( CONFIG_MEMENTO_MAJOR_VERSION )
	, _minor_version ( CONFIG_MEMENTO_MINOR_VERSION )
	, _fog_mode(0)
{
  initialize ( console );
}

// Duplication
// Element Change
void
Config_Memento::initialize ( Console const* console )
{
  _numCameras = console->numDisplaysSpinBox->value();
  _num_displays = console->numDisplaysSpinBox->value();
  _starting_display = console->startingDisplaySpinBox->value();

  _retries = console->retries();
  from_string<int> ( _port, console->port(), std::dec );
  _host = console->hostname();
  _initialized = true;
}

// Basic operations
void
Config_Memento::load ( std::ifstream& config )
{
  char v[256];
  // char ch;
  config >> v;
  std::string str ( v );
  int pos = str.find ( "." );
  from_string<int> ( _major, str.substr ( 0, pos ), std::dec );
  from_string<int> ( _minor, str.substr ( pos + 1 ), std::dec );

  if ( _major == 1 && _minor == 0 )
  {
    cout << "Can only process version 1.1 or later configuration files." << endl;
    return;
  }

  if ( ! ( _major <= _major_version || ( _major_version == _major && _minor_version <= _minor ) ) )
  {
    cout << "Can only process version 1.1 or later configuration files." << endl;
    return;
  }

  if ( _major == 1 )
    read_v1 ( config );
  else if ( _major == 2 )
    read_v2 ( config );
}

void
Config_Memento::save ( std::ofstream& config )
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
    for ( unsigned int j = 0; j < _graph[i].size(); j++ )
    {
      for ( unsigned int k = 0; k < _graph[i][0].size(); k++ )
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

  config << _fog_enabled << "," << _fog_mode
         << "," << _fog_color.name().toStdString()
         << "," << _fog_density << "," << _fog_start << "," << _fog_end
         << endl;

  config << _calib_x << "," << _gain_x << endl;
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

  config << _use_custom_matrix << "," << _edit_per_camera << endl;
  for ( int i = 0; i < _numCameras; i++ )
  	config << _frustum[i] << endl;

  config << _enable_threshold_turning << ","
         << _min_threshold_scale << ","
         << _max_threshold_scale << ","
         << _current_threshold_scale << endl;

  config <<  _plot_x.size() << ", ";
  for ( unsigned int i = 0; i < _plot_x.size(); i++ )
    config << "(" << _plot_x[i] << "," <<  _plot_y[i] << "),";

  config << endl;

  config <<  _plot_vel_x.size() << ", ";
  for ( unsigned int i = 0; i < _plot_vel_x.size(); i++ )
    config << "(" << _plot_vel_x[i] << "," <<  _plot_vel_y[i] << "),";

  config << endl;
}

static QString fog_modes[] = {"linear", "exponential", "exponential_squared"};
static QString shading_methods[] = {"osg_default", "per_vertex", "per_pixel"};

bool Config_Memento::load_xml ( QIODevice& input )
{
	QXmlStreamReader xml(&input);
	xml.readNext();
	if (xml.atEnd()) return false;

	if (xml.isStartDocument()) // Skip first node
		xml.readNext();

	// Always expect "flyover_configuration" root element
	if (! xml.isStartElement()) return false;
	if (! (xml.name() == "flyover_configuration")) return false;
	int major_version = 0; // local because member _major_version/_minor_version are const
	int minor_version = 1;
	if (xml.attributes().hasAttribute("major_version"))
		major_version = xml.attributes().value("major_version").toString().toInt();
	if (xml.attributes().hasAttribute("minor_version"))
		minor_version = xml.attributes().value("minor_version").toString().toInt();

	// Initialize
	_numCameras = 0;

	while ( (!xml.atEnd()) && (!xml.hasError()) ) {
		if (xml.readNextStartElement()) {

			if (xml.name() == "display") {
				_num_displays = xml.attributes().value("number_of_displays").toString().toInt();
				_starting_display = xml.attributes().value("starting_display").toString().toInt();
				_frame_packing = xml.attributes().value("frame_packing").toString() == "true";
			}

			else if (xml.name() == "remote_data_connection") {
				_host = xml.attributes().value("hostname").toString().toStdString();
				_port = xml.attributes().value("port").toString().toInt();
				_retries = xml.attributes().value("num_retries").toString().toInt();
				_gangMotion = xml.attributes().value("gang_motion_and_blanking").toString() == "true";
			}

			else if (xml.name() == "lighting") {
				_ambient = QColor(xml.attributes().value("ambient").toString());
				_diffuse = QColor(xml.attributes().value("diffuse").toString());
				_background = QColor(xml.attributes().value("background").toString());
				_power = xml.attributes().value("diffuse_intensity").toString().toInt();
				QString shaderStr = xml.attributes().value("shading_method").toString();
				for (int i = 0; i < 3; ++i) {
					if (shaderStr == shading_methods[i]) {
						_shader_choice = i;
						break;
					}
				}
			}

			else if (xml.name() == "fog") {
				_fog_enabled = xml.attributes().value("enabled").toString() == "true";
				_fog_radial = xml.attributes().value("radial").toString() == "true";
				QString fogModeStr = xml.attributes().value("mode").toString();
				for (int i = 0; i < 3; ++i) {
					if (fogModeStr == fog_modes[i]) {
						_fog_mode = i;
						break;
					}
				}
				_fog_color = QColor(xml.attributes().value("color").toString());
				_fog_density = xml.attributes().value("density").toString().toFloat();
				_fog_start = xml.attributes().value("start").toString().toInt();
				_fog_end = xml.attributes().value("end").toString().toInt();
			}

			else if (xml.name() == "calibration") {
				_calib_x = xml.attributes().value("x_calib").toString().toStdString();
				_calib_y = xml.attributes().value("y_calib").toString().toStdString();
				_calib_z = xml.attributes().value("z_calib").toString().toStdString();
				_gain_x = xml.attributes().value("x_gain").toString().toFloat();
				_gain_y = xml.attributes().value("y_gain").toString().toFloat();
				_gain_z = xml.attributes().value("z_gain").toString().toFloat();
			}

			else if (xml.name() == "configuration") {
				_ball_radius = xml.attributes().value("ball_radius").toString().toFloat();
				_rotatedCameras = xml.attributes().value("use_rotated_cameras").toString() == "true";
				_use_custom_matrix = xml.attributes().value("use_custom_matrices").toString() == "true";
				_edit_per_camera = xml.attributes().value("edit_per_camera").toString() == "true";
			}

			else if (xml.name() == "reinforcement") {
				// min_value added Mar 1 2016
				if (xml.attributes().hasAttribute("min_value")) {
					_zone_baseline_value = xml.attributes().value("min_value").toString().toInt();
				} else {
					_zone_baseline_value = 0;
				}

				// reinforcement categories added Mar 24 2016
				if (reinforcementCategories.find("") == reinforcementCategories.end()) {
					reinforcementCategories[""] = ReinforcementCategory("");
				}
				ReinforcementCategory& rc = reinforcementCategories[""];
				rc.innerRadius = xml.attributes().value("inner_radius").toString().toFloat();
				rc.outerRadius = xml.attributes().value("outer_radius").toString().toFloat();
				rc.gammaExponent = xml.attributes().value("gamma_exponent").toString().toFloat();
				rc.innerValue = xml.attributes().value("max_value").toString().toInt();
			}

			else if (xml.name() == "reinforcement_category") {
				std::string catName = xml.attributes().value("name").toString().toStdString();
				if (reinforcementCategories.find(catName) == reinforcementCategories.end()) {
					reinforcementCategories[catName] = ReinforcementCategory(catName);
				}
				ReinforcementCategory& rc = reinforcementCategories[catName];
				rc.innerRadius = xml.attributes().value("inner_radius").toString().toFloat();
				rc.outerRadius = xml.attributes().value("outer_radius").toString().toFloat();
				rc.gammaExponent = xml.attributes().value("gamma_exponent").toString().toFloat();
				rc.innerValue = xml.attributes().value("inner_value").toString().toInt();
				rc.loiteringRadius = xml.attributes().value("loitering_radius").toString().toFloat();
				rc.loiteringTimeout = xml.attributes().value("loitering_timeout").toString().toFloat();

				// Added more fields June 2016
				if (xml.attributes().hasAttribute("duration")) {
					rc.reinforcementDuration = xml.attributes().value("duration").toString().toFloat();
					rc.timeoutResetRadius = xml.attributes().value("reset_radius_offset").toString().toFloat();
				}
			}

			else if (xml.name() == "respawn") {
				_respawn_radius = xml.attributes().value("radius").toString().toFloat();
				_respawn_timeout = xml.attributes().value("timeout").toString().toFloat();
			}

			else if (xml.name() == "output") {
				_port_name = xml.attributes().value("port_name").toString().toStdString();
				_output_rate = xml.attributes().value("rate").toString().toInt();
			}

			else if (xml.name() == "cameras") {
				_screen_width = xml.attributes().value("screen_width").toString().toFloat();
				_screen_height = xml.attributes().value("screen_height").toString().toFloat();
				_gangCameras = xml.attributes().value("gang_cameras").toString() == "true";
				_xCenterDistorts = xml.attributes().value("x_center_distorts_in_y").toString() == "true";
			}

			else if (xml.name() == "camera") {
				_numCameras += 1;

				// Resize all per-camera data structures
				_global_brightness.resize(_numCameras);
				_frame.resize(_numCameras);
				_distort.resize(_numCameras);
				_center.resize(_numCameras);
				_graph.resize(_numCameras);
				_frustum.resize(_numCameras);

				_global_brightness.back() = QColor(xml.attributes().value("global_brightness").toString());
			}

			else if (xml.name() == "frustum") {
				_frustum.back().left = xml.attributes().value("left").toString().toDouble();
				_frustum.back().right = xml.attributes().value("right").toString().toDouble();
				_frustum.back().bottom = xml.attributes().value("bottom").toString().toDouble();
				_frustum.back().top = xml.attributes().value("top").toString().toDouble();
				_frustum.back().near_ = xml.attributes().value("near").toString().toDouble();
				_frustum.back().far_ = xml.attributes().value("far").toString().toDouble();
				_frustum.back().aperture = xml.attributes().value("aperture").toString().toDouble();
				_frustum.back().distance = xml.attributes().value("distance").toString().toDouble();
				_frustum.back().screen_distance = xml.attributes().value("screen_distance").toString().toDouble();
				_frustum.back().eye_sep = xml.attributes().value("eye_sep").toString().toDouble();
				_frustum.back().shift_left = xml.attributes().value("shift_left").toString() == "true";
				_frustum.back().use_matrix = xml.attributes().value("use_matrix").toString() == "true";
			}

			else if (xml.name() == "matrix") {
				osg::Matrixd& m = _frustum.back().mat;
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						QString s = "e" + QString::number(i) + QString::number(j); // e.g. "e03"
						m(i, j) = xml.attributes().value(s).toString().toDouble();
					}
				}
			}

			else if (xml.name() == "distortion") {
				_distort.back().first = xml.attributes().value("enabled").toString() == "true";
			}

			else if (xml.name() == "horizontal_blanking") {
				_frame.back().first = xml.attributes().value("lower").toString().toInt();
				_frame.back().second = xml.attributes().value("upper").toString().toInt();
			}

			else if (xml.name() == "focal_length") {
				_distort.back().second = xml.attributes().value("x").toString().toInt();
				_distort.back().third = xml.attributes().value("y").toString().toInt();
			}

			else if (xml.name() == "center") {
				_center.back().first = xml.attributes().value("x").toString().toInt();
				_center.back().second = xml.attributes().value("y").toString().toInt();
			}

		}
	}

	if (xml.hasError())
		return false;

	assert(_numCameras == (int)_global_brightness.size());
	assert(_numCameras == (int)_frame.size());
	assert(_numCameras == (int)_distort.size());
	assert(_numCameras == (int)_center.size());
	assert(_numCameras == (int)_frustum.size());
	assert(_numCameras == (int)_graph.size());
	// assert(_numCameras == (int)_plot_x.size());
	// assert(_numCameras == (int)_plot_y.size());
	// assert(_numCameras == (int)_plot_vel_x.size());
	// assert(_numCameras == (int)_plot_vel_y.size());

	return true;
}

void
Config_Memento::save_xml ( QIODevice& output )
{
	// Based on code example at https://gist.github.com/lamprosg/2133804
	QXmlStreamWriter xmlWriter(&output);
	xmlWriter.setAutoFormatting(true);
	xmlWriter.writeStartDocument();
	xmlWriter.writeStartElement("flyover_configuration");
		xmlWriter.writeAttribute("major_version", QString::number(_major_version));
		xmlWriter.writeAttribute("minor_version", QString::number(_minor_version));

		xmlWriter.writeStartElement("display");
			xmlWriter.writeAttribute("number_of_displays", QString::number(_num_displays));
			xmlWriter.writeAttribute("starting_display", QString::number(_starting_display));
			xmlWriter.writeAttribute("frame_packing", QString(_frame_packing ? "true" : "false" ));
		xmlWriter.writeEndElement(); // display

		xmlWriter.writeStartElement("remote_data_connection");
			xmlWriter.writeAttribute("hostname", QString::fromStdString(_host));
			xmlWriter.writeAttribute("port", QString::number(_port));
			xmlWriter.writeAttribute("num_retries", QString::number(_retries));
			xmlWriter.writeAttribute("gang_motion_and_blanking", QString(_gangMotion ? "true" : "false" ));
		xmlWriter.writeEndElement();

		xmlWriter.writeStartElement("lighting");
			xmlWriter.writeAttribute("ambient", _ambient.name());
			xmlWriter.writeAttribute("diffuse", _diffuse.name());
			xmlWriter.writeAttribute("diffuse_intensity",  QString::number(_power));
			xmlWriter.writeAttribute("background", _background.name());
			xmlWriter.writeAttribute("shading_method", shading_methods[_shader_choice % 3]);
			xmlWriter.writeStartElement("fog");
				xmlWriter.writeAttribute("enabled", QString(_fog_enabled ? "true" : "false" ));
				xmlWriter.writeAttribute("radial", QString(_fog_radial ? "true" : "false" ));
				xmlWriter.writeAttribute("mode", fog_modes[_fog_mode % 3]);
				xmlWriter.writeAttribute("color", _fog_color.name());
				xmlWriter.writeAttribute("density", QString::number(_fog_density));
				xmlWriter.writeAttribute("start", QString::number(_fog_start));
				xmlWriter.writeAttribute("end", QString::number(_fog_end));
			xmlWriter.writeEndElement(); // fog
		xmlWriter.writeEndElement();

		xmlWriter.writeStartElement("calibration");
			xmlWriter.writeAttribute("x_calib", QString::fromStdString(_calib_x));			
			xmlWriter.writeAttribute("y_calib", QString::fromStdString(_calib_y));			
			xmlWriter.writeAttribute("z_calib", QString::fromStdString(_calib_z));			
			xmlWriter.writeAttribute("x_gain", QString::number(_gain_x));			
			xmlWriter.writeAttribute("y_gain", QString::number(_gain_y));			
			xmlWriter.writeAttribute("z_gain", QString::number(_gain_z));
		xmlWriter.writeEndElement(); // calibration

		xmlWriter.writeStartElement("configuration");
			xmlWriter.writeAttribute("ball_radius", QString::number(_ball_radius));
			xmlWriter.writeAttribute("use_rotated_cameras", QString(_rotatedCameras ? "true" : "false" ));
			xmlWriter.writeAttribute("use_custom_matrices", QString(_use_custom_matrix ? "true" : "false" ));
			xmlWriter.writeAttribute("edit_per_camera", QString(_edit_per_camera ? "true" : "false" ));
		xmlWriter.writeEndElement(); // configuration

		xmlWriter.writeStartElement("reinforcement");
		{
			xmlWriter.writeAttribute("min_value", QString::number(_zone_baseline_value));

			ReinforcementCategory& rcd = reinforcementCategories[""]; // populate top level with default category
			xmlWriter.writeAttribute("inner_radius", QString::number(rcd.innerRadius));
			xmlWriter.writeAttribute("outer_radius", QString::number(rcd.outerRadius));
			xmlWriter.writeAttribute("gamma_exponent", QString::number(rcd.gammaExponent));
			xmlWriter.writeAttribute("max_value", QString::number(rcd.innerValue));

			std::map<std::string, ReinforcementCategory>& rcmap = reinforcementCategories;
			std::map<std::string, ReinforcementCategory>::const_iterator rci;
			for (rci = rcmap.cbegin(); rci != rcmap.cend(); ++rci) {
				const ReinforcementCategory& rc = rci->second;
				xmlWriter.writeStartElement("reinforcement_category");
					xmlWriter.writeAttribute("name", QString::fromStdString(rc.m_categoryName));
					xmlWriter.writeAttribute("inner_radius", QString::number(rc.innerRadius));
					xmlWriter.writeAttribute("outer_radius", QString::number(rc.outerRadius));
					xmlWriter.writeAttribute("gamma_exponent", QString::number(rc.gammaExponent));
					xmlWriter.writeAttribute("inner_value", QString::number(rc.innerValue));
					xmlWriter.writeAttribute("loitering_radius", QString::number(rc.loiteringRadius));
					xmlWriter.writeAttribute("loitering_timeout", QString::number(rc.loiteringTimeout));
					xmlWriter.writeAttribute("duration", QString::number(rc.reinforcementDuration));
					xmlWriter.writeAttribute("reset_radius_offset", QString::number(rc.timeoutResetRadius));
					
					xmlWriter.writeEndElement(); // reinforcement_category
			}
		}
		xmlWriter.writeEndElement(); // reinforcement

		xmlWriter.writeStartElement("respawn");
			xmlWriter.writeAttribute("radius", QString::number(_respawn_radius));
			xmlWriter.writeAttribute("timeout", QString::number(_respawn_timeout));
		xmlWriter.writeEndElement(); // respawn

		xmlWriter.writeStartElement("smoothing_enabled");
			xmlWriter.writeAttribute("velocity", QString(_enable_vel_smoothing ? "true" : "false"));
			xmlWriter.writeAttribute("input", QString(_enable_input_smoothing ? "true" : "false"));
			xmlWriter.writeAttribute("intermediary", QString(_enable_intermediary_smoothing ? "true" : "false"));
			xmlWriter.writeAttribute("output", QString(_enable_output_smoothing ? "true" : "false"));
		xmlWriter.writeEndElement(); // smoothing_enabled
		xmlWriter.writeStartElement("smoothing_interval");
			xmlWriter.writeAttribute("velocity", QString::number(_vel_smoothing_interval));
			xmlWriter.writeAttribute("input", QString::number(_input_interval));
			xmlWriter.writeAttribute("intermediary", QString::number(_intermediary_interval));
			xmlWriter.writeAttribute("output", QString::number(_output_interval));
		xmlWriter.writeEndElement(); // smoothing_enabled

		xmlWriter.writeStartElement("output");
			xmlWriter.writeAttribute("port_name", QString::fromStdString(_port_name));
			xmlWriter.writeAttribute("rate", QString::number(_output_rate));
		xmlWriter.writeEndElement(); // output

		xmlWriter.writeStartElement("cameras");
			xmlWriter.writeAttribute("screen_width", QString::number(_screen_width));
			xmlWriter.writeAttribute("screen_height", QString::number(_screen_height));
			xmlWriter.writeAttribute("gang_cameras", QString(_xCenterDistorts ? "true" : "false" ));
			xmlWriter.writeAttribute("x_center_distorts_in_y", QString(_gangCameras ? "true" : "false" ));
		// cameras
		for ( int i = 0; i < _numCameras; i++ )
		{
			xmlWriter.writeStartElement("camera");
				xmlWriter.writeAttribute("id", QString::number(i+1));
				xmlWriter.writeAttribute("global_brightness", _global_brightness[i].name());
				xmlWriter.writeStartElement("frustum");
					xmlWriter.writeAttribute("left", QString::number(_frustum[i].left));
					xmlWriter.writeAttribute("right", QString::number(_frustum[i].right));
					xmlWriter.writeAttribute("bottom", QString::number(_frustum[i].bottom));
					xmlWriter.writeAttribute("top", QString::number(_frustum[i].top));
					xmlWriter.writeAttribute("near", QString::number(_frustum[i].near_));
					xmlWriter.writeAttribute("far", QString::number(_frustum[i].far_));
					xmlWriter.writeAttribute("aperture", QString::number(_frustum[i].aperture));
					xmlWriter.writeAttribute("distance", QString::number(_frustum[i].distance));
					xmlWriter.writeAttribute("screen_distance", QString::number(_frustum[i].screen_distance));
					xmlWriter.writeAttribute("eye_sep", QString::number(_frustum[i].eye_sep));
					xmlWriter.writeAttribute("shift_left", _frustum[i].shift_left? "true" : "false" );
					xmlWriter.writeAttribute("use_matrix", _frustum[i].use_matrix? "true" : "false" );
					xmlWriter.writeStartElement("matrix");
						osg::Matrixd& m = _frustum[i].mat;
						for(int row=0; row<4; ++row) {
							for(int col=0; col<4; ++col) {
								// xml attribute names may not begin with a digit
								QString att = "e" + QString::number(row) + QString::number(col);
								xmlWriter.writeAttribute(att, QString::number(m(row, col)));
							}
						}
					xmlWriter.writeEndElement(); // matrix
				xmlWriter.writeEndElement(); // frustum
				xmlWriter.writeStartElement("distortion");
					xmlWriter.writeAttribute("enabled", QString(_distort[ i ].first ? "true" : "false" ));
					xmlWriter.writeStartElement("horizontal_blanking");
						xmlWriter.writeAttribute("lower", QString::number(_frame[i].first));
						xmlWriter.writeAttribute("upper", QString::number(_frame[i].second));
					xmlWriter.writeEndElement(); // horizontal blanking
					xmlWriter.writeStartElement("focal_length");
						xmlWriter.writeAttribute("x", QString::number(_distort[i].third)); // third!
						xmlWriter.writeAttribute("y", QString::number(_distort[i].second));
					xmlWriter.writeEndElement(); // focal length
					xmlWriter.writeStartElement("center");
						xmlWriter.writeAttribute("x", QString::number(_center[i].first));
						xmlWriter.writeAttribute("y", QString::number(_center[i].second));
					xmlWriter.writeEndElement(); // center
					// distortion graph
					xmlWriter.writeStartElement("graph");
						xmlWriter.writeAttribute("rows", QString::number(_graph.back().size()));
						xmlWriter.writeAttribute("cols", QString::number(_graph.back()[0].size()));
						for ( unsigned int j = 0; j < _graph[i].size(); j++ )
						{
							for ( unsigned int k = 0; k < _graph[i][0].size(); k++ ) {
								xmlWriter.writeStartElement("control_point");
									xmlWriter.writeAttribute("i", QString::number(j));
									xmlWriter.writeAttribute("j", QString::number(k));
									xmlWriter.writeAttribute("x", QString::number(_graph[i][j][k].first));
									xmlWriter.writeAttribute("y", QString::number(_graph[i][j][k].second));
								xmlWriter.writeEndElement();
							}
						}
					xmlWriter.writeEndElement(); // graph
				xmlWriter.writeEndElement(); // distortion
			xmlWriter.writeEndElement(); // Camera
		}
		xmlWriter.writeEndElement(); // Cameras

		// TODO: All contents, representing fields below.

		/* 

		config << _enable_vel_smoothing << "," << _vel_smoothing_interval << endl;
		config << _enable_input_smoothing << "," << _input_interval << endl;
		config << _enable_intermediary_smoothing << "," << _intermediary_interval << endl;
		config << _enable_output_smoothing << "," << _output_interval << endl;
		config << _crossbar_enabled << "," << _crossbar_width << endl;
		config << _minimum_velocity_thresold << endl;
		config << _restrict_vertical_motion << endl;

		config << _frame_mode << ","
				<< _indicator_size << ","
				<< _border_size << ","
				<< _screen_select << ","
				<< _indicator_horizontal_pos << ","
				<< _indicator_vertical_pos << endl;

		config << _enable_threshold_turning << ","
				<< _min_threshold_scale << ","
				<< _max_threshold_scale << ","
				<< _current_threshold_scale << endl;

		config <<  _plot_x.size() << ", ";
		for ( unsigned int i = 0; i < _plot_x.size(); i++ )
		config << "(" << _plot_x[i] << "," <<  _plot_y[i] << "),";

		config << endl;

		config <<  _plot_vel_x.size() << ", ";
		for ( unsigned int i = 0; i < _plot_vel_x.size(); i++ )
		config << "(" << _plot_vel_x[i] << "," <<  _plot_vel_y[i] << "),";

		config << endl;
		*/

	xmlWriter.writeEndElement(); // FlyOverConfiguration

	xmlWriter.writeEndDocument();
}

// Implementation
void
Config_Memento::read_v1 ( std::ifstream& config )
{
  char ch;
  int pos;

  config >> _numCameras;

  config >> _gangCameras;

  if ( _minor >= 3 )
    config >> _xCenterDistorts;

  _center.resize ( _numCameras );
  _distort.resize ( _numCameras );
  _frame.resize ( _numCameras );
  _global_brightness.resize ( _numCameras );

  for ( int i = 0; i < _numCameras; i++ )
  {
    int lower, upper;
    bool checked;
    int focalLength_y, focalLength_x;
	// char ch;

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
    for ( int cam = 0; cam < _numCameras; cam++ )
    {
      // char ch;
      int r, c;
      GraphPositions g;

      config >> r >> ch >> c;
      g.resize ( r );

      for ( int i = 0; i < r; i++ )
      {
        qreal x, y;
        g[i].resize ( c );

        for ( int j = 0; j < c; j++ )
        {
          config >> ch >> x >> ch >> y >> ch >> ch;

          g[i][j] = std::make_pair ( (float)x, (float)y );
        }
      }

      _graph.push_back ( g );
    }
  }

  if ( _minor >= 4 )
  {
    char s[256];
    std::string sss;

    // Lighting Controls
    config >> s;
    std::string ss ( s );
    pos = ss.find ( "," );
    sss = ss.substr ( 0, pos );
    _ambient = QColor ( sss.c_str() );

    ss = ss.substr ( pos + 1 );
    pos = ss.find ( "," );
    sss = ss.substr ( 0, pos );
    _diffuse = QColor ( sss.c_str() );

    ss = ss.substr ( pos + 1 );
    pos = ss.find ( "," );
    sss = ss.substr ( 0, pos );
    from_string<int> ( _power, sss, std::dec );

    ss = ss.substr ( pos + 1 );
    pos = ss.find ( "," );
    sss = ss.substr ( 0, pos );
    _background = QColor ( sss.c_str() );

    if ( _minor == 4 )
    {
      ss = ss.substr ( pos + 1 );
      pos = ss.find ( "," );
      sss = ss.substr ( 0, pos );
      QColor color = QColor ( sss.c_str() );
      for ( int i = 0; i < _numCameras; i++ )
        _global_brightness[ i ] = color;
    }
    else
    {
      config >> s;
      ss = std::string ( s );
      for ( int i = 0; i < _numCameras; i++ )
      {
        pos = ss.find ( "," );
        sss = ss.substr ( 0, pos );
        _global_brightness[ i ] = QColor ( sss.c_str() );
        ss = ss.substr ( pos + 1 );
      }
    }

    // Calibration Parameters
    config >> s;
    ss = std::string ( s );
    pos = ss.find ( "," );
    _calib_x = ss.substr ( 0, pos );
    sss = ss.substr ( pos + 1 );
    from_string<float> ( _gain_x, sss, std::dec );

    config >> s;
    ss = std::string ( s );
    pos = ss.find ( "," );
    _calib_y = ss.substr ( 0, pos );
    sss = ss.substr ( pos + 1 );
    from_string<float> ( _gain_y, sss, std::dec );

    config >> s;
    ss = std::string ( s );
    pos = ss.find ( "," );
    _calib_z = ss.substr ( 0, pos );
    sss = ss.substr ( pos + 1 );
    from_string<float> ( _gain_z, sss, std::dec );

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
    std::string ss ( s );
    pos = ss.find ( "," );
    sss = ss.substr ( 0, pos );
    from_string<int> ( _frame_mode, sss, std::dec );

    ss = ss.substr ( pos + 1 );
    pos = ss.find ( "," );
    sss = ss.substr ( 0, pos );
    from_string<int> ( _indicator_size, sss, std::dec );

    ss = ss.substr ( pos + 1 );
    pos = ss.find ( "," );
    sss = ss.substr ( 0, pos );
    from_string<int> ( _border_size, sss, std::dec );

    ss = ss.substr ( pos + 1 );
    pos = ss.find ( "," );
    sss = ss.substr ( 0, pos );
    from_string<int> ( _screen_select, sss, std::dec );

    ss = ss.substr ( pos + 1 );
    pos = ss.find ( "," );
    sss = ss.substr ( 0, pos );
    from_string<int> ( _indicator_horizontal_pos, sss, std::dec );

    ss = ss.substr ( pos + 1 );
    pos = ss.find ( "," );
    sss = ss.substr ( 0, pos );
    from_string<int> ( _indicator_vertical_pos, sss, std::dec );
  }
}

void
Config_Memento::read_v2 ( std::ifstream& config )
{
  char ch;
  int pos;

  config >> _numCameras >> ch >> _num_displays >> ch >> _starting_display;
  config >> _gangCameras;
  config >> _xCenterDistorts;

  _center.resize ( _numCameras );
  _distort.resize ( _numCameras );
  _frame.resize ( _numCameras );
  _global_brightness.resize ( _numCameras );

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
  for ( int cam = 0; cam < _numCameras; cam++ )
  {
    // char ch;
    int r, c;
    GraphPositions g;

    config >> r >> ch >> c;
    g.resize ( r );

    for ( int i = 0; i < r; i++ )
    {
      qreal x, y;
      g[i].resize ( c );

      for ( int j = 0; j < c; j++ )
      {
        config >> ch >> x >> ch >> y >> ch >> ch;

        g[i][j] = std::make_pair ( (float)x, (float)y );
      }
    }

    _graph.push_back ( g );
  }

  char s[256];
  std::string sss;

  // Lighting Controls
  config >> s;
  std::string ss ( s );
  pos = ss.find ( "," );
  sss = ss.substr ( 0, pos );
  _ambient = QColor ( sss.c_str() );

  ss = ss.substr ( pos + 1 );
  pos = ss.find ( "," );
  sss = ss.substr ( 0, pos );
  _diffuse = QColor ( sss.c_str() );

  ss = ss.substr ( pos + 1 );
  pos = ss.find ( "," );
  sss = ss.substr ( 0, pos );
  from_string<int> ( _power, sss, std::dec );

  ss = ss.substr ( pos + 1 );
  pos = ss.find ( "," );
  sss = ss.substr ( 0, pos );
  _background = QColor ( sss.c_str() );

  config >> s;
  ss = std::string ( s );
  for ( int i = 0; i < _numCameras; i++ )
  {
    pos = ss.find ( "," );
    sss = ss.substr ( 0, pos );
    _global_brightness[ i ] = QColor ( sss.c_str() );
    ss = ss.substr ( pos + 1 );
  }

  // Fog Controls
  if ( _minor >= 7 )
  {
    config >> s;
    ss = std::string ( s );
    pos = ss.find ( "," );
    sss = ss.substr ( 0, pos );
    from_string<bool> ( _fog_enabled, sss, std::dec );

    ss = ss.substr ( pos + 1 );
    pos = ss.find ( "," );
    sss = ss.substr ( 0, pos );
    from_string<int> ( _fog_mode, sss, std::dec );

    ss = ss.substr ( pos + 1 );
    pos = ss.find ( "," );
    sss = ss.substr ( 0, pos );
    _fog_color = QColor ( sss.c_str() );

    ss = ss.substr ( pos + 1 );
    pos = ss.find ( "," );
    sss = ss.substr ( 0, pos );
    from_string<float> ( _fog_density, sss, std::dec );

    ss = ss.substr ( pos + 1 );
    pos = ss.find ( "," );
    sss = ss.substr ( 0, pos );
    from_string<int> ( _fog_start, sss, std::dec );

    ss = ss.substr ( pos + 1 );
    pos = ss.find ( "," );
    sss = ss.substr ( 0, pos );
    from_string<int> ( _fog_end, sss, std::dec );
  }
  else
  {
  	// Sensible defaults
  	_fog_enabled = false;
  	_fog_mode = 0;
  	_fog_start = 1;
  	_fog_end = 1000;
  	_fog_density = 0.05;
  	_fog_color = QColor( "#252525" );
  }

  // Calibration Parameters
  config >> s;
  ss = std::string ( s );
  pos = ss.find ( "," );
  _calib_x = ss.substr ( 0, pos );
  sss = ss.substr ( pos + 1 );
  from_string<float> ( _gain_x, sss, std::dec );

  config >> s;
  ss = std::string ( s );
  pos = ss.find ( "," );
  _calib_y = ss.substr ( 0, pos );
  sss = ss.substr ( pos + 1 );
  from_string<float> ( _gain_y, sss, std::dec );

  config >> s;
  ss = std::string ( s );
  pos = ss.find ( "," );
  _calib_z = ss.substr ( 0, pos );
  sss = ss.substr ( pos + 1 );
  from_string<float> ( _gain_z, sss, std::dec );

  // double value;
  // int iVal;

  config >> _enable_vel_smoothing >> ch >> _vel_smoothing_interval;
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
  ss = std::string ( s );
  pos = ss.find ( "," );
  sss = ss.substr ( 0, pos );
  from_string<int> ( _frame_mode, sss, std::dec );

  ss = ss.substr ( pos + 1 );
  pos = ss.find ( "," );
  sss = ss.substr ( 0, pos );
  from_string<int> ( _indicator_size, sss, std::dec );

  ss = ss.substr ( pos + 1 );
  pos = ss.find ( "," );
  sss = ss.substr ( 0, pos );
  from_string<int> ( _border_size, sss, std::dec );

  ss = ss.substr ( pos + 1 );
  pos = ss.find ( "," );
  sss = ss.substr ( 0, pos );
  from_string<int> ( _screen_select, sss, std::dec );

  ss = ss.substr ( pos + 1 );
  pos = ss.find ( "," );
  sss = ss.substr ( 0, pos );
  from_string<int> ( _indicator_horizontal_pos, sss, std::dec );

  ss = ss.substr ( pos + 1 );
  pos = ss.find ( "," );
  sss = ss.substr ( 0, pos );
  from_string<int> ( _indicator_vertical_pos, sss, std::dec );

  if ( _minor >= 1 )
  {
    config >> _shader_choice;
  }

  if ( _minor >= 7 )
  {
  	config >> _use_custom_matrix >> ch >> _edit_per_camera;
  	_frustum.clear();
    for ( int i = 0; i < _numCameras; i++ )
    {
      _frustum.push_back ( Frustum() );
      config >> _frustum[i];
    }
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
    _plot_x.resize ( r );
    _plot_y.resize ( r );

    for ( int i = 0; i < r; i++ )
    {
      double x, y;
      config >> ch >> x >> ch >> y >> ch >> ch;

      _plot_x[i] = x;
      _plot_y[i] = y;
    }

    // velocity plot widget
    config >> r >> ch;
    _plot_vel_x.resize ( r );
    _plot_vel_y.resize ( r );

    for ( int i = 0; i < r; i++ )
    {
      double x, y;
      config >> ch >> x >> ch >> y >> ch >> ch;

      _plot_vel_x[i] = x;
      _plot_vel_y[i] = y;
    }

  }
}

