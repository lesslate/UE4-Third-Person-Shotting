// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class FPSRPG_API AZombieAIController : public AAIController
{
	GENERATED_BODY()
	

	
public:
	AZombieAIController();
	virtual void Possess(APawn* InPawn) override;

	void StopAI();

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
