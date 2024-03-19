#pragma once
#include <string>
#include <memory>
#include <vector>

class KinesisXMLFunctions
{
public:
	static int getSupportedStages(uint32_t device_id, std::vector<std::string>* devices);
};
