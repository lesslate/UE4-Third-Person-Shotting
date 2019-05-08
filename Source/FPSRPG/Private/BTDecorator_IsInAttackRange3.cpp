// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_IsInAttackRange3.h"
#include "BossZombieAIController.h"
#include "FPSPlayer.h"
#include "BossZombie.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange3::UBTDecorator_IsInAttackRange3()
{
	NodeName = TEXT("CanAttack3");
}

bool UBTDecorator_IsInAttackRange3::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABossZombie>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
		return false;

	auto Target = Cast<AFPSPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ABossZombieAIController::TargetKey));
	if (nullptr == Target)
		return false;

	bResult = (Target->GetDistanceTo(ControllingPawn) <= 120.0f);
	// UE_LOG(LogTemp, Log, TEXT("can Attack: %s"),bResult ? TEXT("true") : TEXT("false"));
	return bResult;
}







