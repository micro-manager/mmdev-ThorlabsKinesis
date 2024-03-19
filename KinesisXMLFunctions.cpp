#include "KinesisXMLFunctions.h"
#include "tinyxml2.h"

namespace {
    std::string const SETTINGS_XML_PATH = "C:\\ProgramData\\Thorlabs\\MotionControl\\ThorlabsDefaultSettings.xml";
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