#pragma once
#include <string>
#include <memory>
#include <vector>
#include <map>
#include "tinyxml2.h"

class KinesisXMLFunctions
{
public:
	static int getSupportedStages(uint32_t device_id, std::vector<std::string>* devices);
	static int getStageSettings(std::string settingsName, std::map<int, double>* settings);

private:
	static bool isNameFoundInDefaultSettings(std::string name, tinyxml2::XMLDocument* doc);
	static bool isNameFoundInBBDSettings(std::string name, tinyxml2::XMLDocument* doc);
	static void populateSettingsFromDefaultDoc(std::string name, tinyxml2::XMLDocument* doc, std::map<int, double>* settings);
	static void populateSettingsFromBBDDoc(std::string name, tinyxml2::XMLDocument* doc, std::map<int, double>* settings);
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
