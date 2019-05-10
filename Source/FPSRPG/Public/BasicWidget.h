// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BasicWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPSRPG_API UBasicWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Bomb")
	void TextChange();
	
	virtual void NativeConstruct();
};
