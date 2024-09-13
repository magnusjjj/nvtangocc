#include "NVTangoMainThread.h"
#include <iostream>
#include "NVTangoGlobals.h"
#include <chrono>
#include "NVEffectRun.h"
using namespace std;

void NVTangoMainThread::MainThread()
{
    connector = new CCConnector();
    connector->AddEffect(new std::regex("run_(.*)"), []() {return (CCEffect*) new NVEffectRun(); });
    //auto lambda = []() {return new TestNormalEffect(); };
    //connector->AddEffect(new std::regex("playsound.*"), []() {return (CCEffect*) new TestNormalEffect(); });
    //connector->AddEffect(new std::regex("controlsfasttraveloff_0_60"), []() {return (CCEffect*) new TestNormalEffect(); });

    // Todo: Automatic reconnect
    _MESSAGE("Starting to connect");

    while (true) {
        if (connector->Connect()) {
            break;
        }
        else {
            if (connector->hasError)
            {
                _MESSAGE(connector->error);
            }
            std::this_thread::sleep_for(500ms);
        }

    }
    std::cout << "Connected!" << std::endl;
    connector->StartTimerThread();

    while (true) {
        std::this_thread::sleep_for(500ms);
    }
}

void NVTangoMainThread::Start()
{
	this->mainthread = new std::thread(&NVTangoMainThread::MainThread, this);
}
