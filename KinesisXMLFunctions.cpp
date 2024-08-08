#include "KinesisXMLFunctions.h"

namespace {
    std::string const SETTINGS_XML_PATH = "C:\\ProgramData\\Thorlabs\\MotionControl\\ThorlabsDefaultSettings.xml";
    std::string const BBD_SETTINGS_XML_PATH = "C:\\Program Files\\Thorlabs\\Kinesis\\BBD_Stages.xml";
}

int KinesisXMLFunctions::getSupportedStages(uint32_t device_id, std::vector<std::string>* devices)
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile(SETTINGS_XML_PATH.c_str());


    tinyxml2::XMLElement* devicesElement = doc.FirstChildElement("SettingsCollection")->FirstChildElement("Devices");
    if (devicesElement == nullptr)
    {
        return -1;
    }
    for (const tinyxml2::XMLElement* deviceType = devicesElement->FirstChildElement(); deviceType != nullptr; deviceType = deviceType->NextSiblingElement())
    {
        const tinyxml2::XMLAttribute* id = deviceType->FindAttribute("ID");
        if (device_id == atoi(id->Value()))
        {
            const tinyxml2::XMLElement* deviceSettingsElement = deviceType->FirstChildElement("DeviceSettings");
            if (deviceSettingsElement == nullptr)
            {
                return -1;
            }
            for (const tinyxml2::XMLElement* deviceSettingsTypeElement = deviceSettingsElement->FirstChildElement(); deviceSettingsTypeElement != nullptr; deviceSettingsTypeElement = deviceSettingsTypeElement->NextSiblingElement())
            {
                const tinyxml2::XMLAttribute* devName = deviceSettingsTypeElement->FindAttribute("Name");
                devices->push_back(std::string(devName->Value()));
            }
            break;
        }
    }
    return doc.ErrorID();
}

int KinesisXMLFunctions::getStageSettings(std::string stageName, std::map<int, double>* settings)
{

    tinyxml2::XMLDocument defaultDoc;
    tinyxml2::XMLDocument bbdDoc;
    auto ret = defaultDoc.LoadFile(SETTINGS_XML_PATH.c_str());
    if (ret)
    {
        return ret;
    }
    ret = bbdDoc.LoadFile(BBD_SETTINGS_XML_PATH.c_str());
    if (ret)
    {
        return ret;
    }

    if (isNameFoundInDefaultSettings(stageName, &defaultDoc))
    {
        //populate from default xml
        populateSettingsFromDefaultDoc(stageName, &defaultDoc, settings);
    }
    else if (isNameFoundInBBDSettings(stageName, &bbdDoc))
    {
        //populate from bbd xml
        populateSettingsFromBBDDoc(stageName, &bbdDoc, settings);
    }

    return EXIT_SUCCESS;
}

bool KinesisXMLFunctions::isNameFoundInDefaultSettings(std::string name, tinyxml2::XMLDocument* doc)
{

    tinyxml2::XMLElement* deviceSettingsListElement = doc->FirstChildElement("SettingsCollection")->FirstChildElement("DeviceSettingsList");
    if (deviceSettingsListElement == nullptr)
    {
        return false;
    }
    for (const tinyxml2::XMLElement* deviceSettingsDefinition = deviceSettingsListElement->FirstChildElement(); deviceSettingsDefinition != nullptr; deviceSettingsDefinition = deviceSettingsDefinition->NextSiblingElement())
    {
        const tinyxml2::XMLAttribute* xmlName = deviceSettingsDefinition->FindAttribute("Name");
        if (strcmp(name.c_str(), xmlName->Value()) == 0)
        {
            return true;
        }
    }

    return false;
}

bool KinesisXMLFunctions::isNameFoundInBBDSettings(std::string name, tinyxml2::XMLDocument* doc)
{
    tinyxml2::XMLElement* recordsElement = doc->FirstChildElement("data-set")->FirstChildElement("Records");
    if (recordsElement == nullptr)
    {
        return false;
    }
    for (const tinyxml2::XMLElement* record = recordsElement->FirstChildElement("record"); record != nullptr; record = record->NextSiblingElement())
    {
        const tinyxml2::XMLElement* xmlStage = record->FirstChildElement("Stage");
        if (strcmp(name.c_str(), xmlStage->GetText()) == 0)
        {
            return true;
        }
    }
    return false;
}

void KinesisXMLFunctions::populateSettingsFromDefaultDoc(std::string name, tinyxml2::XMLDocument* doc, std::map<int, double>* settings)
{
    tinyxml2::XMLElement* deviceSettingsListElement = doc->FirstChildElement("SettingsCollection")->FirstChildElement("DeviceSettingsList");
    if (deviceSettingsListElement == nullptr)
    {
        return;
    }
    for (const tinyxml2::XMLElement* deviceSettingsDefinition = deviceSettingsListElement->FirstChildElement(); deviceSettingsDefinition != nullptr; deviceSettingsDefinition = deviceSettingsDefinition->NextSiblingElement())
    {
        const tinyxml2::XMLAttribute* xmlName = deviceSettingsDefinition->FindAttribute("Name");
        if (strcmp(name.c_str(), xmlName->Value()) == 0)
        {
            const tinyxml2::XMLElement* physicalElement = deviceSettingsDefinition->FirstChildElement("Physical");
            if (physicalElement != nullptr)
            {
                const tinyxml2::XMLElement* unitsElement = physicalElement->FirstChildElement("Units");
                if (unitsElement != nullptr)
                {
                    settings->insert(std::pair<int, double>(SettingsTypeMotorUnits, atof(unitsElement->GetText())));
                }

                const tinyxml2::XMLElement* motorPitchElement = physicalElement->FirstChildElement("Pitch");
                if (motorPitchElement != nullptr)
                {
                    settings->insert(std::pair<int, double>(SettingsTypeMotorPitch, atof(motorPitchElement->GetText())));
                }

                const tinyxml2::XMLElement* gearboxRationElement = physicalElement->FirstChildElement("GearboxRatio");
                if (gearboxRationElement != nullptr)
                {
                    settings->insert(std::pair<int, double>(SettingsTypeMotorGearboxRatio, atof(gearboxRationElement->GetText())));
                }

                const tinyxml2::XMLElement* stepsPerRevElement = physicalElement->FirstChildElement("StepsPerRev");
                if (stepsPerRevElement != nullptr)
                {
                    settings->insert(std::pair<int, double>(SettingsTypeMotorStepsPerRev, atof(stepsPerRevElement->GetText())));
                }

                // Add any additional settings to the map here
            }
            break;
        }
    }
}

void KinesisXMLFunctions::populateSettingsFromBBDDoc(std::string name, tinyxml2::XMLDocument* doc, std::map<int, double>* settings)
{
    tinyxml2::XMLElement* recordsElement = doc->FirstChildElement("data-set")->FirstChildElement("Records");
    if (recordsElement == nullptr)
    {
        return;
    }
    bool travelUnitsSet = false;
    for (const tinyxml2::XMLElement* record = recordsElement->FirstChildElement("record"); record != nullptr; record = record->NextSiblingElement())
    {
        const tinyxml2::XMLElement* xmlStage = record->FirstChildElement("Stage");
        if (strcmp(name.c_str(), xmlStage->GetText()) == 0)
        {
            if (travelUnitsSet)
            {
                //BBD XML does not contain the definitions for travel units. Setting this here for loading after
                if (name.find("R") != std::string::npos) //Stage names for rotation BBD stages contain the letter R
                {
                    settings->insert(std::pair<int, double>(SettingsTypeMotorUnits, 2.0));
                }
                else
                {
                    settings->insert(std::pair<int, double>(SettingsTypeMotorUnits, 2.0));
                }
                travelUnitsSet = true;
            }
            const tinyxml2::XMLElement* recordName = record->FirstChildElement("Name");
            if (recordName != nullptr)
            {
                if (strcmp("Encoder count per unit", recordName->GetText()) == 0)
                {
                    const tinyxml2::XMLElement* recordValue = record->FirstChildElement("Data");
                    if (recordValue != nullptr)
                    {
                        settings->insert(std::pair<int, double>(SettingsTypeMotorStepsPerRev, atoi(recordValue->GetText())));
                    }
                    
                }
                else if(strcmp("Encoder To Step Ratio Counts", recordName->GetText()) == 0)
                {
                    const tinyxml2::XMLElement* recordValue = record->FirstChildElement("Data");
                    if (recordValue != nullptr)
                    {
                        settings->insert(std::pair<int, double>(SettingsTypeMotorGearboxRatio, atoi(recordValue->GetText())));
                    }
                }
                else if(strcmp("ADD VALUES THAT NEED TO BE READ", recordName->GetText()) == 0)
                {
                    // Add any additional settings to the map here
                }
            }
        }
    }
}


