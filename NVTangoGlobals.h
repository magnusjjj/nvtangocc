#pragma once
#include "nvse/PluginAPI.h"
#include "nvse/CommandTable.h"
#include "nvse/GameAPI.h"
#include "nvse/ParamInfos.h"
#include "nvse/GameObjects.h"
class NVTangoGlobals
{
public:
	static void Init() {
		gLog = ("nvse_plugin_example.log");
	}
	static IDebugLog gLog;
	static PluginHandle	g_pluginHandle;

	static NVSEMessagingInterface* g_messagingInterface;
	static NVSEInterface* g_nvseInterface;
	static NVSECommandTableInterface* g_cmdTableInterface;

	// RUNTIME = Is not being compiled as a GECK plugin.
#if RUNTIME
	static NVSEScriptInterface* g_script;
	static NVSEStringVarInterface* g_stringInterface;
	static NVSEArrayVarInterface* g_arrayInterface;
	static NVSEDataInterface* g_dataInterface;
	static NVSESerializationInterface* g_serializationInterface;
	static NVSEConsoleInterface* g_consoleInterface;
	static NVSEEventManagerInterface* g_eventInterface;
	static bool (*ExtractArgsEx)(COMMAND_ARGS_EX, ...);

#define WantInventoryRefFunctions 0 // set to 1 if you want these PluginAPI functions
#if WantInventoryRefFunctions
	_InventoryReferenceCreate InventoryReferenceCreate{};
	_InventoryReferenceGetForRefID InventoryReferenceGetForRefID{};
	_InventoryReferenceGetRefBySelf InventoryReferenceGetRefBySelf{};
	_InventoryReferenceCreateEntry InventoryReferenceCreateEntry{};
#endif

#define WantLambdaFunctions 0 // set to 1 if you want these PluginAPI functions
#if WantLambdaFunctions
	_LambdaDeleteAllForScript LambdaDeleteAllForScript{};
	_LambdaSaveVariableList LambdaSaveVariableList{};
	_LambdaUnsaveVariableList LambdaUnsaveVariableList{};
	_IsScriptLambda IsScriptLambda{};
#endif

#define WantScriptFunctions 0 // set to 1 if you want these PluginAPI functions
#if WantScriptFunctions
	_HasScriptCommand HasScriptCommand{};
	_DecompileScript DecompileScript{};
#endif

#endif
};

