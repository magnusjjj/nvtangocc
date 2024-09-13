#include "NVEffectRun.h"
#include "NVTangoGlobals.h"
#include <ranges>

void NVEffectRun::Start() {
	std::string command = this->arguments->at(1);
	std::ranges::replace(command, '_', ' ');
	NVTangoGlobals::g_consoleInterface->RunScriptLine(strdup(command.c_str()), NULL);
}

void NVEffectRun::Stop()
{
}
