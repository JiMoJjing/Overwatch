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
	// SwiftStrikeStartLocation 설정
	SetSwiftStrikeStartLocation();

	// SwiftStrikeEndLocation 설정
	SetSwiftStrikeEndLocation();

	// Timeline 시작하기 전에 실행
	SwiftStrikeTimelineStarted();

	// Timeline 시작
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
	// SwiftStrikeStartLocation 설정
	SwiftStrikeStartLocation = GetActorLocation();
}

void AGenji::SetSwiftStrikeEndLocation()
{
	// LineTrace Start, End Location 설정
	const FVector traceStartLocation = FollowCamera->GetComponentLocation();
	const FVector traceEndLocation = traceStartLocation + FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X) * SwiftStrikeRange;

	// LineTrace 실행
	UWorld* world = GetWorld();

	// 월드 유효성 검사
	if (world == nullptr) return;

	// 진행 방향으로 사거리 길이 만큼 LineTrace 진행
	FHitResult hitResult;
	bool bLineTraceSuccess = world->LineTraceSingleByChannel(hitResult, traceStartLocation, traceEndLocation, ECollisionChannel::ECC_Visibility);

	// 지형 감지시 SwiftStrikeEndLocation 을 HitResult의 Location으로 설정, 실패 시 traceEndLocation으로 설정
	if (bLineTraceSuccess)
	{
		// Hit되었다면 해당 위치에서 캡슐사이즈만큼 보정해서 위치를 정한다.
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
	// AnimMontage 실행
	if (SwiftStrikeMontage)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(SwiftStrikeMontage);
	}

	// 캡슐 사이즈 조정
	GetCapsuleComponent()->SetCapsuleSize(SwiftStrikeCapsuleSize2D.X, SwiftStrikeCapsuleSize2D.Y);

	// 질풍참 동안 Input 무시
	GetController()->SetIgnoreMoveInput(true);
	GetController()->SetIgnoreLookInput(true);

	// 질풍참 시전중임을 나타낼 bool
	bSwiftStrike = true;

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilationValue);
}

void AGenji::SwiftStrikeTimelineUpdate(float Alpha)
{
	// 질풍참 중 지상에 닿아 Walking이 되면 움직임이 이상해지므로 Falling 으로 보정
	if (GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Falling)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	}

	// 질풍참이 장애물을 만나지 않고 정상 이동했을 때 위치
	const FVector targetLocation = FMath::Lerp(SwiftStrikeStartLocation, SwiftStrikeEndLocation, Alpha);

	// 현재 엑터의 도착 위치로의 방향벡터
	FVector directionVector = SwiftStrikeEndLocation - GetActorLocation();
	directionVector.Normalize();

	// 정상 이동 위치와 현재 위치의 거리 차이를 통해 속도 보정
	const float targetToEndDistance = FVector::Dist(SwiftStrikeEndLocation, targetLocation);
	const float actorToEndDistance = FVector::Dist(SwiftStrikeEndLocation, GetActorLocation());

	float gap = actorToEndDistance / targetToEndDistance;

	// 정상 이동 위치가 너무 작아질 경우 gap이 엄청 커지므로 최소값 보정
	if (FMath::IsNearlyZero(targetToEndDistance, 10.f))
	{
		gap = 1.f;
	}

	// gap 최대값 보정
	if (gap > 4.f)
	{
		gap = 4.f;
	}

	// 벽을 만났을 때 얻는 가속도는 계속 유지가 아니고 시간에 따라 0이 되도록 보간
	SwiftStrikeHitNormalProjection = FMath::VInterpTo(SwiftStrikeHitNormalProjection, FVector::ZeroVector, GetWorld()->GetDeltaSeconds(), HitNormalProjectionInterpSpeed * Alpha);

	// 마지막에 벽에 닿았을때 순간 가속 얻어 튕겨나가는것 방지
	if (Alpha > 0.9f)
	{
		bSwiftStrike = false;
		SwiftStrikeHitNormalProjection = FVector::ZeroVector;
	}

	// 방향벡터에 벽가속벡터 더함
	directionVector += SwiftStrikeHitNormalProjection;
	// 이동은 Velocity값을 직접 제어
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

	// 공중에 있는동안 쌓인 중력가속도 리셋
	StopMovement();

	// Input 다시 받기
	GetController()->SetIgnoreMoveInput(false);
	GetController()->SetIgnoreLookInput(false);

	// 캡슐 컴포넌트 크기 리셋
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
	// 벽에 닿았고, 질풍참 진행중일 경우
	if (Hit.bBlockingHit && bSwiftStrike)
	{
		// 벽의 노말 벡터와 액터의 Right벡터를 내적
		const FVector hitNormal = Hit.Normal;
		const FVector actorRightVector = GetActorRightVector();
		const float dot = FVector::DotProduct(actorRightVector, hitNormal);

		// 방향에 맞게 투영 벡터값 입력, 벽과 거의 수직인 경우 값 적용 x
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
