// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogConfig.h"
#include "Components/ActorComponent.h"
#include "Tasks/HttpGPTChatRequest.h"
#include "Structures/HttpGPTChatTypes.h"
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

	UFUNCTION()
	void SendCurrentConversationToGPT();

	UFUNCTION()
	void OnGPTResponse(const FHttpGPTChatResponse& Response);

	/** Delegate Binding **/
	UFUNCTION()
	void OnGPTProcessCompleted(const FHttpGPTChatResponse& Response);

	UFUNCTION()
	void OnGPTErrorReceived(const FHttpGPTChatResponse& Response);

	UFUNCTION()
	void OnGPTProgressStarted(const FHttpGPTChatResponse& Response);

	UFUNCTION()
	void OnGPTProgressUpdated(const FHttpGPTChatResponse& Response);
	/** End Delegate Binding **/

private:

	UPROPERTY()
	TArray<FHttpGPTChatMessage> ConversationHistory;

	FString PlayerInput_Untailored;

		
};
