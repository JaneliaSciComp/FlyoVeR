#ifndef REINFORCEMENT_ZONE_WIDGET_H
#define REINFORCEMENT_ZONE_WIDGET_H

#include <QGroupBox>
#include "ui_ReinforcementCategory.h"

class ReinforcementZoneWidget : public QGroupBox, public Ui::ReinforcementCategoryGroupBox
{
	Q_OBJECT

public:
	ReinforcementZoneWidget(QString categoryName, QWidget* parent);
	ReinforcementZoneWidget(QWidget* parent);
	QString getCategoryName() const;

signals:
	void innerRadiusChanged( double innerRadius );
	void outerRadiusChanged( double outerRadius );
	void respawnRadiusChanged( double respawnRadius );
	void centralValueChanged( int value );
	void gammaExponentChanged( double gammaExponent );
	void respawnTimeoutChanged( double respawnTimeout );
	void somethingChanged();

public slots:
	void setInnerRadius( double innerRadius );
	void setOuterRadius( double outerRadius );
	void setRespawnRadius( double respawnRadius );
	void setCentralValue( int centralValue );
	void setGammaExponent( double gammaExponent );
	void setRespawnTimeout( double respawnTimeout );

    void setReinforcementDuration(double reinforcementDuration);

    void setZoneResetRadius(double zoneResetRadius);

private:
	void delegate_signals();
	QString zoneID;
};

#endif // REINFORCEMENT_ZONE_WIDGET_H
