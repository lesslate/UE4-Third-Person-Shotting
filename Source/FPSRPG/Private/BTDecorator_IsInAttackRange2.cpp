// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_IsInAttackRange2.h"
#include "ZombieAIController2.h"
#include "FPSPlayer.h"
#include "Zombie2.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange2::UBTDecorator_IsInAttackRange2()
{
	NodeName = TEXT("CanAttack2");
}

bool UBTDecorator_IsInAttackRange2::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<AZombie2>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
		return false;

	auto Target = Cast<AFPSPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AZombieAIController2::TargetKey));
	if (nullptr == Target)
		return false;

	bResult = (Target->GetDistanceTo(ControllingPawn) <= 120.0f);
	// UE_LOG(LogTemp, Log, TEXT("can Attack: %s"),bResult ? TEXT("true") : TEXT("false"));
	return bResult;
}



