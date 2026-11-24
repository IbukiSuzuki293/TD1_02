#include "Player.h"


Player::Player() : pos_{ 640.0f, kGroundY_ } 
{
	textureHandle_ = Novice::LoadTexture("./Resources/stand.png");
	textureHandleReverse_ = Novice::LoadTexture("./Resources/stand2.png");
}

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
	animTimer_++;
	if (animTimer_ >= kAnimInterval_)
	{
		animTimer_ = 0;
		animFrame_ = (animFrame_ + 1) % kMaxAnimFrames_;
	}

	// クールタイムのカウントダウン
	if (dodgeCooldown_ > 0) {
		dodgeCooldown_--;
	}

	// 回避中の移動処理
	if (isDodging_) {
		const int kDodgeDuration = 10;                             // 回避にかかるフレーム数
		const float kDodgeDistance = size_.x * 3.0f;                // プレイヤー3つ分
		const float kDodgeSpeed = kDodgeDistance / kDodgeDuration; // 1フレームあたりの移動量

		pos_.x += dodgeDir_ * kDodgeSpeed;
		dodgeTimer_++;

		if (dodgeTimer_ >= kDodgeDuration) {
			isDodging_ = false;
			dodgeCooldown_ = 15; // 連続回避を防ぐクールタイム
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



void Player::Draw(float facing) const
{
	int left = (int)pos_.x;
	int top = (int)pos_.y * -1 + 800;
	int right = left + (int)size_.x;
	int bottom = top + (int)size_.y;

	// 左から64ずつ切り取る位置
	int srcX = animFrame_ * kSrcWidth_;
	int srcY = 0;

	if (facing > 0)
	{
		Novice::DrawQuad(
			left, top,       // 左上
			right, top,      // 右上
			left, bottom,    // 左下
			right, bottom,   // 右下
			srcX, srcY,      // 画像の切り取り開始位置
			kSrcWidth_, kSrcHeight_, // 切り取る幅・高さ
			textureHandle_,
			WHITE
		);
	}
	else
	{
		Novice::DrawQuad(
			left, top,       // 左上
			right, top,      // 右上
			left, bottom,    // 左下
			right, bottom,   // 右下
			srcX, srcY,      // 画像の切り取り開始位置
			kSrcWidth_, kSrcHeight_, // 切り取る幅・高さ
			textureHandleReverse_,
			WHITE
		);
	}
}
