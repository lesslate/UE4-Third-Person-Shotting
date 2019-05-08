// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSGameMode.h"
#include "FPSPlayer.h"
#include "ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

AFPSGameMode::AFPSGameMode()
{
	static ConstructorHelpers::FObjectFinder<UClass>BPFPSPlayer(TEXT("Blueprint'/Game/Character/BP_FPSPlayer.BP_FPSPlayer_C'"));
	if (BPFPSPlayer.Succeeded())
	{
		DefaultPawnClass = BPFPSPlayer.Object;
	}

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Monster"), Actors);
	
	RemainZombie = Actors.Num();

	UE_LOG(LogTemp, Log, TEXT("Remain Zombie: %d"),RemainZombie);
	
}

void AFPSGameMode::DecreaseMonster()
{
	RemainZombie--;
	if (RemainZombie == 0)
	{
		Victory.Broadcast();
	}
}
