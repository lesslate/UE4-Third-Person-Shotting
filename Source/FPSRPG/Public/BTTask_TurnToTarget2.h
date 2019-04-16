// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TurnToTarget2.generated.h"

/**
 * 
 */
UCLASS()
class FPSRPG_API UBTTask_TurnToTarget2 : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_TurnToTarget2();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
	

