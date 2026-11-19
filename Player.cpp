#include "Player.h"


Player::Player() : pos_{ 640.0f, kGroundY_ } {}

void Player::Move(const Vector2& delta)
{
	pos_.x += delta.x;
	pos_.y += delta.y;
}

void Player::Jump()
{
	if (isGrounded_) {
		velocityY_ = 18.0f;
		isGrounded_ = false;
	}
}

void Player::Dodge(float facing)
{
	if (isDodging_ || dodgeCooldown_ > 0) {
		return;
	}

	isDodging_ = true;
	dodgeTimer_ = 0;
	dodgeDir_ = (facing >= 0.0f) ? 1.0f : -1.0f;

	if (velocityY_ > 0.0f) {
		velocityY_ = 0.0f;
	}
}

void Player::Update()
{
	// クールタイムのカウントダウン
	if (dodgeCooldown_ > 0) {
		dodgeCooldown_--;
	}

	// 回避中の移動処理
	if (isDodging_) {
		const int kDodgeDuration = 10;                             // 回避にかかるフレーム数
		const float kDodgeDistance = kSize_ * 3.0f;                // プレイヤー3つ分 (100.0f)
		const float kDodgeSpeed = kDodgeDistance / kDodgeDuration; // 1フレームあたりの移動量

		pos_.x += dodgeDir_ * kDodgeSpeed;
		dodgeTimer_++;

		if (dodgeTimer_ >= kDodgeDuration) {
			isDodging_ = false;
			dodgeCooldown_ = 15; // 連続回避を防ぐクールタイム(15フレーム)
		}
	}

	if (!isGrounded_ && !isDodging_) {
		const float kGravity = 0.8f;
		velocityY_ -= kGravity;
		pos_.y += velocityY_;

		if (pos_.y <= kGroundY_) {
			pos_.y = kGroundY_;
			velocityY_ = 0.0f;
			isGrounded_ = true;
		}
	}
}

Vector2 Player::GetPos() const
{
	return pos_;
}


void Player::Draw() const
{
	Novice::DrawBox((int)pos_.x, (int)pos_.y * -1 + 800, (int)kSize_, (int)kSize_, 0.0f, WHITE, kFillModeSolid);
}
