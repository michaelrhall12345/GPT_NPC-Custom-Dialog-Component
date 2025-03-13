// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogConfig.h"
#include "Components/ActorComponent.h"
#include "Tasks/HttpGPTChatRequest.h"
#include "Structures/HttpGPTChatTypes.h"
#include "NPCDialogComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPartialTextUpdatedSignature, const FString&, UpdatedText);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GPT_NPC_API UNPCDialogComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UNPCDialogComponent();
	
	UPROPERTY(BlueprintAssignable, Category = "NPC Dialog|Streaming")
	FOnPartialTextUpdatedSignature OnPartialTextUpdated;

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
	void OnGPTErrorReceived(const FHttpGPTChatResponse& Response);

	UFUNCTION()
	void OnGPTProgressStarted(const FHttpGPTChatResponse& Response);

	UFUNCTION()
	void OnGPTProgressUpdated(const FHttpGPTChatResponse& Response);

	/*
	UFUNCTION()
	void OnGPTProcessCompleted(const FHttpGPTChatResponse& Response); - @brief - enable if you want total dialog output 
	*/

	/** End Delegate Binding **/

	// A small helper to broadcast our event
	void BroadcastPartialText(const FString& NewText);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC Dialog|Streaming")
	FString CurrentStreamedReply;
private:

	UPROPERTY()
	TArray<FHttpGPTChatMessage> ConversationHistory;

	FString PlayerInput_Untailored;

		
};
