// Fill out your copyright notice in the Description page of Project Settings.


// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCameraTransition.h"
#include <math.h>


AMyCameraTransition::AMyCameraTransition()
{
    PrimaryActorTick.bCanEverTick = true;
    //Basic constructor
    SetRootComponent(RootComponent);

    //Positioning of camera?
    CircleCenterX = 300.0f;
    CircleCenterY = 300.0f;
    CamZ = 475.0f;
}

AMyCameraTransition::AMyCameraTransition(float x, float y, float z) {
    SetRootComponent(RootComponent);
    PrimaryActorTick.bCanEverTick = true;
    CamX = x;
    CamY = y;
    CamZ = z;
}

void AMyCameraTransition::Tick(float DeltaTime)
{

    Super::Tick(DeltaTime);

     //FString camTransString = TEXT("Tick event!");
     //UE_LOG(LogTemp, Warning, TEXT("Output: %s"), *camTransString);

    //const float TimeToPass = 0.313f;

    const float TimeToPass = 0.0083f;

    UpdateTimer -= DeltaTime;

    //Event
    if (UpdateTimer <= 0) {
        UpdateTimer = TimeToPass; //Reset thing
        //Update Degrees
        
        float newX = CircleRadius * sin(CircleRadians) + CircleCenterX;
        float newY = CircleRadius * cos(CircleRadians) + CircleCenterY;
        
        const float piConv = 180 / 3.14159265359;
        float angle = CircleRadians * piConv;

        float pitch = CamPitch * piConv;
        float roll = CamRoll * piConv;

        //Apply changes to this. 270 offset
        this->SetActorRotation(FRotator(CamPitch, 270 - angle, roll));
        this->SetActorLocation(FVector(newX, newY, CamZ));
        
        //Right to Left;
        //CircleRadians += 2;

        //Left to right
        //CircleRadians -= 2;

        //Rando left to right (not chaning angle? or just providing the base angle
        //CircleRadians += 57.2958 / 8;

        //bpm based left to right

        if (PauseTransitionUpdate == false) {
            CircleRadians += 0.005;
        }
        

        //CircleRadians += 1.0f; //reduce this to get a smaller movement, and change the timer for a smoother action

       /* UE_LOG(LogTemp, Warning, TEXT("Output: radians %s"), *FString::FromInt(CircleRadians));
        UE_LOG(LogTemp, Warning, TEXT("Output: radius %s"), *FString::FromInt(CircleRadius));
        UE_LOG(LogTemp, Warning, TEXT("Output: angle %s"), *FString::SanitizeFloat(angle));*/

        /*if (CircleRadians > 360) {
            CircleRadians = 180;
        }*/
    }
}

void AMyCameraTransition::ResetCamera() {
    PauseTransitionUpdate = false;
    SetPosition(initX, initY, initZ);
    SetRotation(initYaw, initRoll, initPitch);
}

void AMyCameraTransition::SetPosition(float x, float y, float z)
{
    CamY = y;
    CamX = x;
    CamZ = z;
    this->SetActorLocation(FVector(CamX, CamY, CamZ));
}

void AMyCameraTransition::SetRotation(float yaw, float roll, float pitch)
{
    CamYaw = yaw;
    CamRoll = roll;
    CamPitch = pitch;

    this->SetActorRotation(FRotator(CamPitch, CamYaw, CamRoll));
}



void AMyCameraTransition::SetDataAll(float x, float y, float z, float yaw, float roll, float pitch, float displayTime, float transitionTime)
{
    CamX = x;
    CamY = y;
    CamZ = z;

    CamYaw = yaw;
    CamRoll = roll;
    CamPitch = pitch;

    DisplayTime = displayTime;
    TransitionTime = transitionTime;

    this->SetActorRotation(FRotator(CamPitch, CamYaw, CamRoll));
    this->SetActorLocation(FVector(CamX, CamY, CamZ));
}

void AMyCameraTransition::MoveInCircle(float radians, float centerX, float centerY, float updateTimer)
{
    CircleRadians = radians;
    CircleCenterY = centerY;
    CircleCenterX = centerX;
    UpdateTimer = updateTimer; //Frequency of updating (time in seconds)
}

void AMyCameraTransition::ZoomOut(float amount)
{
    CircleRadius += 10 * amount;
}

void AMyCameraTransition::ZoomIn(float amount)
{
    CircleRadius -= 10 * amount;
}

//Enabling Manual control over the update direction
void AMyCameraTransition::UpdateRadius(float radius)
{
    CircleRadians = radius;
    //Pause the default rotation..
    PauseTransitionUpdate = true;
}

void AMyCameraTransition::UpdateRoll(float radius)
{
    CamRoll += radius;
    PauseTransitionUpdate = true;
}

void AMyCameraTransition::UpdatePitch(float radius)
{
    CamPitch += radius;
    PauseTransitionUpdate = true;
}

