#include "NVEffectRun.h"
#include "NVTangoGlobals.h"
#include "NVTangoRunOnMainThread.h"
#include <ranges>

void NVEffectRun::Start() {
	std::string command = this->arguments->at(1);
	std::ranges::replace(command, '_', ' ');
	NVTangoRunOnMainThread::RunOnMainThread([command] {
		NVTangoGlobals::g_consoleInterface->RunScriptLine(strdup(command.c_str()), NULL);
	});
	
}

void NVEffectRun::Stop()
{
}
