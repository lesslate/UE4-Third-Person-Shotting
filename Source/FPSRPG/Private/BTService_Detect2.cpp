// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_Detect2.h"
#include "ZombieAIController2.h"
#include "FPSPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Zombie2.h"



UBTService_Detect2::UBTService_Detect2()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;

}

void UBTService_Detect2::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();

	auto ZombieAI = Cast<AZombieAIController2>(OwnerComp.GetAIOwner());
	DetectRadius = ZombieAI->Radius;

	auto Zombie = Cast<AZombie2>(OwnerComp.GetAIOwner()->GetPawn());


	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			AFPSPlayer* FPSPlayer = Cast<AFPSPlayer>(OverlapResult.GetActor());
			if (FPSPlayer && FPSPlayer->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AZombieAIController2::TargetKey, FPSPlayer); // Å¸°Ù ÀúÀå

				//ZombieAI->OnSprint.Broadcast();
				Zombie->Run();

				/*	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
					DrawDebugPoint(World, FPSPlayer->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
					DrawDebugLine(World, ControllingPawn->GetActorLocation(), FPSPlayer->GetActorLocation(), FColor::Blue, false, 0.27f);*/
				return;
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AZombieAIController2::TargetKey, nullptr);

		//ZombieAI->OnStopSprint.Broadcast();
		Zombie->Walk();
	}

	//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);

}



