#include "Characters/Player/Genji.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/MovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Utilities.h"


AGenji::AGenji() : TimeDilationValue(0.1f), SwiftStrikeRange(1884.f), SwiftStrikeSpeed(5000.f), bSwiftStrike(false), bSwiftStrikeHit(false), bSecondJump(true), CapsuleSize2D(42.f, 96.f)
, SwiftStrikeCapsuleSize2D(24.f, 24.f)
{
	OnSwiftStrikeTimelineUpdate.BindUFunction(this, FName("SwiftStrikeTimelineUpdate"));
	OnSwiftStrikeTimelineFinished.BindUFunction(this, FName("SwiftStrikeTimelineFinished"));

	OnSwiftStrikeCapsuleSizeTimelineUpdate.BindUFunction(this, FName("SwiftStrikeCapsuleSizeTimelineUpdate"));
	OnSwiftStrikeCapsuleSizeTimelineFinished.BindUFunction(this, FName("SwiftStrikeCapsuleSizeTimelineFinished"));
}

void AGenji::BeginPlay()
{
	Super::BeginPlay();

	SwiftStrikeTimelineSettings();
	SwiftStrikeCapsuleSizeTimelineSettings();

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AGenji::OnSwiftStrikeComponentHit);
}

void AGenji::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector traceStartLocation = FollowCamera->GetComponentLocation();
	const FVector traceEndLocation = traceStartLocation + FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X) * SwiftStrikeRange;
	FHitResult hitresult;

	DrawDebugSphere(GetWorld(), traceEndLocation, 10.f, 10, FColor::Blue, false, 0.1f, 0U, 1.f);

	SwiftStrikeTimeline.TickTimeline(DeltaTime);
	SwiftStrikeCapsuleSizeTimeline.TickTimeline(DeltaTime);
}

void AGenji::Jump()
{
	Super::Jump();
}

void AGenji::StopJumping()
{
	Super::StopJumping();
}

void AGenji::AbilityOne()
{
	SwiftStrike();
}

void AGenji::AbilityTwo()
{
}

void AGenji::AbilityThree()
{
}

void AGenji::PrimaryFire()
{
}

void AGenji::SecondaryFire()
{
}

void AGenji::Reload()
{
}

void AGenji::QuickMelee()
{
}

void AGenji::SwiftStrike()
{
	// SwiftStrikeStartLocation ����
	SetSwiftStrikeStartLocation();

	// SwiftStrikeEndLocation ����
	SetSwiftStrikeEndLocation();

	// Timeline �����ϱ� ���� ����
	SwiftStrikeTimelineStarted();

	// Timeline ����
	SwiftStrikeTimeline.PlayFromStart();
}

void AGenji::SwiftStrikeTimelineSettings()
{
	if (SwiftStrikeCurveFloat)
	{
		SwiftStrikeTimeline.AddInterpFloat(SwiftStrikeCurveFloat, OnSwiftStrikeTimelineUpdate);
		SwiftStrikeTimeline.SetTimelineFinishedFunc(OnSwiftStrikeTimelineFinished);
		SwiftStrikeTimeline.SetLooping(false);
	}
}

void AGenji::SetSwiftStrikeStartLocation()
{
	// SwiftStrikeStartLocation ����
	SwiftStrikeStartLocation = GetActorLocation();
}

void AGenji::SetSwiftStrikeEndLocation()
{
	// LineTrace Start, End Location ����
	const FVector traceStartLocation = FollowCamera->GetComponentLocation();
	const FVector traceEndLocation = traceStartLocation + FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X) * SwiftStrikeRange;

	// LineTrace ����
	UWorld* world = GetWorld();

	// ���� ��ȿ�� �˻�
	if (world == nullptr) return;

	// ���� �������� ��Ÿ� ���� ��ŭ LineTrace ����
	FHitResult hitResult;
	bool bLineTraceSuccess = world->LineTraceSingleByChannel(hitResult, traceStartLocation, traceEndLocation, ECollisionChannel::ECC_Visibility);

	// ���� ������ SwiftStrikeEndLocation �� HitResult�� Location���� ����, ���� �� traceEndLocation���� ����
	if (bLineTraceSuccess)
	{
		// Hit�Ǿ��ٸ� �ش� ��ġ���� ĸ�������ŭ �����ؼ� ��ġ�� ���Ѵ�.
		const FVector hitNormal = hitResult.Normal;
		const FVector adjustedVector(hitNormal.X * 24.f, hitNormal.Y * 24.f, hitNormal.Z * 96.f);
		SwiftStrikeEndLocation = hitResult.Location + adjustedVector;
	}
	else
	{
		SwiftStrikeEndLocation = traceEndLocation;
	}

	// Draw Debug
	DrawDebugSphere(world, SwiftStrikeEndLocation, 10.f, 20, FColor::Green, false, 5.f, 0U, 1.f);
	DrawDebugLine(world, traceStartLocation, SwiftStrikeEndLocation, FColor::Green, false, 5.f, 0U, 1.f);
}


void AGenji::SwiftStrikeTimelineStarted()
{
	// AnimMontage ����
	if (SwiftStrikeMontage)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(SwiftStrikeMontage);
	}

	// ĸ�� ������ ����
	GetCapsuleComponent()->SetCapsuleSize(SwiftStrikeCapsuleSize2D.X, SwiftStrikeCapsuleSize2D.Y);

	// ��ǳ�� ���� Input ����
	GetController()->SetIgnoreMoveInput(true);
	GetController()->SetIgnoreLookInput(true);

	// ��ǳ�� ���������� ��Ÿ�� bool
	bSwiftStrike = true;

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilationValue);
}

void AGenji::SwiftStrikeTimelineUpdate(float Alpha)
{
	// ��ǳ�� �� ���� ��� Walking�� �Ǹ� �������� �̻������Ƿ� Falling ���� ����
	if (GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Falling)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	}

	// ��ǳ���� ��ֹ��� ������ �ʰ� ���� �̵����� �� ��ġ
	const FVector targetLocation = FMath::Lerp(SwiftStrikeStartLocation, SwiftStrikeEndLocation, Alpha);

	// ���� ������ ���� ��ġ���� ���⺤��
	FVector directionVector = SwiftStrikeEndLocation - GetActorLocation();
	directionVector.Normalize();

	// ���� �̵� ��ġ�� ���� ��ġ�� �Ÿ� ���̸� ���� �ӵ� ����
	const float targetToEndDistance = FVector::Dist(SwiftStrikeEndLocation, targetLocation);
	const float actorToEndDistance = FVector::Dist(SwiftStrikeEndLocation, GetActorLocation());

	float gap = actorToEndDistance / targetToEndDistance;

	// ���� �̵� ��ġ�� �ʹ� �۾��� ��� gap�� ��û Ŀ���Ƿ� �ּҰ� ����
	if (FMath::IsNearlyZero(targetToEndDistance, 10.f))
	{
		gap = 1.f;
	}

	// gap �ִ밪 ����
	if (gap > 4.f)
	{
		gap = 4.f;
	}

	// ���� ������ �� ��� ���ӵ��� ��� ������ �ƴϰ� �ð��� ���� 0�� �ǵ��� ����
	SwiftStrikeHitNormalProjection = FMath::VInterpTo(SwiftStrikeHitNormalProjection, FVector::ZeroVector, GetWorld()->GetDeltaSeconds(), HitNormalProjectionInterpSpeed * Alpha);

	// �������� ���� ������� ���� ���� ��� ƨ�ܳ����°� ����
	if (Alpha > 0.9f)
	{
		bSwiftStrike = false;
		SwiftStrikeHitNormalProjection = FVector::ZeroVector;
	}

	// ���⺤�Ϳ� �����Ӻ��� ����
	directionVector += SwiftStrikeHitNormalProjection;
	// �̵��� Velocity���� ���� ����
	GetCharacterMovement()->Velocity = directionVector * SwiftStrikeSpeed * gap;


	//FString str = FString::Printf(TEXT("gap : %f"), gap);
	//CLog::Log(str);
	//
	//FString str2 = FString::Printf(TEXT("SwiftStrikeHitNormalProjection : %s"), *SwiftStrikeHitNormalProjection.ToString());
	//CLog::Print(str2, 5, 10.f, FColor::Blue);

	//FString str3 = FString::Printf(TEXT("Velocity : %f"), GetVelocity().Size());
	//CLog::Log(str3);
}

void AGenji::SwiftStrikeTimelineFinished()
{
	CLog::Print(TEXT("SwiftStrikeTimelineFinished"), -1, 2.f, FColor::Red);

	// ���߿� �ִµ��� ���� �߷°��ӵ� ����
	StopMovement();

	// Input �ٽ� �ޱ�
	GetController()->SetIgnoreMoveInput(false);
	GetController()->SetIgnoreLookInput(false);

	// ĸ�� ������Ʈ ũ�� ����
	SwiftStrikeCapsuleSizeTimeline.PlayFromStart();

	bSwiftStrike = false;
	
	FVector zeroVector = FVector::ZeroVector;
	SwiftStrikeStartLocation = zeroVector;
	SwiftStrikeEndLocation = zeroVector;
	SwiftStrikeHitNormalProjection = zeroVector;
	
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}

void AGenji::OnSwiftStrikeComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// ���� ��Ұ�, ��ǳ�� �������� ���
	if (Hit.bBlockingHit && bSwiftStrike)
	{
		// ���� �븻 ���Ϳ� ������ Right���͸� ����
		const FVector hitNormal = Hit.Normal;
		const FVector actorRightVector = GetActorRightVector();
		const float dot = FVector::DotProduct(actorRightVector, hitNormal);

		// ���⿡ �°� ���� ���Ͱ� �Է�, ���� ���� ������ ��� �� ���� x
		if (dot < -0.1f)
		{
			SwiftStrikeHitNormalProjection = FVector(hitNormal.Y * -1, hitNormal.X, 0.f);
		}
		else if(dot > 0.1f)
		{
			SwiftStrikeHitNormalProjection = FVector(hitNormal.Y, hitNormal.X * -1, 0.f);
		}
	}
}

void AGenji::SwiftStrikeCapsuleSizeTimelineSettings()
{
	if (SwiftStrikeCapsuleSizeCurveFloat)
	{
		SwiftStrikeCapsuleSizeTimeline.AddInterpFloat(SwiftStrikeCapsuleSizeCurveFloat, OnSwiftStrikeCapsuleSizeTimelineUpdate);
		SwiftStrikeCapsuleSizeTimeline.SetTimelineFinishedFunc(OnSwiftStrikeCapsuleSizeTimelineFinished);
		SwiftStrikeCapsuleSizeTimeline.SetLooping(false);
	}
}

void AGenji::SwiftStrikeCapsuleSizeTimelineUpdate(float Alpha)
{
	FVector2D capsuleSize = FMath::Lerp(SwiftStrikeCapsuleSize2D, CapsuleSize2D, Alpha);

	GetCapsuleComponent()->SetCapsuleSize(capsuleSize.X, capsuleSize.Y);
}

void AGenji::SwiftStrikeCapsuleSizeTimelineFinished()
{
	GetCapsuleComponent()->SetCapsuleSize(CapsuleSize2D.X, CapsuleSize2D.Y);
}
