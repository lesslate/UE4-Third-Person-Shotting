// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "ConstructorHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AZombieAIController::HomePosKey(TEXT("HomePos"));
const FName AZombieAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AZombieAIController::TargetKey(TEXT("Target"));

AZombieAIController::AZombieAIController()
{
	// BlackBoard 초기화
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/AI/BB_Zombie.BB_Zombie'")); 
	if (BBObject.Succeeded())
	{
		BBZombie = BBObject.Object;
	}
	// BehaviorTree 초기화
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/AI/BT_Zombie.BT_Zombie'"));
	if (BTObject.Succeeded())
	{
		BTZombie = BTObject.Object;
	}
}

void AZombieAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);
	if (UseBlackboard(BBZombie, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTZombie))
		{
			UE_LOG(LogTemp, Log, TEXT("BT NOT FOUND"));
		}
	}
}
