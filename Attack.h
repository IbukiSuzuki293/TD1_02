#pragma once
#include "Vector2.h"

enum class WeaponType {
	Sword,
	Bow,
	PileBunker,
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

struct Arrow {
	Vector2 pos;
	Vector2 vel;
	Vector2 size;
	int damage;
	bool isActive = false;
};

class Attack
{
public:
	static constexpr int kMaxArrows = 10;

	void SetWeapon(WeaponType type);
	void NextWeapon();
	WeaponType GetWeapon() const { return weapon_; }

	void Start();        // 通常攻撃開始
	void StartSpecial(); // 必殺技開始(Hキー)
	void Update(const Vector2& playerPos, const Vector2& playerSize, float facing);

	bool IsAttacking() const;        // 前隙〜後隙のどこか
	bool IsSpecialAttacking() const; // 必殺技による硬直中かどうか
	bool IsHitboxActive() const;     // 当たり判定が出ている間だけtrue
	int GetDamage() const;
	const WeaponData& GetWeaponData() const;

	// プレイヤー位置と向きから当たり判定の矩形を取得
	void GetHitbox(const Vector2& playerPos, const Vector2& playerSize, float facing, Vector2& outPos, Vector2& outSize) const;

	Arrow* GetArrows() { return arrows_; }
	const Arrow* GetArrows() const { return arrows_; }

	// 1回の振りで同じ敵に複数回当たらないようにする
	bool HasHit() const { return hasHit_; }
	void MarkHit() { hasHit_ = true; }

private:
	void SpawnArrows(const Vector2& playerPos, const Vector2& playerSize, float facing);

	enum class State { Idle, Startup, Active, Recovery };

	WeaponType weapon_ = WeaponType::Sword;
	WeaponType prevWeapon_ = WeaponType::Sword; // 必殺技前に持っていた武器を記憶
	State state_ = State::Idle;
	int timer_ = 0;
	bool hasHit_ = false;

	Arrow arrows_[kMaxArrows];
};