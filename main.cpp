#include "nvse/PluginAPI.h"
#include "nvse/CommandTable.h"
#include "nvse/GameAPI.h"
#include "nvse/ParamInfos.h"
#include "nvse/GameObjects.h"
#include "AnotherCCLib/CCConnector.h"
#include "NVTangoGlobals.h"
#include "NVTangoMainThread.h"
#include "NVTangoRunOnMainThread.h"
#include <string>

NVTangoMainThread* mainthread;


// Shortcut macro to register a script command (assigning it an Opcode).
#define RegisterScriptCommand(name) 	nvse->RegisterCommand(&kCommandInfo_ ##name)

// Short version of RegisterScriptCommand.
#define REG_CMD(name) RegisterScriptCommand(name)

// Use this when the function's return type is not a number (when registering array/form/string functions).
//Credits: taken from JohnnyGuitarNVSE.
#define REG_TYPED_CMD(name, type)	nvse->RegisterTypedCommand(&kCommandInfo_##name,kRetnType_##type)


// This is a message handler for nvse events
// With this, plugins can listen to messages such as whenever the game loads
void MessageHandler(NVSEMessagingInterface::Message* msg)
{
	switch (msg->type)
	{
	case NVSEMessagingInterface::kMessage_PostLoad: break;
	case NVSEMessagingInterface::kMessage_ExitGame: break;
	case NVSEMessagingInterface::kMessage_ExitToMainMenu: break;
	case NVSEMessagingInterface::kMessage_LoadGame: break;
	case NVSEMessagingInterface::kMessage_SaveGame: break;
#if EDITOR
	case NVSEMessagingInterface::kMessage_ScriptEditorPrecompile: break;
#endif
	case NVSEMessagingInterface::kMessage_PreLoadGame: break;
	case NVSEMessagingInterface::kMessage_ExitGame_Console: break;
	case NVSEMessagingInterface::kMessage_PostLoadGame: break;
	case NVSEMessagingInterface::kMessage_PostPostLoad: break;
	case NVSEMessagingInterface::kMessage_RuntimeScriptError: break;
	case NVSEMessagingInterface::kMessage_DeleteGame: break;
	case NVSEMessagingInterface::kMessage_RenameGame: break;
	case NVSEMessagingInterface::kMessage_RenameNewGame: break;
	case NVSEMessagingInterface::kMessage_NewGame: break;
	case NVSEMessagingInterface::kMessage_DeleteGameName: break;
	case NVSEMessagingInterface::kMessage_RenameGameName: break;
	case NVSEMessagingInterface::kMessage_RenameNewGameName: break;
	case NVSEMessagingInterface::kMessage_DeferredInit: break;
	case NVSEMessagingInterface::kMessage_ClearScriptDataCache: break;
	case NVSEMessagingInterface::kMessage_MainGameLoop:
		NVTangoRunOnMainThread::ExecuteList();
		break;
	case NVSEMessagingInterface::kMessage_ScriptCompile: break;
	case NVSEMessagingInterface::kMessage_EventListDestroyed: break;
	case NVSEMessagingInterface::kMessage_PostQueryPlugins: break;
	default: break;
	}
}

bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info)
{
	_MESSAGE("nvtangocc has been queried, this is the first part about getting loaded.");

	// fill out the info structure
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "nvtangocc";
	info->version = 2;

	// version checks
	if (nvse->nvseVersion < PACKED_NVSE_VERSION)
	{
		_ERROR("NVSE version too old (got %08X expected at least %08X)", nvse->nvseVersion, PACKED_NVSE_VERSION);
		return false;
	}

	if (!nvse->isEditor)
	{
		if (nvse->runtimeVersion < RUNTIME_VERSION_1_4_0_525)
		{
			_ERROR("incorrect runtime version (got %08X need at least %08X)", nvse->runtimeVersion, RUNTIME_VERSION_1_4_0_525);
			return false;
		}

		if (nvse->isNogore)
		{
			_ERROR("NoGore is not supported");
			return false;
		}
	}
	else
	{
		if (nvse->editorVersion < CS_VERSION_1_4_0_518)
		{
			_ERROR("incorrect editor version (got %08X need at least %08X)", nvse->editorVersion, CS_VERSION_1_4_0_518);
			return false;
		}
	}

	// version checks pass
	// any version compatibility checks should be done here
	return true;
}

bool NVSEPlugin_Load(NVSEInterface* nvse)
{
	_MESSAGE("nvtangocc has been loaded!");
	
	NVTangoGlobals::g_pluginHandle = nvse->GetPluginHandle();

	// save the NVSE interface in case we need it later
	NVTangoGlobals::g_nvseInterface = nvse;

	// register to receive messages from NVSE
	NVTangoGlobals::g_messagingInterface = static_cast<NVSEMessagingInterface*>(nvse->QueryInterface(kInterface_Messaging));
	NVTangoGlobals::g_messagingInterface->RegisterListener(NVTangoGlobals::g_pluginHandle, "NVSE", MessageHandler);

	if (!nvse->isEditor)
	{
#if RUNTIME
		// script and function-related interfaces
		NVTangoGlobals::g_script = static_cast<NVSEScriptInterface*>(nvse->QueryInterface(kInterface_Script));
		NVTangoGlobals::g_stringInterface = static_cast<NVSEStringVarInterface*>(nvse->QueryInterface(kInterface_StringVar));
		NVTangoGlobals::g_arrayInterface = static_cast<NVSEArrayVarInterface*>(nvse->QueryInterface(kInterface_ArrayVar));
		NVTangoGlobals::g_dataInterface = static_cast<NVSEDataInterface*>(nvse->QueryInterface(kInterface_Data));
		NVTangoGlobals::g_eventInterface = static_cast<NVSEEventManagerInterface*>(nvse->QueryInterface(kInterface_EventManager));
		NVTangoGlobals::g_serializationInterface = static_cast<NVSESerializationInterface*>(nvse->QueryInterface(kInterface_Serialization));
		NVTangoGlobals::g_consoleInterface = static_cast<NVSEConsoleInterface*>(nvse->QueryInterface(kInterface_Console));
		NVTangoGlobals::ExtractArgsEx = NVTangoGlobals::g_script->ExtractArgsEx;

#if WantInventoryRefFunctions
		InventoryReferenceCreate = (_InventoryReferenceCreate)g_dataInterface->GetFunc(NVSEDataInterface::kNVSEData_InventoryReferenceCreate);
		InventoryReferenceGetForRefID = (_InventoryReferenceGetForRefID)g_dataInterface->GetFunc(NVSEDataInterface::kNVSEData_InventoryReferenceGetForRefID);
		InventoryReferenceGetRefBySelf = (_InventoryReferenceGetRefBySelf)g_dataInterface->GetFunc(NVSEDataInterface::kNVSEData_InventoryReferenceGetRefBySelf);
		InventoryReferenceCreateEntry = (_InventoryReferenceCreateEntry)g_dataInterface->GetFunc(NVSEDataInterface::kNVSEData_InventoryReferenceCreateEntry);
#endif

#if WantLambdaFunctions
		LambdaDeleteAllForScript = (_LambdaDeleteAllForScript)g_dataInterface->GetFunc(NVSEDataInterface::kNVSEData_LambdaDeleteAllForScript);
		LambdaSaveVariableList = (_LambdaSaveVariableList)g_dataInterface->GetFunc(NVSEDataInterface::kNVSEData_LambdaSaveVariableList);
		LambdaUnsaveVariableList = (_LambdaUnsaveVariableList)g_dataInterface->GetFunc(NVSEDataInterface::kNVSEData_LambdaUnsaveVariableList);
		IsScriptLambda = (_IsScriptLambda)g_dataInterface->GetFunc(NVSEDataInterface::kNVSEData_IsScriptLambda);
#endif

#if WantScriptFunctions
		HasScriptCommand = (_HasScriptCommand)g_dataInterface->GetFunc(NVSEDataInterface::kNVSEData_HasScriptCommand);
		DecompileScript = (_DecompileScript)g_dataInterface->GetFunc(NVSEDataInterface::kNVSEData_DecompileScript);
#endif

#endif
	}
	

	/***************************************************************************
	 *
	 *	READ THIS!
	 *
	 *	Before releasing your plugin, you need to request an opcode range from
	 *	the NVSE team and set it in your first SetOpcodeBase call. If you do not
	 *	do this, your plugin will create major compatibility issues with other
	 *	plugins, and will not load in release versions of NVSE. See
	 *	nvse_readme.txt for more information.
	 *
	 *	See https://geckwiki.com/index.php?title=NVSE_Opcode_Base
	 *
	 **************************************************************************/

	// Do NOT use this value when releasing your plugin; request your own opcode range.
	//UInt32 const examplePluginOpcodeBase = 0x2000;
	
	 // register commands
	//nvse->SetOpcodeBase(examplePluginOpcodeBase);

	mainthread = new NVTangoMainThread();
	mainthread->Start();
	
	return true;
}
