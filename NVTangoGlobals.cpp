#include "NVTangoGlobals.h"
	IDebugLog NVTangoGlobals::gLog;
	PluginHandle	NVTangoGlobals::g_pluginHandle = kPluginHandle_Invalid;

	NVSEMessagingInterface* NVTangoGlobals::g_messagingInterface{};
	NVSEInterface* NVTangoGlobals::g_nvseInterface{};
	NVSECommandTableInterface* NVTangoGlobals::g_cmdTableInterface{};

	// RUNTIME = Is not being compiled as a GECK plugin.
#if RUNTIME
	NVSEScriptInterface* NVTangoGlobals::g_script{};
	NVSEStringVarInterface* NVTangoGlobals::g_stringInterface{};
	NVSEArrayVarInterface* NVTangoGlobals::g_arrayInterface{};
	NVSEDataInterface* NVTangoGlobals::g_dataInterface{};
	NVSESerializationInterface* NVTangoGlobals::g_serializationInterface{};
	NVSEConsoleInterface* NVTangoGlobals::g_consoleInterface{};
	NVSEEventManagerInterface* NVTangoGlobals::g_eventInterface{};
	bool (*NVTangoGlobals::ExtractArgsEx)(COMMAND_ARGS_EX, ...);

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
