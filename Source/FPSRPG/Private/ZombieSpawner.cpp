// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieSpawner.h"
#include "ConstructorHelpers.h"
#include "Components/WidgetComponent.h"
#include "FPSGameMode.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/TimerManager.h"

// Sets default values
AZombieSpawner::AZombieSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<AActor> BossZombie(TEXT("Blueprint'/Game/Character/BP_BossZombie.BP_BossZombie_C'"));
	if (BossZombie.Class)
	{
		BossActor = (UClass*)BossZombie.Class;
	}
}

// Called when the game starts or when spawned
void AZombieSpawner::BeginPlay()
{
	Super::BeginPlay();

}

void AZombieSpawner::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AFPSGameMode* gameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gameMode != nullptr)
	{
		gameMode->Spawnning.AddUObject(this, &AZombieSpawner::SpawnBossZombieDelay);
	}
}

// Called every frame
void AZombieSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AZombieSpawner::SpawnBossZombieDelay()
{
	UE_LOG(LogTemp, Log, TEXT("Waiting Boss"));
	GetWorld()->GetTimerManager().SetTimer(timer, this, &AZombieSpawner::SpawnBossZombie, 5.0f, false);
}

void AZombieSpawner::SpawnBossZombie()
{
	UWorld* world = GetWorld();
	if (world)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		FRotator rotator(0, 0, 0);
		FVector  SpawnLocation = GetActorLocation();

		world->SpawnActor<AActor>(BossActor, SpawnLocation, rotator, SpawnParams);
	}
}

