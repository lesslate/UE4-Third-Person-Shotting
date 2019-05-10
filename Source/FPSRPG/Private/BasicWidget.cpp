// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "FPSGameMode.h"


void UBasicWidget::NativeConstruct()
{
	AFPSGameMode* gameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gameMode != nullptr)
	{
		gameMode->Spawnning.AddUObject(this, &UBasicWidget::TextChange);
	}
}