#pragma once
#include "IDetailCustomization.h"


class FCreatureDetails : public IDetailCustomization
{
    
public:
   
    ~FCreatureDetails();
    static TSharedRef<IDetailCustomization> MakeInstance();

    /** IDetailCustomization interface */
    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

    FReply ClickedOnButton();
private:
    TArray<TWeakObjectPtr<UObject>> CreatureInfos;
};