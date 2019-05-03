// Fill out your copyright notice in the Description page of Project Settings.

#include "Jet.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "FPSGameMode.h"

// Sets default values
AJet::AJet()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJet::BeginPlay()
{
	Super::BeginPlay();

	AFPSGameMode* gameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gameMode != nullptr)
	{
		gameMode->StartBombing.AddUObject(this, &AJet::StartFly);
	}

}

// Called every frame
void AJet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AJet::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


