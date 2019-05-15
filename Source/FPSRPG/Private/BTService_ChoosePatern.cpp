// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_ChoosePatern.h"
#include "Math/UnrealMathUtility.h"
#include "BossZombieAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BossZombie.h"

UBTService_ChoosePatern::UBTService_ChoosePatern()
{
	NodeName = TEXT("Choose");
	Interval = 1.0f;
}

void UBTService_ChoosePatern::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	OwnerComp.GetBlackboardComponent()->SetValueAsInt(ABossZombieAIController::TaskNumberKey, 3);

}
