#pragma once
#include <mutex>
#include <functional>
class NVTangoRunOnMainThread
{
	static std::mutex codeToRunMutex;
	static std::list<std::function <void()>> codelist;
public:
	static void RunOnMainThread(std::function <void()> f);
	static void ExecuteList();
};

