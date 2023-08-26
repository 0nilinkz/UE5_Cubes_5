// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "MyCubeActor.h"
#include "InputController.h"
#include "MyCameraController.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <UE5_Cubes/CubeFont.h>
#include "GameFramework/Actor.h"
#include "GameFramework/GameModeBase.h"
#include "UE5_CubesGameModeBase.generated.h"


UCLASS()
class UE5_CUBES_API AUE5_CubesGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	AUE5_CubesGameModeBase();

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	UPROPERTY()
		AMyCameraController* CameraThing;
	UPROPERTY()
		TArray<AMyCubeActor*> Cubes;
	UPROPERTY()
		FTimerHandle MemberTimerHandle;
	UPROPERTY()
		float ZoomRadius = 3000.0f;
	UPROPERTY()
		AMyCameraTransition* newT;
	UPROPERTY()
		UInputComponent* InputThing;

	//Functions
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VisualEffectVariables")
		AMyCubeActor* CubeParent;
	
public:
	
	UFUNCTION()
		void W_Event();
	UFUNCTION()
		void A_Event();
	UFUNCTION()
		void S_Event();
	UFUNCTION()
		void D_Event();

	UFUNCTION()
		void Action1_Event();
	UFUNCTION()
		void Action2_Event();
	UFUNCTION()
		void Action3_Event();
	UFUNCTION()
		void Action4_Event();

	UFUNCTION()
		void LeftShoulder_Event();
	UFUNCTION()
		void RightShoulder_Event();
	UFUNCTION()
		void RightTrigger_Event(float AxisValue);
	UFUNCTION()
		void LeftTrigger_Event(float AxisValue);
	UFUNCTION()
		void RightThumb_Event();
	UFUNCTION()
		void LeftThumb_Event();
	UFUNCTION()
		void LeftAxisX(float AxisValue);
	UFUNCTION()
		void LeftAxisY(float AxisValue);
	UFUNCTION()
		void RightAxisX(float AxisValue);
	UFUNCTION()
		void RightAxisY(float AxisValue);
};
