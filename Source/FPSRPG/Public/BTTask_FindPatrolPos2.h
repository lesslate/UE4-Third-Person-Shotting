// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPatrolPos2.generated.h"

/**
 * 
 */
UCLASS()
class FPSRPG_API UBTTask_FindPatrolPos2 : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FindPatrolPos2();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
	
};
