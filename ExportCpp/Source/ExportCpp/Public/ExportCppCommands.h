// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "ExportCppStyle.h"

class FExportCppCommands : public TCommands<FExportCppCommands>
{
public:

	FExportCppCommands()
		: TCommands<FExportCppCommands>(TEXT("ExportCpp"), NSLOCTEXT("Contexts", "ExportCpp", "ExportCpp Plugin"), NAME_None, FExportCppStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
