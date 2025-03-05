// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCDialogComponent.h"

UNPCDialogComponent::UNPCDialogComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}



void UNPCDialogComponent::BeginPlay()
{
	Super::BeginPlay();
	// likely need to init plugin here
}

/*
 * HandleInput: A function which takes in the player passed input. It is stored, then accessed 
 * by other functions. 
 */
void UNPCDialogComponent::HandleInput(const FString PlayerInput)
{
	// The input is called twice for some reason. To combat this, test below. 
	if (PlayerInput.IsEmpty() || PlayerInput_Untailored != PlayerInput)
	{
		PlayerInput_Untailored = PlayerInput;
	}
}



void UNPCDialogComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// currently unclear if tick will need to be utilized- possibly for streaming to and from ai. 
}

