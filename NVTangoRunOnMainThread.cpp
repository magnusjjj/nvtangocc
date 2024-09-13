#include "NVTangoRunOnMainThread.h"

std::mutex NVTangoRunOnMainThread::codeToRunMutex;
std::list<std::function <void()>> NVTangoRunOnMainThread::codelist;

void NVTangoRunOnMainThread::RunOnMainThread(std::function<void()> f)
{
	codeToRunMutex.lock();
	codelist.push_back(f);
	codeToRunMutex.unlock();
}

void NVTangoRunOnMainThread::ExecuteList()
{
	codeToRunMutex.lock();
	for (auto f : codelist) {
		f();
	}
	codelist.clear();
	codeToRunMutex.unlock();
}
