// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsInAttackRange2.generated.h"

/**
 * 
 */
UCLASS()
class FPSRPG_API UBTDecorator_IsInAttackRange2 : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_IsInAttackRange2();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
	
};
