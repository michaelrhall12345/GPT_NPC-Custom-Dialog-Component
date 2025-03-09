// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCDialogComponent.h"
#include "Management/HttpGPTSettings.h"

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

    const UHttpGPTSettings* GPTSettings = UHttpGPTSettings::Get();
    if (!GPTSettings)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to get UHttpGPTSettings. Using fallback defaults."));
        return;
    }

    const FHttpGPTCommonOptions& CommonOptions = GPTSettings->CommonOptions;
    const FHttpGPTChatOptions& ChatOptions = GPTSettings->ChatOptions;

    UHttpGPTChatRequest* ChatRequest = UHttpGPTChatRequest::SendMessages_CustomOptions(GetWorld(), ConversationHistory, TArray<FHttpGPTFunction>(), CommonOptions, ChatOptions);

    if (ChatRequest) // Binding delegates so we can handle success or error
    {
        ChatRequest->ProcessCompleted.AddDynamic(this, &UNPCDialogComponent::OnGPTProcessCompleted);
        ChatRequest->ErrorReceived.AddDynamic(this, &UNPCDialogComponent::OnGPTErrorReceived);


        ChatRequest->ProgressStarted.AddDynamic(this, &UNPCDialogComponent::OnGPTProgressStarted);
        ChatRequest->ProgressUpdated.AddDynamic(this, &UNPCDialogComponent::OnGPTProgressUpdated);

        ChatRequest->Activate(); // GO GO Chat GPT!
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SendCurrentConversationToGPT: Failed to create HttpGPTChatRequest."));
        return;
    }
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

void UNPCDialogComponent::OnGPTErrorReceived(const FHttpGPTChatResponse& Response)
{
    UE_LOG(LogTemp, Error, TEXT("GPT Error Delegate Called."));
}

void UNPCDialogComponent::OnGPTProcessCompleted(const FHttpGPTChatResponse& Response)
{
    UE_LOG(LogTemp, Log, TEXT("OnGPTProcessCompleted: Finished successfully."));

    if (Response.Choices.Num() > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Assistant says: %s"), *Response.Choices[0].Message.Content);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No choices found in GPT response."));
    }
}

void UNPCDialogComponent::OnGPTProgressStarted(const FHttpGPTChatResponse& Response)
{
    UE_LOG(LogTemp, Log, TEXT("OnGPTProgressStarted: Streaming has begun!"));
}

void UNPCDialogComponent::OnGPTProgressUpdated(const FHttpGPTChatResponse& Response)
{
    UE_LOG(LogTemp, Log, TEXT("OnGPTProgressUpdated: Got a streamed partial update."));
    if (Response.Choices.Num() > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Partial text so far: %s"), *Response.Choices[0].Message.Content);
    }
}

void UNPCDialogComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// currently unclear if tick will need to be utilized- possibly for streaming to and from ai. 
}

