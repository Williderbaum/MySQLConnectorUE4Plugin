#pragma once
#include "IDetailCustomization.h"


class FGameobjectDetails : public IDetailCustomization
{
    
public:
   
    ~FGameobjectDetails();
    static TSharedRef<IDetailCustomization> MakeInstance();

    /** IDetailCustomization interface */
    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

    FReply ClickedOnButton();
private:
    TArray<TWeakObjectPtr<UObject>> GameobjectInfos;
};