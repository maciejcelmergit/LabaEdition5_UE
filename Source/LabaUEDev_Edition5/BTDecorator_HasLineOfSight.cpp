// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BTDecorator_HasLineOfSight.h"

UBTDecorator_HasLineOfSight::UBTDecorator_HasLineOfSight()
{
	NodeName = TEXT("HasLineOfSight Condition");
}

bool UBTDecorator_HasLineOfSight::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController* Controller = OwnerComp.GetAIOwner();
	const APawn* Pawn = Controller->GetPawn();
	const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	UWorld* World = OwnerComp.GetWorld();

	if (!Pawn || !Blackboard || !World)
	{
		return false;
	}

	const AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(GetSelectedBlackboardKey()));

	if (!Target)
	{
		return false;
	}

	const FVector Start = Pawn->GetActorLocation() + TraceOffset;
	const FVector End = Target->GetActorLocation() + TraceOffset;

	FCollisionQueryParams Params;
	Params.TraceTag = FName(TEXT("LOSCheck"));
	Params.AddIgnoredActor(Pawn);
	Params.AddIgnoredActor(Target);

	return !World->LineTraceTestByChannel(Start, End, ECC_Visibility, Params);
}