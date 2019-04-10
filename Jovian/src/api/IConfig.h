#ifndef JOVIAN_API_ICONFIG_H
#define JOVIAN_API_ICONFIG_H

// Reinforcement zone interface
class IZone {
public:
	virtual ~IZone() {}

	virtual double getReinforcementDuration() const = 0;
	virtual void setReinforcementDuration(double) = 0;
};

// base interface for FlyOver/Jovian session configuration information
// Intended to be common base class for ConfigMemento, Console, and <some intermediate xml representation>
class IConfig {
public:
	virtual ~IConfig() {}

	virtual bool isFogEnabled() const = 0;
	virtual void setFogEnabled(bool) = 0;
};

#endif // JOVIAN_API_ICONFIG_H
