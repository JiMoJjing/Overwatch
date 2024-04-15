#include "ActorComponents/Ability/Genji/SwiftStrikeComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

#include "Components/CapsuleComponent.h"

#include "Kismet/GameplayStatics.h"

#include "Characters/Player/Genji/Genji.h"
#include "Colliders/SwiftStrikeCollider.h"

#include "Utilities.h"


USwiftStrikeComponent::USwiftStrikeComponent() : TimeDilationValue(0.2f), SwiftStrikeDistance(1884.f), SwiftStrikeSpeed(5000.f), bSwiftStrike(false), CapsuleSize2D(42.f, 96.f)
, SwiftStrikeCapsuleSize2D(21.f, 48.f)
{
	PrimaryComponentTick.bCanEverTick = true;

	SwiftStrikeCollider = nullptr;
	PlayerBase = nullptr;

	OnSwiftStrikeCapsuleSizeTimelineUpdate.BindUFunction(this, FName("SwiftStrikeCapsuleSizeTimelineUpdate"));
	OnSwiftStrikeCapsuleSizeTimelineFinished.BindUFunction(this, FName("SwiftStrikeCapsuleSizeTimelineFinished"));
}

void USwiftStrikeComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerBase = Cast<AGenji>(GetOwner());
	if (PlayerBase)
	{
		PlayerBase->GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &USwiftStrikeComponent::OnSwiftStrikeComponentHit);
		PlayerBase->GetMesh()->GetAnimInstance()->OnMontageBlendingOut.AddDynamic(this, &USwiftStrikeComponent::SwiftStrikeMontageInterrupted);
		
		PlayerBase->GetCharacterMovement()->MaxFlySpeed = SwiftStrikeSpeed;

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.Owner = PlayerBase;
		ActorSpawnParams.Instigator = PlayerBase;

		FVector SpawnLocation = FVector::ZeroVector;
		FRotator SpawnRotation = FRotator::ZeroRotator;
		if (SwiftStrikeColliderClass)
		{
			SwiftStrikeCollider = GetWorld()->SpawnActor<ASwiftStrikeCollider>(SwiftStrikeColliderClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (SwiftStrikeCollider)
			{
				FAttachmentTransformRules transformRules(EAttachmentRule::KeepRelative, true);
				SwiftStrikeCollider->AttachToActor(PlayerBase, transformRules);
			}
		}
	}
	else
	{
		CLog::Log(TEXT("USwiftStrikeComponent BeginPlay PlayerBase nullptr"));
	}

	SwiftStrikeCapsuleSizeTimelineSettings();
}

void USwiftStrikeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SwiftStrikeCapsuleSizeTimeline.TickTimeline(DeltaTime);
}

void USwiftStrikeComponent::UseAbility()
{
	if (CanActivateAbility())
	{
		ActivateAbility();
	}
}

void USwiftStrikeComponent::ActivateAbility()
{
	Super::ActivateAbility();

	SwiftStrikeStarted();
}

void USwiftStrikeComponent::DeactivateAbility()
{
	Super::DeactivateAbility();
	SwiftStrikeFinishSetting();
	CooldownStart();
}

void USwiftStrikeComponent::SwiftStrikeStarted()
{
	SwiftStrikeStartSetting();

	if (SwiftStrikeMontage && PlayerBase)
	{
		PlayerBase->GetMesh()->GetAnimInstance()->Montage_Play(SwiftStrikeMontage);
	}
	else
	{
		CLog::Log(TEXT("USwiftStrikeComponent SwiftStrike PlayerBase or SwiftStrikeMontage nullptr"));
	}
}

//void USwiftStrikeComponent::SwiftStrikeFinished()
//{
//	DeactivateAbility();
//}

void USwiftStrikeComponent::SetSwiftStrikeStartLocation()
{
	if (PlayerBase == nullptr)
	{
		CLog::Log(TEXT("USwiftStrikeComponent SetSwiftStrikeStartLocation PlayerBase nullptr"));
		return;
	}

	// SwiftStrikeStartLocation 설정
	SwiftStrikeStartLocation = PlayerBase->GetActorLocation();
}

void USwiftStrikeComponent::SetSwiftStrikeEndLocation()
{
	if (PlayerBase == nullptr)
	{
		CLog::Log(TEXT("USwiftStrikeComponent SetSwiftStrikeEndLocation PlayerBase nullptr"));
		return;
	}

	FHitResult HitResult;
	FVector TraceEndLocation;

	// 진행 방향으로 사거리 길이 만큼 LineTrace 진행
	bool bLineTraceSuccess = PlayerBase->TraceUnderCrosshair(SwiftStrikeDistance, HitResult, TraceEndLocation, ECollisionChannel::ECC_Camera);

	// 지형 감지시 SwiftStrikeEndLocation 을 HitResult의 Location으로 설정, 실패 시 traceEndLocation으로 설정
	if (bLineTraceSuccess)
	{
		// Hit되었다면 해당 위치에서 캡슐사이즈만큼 보정해서 위치를 정한다.
		const FVector HitNormal = HitResult.Normal;
		const FVector AdjustedVector(HitNormal.X * CapsuleSize2D.X, HitNormal.Y * CapsuleSize2D.X, HitNormal.Z * CapsuleSize2D.Y);
		SwiftStrikeEndLocation = HitResult.Location + AdjustedVector;
	}
	else
	{
		SwiftStrikeEndLocation = TraceEndLocation;
	}

	// Draw Debug
	DrawDebugSphere(GetWorld(), SwiftStrikeEndLocation, 10.f, 20, FColor::Green, false, 5.f, 0U, 1.f);
	DrawDebugLine(GetWorld(), PlayerBase->GetActorLocation(), SwiftStrikeEndLocation, FColor::Green, false, 5.f, 0U, 1.f);
}

void USwiftStrikeComponent::SwiftStrikeStartSetting()
{
	// 시작점, 도착점 세팅
	SetSwiftStrikeStartLocation();
	SetSwiftStrikeEndLocation();

	if (PlayerBase == nullptr)
	{
		CLog::Log(TEXT("USwiftStrikeComponent SwiftStrikeStartSetting PlayerBase nullptr"));
		return;
	}

	bSwiftStrike = true;

	// 쿨초로 캡슐 크기 타임라인 끝나기 전에 실행되면 타임라인 끄기
	if (SwiftStrikeCapsuleSizeTimeline.IsPlaying())
	{
		SwiftStrikeCapsuleSizeTimeline.Stop();
	}

	// 캡슐 사이즈 조정
	PlayerBase->GetCapsuleComponent()->SetCapsuleSize(SwiftStrikeCapsuleSize2D.X, SwiftStrikeCapsuleSize2D.Y);

	// 캡슐 콜리전이 적 캡슐 콜리전을 무시하도록 하기
	PlayerBase->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);

	// 질풍참 동안 Input 무시
	AController* Controller = PlayerBase->GetController();
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(true);
		Controller->SetIgnoreLookInput(true);
	}
	else
	{
		CLog::Log(TEXT("USwiftStrikeComponent SwiftStrikeStartSetting Controller nullptr"));
	}

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilationValue);

	// MovementMode Flying 설정
	PlayerBase->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	// 최대 가속도 큰 값 -> Velocity 0에서 5000으로 급가속
	PlayerBase->GetCharacterMovement()->MaxAcceleration = 1000000.f;

	// 질풍참 콜라이더 콜리전 On
	if (SwiftStrikeCollider)
	{
		SwiftStrikeCollider->SwiftStrikeStarted();
	}
	else
	{
		CLog::Log(TEXT("USwiftStrikeComponent SwiftStrikeStartSetting SwiftStrikeCollider nullptr"));
	}
}

void USwiftStrikeComponent::SwiftStrikeFinishSetting()
{
	if (PlayerBase == nullptr)
	{
		CLog::Log(TEXT("USwiftStrikeComponent SwiftStrikeFinished PlayerBase nullptr"));
		return;
	}

	if (PlayerBase->GetMesh()->GetAnimInstance()->Montage_IsPlaying(SwiftStrikeMontage))
	{
		PlayerBase->GetMesh()->GetAnimInstance()->Montage_Stop(0.f, SwiftStrikeMontage);
	}

	bSwiftStrike = false;

	// MovementMode Falling 으로 변경
	PlayerBase->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);

	// 최대 가속도 정상화
	PlayerBase->GetCharacterMovement()->MaxAcceleration = 2048.f;

	// 공중에 있는동안 쌓인 중력가속도 리셋
	PlayerBase->StopMovement();

	// Input 다시 받기
	AController* Controller = PlayerBase->GetController();
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(false);
		Controller->SetIgnoreLookInput(false);
	}
	else
	{
		CLog::Log(TEXT("USwiftStrikeComponent SwiftStrikeFinished Controller nullptr"));
	}

	// 캡슐 컴포넌트 크기 리셋
	SwiftStrikeCapsuleSizeTimeline.PlayFromStart();

	// 적 캡슐 반응 다시 Block
	PlayerBase->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);

	FVector zeroVector = FVector::ZeroVector;
	SwiftStrikeStartLocation = zeroVector;
	SwiftStrikeEndLocation = zeroVector;
	SwiftStrikeHitNormalProjection = zeroVector;

	// 질풍참 콜라이더 콜리전 끄기
	if (SwiftStrikeCollider)
	{
		SwiftStrikeCollider->SwiftStrikeFinished();
	}
	else
	{
		CLog::Log(TEXT("USwiftStrikeComponent SwiftStrikeFinished SwiftStrikeCollider nullptr"));
	}
}

void USwiftStrikeComponent::SwiftStrikeUpdate(float DeltaTime)
{
	if (PlayerBase == nullptr)
	{
		CLog::Log(TEXT("USwiftStrikeComponent SwiftStrikeUpdate PlayerBase nullptr"));
		return;
	}

	// 현재 엑터의 도착 위치로의 방향벡터
	FVector DirectionVector = SwiftStrikeEndLocation - PlayerBase->GetActorLocation();
	DirectionVector.Normalize();

	// 투영 벡터 더하기
	DirectionVector += SwiftStrikeHitNormalProjection;
	DirectionVector.Normalize();

	// 이동
	PlayerBase->AddMovementInput(DirectionVector, 1.f, true);

	// 투영 벡터는 점차 감소
	SwiftStrikeHitNormalProjection = FMath::VInterpTo(SwiftStrikeHitNormalProjection, FVector::ZeroVector, DeltaTime, HitNormalProjectionInterpSpeed);

	// 목표 위치에 도달하면 몽타주 종료
	if (PlayerBase->GetActorLocation().Equals(SwiftStrikeEndLocation, 10.f))
	{
		PlayerBase->GetMesh()->GetAnimInstance()->Montage_Stop(0.f, SwiftStrikeMontage);
	}
}

void USwiftStrikeComponent::OnSwiftStrikeComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (PlayerBase == nullptr)
	{
		CLog::Log(TEXT("USwiftStrikeComponent OnSwiftStrikeComponentHit PlayerBase nullptr"));
		return;
	}

	// 벽에 닿았고, 질풍참 진행중일 경우
	if (Hit.bBlockingHit && bSwiftStrike)
	{
		// 벽의 노말 벡터와 액터의 Right벡터를 내적
		const FVector HitNormal = Hit.Normal;
		const FVector ActorRightVector = PlayerBase->GetActorRightVector();
		const float dot = FVector::DotProduct(ActorRightVector, HitNormal);

		// 방향에 맞게 투영 벡터값 입력, 벽과 거의 수직인 경우 값 적용 x
		if (dot < -0.1f)
		{
			SwiftStrikeHitNormalProjection = FVector(HitNormal.Y * -1, HitNormal.X, 0.f);
		}
		else if (dot > 0.1f)
		{
			SwiftStrikeHitNormalProjection = FVector(HitNormal.Y, HitNormal.X * -1, 0.f);
		}
	}
}

void USwiftStrikeComponent::SwiftStrikeCapsuleSizeTimelineSettings()
{
	if (SwiftStrikeCapsuleSizeCurveFloat)
	{
		SwiftStrikeCapsuleSizeTimeline.AddInterpFloat(SwiftStrikeCapsuleSizeCurveFloat, OnSwiftStrikeCapsuleSizeTimelineUpdate);
		SwiftStrikeCapsuleSizeTimeline.SetTimelineFinishedFunc(OnSwiftStrikeCapsuleSizeTimelineFinished);
		SwiftStrikeCapsuleSizeTimeline.SetLooping(false);
	}
	else
	{
		CLog::Log(TEXT("USwiftStrikeComponent SwiftStrikeCapsuleSizeTimelineSettings SwiftStrikeCapsuleSizeCurveFloat nullptr"));
	}
}

void USwiftStrikeComponent::SwiftStrikeCapsuleSizeTimelineUpdate(float Alpha)
{
	if (PlayerBase == nullptr)
	{
		CLog::Log(TEXT("USwiftStrikeComponent SwiftStrikeCapsuleSizeTimelineUpdate PlayerBase nullptr"));
		return;
	}

	FVector2D CapsuleSize = FMath::Lerp(SwiftStrikeCapsuleSize2D, CapsuleSize2D, Alpha);
	 
	PlayerBase->GetCapsuleComponent()->SetCapsuleSize(CapsuleSize.X, CapsuleSize.Y);
}

void USwiftStrikeComponent::SwiftStrikeCapsuleSizeTimelineFinished()
{
	if (PlayerBase == nullptr)
	{
		CLog::Log(TEXT("USwiftStrikeComponent SwiftStrikeCapsuleSizeTimelineFinished PlayerBase nullptr"));
		return;
	}

	PlayerBase->GetCapsuleComponent()->SetCapsuleSize(CapsuleSize2D.X, CapsuleSize2D.Y);
}

void USwiftStrikeComponent::SwiftStrikeMontageInterrupted(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == SwiftStrikeMontage && bInterrupted)
	{
		DeactivateAbility();
	}
}

void USwiftStrikeComponent::SwiftStrikeMontageFinished()
{
	DeactivateAbility();
}