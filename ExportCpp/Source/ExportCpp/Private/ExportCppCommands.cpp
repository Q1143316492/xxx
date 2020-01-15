// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ExportCppCommands.h"

#define LOCTEXT_NAMESPACE "FExportCppModule"

void FExportCppCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "ExportCpp", "Execute ExportCpp action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
