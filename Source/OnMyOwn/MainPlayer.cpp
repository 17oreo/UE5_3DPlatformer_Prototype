// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleSize(42.f, 96.0f);

	//Don't rotate when the controller rotates
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Create a camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; //The camera follows at
	CameraBoom->bUsePawnControlRotation = true; //Rotate the arm based on the controller
	CameraBoom->bDoCollisionTest = false; //Tell the camera boom NOT to follow collision changes instantly

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); //Attach the camera to the end of the boom
	FollowCamera->bUsePawnControlRotation = false; //Camera does not rotate relative to arm

	bDead = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; //Character moves in the direction of input
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f); //Rotation rate	
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.2f;

	Coins = 0;

	//Jump stuff
	JumpCount = 0;
	MaxJumpCount = 2;
		 
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	bIsCrouchingCustom = false;
}

void AMainPlayer::MoveForward(float axis)
{
	if (!bDead)
	{
		const FRotator Rotation = Controller->GetControlRotation(); //Get the rotation of the controller
		const FRotator YawRotation(0, Rotation.Yaw, 0); //Zero out pitch and roll

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); //Get the forward vector

		AddMovementInput(Direction, axis); //Move the player in that direction)
	}
}

void AMainPlayer::MoveRight(float axis)
{
	if (!bDead)
	{
		const FRotator Rotation = Controller->GetControlRotation(); //Get the rotation of the controller
		const FRotator YawRotation(0, Rotation.Yaw, 0); //Zero out pitch and roll

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); //Get the forward vector

		AddMovementInput(Direction, axis); //Move the player in that direction)
	}
}

void AMainPlayer::OnBeginOverlap(class UPrimitiveComponent* HitComp,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor->ActorHasTag("Coin"))
	{
		Coins++;
		UGameplayStatics::PlaySoundAtLocation(this, CoinSound, OtherActor->GetActorLocation()); //Play coin sound at the location of the coin
		OtherActor->Destroy();
	}
}

void AMainPlayer::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false); //Restart the level
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMainPlayer::OnBeginOverlap);
	
	if (PlayerCoinWidgetClass != nullptr)
	{
		PlayerCoinWidget = CreateWidget(GetWorld(), PlayerCoinWidgetClass);
		PlayerCoinWidget->AddToViewport();
	}

	//CurrentBoomZ = CameraBoom->TargetOffset.Z;
}


// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector PlayerLocation = GetActorLocation();

	//Check to see if the player falls off the map
	if (PlayerLocation.Z < -50.0f)
	{
		bDead = true;
		GetMesh()->SetSimulatePhysics(true); //Enable physics simulation on the mesh

		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMainPlayer::RestartGame, 1.0f, false); //Restart the level after 3 seconds
	}

	//Idle dance check
	if (GetVelocity().Size() > 0.0f)
	{
		timeSinceLastMovement = 0.0f;
		bIsDancing = false;
	}
	else
	{
		timeSinceLastMovement += DeltaTime;
		if (timeSinceLastMovement >= 3.0f && !bIsDancing) //If the player hasn't moved for 5 seconds
		{
			bIsDancing = true;
		}
	}

	//Crouch camera adjustment
	float HeightDiff = StandingCapsuleHalfHeight - CrouchingCapsuleHalfHeight;
	float TargetOffsetZ = bIsCrouchingCustom ? HeightDiff : 0.f;

	CurrentCameraOffsetZ = FMath::FInterpTo(CurrentCameraOffsetZ, TargetOffsetZ, DeltaTime, 500.0f);

	FollowCamera->SetRelativeLocation(FVector(0.f, 0.f, CurrentCameraOffsetZ));

}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainPlayer::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMainPlayer::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMainPlayer::ToggleCrouch);
}

void AMainPlayer::Jump()
{
	// Prevent jumping while crouched
	if (bIsCrouchingCustom)
	{
		return;
	}
	//Check if the character is on the floor 
	if (!GetCharacterMovement()->IsFalling())
	{
		Super::Jump();
		JumpCount++;
	}
	//check if the character is in the air and has jumps left
	else if (GetCharacterMovement()->IsFalling())
	{
		//Try to double jump
		TryDoubleJump();
	}
}

void AMainPlayer::TryDoubleJump()
{
	if (JumpCount < MaxJumpCount)
	{
		
		// Give an upward impulse manually
		FVector JumpVelocity = FVector(0.f, 0.f, GetCharacterMovement()->JumpZVelocity * 0.8f);
		LaunchCharacter(JumpVelocity, false, true);

		JumpCount++;
	}
}

void AMainPlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	JumpCount = 0; // Reset jump count when touching ground
}

void AMainPlayer::ToggleCrouch()
{
	if (bIsCrouchingCustom)
	{
		StopCrouch();
	}
	else
	{
		StartCrouch();
	}
}

void AMainPlayer::StartCrouch()
{
	Crouch();
	//GetCharacterMovement()->bWantsToCrouch = true;
	bIsCrouchingCustom = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void AMainPlayer::StopCrouch()
{
	UnCrouch();
	//GetCharacterMovement()->bWantsToCrouch = false;
	bIsCrouchingCustom = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}



