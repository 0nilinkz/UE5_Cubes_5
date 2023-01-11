// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputController.generated.h"

UCLASS()
class UE5_CUBES_API AInputController : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AInputController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()
		void OutputText();
	UFUNCTION()
		void DoForward();
	UFUNCTION()
		void DoBackward();
	UFUNCTION()
		void DoLeft();
	UFUNCTION()
		void DoRight();
	UFUNCTION()
		void DoAction1();
	UFUNCTION()
		void DoAction2();
	UFUNCTION()
		void DoAction3();
	UFUNCTION()
		void DoAction4();
	UFUNCTION()
		void DoRightTrigger(float AxisValue);
	UFUNCTION()
		void DoLeftTrigger(float AxisValue);
	UFUNCTION()
		void DoLeftThumb();
	UFUNCTION()
		void DoRightThumb();
	UFUNCTION()
		void DoRightX(float AxisValue);
	UFUNCTION()
		void DoRightY(float AxisValue);
	UFUNCTION()
		void DoLeftX(float AxisValue);
	UFUNCTION()
		void DoLeftY(float AxisValue);
	UFUNCTION()
		void DoRightShoulder();
	UFUNCTION()
		void DoLeftShoulder();
};
