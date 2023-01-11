// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCameraController.h"
#include "Camera/CameraActor.h"
#include "MyCameraTransition.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values

AMyCameraController::AMyCameraController()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //Attach to the root component
    SetRootComponent(RootComponent);
}

// Called when the game starts or when spawned
void AMyCameraController::BeginPlay()
{
    Super::BeginPlay();

    if (CameraTransitionArray.Num() > 0) {
        //Set view to first camera.
        APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
        OurPlayerController->SetViewTarget(CameraTransitionArray[0]);
    }
}

void AMyCameraController::AddNewCamera(AMyCameraTransition* newCam) {

    FActorSpawnParameters SpawnInfo;
    SpawnInfo.Owner = this;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    CameraTransitionArray.Add(newCam);
    //Add Camera to the Array
    //CameraTransitionArray.Add(GetWorld()->SpawnActor<AMyCameraTransition>(AMyCameraTransition::StaticClass(), FVector(newCam->CamX, newCam->CamY, newCam->CamZ), FRotator(newCam->CamPitch, newCam->CamYaw, newCam->CamRoll), SpawnInfo));

    //display first camera?
    if (CameraTransitionArray.Num() == 1) {
        //Display the first camera provided
        APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
        OurPlayerController->SetViewTarget(CameraTransitionArray[0]);
    }
}

void AMyCameraController::PauseCamera()
{
    this->Paused = true;
}

void AMyCameraController::ResumeCamera()
{
    this->Paused = false;
}

void AMyCameraController::TogglePauseCamera()
{
    if (this->Paused) {
        Paused = false;
    }
    else {
        Paused = true;
    }
}


// Called every frame
void AMyCameraController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TimeToNextCameraChange -= DeltaTime;

    //updates camera rotation

    if (!this->Paused) {
        if (TimeToNextCameraChange <= 0.0f && CameraTransitionArray.Num() >= 2)
        {
            SelectedCamera++;
            TimeToNextCameraChange += CameraTransitionArray[SelectedCamera]->DisplayTime;
            APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);

            if (OurPlayerController)
            {
                if (CameraTransitionArray[SelectedCamera] && (OurPlayerController->GetViewTarget() != CameraTransitionArray[SelectedCamera]))
                {
                    //SetViewTargetWithBlend
                    OurPlayerController->SetViewTargetWithBlend(CameraTransitionArray[SelectedCamera]);
                    //OurPlayerController->SetViewTarget(CameraTransitionArray[SelectedCamera]);
                }
            }

            if (SelectedCamera == CameraTransitionArray.Num() - 1) {
                //Reset
                SelectedCamera = -1;
            }
        }
    }
}

