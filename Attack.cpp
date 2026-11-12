#include "Attack.h"

namespace {
	// 武器の数値はここだけ触れば調整できる
	const WeaponData kWeapons[(int)WeaponType::kCount] = {
		//  名前     攻撃力 前隙 持続 後隙  幅    高さ
		{ "Sword",   10,    4,   6,   8,   70.0f, 40.0f },
		{ "BigSword",     30,    18,  8,   24,  100.0f, 50.0f },
		{ "Spear",   15,    8,   6,   14,  120.0f, 25.0f },
	};
}

void Attack::SetWeapon(WeaponType type)
{
	if (IsAttacking()) return; // 攻撃中は切り替え不可
	weapon_ = type;
}

void Attack::NextWeapon()
{
	int next = ((int)weapon_ + 1) % (int)WeaponType::kCount;
	SetWeapon((WeaponType)next);
}

void Attack::PrevWeapon()
{
	int count = (int)WeaponType::kCount;
	int prev = ((int)weapon_ + count - 1) % count;
	SetWeapon((WeaponType)prev);
}

void Attack::Start()
{
	if (state_ != State::Idle) return;
	state_ = State::Startup;
	timer_ = 0;
	hasHit_ = false;
}

void Attack::Update()
{
	if (state_ == State::Idle) return;

	const WeaponData& w = GetWeaponData();
	timer_++;

	switch (state_) {
	case State::Startup:
		if (timer_ >= w.startupFrames) { state_ = State::Active; timer_ = 0; }
		break;
	case State::Active:
		if (timer_ >= w.activeFrames) { state_ = State::Recovery; timer_ = 0; }
		break;
	case State::Recovery:
		if (timer_ >= w.recoveryFrames) { state_ = State::Idle; timer_ = 0; }
		break;
	default:
		break;
	}
}

bool Attack::IsAttacking() const { return state_ != State::Idle; }
bool Attack::IsHitboxActive() const { return state_ == State::Active; }
int Attack::GetDamage() const { return GetWeaponData().damage; }

const WeaponData& Attack::GetWeaponData() const
{
	return kWeapons[(int)weapon_];
}

void Attack::GetHitbox(const Vector2& playerPos, float facing, Vector2& outPos, Vector2& outSize) const
{
	const WeaponData& w = GetWeaponData();
	const float playerSize = 50.0f; // main.cppの描画サイズに合わせる

	outSize = { w.width, w.height };
	// facing: 右向き=1.0f, 左向き=-1.0f
	float x = (facing > 0.0f) ? playerPos.x + playerSize : playerPos.x - w.width;
	float y = playerPos.y + (playerSize - w.height) * 0.5f;
	outPos = { x, y };
}
