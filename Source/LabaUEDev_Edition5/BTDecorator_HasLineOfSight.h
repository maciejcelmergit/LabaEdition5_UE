// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_HasLineOfSight.generated.h"

/**
 * 
 */
UCLASS()
class LABAUEDEV_EDITION5_API UBTDecorator_HasLineOfSight : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTDecorator_HasLineOfSight();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

	UPROPERTY(EditAnywhere)
	FVector TraceOffset = FVector();
};
