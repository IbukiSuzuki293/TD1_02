#include "Player.h"


Player::Player() : pos_{ 640.0f, 360.0f } {}

void Player::Move(const Vector2& delta)
{
	pos_.x += delta.x;
	pos_.y += delta.y;
}

Vector2 Player::GetPos() const
{
	return pos_;
}


void Player::Draw() const
{
	Novice::DrawBox((int)pos_.x, (int)pos_.y * -1 + 500, 50, 50, 0.0f, WHITE, kFillModeSolid);
}
