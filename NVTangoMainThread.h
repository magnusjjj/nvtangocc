#pragma once
#include <thread>
#include "AnotherCCLib/CCConnector.h"
class NVTangoMainThread
{
	std::thread* mainthread;
	CCConnector* connector;

	void MainThread();
public:
	void Start();
};

