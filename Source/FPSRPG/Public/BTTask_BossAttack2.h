// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossAttack2.generated.h"

/**
 * 
 */
UCLASS()
class FPSRPG_API UBTTask_BossAttack2 : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_BossAttack2();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSecondes) override;

private:
	bool IsAttacking = false;


};
