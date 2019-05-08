// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Detect3.generated.h"

/**
 * 
 */
UCLASS()
class FPSRPG_API UBTService_Detect3 : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_Detect3();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float DetectRadius;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	
	
	
};
