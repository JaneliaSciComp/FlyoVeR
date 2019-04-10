#include "ReinforcementZoneWidget.h"

ReinforcementZoneWidget::ReinforcementZoneWidget(QString categoryName, QWidget* parent)
	: QGroupBox(categoryName, parent)
	, zoneID(categoryName)
{
	setupUi(this);
	setTitle("Zone " + categoryName);
	delegate_signals();
}

ReinforcementZoneWidget::ReinforcementZoneWidget(QWidget* parent)
	: QGroupBox(parent)
	, zoneID("")
{
	setupUi(this);
	setTitle("Zone " + zoneID);
	delegate_signals();
}
void ReinforcementZoneWidget::delegate_signals() 
{
	// Delegate subwidget signals to top level
	connect( zone_inner_radius_spinBox, SIGNAL( valueChanged(double) ), 
		this, SIGNAL( innerRadiusChanged(double) ) );
	connect( zone_outer_radius_spinBox, SIGNAL( valueChanged(double) ), 
		this, SIGNAL( outerRadiusChanged(double) ) );
	connect( zone_respawn_radius_spinBox, SIGNAL( valueChanged(double) ), 
		this, SIGNAL( respawnRadiusChanged(double) ) );
	connect( zone_max_val_spinBox, SIGNAL( valueChanged(int) ), 
		this, SIGNAL( centralValueChanged(int) ) );
	connect( zone_gamma_spinBox, SIGNAL( valueChanged(double) ), 
		this, SIGNAL( gammaExponentChanged(double) ) );
	connect( zone_respawn_timeout_spinBox, SIGNAL( valueChanged(double) ), 
		this, SIGNAL( respawnTimeoutChanged(double) ) );

	// Delegate fine-grained signals, to summary signal
	connect(this, SIGNAL( innerRadiusChanged(double) ), 
		this, SIGNAL(somethingChanged()) );
	connect(this, SIGNAL( outerRadiusChanged(double) ), 
		this, SIGNAL(somethingChanged()) );
	connect(this, SIGNAL( respawnRadiusChanged(double) ), 
		this, SIGNAL(somethingChanged()) );
	connect(this, SIGNAL( centralValueChanged(int) ), 
		this, SIGNAL(somethingChanged()) );
	connect(this, SIGNAL( gammaExponentChanged(double) ), 
		this, SIGNAL(somethingChanged()) );
	connect(this, SIGNAL( respawnTimeoutChanged(double) ), 
		this, SIGNAL(somethingChanged()) );

	connect( zone_duration_spinBox, SIGNAL( valueChanged(double) ),
		this, SIGNAL( somethingChanged() ));
	connect( zone_reset_radius_spinBox, SIGNAL(valueChanged(double)),
		this, SIGNAL(somethingChanged()));
}

QString ReinforcementZoneWidget::getCategoryName() const
{
	return zoneID;
}

// slots

void ReinforcementZoneWidget::setInnerRadius( double innerRadius )
{
	zone_inner_radius_spinBox->setValue(innerRadius);
}

void ReinforcementZoneWidget::setOuterRadius( double outerRadius )
{
	zone_outer_radius_spinBox->setValue(outerRadius);
}

void ReinforcementZoneWidget::setRespawnRadius( double respawnRadius )
{
	zone_respawn_radius_spinBox->setValue(respawnRadius);
}

void ReinforcementZoneWidget::setCentralValue( int centralValue )
{
	zone_max_val_spinBox->setValue(centralValue);
}

void ReinforcementZoneWidget::setGammaExponent( double gammaExponent )
{
	zone_gamma_spinBox->setValue(gammaExponent);
}

void ReinforcementZoneWidget::setRespawnTimeout( double respawnTimeout )
{
	zone_respawn_timeout_spinBox->setValue(respawnTimeout);
}

void ReinforcementZoneWidget::setReinforcementDuration(double reinforcementDuration)
{
    zone_duration_spinBox->setValue(reinforcementDuration);
}

void ReinforcementZoneWidget::setZoneResetRadius(double zoneResetRadius)
{
    zone_reset_radius_spinBox->setValue(zoneResetRadius);
}
