// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "MyCameraTransition.generated.h"

/**
 * 
 */
UCLASS()
class UE5_CUBES_API AMyCameraTransition : public ACameraActor
{
	GENERATED_BODY()

public:
	AMyCameraTransition();

	AMyCameraTransition(float x, float y, float z);

	float initX = 0.0f;
	float initY = 0.0f;
	float initZ = 0.0f;
	float initYaw = 0.0f;
	float initRoll = 0.0f;
	float initPitch = 0.0f;

	float CamX = 0.0f;
	float CamY = 0.0f;
	float CamZ = 0.0f;

	float CamYaw = 0.0f;
	float CamRoll = 0.0f;
	float CamPitch = 0.0f;

	float DisplayTime = 1.0f;
	float TransitionTime = 0.2f;

	float CircleCenterX = 0.0f;
	float CircleCenterY = 0.0f;
	float CircleRadians = 0.0f;
	float CircleRadius = 0.0f;

	float UpdateTimer = 1.0f; //Frequency of updating (time in seconds)

	bool PauseTransitionUpdate = false;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ResetCamera();

	UFUNCTION()
		void SetPosition(float x, float y, float z);
	UFUNCTION()
		void SetRotation(float yaw, float roll, float pitch);
	UFUNCTION()
		void SetDataAll(float x, float y, float z, float yaw, float roll, float pitch, float displayTime, float transitionTime);
	UFUNCTION()
		void MoveInCircle(float radius, float centerX, float centerY, float updateTimer);
	UFUNCTION()
		void ZoomOut(float amount);
	UFUNCTION()
		void ZoomIn(float amount);
	UFUNCTION()
		void UpdateRadius(float radius);
	UFUNCTION()
		void UpdateRoll(float radius);
	UFUNCTION()
		void UpdatePitch(float radius);

};