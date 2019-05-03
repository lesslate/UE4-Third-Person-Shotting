// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"

DECLARE_MULTICAST_DELEGATE(FStartBomb);


/**
 * 
 */
UCLASS()
class FPSRPG_API AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AFPSGameMode();
	
	FStartBomb StartBombing;



	
};
