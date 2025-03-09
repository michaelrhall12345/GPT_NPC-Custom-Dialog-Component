// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCDialogComponent.h"

UNPCDialogComponent::UNPCDialogComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}



void UNPCDialogComponent::BeginPlay()
{
	Super::BeginPlay();
	
    // Create a system message from player config
    FHttpGPTChatMessage SystemMsg;
    SystemMsg.Role = EHttpGPTChatRole::System;

    if (NPCPersonality)
    {
        SystemMsg.Content = NPCPersonality->ToSystemPrompt();
    }

    // Add a "starter" user message, tasking GPT to greet the player
    FHttpGPTChatMessage UserStarterMsg;
    UserStarterMsg.Role = EHttpGPTChatRole::User;
    UserStarterMsg.Content = TEXT("Please introduce yourself to the player.");

    // Add them to the conversation
    ConversationHistory.Empty();
    ConversationHistory.Add(SystemMsg);
    ConversationHistory.Add(UserStarterMsg);

    // Send it right away to get an initial "greeting" from GPT
    SendCurrentConversationToGPT();
}

/*
 * HandleInput: A function which takes in the player passed input. It is stored, then accessed 
 * by other functions. 
 */
void UNPCDialogComponent::HandleInput(const FString PlayerInput)
{
    // Check for empty or duplicate input
    if (PlayerInput.IsEmpty() || PlayerInput == PlayerInput_Untailored)
    {
        return;
    }

    // Store the new input
    PlayerInput_Untailored = PlayerInput;

    // Create a user message for the conversation
    FHttpGPTChatMessage UserMsg;
    UserMsg.Role = EHttpGPTChatRole::User;   
    UserMsg.Content = PlayerInput;

    ConversationHistory.Add(UserMsg);
    SendCurrentConversationToGPT();
}

void UNPCDialogComponent::SendCurrentConversationToGPT()
{
    // Safety check: no messages to send?
    if (ConversationHistory.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("SendCurrentConversationToGPT: ConversationHistory is empty."));
        return;
    }

    // Call the HttpGPT plugin to send the entire conversation
    UHttpGPTChatRequest* ChatRequest = UHttpGPTChatRequest::SendMessages_DefaultOptions(
        GetWorld(),              
        ConversationHistory,     
        TArray<FHttpGPTFunction>()  // intentionally empty
    );

    if (!ChatRequest)
    {
        UE_LOG(LogTemp, Error, TEXT("SendCurrentConversationToGPT: Failed to create HttpGPTChatRequest."));
        return;
    }

    // Binding delegates so we can handle success or error
    ChatRequest->ProcessCompleted.AddDynamic(this, &UNPCDialogComponent::OnGPTResponse);

    // @todo bind to ProgressStarted, ProgressUpdated, etc. if you need streaming updates
    // ChatRequest->ProgressStarted.AddDynamic(...);
    // ChatRequest->ProgressUpdated.AddDynamic(...);
}

void UNPCDialogComponent::OnGPTResponse(const FHttpGPTChatResponse& Response)
{
    if (Response.Choices.Num() > 0)
    {
        const FString AIReply = Response.Choices[0].Message.Content;
        UE_LOG(LogTemp, Log, TEXT("GPT replied: %s"), *AIReply);

        FHttpGPTChatMessage AssistantMsg;
        AssistantMsg.Role = EHttpGPTChatRole::Assistant;
        AssistantMsg.Content = AIReply;
        ConversationHistory.Add(AssistantMsg);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnGPTResponse: No choices received from GPT. :("));
    }
}



void UNPCDialogComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// currently unclear if tick will need to be utilized- possibly for streaming to and from ai. 
}

