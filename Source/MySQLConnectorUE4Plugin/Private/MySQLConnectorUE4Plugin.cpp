#include "MySQLConnectorUE4Plugin.h"
#include "CreatureDetails.h"
#include "GameobjectDetails.h"
#include "Entities/Creature.h"
#include "Entities/Gameobject.h"
#include "PropertyEditorModule.h"

void FMySQLConnectorUE4Plugin::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	
	// Custom detail views
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(FName("PropertyEditor"));

	PropertyModule.RegisterCustomClassLayout(ACreature::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FCreatureDetails::MakeInstance));
	PropertyModule.RegisterCustomClassLayout(AGameObject::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FGameobjectDetails::MakeInstance));

	//open DB connection
	FString host("127.0.0.1"), userName("root"), userPassword("Despotar.94"), databaseName("development");

	MysqlConnection.MySQLInitConnection(host, userName, userPassword, databaseName);
}

void FMySQLConnectorUE4Plugin::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	MysqlConnection.MySQLCloseConnection();
}