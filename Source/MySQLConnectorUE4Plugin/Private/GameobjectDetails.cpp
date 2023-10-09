#include "GameobjectDetails.h"
#include "IDetailsView.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "Entities/Gameobject.h"
#include "ObjectProperties.h"
#include "MySQLConnectorUE4Plugin.h"



FGameobjectDetails::~FGameobjectDetails()
{
	for(const auto& info : GameobjectInfos)
		info->RemoveFromRoot();
}

TSharedRef<IDetailCustomization> FGameobjectDetails::MakeInstance()
{
	return TSharedRef<IDetailCustomization>(new FGameobjectDetails);
}

void FGameobjectDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{

	TArray<TWeakObjectPtr<UObject>> selectedObjects;

	DetailBuilder.GetObjectsBeingCustomized(selectedObjects);

	//Load our Property Module
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FMySQLConnectorUE4Plugin& dbEditor = FModuleManager::LoadModuleChecked<FMySQLConnectorUE4Plugin>("FMySQLConnectorUE4Plugin");
	//FDetailsViewArgs is a struct of settings to customize our Details View Widget
	FDetailsViewArgs Args;
	Args.bHideSelectionTip = true;
	Args.bAllowSearch = false;

	//Create the widget and store it in the PropertyWidget pointer
	TSharedRef<IDetailsView> PropertyWidget = PropertyModule.CreateDetailView(Args);

	IDetailCategoryBuilder& CustomCategory = DetailBuilder.EditCategory("DB Update", FText::GetEmpty(), ECategoryPriority::Important);


	for (const auto& object : selectedObjects)
	{
		AGameObject* go = Cast<AGameObject>(object);
		UGameobjectInfo* info = NewObject<UGameobjectInfo>();
		info->AddToRoot();
		FString query = FString::Printf(TEXT("SELECT * FROM creature WHERE guid %ui"),go->GetGUID());

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
		info->Position = FVector3f(go->GetActorLocation());

		GameobjectInfos.Emplace(info);
	}

	PropertyWidget->SetObjects(GameobjectInfos);


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
					.OnClicked(this, &FGameobjectDetails::ClickedOnButton)
					.Content()
					[
						SNew(STextBlock).Text(FText::FromString("Update Database!"))
					]
			]			
	];
}

FReply FGameobjectDetails::ClickedOnButton()
{
	FMySQLConnectorUE4Plugin& dbEditor = FModuleManager::LoadModuleChecked<FMySQLConnectorUE4Plugin>("FMySQLConnectorUE4Plugin");
	
	FString query("Update gameobject Set map = 1 Where guid = 11;");
	dbEditor.GetDBConnection().MySQLConnectorExecuteQuery(query);

	return FReply::Handled();
}
