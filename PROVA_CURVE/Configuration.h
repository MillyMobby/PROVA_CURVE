#pragma once
#include "Viewer.h"
#include "ImguiController.h"
#include <math.h>
#include "Scene.h"
class Configuration
{
private:

	Configuration();
	//Configuration(std::string fileName);
	~Configuration() = default;



public:
	static Configuration& getIstance();
	Viewer& getView();
};

