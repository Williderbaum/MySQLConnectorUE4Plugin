#include "CreatureDetails.h"
#include "IDetailsView.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "Entities/Creature.h"
#include "ObjectProperties.h"
#include "DBEditor.h"



FCreatureDetails::~FCreatureDetails()
{
	for(const auto& info : CreatureInfos)
		info->RemoveFromRoot();
}

TSharedRef<IDetailCustomization> FCreatureDetails::MakeInstance()
{
	return TSharedRef<IDetailCustomization>(new FCreatureDetails);
}

void FCreatureDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{

	TArray<TWeakObjectPtr<UObject>> selectedObjects;

	DetailBuilder.GetObjectsBeingCustomized(selectedObjects);

	//Load our Property Module
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDBEditorModule& dbEditor = FModuleManager::LoadModuleChecked<FDBEditorModule>("DBEditor");
	//FDetailsViewArgs is a struct of settings to customize our Details View Widget
	FDetailsViewArgs Args;
	Args.bHideSelectionTip = true;
	Args.bAllowSearch = false;

	//Create the widget and store it in the PropertyWidget pointer
	TSharedRef<IDetailsView> PropertyWidget = PropertyModule.CreateDetailView(Args);

	IDetailCategoryBuilder& CustomCategory = DetailBuilder.EditCategory("DB Update", FText::GetEmpty(), ECategoryPriority::Important);


	for (const auto& object : selectedObjects)
	{
		ACreature* creature = Cast<ACreature>(object);
		UCreatureInfo* info = NewObject<UCreatureInfo>();
		info->AddToRoot();
		FString query = FString::Printf(TEXT("SELECT * FROM creature WHERE guid %ui"),creature->GetGUID());

		UE_LOG(LogTemp, Warning, TEXT("query: %s"), *query);
		/*
		FMySQLConnectoreQueryResult result = UMySQLDatabase::MySQLConnectorGetData(query, dbEditor.GetDBConnection());

		if (result.Success)
		{
			for (const auto& row : result.ResultRows)
			{
				for (const auto& field : row.Fields)
				{
					UE_LOG(LogTemp, Warning, TEXT("Field: %s Value: %s"), *field.Key, *field.Value);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Error: %s"), *result.ErrorMessage);
		}*/
		//set values from selected objects
		info->Position = FVector3f(creature->GetActorLocation());

		CreatureInfos.Emplace(info);
	}

	PropertyWidget->SetObjects(CreatureInfos);


    CustomCategory.AddCustomRow(FText::FromString("Outline Color Changing Category"))
	.ValueContent()
	.VAlign(VAlign_Fill)
	.HAlign(HAlign_Fill)
	[ 
		SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[			
				PropertyWidget
			]
			+SVerticalBox::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Fill)
			[
				SNew(SButton)
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					.OnClicked(this, &FCreatureDetails::ClickedOnButton)
					.Content()
					[
						SNew(STextBlock).Text(FText::FromString("Update Database!"))
					]
			]			
	];
}

FReply FCreatureDetails::ClickedOnButton()
{
	FDBEditorModule& dbEditor = FModuleManager::LoadModuleChecked<FDBEditorModule>("DBEditor");
	
	FString query("Update creature Set map = 1 Where guid = 11;");
	dbEditor.GetDBConnection().MySQLConnectorExecuteQuery(query);

	return FReply::Handled();
}
