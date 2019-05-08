// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"


DECLARE_MULTICAST_DELEGATE(FStartBomb);
DECLARE_MULTICAST_DELEGATE(FVictory);


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
	FVictory Victory;
	UFUNCTION()
	void DecreaseMonster();
	

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 RemainZombie;


};
