// Fill out your copyright notice in the Description page of Project Settings.

#include "Bomb.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "FPSGameMode.h"
#include "Runtime/Engine/Classes/Engine/World.h"

// Sets default values
ABomb::ABomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABomb::BeginPlay()
{
	Super::BeginPlay();

	AFPSGameMode* gameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gameMode != nullptr)
	{
		gameMode->StartBombing.AddUObject(this, &ABomb::Bomb);
	}
}

// Called every frame
void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
