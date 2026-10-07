// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabberComponent.h"
#include "EnhancedInputComponent.h"


// Sets default values for this component's properties
UGrabberComponent::UGrabberComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabberComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber Begin Play"));

	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (GrabAction && PC)
	{
		if (UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(PC->InputComponent))
		{
			InputComponent->BindAction(GrabAction, ETriggerEvent::Started, this, &UGrabberComponent::Grab);
		}
	}

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Physics Handle Component"));
	}
}


// Called every frame
void UGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		FVector Start;
		FVector End;
		FRotator CameraRotation;

		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(Start, CameraRotation);
		End = Start + CameraRotation.Vector() * GrabRange;
		PhysicsHandle->SetTargetLocationAndRotation(End, CameraRotation);
	}
}

void UGrabberComponent::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab"));

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		PhysicsHandle->ReleaseComponent();
		return;
	}

	// znalezc obiekt
	FVector Start;
	FVector End;
	FRotator CameraRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(Start, CameraRotation);
	End = Start + CameraRotation.Vector() * GrabRange;

	FHitResult Result;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	GetWorld()->LineTraceSingleByChannel(Result, Start, End, ECollisionChannel::ECC_PhysicsBody, Params);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Cyan, false, 5.0f, 0, 5.0f);

	if (UPrimitiveComponent* Comp = Result.GetComponent()) 
	{
		// chwycic obiekt
		UE_LOG(LogTemp, Warning, TEXT("Grabbing object: %s"), *Comp->GetName());
		PhysicsHandle->GrabComponentAtLocationWithRotation(Comp, FName(), Result.Location, CameraRotation);
	}
}

