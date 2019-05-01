// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSGameMode.h"
#include "FPSPlayer.h"
#include "ConstructorHelpers.h"

AFPSGameMode::AFPSGameMode()
{
	static ConstructorHelpers::FObjectFinder<UClass>BPFPSPlayer(TEXT("Blueprint'/Game/Character/BP_FPSPlayer.BP_FPSPlayer_C'"));
	if (BPFPSPlayer.Succeeded())
	{
		DefaultPawnClass = BPFPSPlayer.Object;
	}



}



