// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieAIController2.generated.h"

DECLARE_MULTICAST_DELEGATE(FSprintDelegate);
/**
 * 
 */
UCLASS()
class FPSRPG_API AZombieAIController2 : public AAIController
{
	GENERATED_BODY()
	
public:
	AZombieAIController2();
	virtual void Possess(APawn* InPawn) override;


	void StopAI();
	FSprintDelegate OnSprint;
	FSprintDelegate OnStopSprint;
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

	UPROPERTY()
	float Radius;
private:
	UPROPERTY()
	class UBehaviorTree* BTZombie;

	UPROPERTY()
	class UBlackboardData* BBZombie;
};

	
	