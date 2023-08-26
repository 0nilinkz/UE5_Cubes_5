// Copyright Martin Gleeson
// Last Modified 3/09/22


#include "UE5_CubesGameModeBase.h"

#include "Runtime/Engine/Public/EngineGlobals.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "UObject/Object.h"
#include "MyCameraTransition.h"
#include "MyCubeActor.h"
#include "InputController.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "MyCameraController.h"

//New FX
#include <Runtime/Engine/Classes/GameFramework/InputSettings.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <Runtime/Core/Public/Misc/LocalTimestampDirectoryVisitor.h>

AUE5_CubesGameModeBase::AUE5_CubesGameModeBase()
{
	//Enabling tick function
	PrimaryActorTick.bStartWithTickEnabled = true;

	//User input class
	DefaultPawnClass = AInputController::StaticClass();

	CameraThing = CreateDefaultSubobject<AMyCameraController>(TEXT("AMyCameraObject"));

};

void AUE5_CubesGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

//This runs the game
void AUE5_CubesGameModeBase::BeginPlay()
{

	FString StringExample2 = TEXT("Play started!");
	UE_LOG(LogTemp, Warning, TEXT("Output: %s"), *StringExample2);


}


//This is an example of running effect with a button press.
void AUE5_CubesGameModeBase::W_Event()
{
	UE_LOG(LogTemp, Warning, TEXT("W_Event"));
}

void AUE5_CubesGameModeBase::A_Event()
{
	UE_LOG(LogTemp, Warning, TEXT("A_Event"));
}

void AUE5_CubesGameModeBase::S_Event()
{
	//ApplyFontA(FVector(168, 50, 50),FVector(58, 50, 168),FVector(58, 50, 168), "A");
	UE_LOG(LogTemp, Warning, TEXT("S_Event"));
}

void AUE5_CubesGameModeBase::D_Event()
{
	UE_LOG(LogTemp, Warning, TEXT("D_Event"));
}

void AUE5_CubesGameModeBase::Action1_Event() {
	UE_LOG(LogTemp, Warning, TEXT("Action1_Event"));
	//JaneEffect(1, 4);
}

void AUE5_CubesGameModeBase::Action2_Event() {
	UE_LOG(LogTemp, Warning, TEXT("Action2_Event"));
}

void AUE5_CubesGameModeBase::Action3_Event() {
	UE_LOG(LogTemp, Warning, TEXT("Action3_Event"));
}

void AUE5_CubesGameModeBase::Action4_Event() {
	UE_LOG(LogTemp, Warning, TEXT("Action4_Event"));
	//Random color
}

void AUE5_CubesGameModeBase::LeftShoulder_Event()
{
	UE_LOG(LogTemp, Warning, TEXT("LeftShoulder_Event"));
}

void AUE5_CubesGameModeBase::RightShoulder_Event()
{
	UE_LOG(LogTemp, Warning, TEXT("RightShoulder_Event"));
}

void AUE5_CubesGameModeBase::RightTrigger_Event(float AxisValue)
{
	UE_LOG(LogTemp, Warning, TEXT("RightTrigger_Event 1"));
	newT->ZoomIn(AxisValue);
}

void AUE5_CubesGameModeBase::LeftTrigger_Event(float AxisValue)
{
	UE_LOG(LogTemp, Warning, TEXT("LeftTrigger_Event"));
	newT->ZoomOut(AxisValue);
}

void AUE5_CubesGameModeBase::RightThumb_Event()
{
	UE_LOG(LogTemp, Warning, TEXT("RightThumb_Event"));
	
}

void AUE5_CubesGameModeBase::LeftThumb_Event()
{
	UE_LOG(LogTemp, Warning, TEXT("LeftThumb_Event"));
	
}

void AUE5_CubesGameModeBase::LeftAxisX(float AxisValue)
{
	UE_LOG(LogTemp, Warning, TEXT("LeftAxisX"));
	this->ZoomRadius += AxisValue * 0.03f;
	//CameraThing->PauseCamera();
	newT->UpdateRadius(ZoomRadius);

}

void AUE5_CubesGameModeBase::LeftAxisY(float AxisValue)
{
	//CameraThing->PauseCamera();
	newT->UpdateRoll(AxisValue * 0.03f);
	UE_LOG(LogTemp, Warning, TEXT("LeftAxisY ?"));
}

void AUE5_CubesGameModeBase::RightAxisX(float AxisValue)
{
	UE_LOG(LogTemp, Warning, TEXT("RightAxisX"));
	newT->UpdatePitch(AxisValue * 0.03f);
}

void AUE5_CubesGameModeBase::RightAxisY(float AxisValue)
{
	UE_LOG(LogTemp, Warning, TEXT("RightAxisY"));
}
