// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ExportCpp.h"
#include "ExportCppStyle.h"
#include "ExportCppCommands.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "LevelEditor.h"

static const FName ExportCppTabName("ExportCpp");

#define LOCTEXT_NAMESPACE "FExportCppModule"

void FExportCppModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FExportCppStyle::Initialize();
	FExportCppStyle::ReloadTextures();

	FExportCppCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FExportCppCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FExportCppModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FExportCppModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FExportCppModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
}

void FExportCppModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FExportCppStyle::Shutdown();

	FExportCppCommands::Unregister();
}

void FExportCppModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here

	UEPreExportObject expObj;
	expObj.ExportUClass();
	//expObj.ExportUEnum(true);
	//expObj.ExportUScriptStruct(true);

	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "export Cpp version {0} begin"),
							FText::FromString(TEXT("(0.0)")));
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FExportCppModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FExportCppCommands::Get().PluginAction);
}

void FExportCppModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FExportCppCommands::Get().PluginAction);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FExportCppModule, ExportCpp)