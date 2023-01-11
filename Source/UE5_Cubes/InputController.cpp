// Fill out your copyright notice in the Description page of Project Settings.


#include "InputController.h"
#include <UE5_Cubes/UE5_CubesGameModeBase.h>
#include "MyCameraController.h"

// Sets default values
AInputController::AInputController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInputController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInputController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AInputController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	//Just ovveride this stuff in the other thing.
	PlayerInputComponent->BindAction("MoveForward", IE_Pressed, this, &AInputController::DoForward);
	PlayerInputComponent->BindAction("MoveBackward", IE_Pressed, this, &AInputController::DoBackward);
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &AInputController::DoRight);
	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &AInputController::DoLeft);
	PlayerInputComponent->BindAction("Action1", IE_Pressed, this, &AInputController::DoAction1);
	PlayerInputComponent->BindAction("Action2", IE_Pressed, this, &AInputController::DoAction2);
	PlayerInputComponent->BindAction("Action3", IE_Pressed, this, &AInputController::DoAction3);
	PlayerInputComponent->BindAction("Action4", IE_Pressed, this, &AInputController::DoAction4);

	

	//Triggers
	PlayerInputComponent->BindAction("LeftShoulder", IE_Pressed, this, &AInputController::DoLeftShoulder);
	PlayerInputComponent->BindAction("RightShoulder", IE_Pressed, this, &AInputController::DoRightShoulder);

	//Thumb Stick
	PlayerInputComponent->BindAction("LeftThumb", IE_Pressed, this, &AInputController::DoLeftThumb);
	PlayerInputComponent->BindAction("RightThumb", IE_Pressed, this, &AInputController::DoRightThumb);


	//Axis could also be set up for the triggers

	//Right Stick Axis
	PlayerInputComponent->BindAxis("RightStick_X", this, &AInputController::DoRightX);
	PlayerInputComponent->BindAxis("RightStick_Y", this, &AInputController::DoRightY);

	//Left Stick Axis
	PlayerInputComponent->BindAxis("LeftStick_X", this, &AInputController::DoLeftX);
	PlayerInputComponent->BindAxis("LeftStick_Y", this, &AInputController::DoLeftY);

	//Shoulders
	PlayerInputComponent->BindAxis("LeftTrigger", this, &AInputController::DoLeftTrigger);
	PlayerInputComponent->BindAxis("RightTrigger", this, &AInputController::DoRightTrigger);
}

void AInputController::OutputText() {
	FString StringExample4 = TEXT("Input Provided!");
	UE_LOG(LogTemp, Warning, TEXT("Output: %s"), *StringExample4);
}

void AInputController::DoForward() {
	AUE5_CubesGameModeBase* GameMode = (AUE5_CubesGameModeBase*)GetWorld()->GetAuthGameMode();
	GameMode->W_Event();
}

void AInputController::DoBackward() {
	AUE5_CubesGameModeBase* GameMode = (AUE5_CubesGameModeBase*)GetWorld()->GetAuthGameMode();
	GameMode->S_Event();
}

void AInputController::DoLeft() {
	AUE5_CubesGameModeBase* GameMode = (AUE5_CubesGameModeBase*)GetWorld()->GetAuthGameMode();
	GameMode->A_Event();
}

void AInputController::DoRight() {
	AUE5_CubesGameModeBase* GameMode = (AUE5_CubesGameModeBase*)GetWorld()->GetAuthGameMode();
	GameMode->D_Event();
}

void AInputController::DoAction1()
{
	AUE5_CubesGameModeBase* GameMode = (AUE5_CubesGameModeBase*)GetWorld()->GetAuthGameMode();
	GameMode->Action1_Event();

}
void AInputController::DoAction2()
{
	AUE5_CubesGameModeBase* GameMode = (AUE5_CubesGameModeBase*)GetWorld()->GetAuthGameMode();
	GameMode->Action2_Event();
}
void AInputController::DoAction3()
{
	AUE5_CubesGameModeBase* GameMode = (AUE5_CubesGameModeBase*)GetWorld()->GetAuthGameMode();
	GameMode->Action3_Event();
}

void AInputController::DoAction4()
{
	AUE5_CubesGameModeBase* GameMode = (AUE5_CubesGameModeBase*)GetWorld()->GetAuthGameMode();
	GameMode->Action4_Event();
}

void AInputController::DoRightShoulder()
{
	AUE5_CubesGameModeBase* GameMode = (AUE5_CubesGameModeBase*)GetWorld()->GetAuthGameMode();
	GameMode->RightShoulder_Event();
}

void AInputController::DoLeftShoulder()
{
	AUE5_CubesGameModeBase* GameMode = (AUE5_CubesGameModeBase*)GetWorld()->GetAuthGameMode();
	GameMode->LeftShoulder_Event();
}

void AInputController::DoRightTrigger(float AxisValue)
{
	if (AxisValue != 0) {
		AUE5_CubesGameModeBase* GameMode = (AUE5_CubesGameModeBase*)GetWorld()->GetAuthGameMode();
		GameMode->RightTrigger_Event(AxisValue);
	}
}

void AInputController::DoLeftTrigger(float AxisValue)
{
	if (AxisValue != 0) {
		AUE5_CubesGameModeBase* GameMode = (AUE5_CubesGameModeBase*)GetWorld()->GetAuthGameMode();
		GameMode->LeftTrigger_Event(AxisValue);
	}
}

void AInputController::DoLeftThumb()
{
	AUE5_CubesGameModeBase* GameMode = (AUE5_CubesGameModeBase*)GetWorld()->GetAuthGameMode();
	GameMode->LeftThumb_Event();
}

void AInputController::DoRightThumb()
{
	AUE5_CubesGameModeBase* GameMode = (AUE5_CubesGameModeBase*)GetWorld()->GetAuthGameMode();
	GameMode->RightThumb_Event();
}

void AInputController::DoRightX(float AxisValue)
{
	if (AxisValue != 0) {
		AUE5_CubesGameModeBase* GameMode = (AUE5_CubesGameModeBase*)GetWorld()->GetAuthGameMode();
		GameMode->RightAxisX(AxisValue);
	}
	
}

void AInputController::DoRightY(float AxisValue)
{
	if (AxisValue != 0) {
		AUE5_CubesGameModeBase* GameMode = (AUE5_CubesGameModeBase*)GetWorld()->GetAuthGameMode();
		GameMode->RightAxisY(AxisValue);
	}
}

void AInputController::DoLeftX(float AxisValue)
{
	if (AxisValue != 0) {
		AUE5_CubesGameModeBase* GameMode = (AUE5_CubesGameModeBase*)GetWorld()->GetAuthGameMode();
		GameMode->LeftAxisX(AxisValue);
	}
}

void AInputController::DoLeftY(float AxisValue)
{
	if (AxisValue != 0) {
		AUE5_CubesGameModeBase* GameMode = (AUE5_CubesGameModeBase*)GetWorld()->GetAuthGameMode();
		GameMode->LeftAxisY(AxisValue);
	}
}
