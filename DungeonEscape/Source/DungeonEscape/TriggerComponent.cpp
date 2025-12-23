// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (MoverActor != nullptr)
	{
		Mover = MoverActor->FindComponentByClass<UMover>();
	}

	if (IsPressurePlate)
	{
		OnComponentBeginOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapBegin);
		OnComponentEndOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapEnd);
	}
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UTriggerComponent::Trigger(bool NewTriggerValue)
{
	IsTriggered = NewTriggerValue;

	if (Mover)
	{
		Mover->SetShouldMove(IsTriggered);
	}
}

void UTriggerComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*UE_LOG(LogTemp, Warning, TEXT("Overlap Begin called!"));*/

	if (!OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("OtherActor is null"));
		return;
	}

	bool HasTag = OtherActor->ActorHasTag("PressurePlateActivator");
	/*UE_LOG(LogTemp, Warning, TEXT("HasTag: %s"), HasTag ? TEXT("true") : TEXT("false"));*/

	if (!HasTag) return;

	ActivatorCount++;
	/*UE_LOG(LogTemp, Warning, TEXT("ActivatorCount: %d"), ActivatorCount);*/

	if (!IsTriggered)
	{
		Trigger(true);
	}
}

void UTriggerComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor) return;

	bool HasTag = OtherActor->ActorHasTag("PressurePlateActivator");

	if (!HasTag) return;

	ActivatorCount--;

	if (!IsTriggered) return;

	if (ActivatorCount > 0) return;

	Trigger(false);

}
