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



void UNPCDialogComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// currently unclear if tick will need to be utilized- possibly for streaming to and from ai. 
}

