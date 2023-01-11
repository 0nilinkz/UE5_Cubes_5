// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyCameraController.generated.h"

UCLASS()
class UE5_CUBES_API AMyCameraController : public AActor
{
	GENERATED_BODY()
	
public:
	//Constructor
	AMyCameraController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void AddNewCamera(AMyCameraTransition* newCamTrans);
	UFUNCTION()
		void PauseCamera();
	UFUNCTION()
		void ResumeCamera();
	UFUNCTION()
		void TogglePauseCamera();

	// declare variables
	UPROPERTY(EditAnywhere)
		AMyCameraTransition* CameraOne;
	UPROPERTY(EditAnywhere)
		AMyCameraTransition* CameraTwo;
	UPROPERTY()
		TArray<AMyCameraTransition*> CameraTransitionArray;
	UPROPERTY()
		float TimeToNextCameraChange;
	UPROPERTY()
		int SelectedCamera = 0;
	UPROPERTY()
		bool Paused = false;
};
