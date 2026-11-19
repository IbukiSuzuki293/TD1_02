#include "Attack.h"
#include <math.h>

namespace {
	const float kPi = 3.14159265f;

	const WeaponData kWeapons[(int)WeaponType::kCount] = {
		//  名前       攻撃力 前隙 持続  後隙      幅    高さ
		{ "Sword",        30,    8,  20,   14,  70.0f,  64.0f },
		{ "Bow",          10,   20,   4,   30,  24.0f,   8.0f },
		{ "PileBunker", 1000,  200,  60,  200, 180.0f,  96.0f },
	};
}

void Attack::SetWeapon(WeaponType type)
{
	if (IsAttacking()) return; // 攻撃中は切り替え不可
	weapon_ = type;
}

void Attack::NextWeapon()
{
	if (IsAttacking()) return;
	// Jキーでは通常武器(Sword と Bow の2種類)のみを切り替える
	const int kNormalWeaponCount = 2;
	int next = ((int)weapon_ + 1) % kNormalWeaponCount;
	SetWeapon((WeaponType)next);
}

void Attack::Start()
{
	// 1. 待機状態からの1段目攻撃
	if (state_ == State::Idle) {
		state_ = State::Startup;
		timer_ = 0;
		hasHit_ = false;
		comboCount_ = (weapon_ == WeaponType::Sword) ? 1 : 0;
		return;
	}

	// 2. 剣のときのみ、後隙(Recovery)中の入力で3コンボ目まで派生
	if (weapon_ == WeaponType::Sword && state_ == State::Recovery && comboCount_ < kMaxCombo) {
		comboCount_++;
		state_ = State::Startup;
		timer_ = 0;
		hasHit_ = false; // 次のコンボでもう一度ヒット判定が出るようにリセット
	}
}

void Attack::StartSpecial()
{
	if (state_ != State::Idle) return;
	prevWeapon_ = weapon_;
	weapon_ = WeaponType::PileBunker;
	state_ = State::Startup;
	timer_ = 0;
	hasHit_ = false;
}

void Attack::SpawnArrows(const Vector2& playerPos, const Vector2& playerSize, float facing)
{
	const WeaponData& w = GetWeaponData();
	const float kArrowSpeed = 14.0f;
	const float kAnglesDeg[3] = { 0.0f, 20.0f, 40.0f };

	// 発射位置（プレイヤーの正面・中央の高さ）
	float x = (facing > 0.0f) ? playerPos.x + playerSize.x : playerPos.x - w.width;
	float y = playerPos.y - (playerSize.y - w.height) * 0.5f;

	for (int angleIdx = 0; angleIdx < 3; angleIdx++) {
		for (int i = 0; i < kMaxArrows; i++) {
			if (!arrows_[i].isActive) {
				float rad = kAnglesDeg[angleIdx] * (kPi / 180.0f);

				arrows_[i].pos = { x, y };
				arrows_[i].vel = {
					cosf(rad) * kArrowSpeed * facing,
					sinf(rad) * kArrowSpeed
				};
				arrows_[i].size = { w.width, w.height };
				arrows_[i].damage = w.damage;
				arrows_[i].isActive = true;
				break;
			}
		}
	}
}


void Attack::Update(const Vector2& playerPos, const Vector2& playerSize, float facing)
{
	// 1. 発射済みの矢の移動・画面外消去処理
	for (int i = 0; i < kMaxArrows; i++) {
		if (!arrows_[i].isActive) continue;

		arrows_[i].pos.x += arrows_[i].vel.x;
		arrows_[i].pos.y += arrows_[i].vel.y;

		if (arrows_[i].pos.x < -100.0f || arrows_[i].pos.x > 1380.0f ||
			arrows_[i].pos.y < 0.0f || arrows_[i].pos.y > 900.0f) {
			arrows_[i].isActive = false;
		}
	}

	if (state_ == State::Idle) return;

	const WeaponData& w = GetWeaponData();
	timer_++;

	switch (state_) {
	case State::Startup:
		if (timer_ >= w.startupFrames) {
			state_ = State::Active;
			timer_ = 0;
			if (weapon_ == WeaponType::Bow) {
				SpawnArrows(playerPos, playerSize, facing);
			}
		}
		break;
	case State::Active:
		if (timer_ >= w.activeFrames) {
			state_ = State::Recovery;
			timer_ = 0;
		}
		break;
	case State::Recovery:
		if (timer_ >= w.recoveryFrames) {
			state_ = State::Idle;
			timer_ = 0;
			comboCount_ = 0; // コンボ終了でリセット
			// 必殺技が終わったら元の武器に戻す
			if (weapon_ == WeaponType::PileBunker) {
				weapon_ = prevWeapon_;
			}
		}
		break;
	default:
		break;
	}
}

bool Attack::IsAttacking() const { return state_ != State::Idle; }

bool Attack::IsSpecialAttacking() const
{
	return state_ != State::Idle && weapon_ == WeaponType::PileBunker;
}

bool Attack::IsHitboxActive() const
{
	// 剣と必殺技のときに近接当たり判定を有効にする
	return (weapon_ == WeaponType::Sword || weapon_ == WeaponType::PileBunker) && state_ == State::Active;
}

int Attack::GetDamage() const { return GetWeaponData().damage; }

const WeaponData& Attack::GetWeaponData() const
{
	return kWeapons[(int)weapon_];
}

void Attack::GetHitbox(const Vector2& playerPos, const Vector2& playerSize, float facing, Vector2& outPos, Vector2& outSize) const
{
	const WeaponData& w = GetWeaponData();

	outSize = { w.width, w.height };
	float x = (facing > 0.0f) ? playerPos.x + playerSize.x : playerPos.x - w.width;
	float y = playerPos.y - (playerSize.y - w.height) * 0.5f;
	outPos = { x, y };
}