```yaml
title: "언리얼4 3인칭 좀비 슈팅게임 포트폴리오"
categories:   - Portfolio
last_modified_at: 2019-08-22T13:00:00+09:00
toc: true
sidebar_main: true
```

---

# 프로젝트 소개

---

![1](https://github.com/lesslate/lesslate.github.io/blob/master/assets/img/Unreal/CPPDamage/1.gif?raw=true)

언리얼 엔진을 이용한 3인칭 슈팅게임 개발

> 사용 엔진

Unreal Engine 4 - 4.19

> 플랫폼

Window 64bit

> 장르

3인칭 슈터



---

# 개발 과정

## 플레이어 캐릭터 구성



### Line Trace를 이용한 공격

```cpp
FHitResult OutHit;
FVector Start = FollowCamera->GetComponentLocation(); // 시작점
FVector ForwardVector = FollowCamera->GetForwardVector();
FVector End = (Start + (ForwardVector*10000.f));  // 끝점
FCollisionQueryParams CollisionParams;
CollisionParams.AddIgnoredActor(this); // 제외할 액터

DrawDebugLine(GetWorld(), Start, End, FColor::Green, true); // 디버그라인 생성

bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);
```

![1](https://github.com/lesslate/lesslate.github.io/blob/master/assets/img/Unreal/LinetraceDamage/1.png?raw=true)

카메라부터 카메라 전방 100미터까지 추적하여 부딪히는 액터 반환



> 실행 결과

![Player](https://github.com/lesslate/lesslate.github.io/blob/master/assets/img/Unreal/LinetraceDamage/GIF.gif?raw=true)



### 데미지 처리

```cpp
if (isHit)
{
    if (OutHit.GetActor())
    {
    DrawDebugSolidBox(GetWorld(), OutHit.ImpactPoint, FVector(10.f), FColor::Blue, true); // 디버그박스
    UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *OutHit.GetActor()->GetName()); // 맞은 액터 이름 출력
    UE_LOG(LogTemp, Log, TEXT("Hit Bone : %s"), *OutHit.BoneName.ToString()); // 맞은 Bone이름 출력
    AActor* HitActor = OutHit.GetActor();
    GameStatic->ApplyPointDamage(HitActor, 50.0f, HitActor->GetActorLocation(), OutHit, nullptr, this,nullptr); // 데미지
    }
}
```

![1](https://github.com/lesslate/lesslate.github.io/blob/master/assets/img/Unreal/Aggro/GIF.gif?raw=true)



### 리로드

![2](https://github.com/lesslate/lesslate.github.io/blob/master/assets/img/cppReload/2.png?raw=true)

재장전 애니메이션 노티파이 추가



```cpp
void AFPSPlayer::ReloadEnd()
{
	UE_LOG(LogTemp, Log, TEXT("ReloadComplete"));
	if ((Magazine - Ammo) > RemainAmmo)
	{
		Ammo = (RemainAmmo + Ammo);
		RemainAmmo = 0;
	}
	else
	{
		RemainAmmo = RemainAmmo - (Magazine - Ammo);
		Ammo = (Magazine - Ammo) + Ammo;
	}
	UE_LOG(LogTemp, Log, TEXT("Current Ammo %d / %d"), Ammo, RemainAmmo);
	IsReloading = false;
}
```

> 실행 결과

![gif](https://github.com/lesslate/lesslate.github.io/blob/master/assets/img/cppReload/GIF.gif?raw=true)

![3](https://github.com/lesslate/lesslate.github.io/blob/master/assets/img/cppReload/3.png?raw=true)



## 적 캐릭터 구성



### 일반 좀비

![2](https://github.com/lesslate/lesslate.github.io/blob/master/assets/img/Unreal/Ai/8.png?raw=true)

비헤이비어 트리를 활용하여 대기-추격-공격을 하는 AI 구현

## 보스 캐릭터 구성

![1](https://github.com/lesslate/lesslate.github.io/blob/master/assets/img/Unreal/BossAI/3.png?raw=true)

더많은 패턴을 사용하는 보스캐릭터 비헤이비어트리 구성



> 실행 결과

![2](https://github.com/lesslate/lesslate.github.io/blob/master/assets/img/Unreal/BossAI/GIF.gif?raw=true)

## 

# 상세 개발과정

[캐릭터 제작](https://lesslate.github.io/unreal4/%EC%96%B8%EB%A6%AC%EC%96%BC4-C++-%EC%BA%90%EB%A6%AD%ED%84%B0-%EC%83%9D%EC%84%B1/)

[점프 애니메이션](https://lesslate.github.io/unreal4/%EC%96%B8%EB%A6%AC%EC%96%BC4-C++-%EC%A0%90%ED%94%84-%EC%95%A0%EB%8B%88%EB%A9%94%EC%9D%B4%EC%85%98/)

[캐릭터 달리기](https://lesslate.github.io/unreal4/%EC%96%B8%EB%A6%AC%EC%96%BC4-C++-%EB%8B%AC%EB%A6%AC%EA%B8%B0/)

[무기 줍기](https://lesslate.github.io/unreal4/%EC%96%B8%EB%A6%AC%EC%96%BC4-%EB%AC%B4%EA%B8%B0-%EC%A4%8D%EA%B8%B0/)

[무기 조준](https://lesslate.github.io/unreal4/%EC%96%B8%EB%A6%AC%EC%96%BC4-Blueprint-%EB%AC%B4%EA%B8%B0-%EC%A1%B0%EC%A4%80/)

[총기 발사](https://lesslate.github.io/unreal4/%EC%96%B8%EB%A6%AC%EC%96%BC4-C++-%EC%B4%9D-%EB%B0%9C%EC%82%AC-%EA%B5%AC%ED%98%84/)

[총구 이펙트](https://lesslate.github.io/unreal4/%EC%96%B8%EB%A6%AC%EC%96%BC4-C++-%EC%B4%9D%EA%B5%AC-%EC%9D%B4%ED%8E%99%ED%8A%B8-(Muzzle-Effect)/)

[데미지 처리및 이펙트](https://lesslate.github.io/unreal4/%EC%96%B8%EB%A6%AC%EC%96%BC4-C++-LineTrace-%EB%8D%B0%EB%AF%B8%EC%A7%80%EC%B2%98%EB%A6%AC/)

[재장전](https://lesslate.github.io/unreal4/%EC%96%B8%EB%A6%AC%EC%96%BC4-C++-%EC%9E%AC%EC%9E%A5%EC%A0%84-(Reload)/)

[좀비 AI](https://lesslate.github.io/unreal4/%EC%96%B8%EB%A6%AC%EC%96%BC4-C++-AI-%EC%BB%A8%ED%8A%B8%EB%A1%A4/)

[데미지 적용](https://lesslate.github.io/unreal4/%EC%96%B8%EB%A6%AC%EC%96%BC4-C++-%EB%8D%B0%EB%AF%B8%EC%A7%80-%EC%A0%81%EC%9A%A9-(Apply-Damage)/)

[좀비 어그로](https://lesslate.github.io/unreal4/%EC%96%B8%EB%A6%AC%EC%96%BC4-C++-%EB%AA%AC%EC%8A%A4%ED%84%B0-%EC%96%B4%EA%B7%B8%EB%A1%9C/)

[데미지 표기 UI](https://lesslate.github.io/unreal4/%EC%96%B8%EB%A6%AC%EC%96%BC4-Blueprint-%EB%8D%B0%EB%AF%B8%EC%A7%80-%ED%91%9C%EA%B8%B0-(Damage-Floating-Text)/)

[탄창 액터 스폰](https://lesslate.github.io/unreal4/%EC%96%B8%EB%A6%AC%EC%96%BC4-C++-%ED%83%84%EC%B0%BD-%EC%95%A1%ED%84%B0-%EC%8A%A4%ED%8F%B0(Spawn-Actor)/)

[범위형 무기및 딜레이](https://lesslate.github.io/unreal4/%EC%96%B8%EB%A6%AC%EC%96%BC4-C++-%EB%B2%94%EC%9C%84%ED%98%95-%EB%AC%B4%EA%B8%B0%EC%99%80-%EB%94%9C%EB%A0%88%EC%9D%B4/)

[델리게이트를 이용한 폭격](https://lesslate.github.io/unreal4/%EC%96%B8%EB%A6%AC%EC%96%BC4-C++-%EB%8D%B8%EB%A6%AC%EA%B2%8C%EC%9D%B4%ED%8A%B8%EB%A5%BC-%EC%9D%B4%EC%9A%A9%ED%95%9C-%EC%83%81%ED%98%B8%EC%9E%91%EC%9A%A9(Interaction-using-delegate)/)

[좀비 액터 숫자 체크](https://lesslate.github.io/unreal4/%EC%96%B8%EB%A6%AC%EC%96%BC4-C++-%EC%95%A1%ED%84%B0-%EC%88%98-%EA%B0%80%EC%A0%B8%EC%98%A4%EA%B8%B0/)

[보스 좀비 스폰](https://lesslate.github.io/unreal4/%EC%96%B8%EB%A6%AC%EC%96%BC4-C++-%EB%B3%B4%EC%8A%A4-%EC%8A%A4%ED%8F%B0/)

[게임 진행 UI](https://lesslate.github.io/unreal4/%EC%96%B8%EB%A6%AC%EC%96%BC4-C++-%EA%B2%8C%EC%9E%84-%EC%A7%84%ED%96%89-UI-%EC%B6%94%EA%B0%80%ED%95%98%EA%B8%B0/)

[보스 좀비 행동패턴 추가](https://lesslate.github.io/unreal4/%EC%96%B8%EB%A6%AC%EC%96%BC4-C++-%EB%AA%AC%EC%8A%A4%ED%84%B0-%ED%96%89%EB%8F%99-%ED%8C%A8%ED%84%B4-%EC%B6%94%EA%B0%80%ED%95%98%EA%B8%B0/)



> 데모 다운로드 링크

https://drive.google.com/open?id=15yaazwQ95s_IqIkbDoJoidC0FLoV1CFX
