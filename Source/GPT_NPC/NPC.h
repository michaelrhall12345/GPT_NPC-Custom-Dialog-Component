// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NPC.generated.h"

UCLASS()
class GPT_NPC_API ANPC : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ANPC();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void Interact();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Interaction", meta= (AllowPrivateAccess = "true"))
	UBoxComponent* InteractionVolume;

	bool bIsOverlapping;

};
