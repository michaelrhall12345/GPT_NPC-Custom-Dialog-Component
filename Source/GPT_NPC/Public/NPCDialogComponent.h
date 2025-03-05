// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogConfig.h"
#include "Components/ActorComponent.h"
#include "NPCDialogComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GPT_NPC_API UNPCDialogComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UNPCDialogComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Personality Configuration")
	UDialogConfig* NPCPersonality;
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void HandleInput(const FString PlayerInput);

private:

	FString PlayerInput_Untailored;

		
};
