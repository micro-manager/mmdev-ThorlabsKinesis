#pragma once
#include <string>
#include <memory>
#include <vector>
#include <map>

class KinesisXMLFunctions
{
public:
	static int getSupportedStages(uint32_t device_id, std::vector<std::string>* devices);
	static int getStageSettings(std::string settingsName, std::map<int, double>* settings);
};

enum KinesisDeviceSettingsID
{
	SettingsTypeHomeDirection, 
	SettingsTypeHomeLimitSwitch, 
	SettingsTypeHomeVelocity, 
	SettingsTypeHomeZeroOffset, 
	SettingsTypeMotorPitch, 
	SettingsTypeMotorStepsPerRev, 
	SettingsTypeMotorGearboxRatio, 
	SettingsTypeMotorDirectionSense, 
	SettingsTypeMotorUnits,
	SettingsTypeLimitCWHardLimit, 
	SettingsTypeLimitCCWHardLimit, 
	SettingsTypeLimitCWSoftLimit, 
	SettingsTypeLimitCCWSoftLimit, 
	SettingsTypeLimitSoftLimitMode, 
	SettingsTypeBacklashDistance
};
