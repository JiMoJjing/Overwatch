#include "ActorComponents/Ability/Genji/Genji_SwiftStrikeComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

#include "Components/CapsuleComponent.h"

#include "Kismet/GameplayStatics.h"

#include "Characters/Player/Genji/Genji.h"
#include "Colliders/Genji/SwiftStrikeCollider.h"

#include "Utilities.h"


UGenji_SwiftStrikeComponent::UGenji_SwiftStrikeComponent() :  SwiftStrikeDistance(1884.f), SwiftStrikeSpeed(5000.f),  CapsuleSize2D(42.f, 96.f)
, SwiftStrikeCapsuleSize2D(21.f, 48.f), SwiftStrikeDamage(50.f), bSwiftStrike(false)
{
	PrimaryComponentTick.bCanEverTick = true;

	SwiftStrikeCollider = nullptr;

	OnSwiftStrikeCapsuleSizeTimelineUpdate.BindUFunction(this, FName("SwiftStrikeCapsuleSizeTimelineUpdate"));
	OnSwiftStrikeCapsuleSizeTimelineFinished.BindUFunction(this, FName("SwiftStrikeCapsuleSizeTimelineFinished"));
	
	CooldownTime = 8.f;
}

void UGenji_SwiftStrikeComponent::BeginPlay()
{
	Super::BeginPlay();

	GenjiRef = Cast<AGenji>(GetOwner());
	
	if (GenjiRef)
	{
		GenjiRef->GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &UGenji_SwiftStrikeComponent::OnSwiftStrikeComponentHit);
		GenjiRef->GetCharacterMovement()->MaxFlySpeed = SwiftStrikeSpeed;

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.Owner = GenjiRef;
		ActorSpawnParams.Instigator = GenjiRef;

		FVector SpawnLocation = FVector::ZeroVector;
		FRotator SpawnRotation = FRotator::ZeroRotator;
		if (SwiftStrikeColliderClass)
		{
			SwiftStrikeCollider = GetWorld()->SpawnActor<ASwiftStrikeCollider>(SwiftStrikeColliderClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (SwiftStrikeCollider)
			{
				FAttachmentTransformRules TransformRules(EAttachmentRule::KeepRelative, true);
				SwiftStrikeCollider->AttachToActor(GenjiRef, TransformRules);
			}
		}
		// 용검 시작 시 질풍참 쿨타임 초기화
		GenjiRef->OnDragonbladeActive.AddDynamic(this, &UGenji_SwiftStrikeComponent::DragonbladeActive);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] GenjiRef is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("BeginPlay"));
	}

	SwiftStrikeCapsuleSizeTimelineSettings();
}

void UGenji_SwiftStrikeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SwiftStrikeCapsuleSizeTimeline.TickTimeline(DeltaTime);
}

void UGenji_SwiftStrikeComponent::StartAbility()
{
	Super::StartAbility();
	
	PlayAbilityMontage();
}

void UGenji_SwiftStrikeComponent::FinishAbility()
{
	Super::FinishAbility();
	SwiftStrikeFinishSetting();
	CooldownTimerStart();
}

void UGenji_SwiftStrikeComponent::PlayAbilityMontage()
{
	if (GenjiRef && AbilityMontage)
	{
		SwiftStrikeStartSetting();
		GenjiRef->GetMesh()->GetAnimInstance()->Montage_Play(AbilityMontage);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] GenjiRef or AbilityMontage is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("StartAbility"));
	}
}

void UGenji_SwiftStrikeComponent::SetSwiftStrikeStartLocation()
{
	SwiftStrikeStartLocation = GenjiRef->GetActorLocation();
}

void UGenji_SwiftStrikeComponent::SetSwiftStrikeEndLocation()
{
	FHitResult HitResult;
	FVector TraceEndLocation;

	// 진행 방향으로 사거리 길이 만큼 LineTrace 진행
	bool bLineTraceSuccess = GenjiRef->TraceUnderCrosshair(SwiftStrikeDistance, HitResult, TraceEndLocation, ECollisionChannel::ECC_Camera);

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

#if WITH_EDITOR
	// Draw Debug
	DrawDebugSphere(GetWorld(), SwiftStrikeEndLocation, 10.f, 20, FColor::Green, false, 5.f, 0U, 1.f);
	DrawDebugLine(GetWorld(), GenjiRef->GetActorLocation(), SwiftStrikeEndLocation, FColor::Green, false, 5.f, 0U, 1.f);
#endif
}

void UGenji_SwiftStrikeComponent::SwiftStrikeStartSetting()
{
	// 시작점, 도착점 세팅
	SetSwiftStrikeStartLocation();
	SetSwiftStrikeEndLocation();

	bSwiftStrike = true;

	// 쿨초로 캡슐 크기 타임라인 끝나기 전에 실행되면 타임라인 끄기
	if (SwiftStrikeCapsuleSizeTimeline.IsPlaying())
	{
		SwiftStrikeCapsuleSizeTimeline.Stop();
	}

	// 캡슐 사이즈 조정
	GenjiRef->GetCapsuleComponent()->SetCapsuleSize(SwiftStrikeCapsuleSize2D.X, SwiftStrikeCapsuleSize2D.Y);

	// 캡슐 콜리전이 적 캡슐 콜리전을 무시하도록 하기
	GenjiRef->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);

	// 질풍참 동안 Input 무시
	if (AController* Controller = GenjiRef->GetController())
	{
		Controller->SetIgnoreMoveInput(true);
		Controller->SetIgnoreLookInput(true);
	}
		
	// MovementMode Flying 설정
	GenjiRef->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	// 최대 가속도 큰 값 -> Velocity 0에서 5000으로 급가속
	GenjiRef->GetCharacterMovement()->MaxAcceleration = 1000000.f;

	// 질풍참 콜라이더 콜리전 On
	if (SwiftStrikeCollider)
	{
		SwiftStrikeCollider->Activate();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] SwiftStrikeCollider is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("BegiSwiftStrikeStartSettingnPlay"));
	}
}

void UGenji_SwiftStrikeComponent::SwiftStrikeFinishSetting()
{
	if (GenjiRef->GetMesh()->GetAnimInstance()->Montage_IsPlaying(AbilityMontage))
	{
		GenjiRef->GetMesh()->GetAnimInstance()->Montage_Stop(0.f, AbilityMontage);
	}

	bSwiftStrike = false;

	// MovementMode Falling 으로 변경
	GenjiRef->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);

	// 최대 가속도 정상화
	GenjiRef->GetCharacterMovement()->MaxAcceleration = 2048.f;

	// 공중에 있는동안 쌓인 중력가속도 리셋
	GenjiRef->StopMovement();

	// Input 다시 받기
	if (AController* Controller = GenjiRef->GetController())
	{
		Controller->SetIgnoreMoveInput(false);
		Controller->SetIgnoreLookInput(false);
	}
	
	// 캡슐 컴포넌트 크기 리셋
	SwiftStrikeCapsuleSizeTimeline.PlayFromStart();

	// 적 캡슐 반응 다시 Block
	GenjiRef->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);

	FVector zeroVector = FVector::ZeroVector;
	SwiftStrikeStartLocation = zeroVector;
	SwiftStrikeEndLocation = zeroVector;
	SwiftStrikeHitNormalProjection = zeroVector;

	// 질풍참 콜라이더 콜리전 끄기
	if (SwiftStrikeCollider)
	{
		SwiftStrikeCollider->Deactivate();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] SwiftStrikeCollider is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("SwiftStrikeFinishSetting"));
	}
}

void UGenji_SwiftStrikeComponent::SwiftStrikeUpdate(float DeltaTime)
{
	// 현재 엑터의 도착 위치로의 방향벡터
	FVector DirectionVector = SwiftStrikeEndLocation - GenjiRef->GetActorLocation();
	DirectionVector.Normalize();

	// 투영 벡터 더하기
	DirectionVector += SwiftStrikeHitNormalProjection;
	DirectionVector.Normalize();

	// 이동
	GenjiRef->AddMovementInput(DirectionVector, 1.f, true);

	// 투영 벡터는 점차 감소
	SwiftStrikeHitNormalProjection = FMath::VInterpTo(SwiftStrikeHitNormalProjection, FVector::ZeroVector, DeltaTime, HitNormalProjectionInterpSpeed);

	// 목표 위치에 도달하면 몽타주 종료
	if (GenjiRef->GetActorLocation().Equals(SwiftStrikeEndLocation, 20.f))
	{
		GenjiRef->GetMesh()->GetAnimInstance()->Montage_Stop(0.f, AbilityMontage);
	}
}

void UGenji_SwiftStrikeComponent::OnSwiftStrikeComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// 벽에 닿았고, 질풍참 진행중일 경우
	if (Hit.bBlockingHit && bSwiftStrike)
	{
		// 벽의 노말 벡터와 액터의 Right벡터를 내적
		const FVector HitNormal = Hit.Normal;
		const FVector ActorRightVector = GenjiRef->GetActorRightVector();
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

void UGenji_SwiftStrikeComponent::SwiftStrikeCapsuleSizeTimelineSettings()
{
	if (SwiftStrikeCapsuleSizeCurveFloat)
	{
		SwiftStrikeCapsuleSizeTimeline.AddInterpFloat(SwiftStrikeCapsuleSizeCurveFloat, OnSwiftStrikeCapsuleSizeTimelineUpdate);
		SwiftStrikeCapsuleSizeTimeline.SetTimelineFinishedFunc(OnSwiftStrikeCapsuleSizeTimelineFinished);
		SwiftStrikeCapsuleSizeTimeline.SetLooping(false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] SwiftStrikeCapsuleSizeCurveFloat is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("SwiftStrikeCapsuleSizeTimelineSettings"));
	}
}

void UGenji_SwiftStrikeComponent::SwiftStrikeCapsuleSizeTimelineUpdate(float Alpha)
{
	FVector2D CapsuleSize = FMath::Lerp(SwiftStrikeCapsuleSize2D, CapsuleSize2D, Alpha);
	 
	GenjiRef->GetCapsuleComponent()->SetCapsuleSize(CapsuleSize.X, CapsuleSize.Y);
}

void UGenji_SwiftStrikeComponent::SwiftStrikeCapsuleSizeTimelineFinished()
{
	GenjiRef->GetCapsuleComponent()->SetCapsuleSize(CapsuleSize2D.X, CapsuleSize2D.Y);
}

void UGenji_SwiftStrikeComponent::DragonbladeActive()
{
	CooldownReset(); 
}

void UGenji_SwiftStrikeComponent::SwiftStrikeMontageFinished()
{
	FinishAbility();
}
