#pragma once
#include <Novice.h>
#include "Vector2.h"

class Player
{
public:
	Player();
	void Move(const Vector2& delta);
	void Jump();
	void Dodge(float facing);
	void Update();
	void Draw() const;
	Vector2 GetPos() const;
	float GetSize() const { return kSize_; }
	bool IsDodging() const { return isDodging_; }
	
private:
	const float kGroundY_ = 360.0f;
	static constexpr float kSize_ = 64.0f;
	Vector2 pos_{ 640.0f, kGroundY_ };
	float velocityY_ = 0.0f;
	bool isGrounded_ = true;

	bool isDodging_ = false;
	int dodgeTimer_ = 0;
	int dodgeCooldown_ = 0;
	float dodgeDir_ = 1.0f;
	
};


