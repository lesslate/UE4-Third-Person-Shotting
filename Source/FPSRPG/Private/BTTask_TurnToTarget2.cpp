// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_TurnToTarget2.h"
#include "ZombieAIController2.h"
#include "FPSPlayer.h"
#include "Zombie2.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget2::UBTTask_TurnToTarget2()
{
	NodeName = TEXT("Turn2");
}

EBTNodeResult::Type UBTTask_TurnToTarget2::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Zombie = Cast<AZombie2>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Zombie)
		return EBTNodeResult::Failed;

	auto Target = Cast<AFPSPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AZombieAIController2::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - Zombie->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	Zombie->SetActorRotation(FMath::RInterpTo(Zombie->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}







