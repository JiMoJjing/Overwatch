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

	// SwiftStrikeStartLocation ����
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

	// ���� �������� ��Ÿ� ���� ��ŭ LineTrace ����
	bool bLineTraceSuccess = PlayerBase->TraceUnderCrosshair(SwiftStrikeDistance, HitResult, TraceEndLocation, ECollisionChannel::ECC_Camera);

	// ���� ������ SwiftStrikeEndLocation �� HitResult�� Location���� ����, ���� �� traceEndLocation���� ����
	if (bLineTraceSuccess)
	{
		// Hit�Ǿ��ٸ� �ش� ��ġ���� ĸ�������ŭ �����ؼ� ��ġ�� ���Ѵ�.
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
	// ������, ������ ����
	SetSwiftStrikeStartLocation();
	SetSwiftStrikeEndLocation();

	if (PlayerBase == nullptr)
	{
		CLog::Log(TEXT("USwiftStrikeComponent SwiftStrikeStartSetting PlayerBase nullptr"));
		return;
	}

	bSwiftStrike = true;

	// ���ʷ� ĸ�� ũ�� Ÿ�Ӷ��� ������ ���� ����Ǹ� Ÿ�Ӷ��� ����
	if (SwiftStrikeCapsuleSizeTimeline.IsPlaying())
	{
		SwiftStrikeCapsuleSizeTimeline.Stop();
	}

	// ĸ�� ������ ����
	PlayerBase->GetCapsuleComponent()->SetCapsuleSize(SwiftStrikeCapsuleSize2D.X, SwiftStrikeCapsuleSize2D.Y);

	// ĸ�� �ݸ����� �� ĸ�� �ݸ����� �����ϵ��� �ϱ�
	PlayerBase->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);

	// ��ǳ�� ���� Input ����
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

	// MovementMode Flying ����
	PlayerBase->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	// �ִ� ���ӵ� ū �� -> Velocity 0���� 5000���� �ް���
	PlayerBase->GetCharacterMovement()->MaxAcceleration = 1000000.f;

	// ��ǳ�� �ݶ��̴� �ݸ��� On
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

	// MovementMode Falling ���� ����
	PlayerBase->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);

	// �ִ� ���ӵ� ����ȭ
	PlayerBase->GetCharacterMovement()->MaxAcceleration = 2048.f;

	// ���߿� �ִµ��� ���� �߷°��ӵ� ����
	PlayerBase->StopMovement();

	// Input �ٽ� �ޱ�
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

	// ĸ�� ������Ʈ ũ�� ����
	SwiftStrikeCapsuleSizeTimeline.PlayFromStart();

	// �� ĸ�� ���� �ٽ� Block
	PlayerBase->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);

	FVector zeroVector = FVector::ZeroVector;
	SwiftStrikeStartLocation = zeroVector;
	SwiftStrikeEndLocation = zeroVector;
	SwiftStrikeHitNormalProjection = zeroVector;

	// ��ǳ�� �ݶ��̴� �ݸ��� ����
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

	// ���� ������ ���� ��ġ���� ���⺤��
	FVector DirectionVector = SwiftStrikeEndLocation - PlayerBase->GetActorLocation();
	DirectionVector.Normalize();

	// ���� ���� ���ϱ�
	DirectionVector += SwiftStrikeHitNormalProjection;
	DirectionVector.Normalize();

	// �̵�
	PlayerBase->AddMovementInput(DirectionVector, 1.f, true);

	// ���� ���ʹ� ���� ����
	SwiftStrikeHitNormalProjection = FMath::VInterpTo(SwiftStrikeHitNormalProjection, FVector::ZeroVector, DeltaTime, HitNormalProjectionInterpSpeed);

	// ��ǥ ��ġ�� �����ϸ� ��Ÿ�� ����
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

	// ���� ��Ұ�, ��ǳ�� �������� ���
	if (Hit.bBlockingHit && bSwiftStrike)
	{
		// ���� �븻 ���Ϳ� ������ Right���͸� ����
		const FVector HitNormal = Hit.Normal;
		const FVector ActorRightVector = PlayerBase->GetActorRightVector();
		const float dot = FVector::DotProduct(ActorRightVector, HitNormal);

		// ���⿡ �°� ���� ���Ͱ� �Է�, ���� ���� ������ ��� �� ���� x
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