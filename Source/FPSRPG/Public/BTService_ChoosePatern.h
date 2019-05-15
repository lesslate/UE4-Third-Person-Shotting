// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_ChoosePatern.generated.h"

/**
 * 
 */
UCLASS()
class FPSRPG_API UBTService_ChoosePatern : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_ChoosePatern();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


};