#pragma once
#include "Vector2.h"

enum class WeaponType {
	Sword, // 剣: 軽くて速い
	Axe,   // 斧: 重くて遅い
	Spear, // 槍: 中間で射程が長い
	kCount
};

struct WeaponData {
	const char* name;
	int damage;
	int startupFrames;  // 前隙
	int activeFrames;   // 持続
	int recoveryFrames; // 後隙
	float width;        // 当たり判定の幅
	float height;       // 当たり判定の高さ
};

class Attack
{
public:
	void SetWeapon(WeaponType type);
	void NextWeapon();
	void PrevWeapon();

	void Start();                       // 攻撃開始(攻撃中なら無視)
	void Update();                      // 毎フレーム呼ぶ

	bool IsAttacking() const;           // 前隙〜後隙のどこか
	bool IsHitboxActive() const;        // 当たり判定が出ている間だけtrue
	int GetDamage() const;
	const WeaponData& GetWeaponData() const;

	// プレイヤー位置と向きから当たり判定の矩形を取得
	void GetHitbox(const Vector2& playerPos, float facing, Vector2& outPos, Vector2& outSize) const;

	// 1回の振りで同じ敵に複数回当たらないようにする
	bool HasHit() const { return hasHit_; }
	void MarkHit() { hasHit_ = true; }

private:
	enum class State { Idle, Startup, Active, Recovery };

	WeaponType weapon_ = WeaponType::Sword;
	State state_ = State::Idle;
	int timer_ = 0;
	bool hasHit_ = false;
};

