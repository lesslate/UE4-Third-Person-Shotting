// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_TurnToTarget3.h"
#include "BossZombieAIController.h"
#include "FPSPlayer.h"
#include "BossZombie.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget3::UBTTask_TurnToTarget3()
{
	NodeName = TEXT("Turn3");
}

EBTNodeResult::Type UBTTask_TurnToTarget3::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Zombie = Cast<ABossZombie>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Zombie)
		return EBTNodeResult::Failed;

	auto Target = Cast<AFPSPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ABossZombieAIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - Zombie->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	Zombie->SetActorRotation(FMath::RInterpTo(Zombie->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}





