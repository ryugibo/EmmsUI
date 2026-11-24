#pragma once
#include "CoreMinimal.h"
#include "MMWidget.h"
#include "Widgets/Docking/SDockTab.h"
#include "Framework/Docking/TabManager.h"
#include "Tickable.h"
#include "MMEditorUtilityTab.generated.h"

UCLASS(BlueprintType)
class EMMSUIEDITOR_API UMMEditorUtilityTab : public UObject
{
	GENERATED_BODY()

public:

	// Keeps a strong reference to itself, because generally
	// windows are spawned without a reference being kept to them.
	// This reference will be cleared when the window is closed
	TStrongObjectPtr<UMMEditorUtilityTab> StrongSelf;
	TSharedPtr<SDockTab> SlateTab;
	bool bOpen = false;

	UPROPERTY(EditDefaultsOnly, Category = "Tab")
	FString TabTitle;
	UPROPERTY(EditDefaultsOnly, Category = "Tab")
	FString ToolTip;
	UPROPERTY(EditDefaultsOnly, Category = "Tab")
	FString Category = TEXT("Scripted Tabs");
	UPROPERTY(EditDefaultsOnly, Category = "Tab")
	FName IconStyleSet;
	UPROPERTY(EditDefaultsOnly, Category = "Tab")
	FName Icon;
	UPROPERTY(EditDefaultsOnly, Category = "Tab")
	bool bShowInToolsMenu = true;

	UPROPERTY()
	UMMWidget* MMWidget;

	UPROPERTY()
	UWorld* World = nullptr;
	UWorld* GetWorld() const override;

	void Spawn();

	UFUNCTION(ScriptCallable)
	bool IsTabFocused() const;

	UFUNCTION(ScriptCallable)
	bool IsTabVisible() const;

	UFUNCTION(ScriptCallable)
	void CloseTab();

	UFUNCTION(BlueprintImplementableEvent)
	void OnTabOpened();

	UFUNCTION(BlueprintImplementableEvent)
	void OnTabClosed();

	UFUNCTION(BlueprintImplementableEvent)
	void DrawTab(float DeltaTime);

	UFUNCTION(ScriptCallable, Meta = (DeterminesOutputType = "TabType"))
	static UMMEditorUtilityTab* SpawnOrFocusTab(TSubclassOf<UMMEditorUtilityTab> TabType);

	virtual void BeginDestroy() override;

	void HandleObjectReinstancing(const TMap<UObject*, UObject*>& ReinstancedObjects);
	FDelegateHandle ReinstanceHandle;
};