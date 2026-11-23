#pragma once
#include <Novice.h>
#include "Vector2.h"
#include "Attack.h"
#include "MapChip.h"

class Player
{
public:
	Player();
	void Move(const Vector2& delta);
	void Jump();
	void Dodge(float facing);
	void Update(MapChip& mapChip);
	void Draw() const;
	Vector2 GetPos() const { return pos_; }
	Vector2 GetSize() const { return size_; }
	void GetHitbox(Vector2& outPos, Vector2& outSize) const
	{
		outPos = pos_;
		outSize = size_;
	}
	bool IsDodging() const { return isDodging_; }
private:
	const float kGroundY_ = 360.0f;
	Vector2 size_ = { 64.0f, 64.0f };
	Vector2 pos_{ 640.0f, kGroundY_ };
	float velocityY_ = 0.0f;
	bool isGrounded_ = true;

	bool isDodging_ = false;
	int dodgeTimer_ = 0;
	int dodgeCooldown_ = 0;
	float dodgeDir_ = 1.0f;

	int textureHandle_ = -1;
	int animTimer_ = 0;
	int animFrame_ = 0;
	static constexpr int kAnimInterval_ = 15;
	static constexpr int kSrcWidth_ = 64;
	static constexpr int kSrcHeight_ = 64;
	static constexpr int kMaxAnimFrames_ = 4;
	
	float moveInputX_ = 0.0f;
};


