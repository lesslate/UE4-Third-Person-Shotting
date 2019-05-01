// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindPatrolPos.h"
#include "ZombieAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/Navigation/NavigationSystem.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return EBTNodeResult::Failed;

	UNavigationSystem* NavSystem = UNavigationSystem::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem)
	{
		return EBTNodeResult::Failed;
		UE_LOG(LogTemp, Log, TEXT("NAv not found"), );
	}



	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AZombieAIController::HomePosKey);
	FNavLocation NextPatrol;

	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 20000.0f, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AZombieAIController::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

