#include "Player.h"

namespace
{
	struct AnimData
	{
		int row;
		int frameCount;
		int interval;
		bool loop;
	};

	const AnimData kAnims[] =
	{ 
		//stand
		{0, 4, 15, true},
		//walk
		{1, 4, 15, true},
		//dodge
		{2, 1, 60, true},
		//up
		{3, 1, 15, false},
		//down
		{4, 1, 15, true},
		//sword
		{5, 4, 15, true},
		//bow
		{6, 4, 15, false},
		//pile
		{7, 4, 15, false},
	};
}
Player::Player() : pos_{ 640.0f, kGroundY_ }
{
	textureHandleStand_ = Novice::LoadTexture("./Resources/stand.png");
	textureHandleRun_ = Novice::LoadTexture("./Resources/run.png");
	//textureHandleDodge_ = Novice::LoadTexture("./Resources/dodge.png");
	textureHandleUp_ = Novice::LoadTexture("./Resources/up.png");
	textureHandleDown_ = Novice::LoadTexture("./Resources/down.png");
	//textureHandleSword_ = Novice::LoadTexture("./Resources/sword.png");
	//textureHandleBow_ = Novice::LoadTexture("./Resources/bow.png");
	//textureHandlePile_ = Novice::LoadTexture("./Resources/pile.png");
}

void Player::Move(const Vector2& delta)
{
	moveInputX_ += delta.x;
	pos_.x += delta.x;
	pos_.y += delta.y;
	if (delta.x != 0.0f)
	{
		isMoving_ = true;
	}
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
	if (isDodging_ || dodgeCooldown_ > 0)
	{
		return;
	}

	isDodging_ = true;
	dodgeTimer_ = 0;
	dodgeDir_ = (facing >= 0.0f) ? 1.0f : -1.0f;

	if (velocityY_ > 0.0f) {
		velocityY_ = 0.0f;
	}
}

void Player::SetAttackingMotion(bool isAttacking, WeaponType weaponType)
{
	isAttacking_ = isAttacking;
	weaponType_ = weaponType;
}

void Player::Update(MapChip& mapChip)
{
	

	// クールタイムのカウントダウン
	if (dodgeCooldown_ > 0) {
		dodgeCooldown_--;
	}

	

	// 回避中の移動処理
	//if (isDodging_) {
	//	const int kDodgeDuration = 10;                             // 回避にかかるフレーム数
	//	const float kDodgeDistance = size_.x * 3.0f;                // プレイヤー3つ分
	//	const float kDodgeSpeed = kDodgeDistance / kDodgeDuration; // 1フレームあたりの移動量

	//	pos_.x += dodgeDir_ * kDodgeSpeed;
	//	dodgeTimer_++;

	//	if (dodgeTimer_ >= kDodgeDuration) {
	//		isDodging_ = false;
	//		dodgeCooldown_ = 15; // 連続回避を防ぐクールタイム
	//	}
	//}

	/*if (!isGrounded_ && !isDodging_) {
		const float kGravity = 0.8f;
		velocityY_ -= kGravity;
		pos_.y += velocityY_;

		if (pos_.y <= kGroundY_) {
			pos_.y = kGroundY_;
			velocityY_ = 0.0f;
			isGrounded_ = true;
		}
	}*/
	//壁の当たり判定
	float moveX = moveInputX_;
	moveInputX_ = 0.0f;

	if (isDodging_) {
		const int kDodgeDuration = 10;
		const float kDodgeDistance = size_.x * 3.0f;
		const float kDodgeSpeed =
			kDodgeDistance / kDodgeDuration;

		moveX = dodgeDir_ * kDodgeSpeed;

		dodgeTimer_++;
		if (dodgeTimer_ >= kDodgeDuration) {
			isDodging_ = false;
			dodgeCooldown_ = 15;
		}
	}
	//移動先のx座標
	float nextX = pos_.x + moveX;
	//プレイヤーの上下端
	int screenTop = static_cast<int>(704.0f - pos_.y);
	int screenBottom = screenTop + static_cast<int>(size_.y);
	//右に移動するとき
	if (moveX > 0.0f) {
		int chipX = static_cast<int>((nextX + size_.x - 1) / 64.0f);
		int chipY1 = screenTop / 64;
		int chipY2 = (screenBottom - 1) / 64;
		
		if (mapChip.IsSolid(chipX, chipY1) ||
			mapChip.IsSolid(chipX, chipY2)) {
			//壁の左側で止める
			nextX = chipX * 64.0f - size_.x;
		}
	}
	//左に移動するとき
	if (moveX < 0.0f) {
		int chipX = static_cast<int>(nextX / 64.0f);
		int chipY1 = screenTop / 64;
		int chipY2 = (screenBottom - 1) / 64;

		if (mapChip.IsSolid(chipX, chipY1) ||
			mapChip.IsSolid(chipX, chipY2)) {
			nextX = (chipX + 1) * 64.0f;
		}
	}
	//判定結果を反映
	pos_.x = nextX;
	//Y方向の移動・当たり判定
	if (!isGrounded_ && !isDodging_) {
		const float kGravity = 0.8f;
		velocityY_ -= kGravity;
		//落下後のプレイヤーの位置
		float nextY = pos_.y + velocityY_;
		//移動先の画面上の上下端
		int nextTop =
			static_cast<int>(704.0f - nextY);
		int nextBottom =
			nextTop + static_cast<int>(size_.y);
		int chipX1 = static_cast<int>(pos_.x / 64.0f);
		int chipX2 = static_cast<int>((pos_.x + size_.x - 1) / 64.0f);
		//下に落ちるとき
		if (velocityY_ < 0.0f) {
			//次に接触するマップチップブロックの行番号
			int chipY = (nextBottom - 1) / 64;
			if (mapChip.IsSolid(chipX1, chipY) ||
				mapChip.IsSolid(chipX2, chipY)) {
				//床の上で止める
				nextY = 704.0f + size_.y - chipY * 64.0f;
				velocityY_ = 0.0f;
				isGrounded_ = true;
			}
		}
		//上にジャンプするとき
		else if (velocityY_ > 0.0f) {
			int chipY = nextTop / 64;
			if (mapChip.IsSolid(chipX1, chipY) ||
				mapChip.IsSolid(chipX2, chipY)) {
				nextY = 704.0f - (chipY + 1) * 64.0f;
				velocityY_ = 0.0f;
			}
		}
		//判定後の座標を反映
		pos_.y = nextY;
	}

	
}

void Player::AnimUpdate()
{

	AnimaState next = AnimaState::stand;
	if (isAttacking_)
	{
		switch (weaponType_)
		{
		case WeaponType::Sword: next = AnimaState::sword; break;
		case WeaponType::Bow: next = AnimaState::bow; break;
		case WeaponType::PileBunker: next = AnimaState::pile; break;
		}
	}
	else if (velocityY_ > 0.0f)
	{
		next = AnimaState::up;
	}
	else if (velocityY_ < 0.0f)
	{
		next = AnimaState::down;
	}
	else if (isMoving_)
	{
		next = AnimaState::walk;
	}
	else if (isDodging_)
	{
		next = AnimaState::dodge;
	}

	if (next != animaState_)
	{
		animaState_ = next;
		animTimer_ = 0;
		animFrame_ = 0;
	}
	else
	{
		const AnimData& anim = kAnims[(int)animaState_];
		animTimer_++;
		if (animTimer_ >= kAnimInterval_)
		{
			animTimer_ = 0;
			if (anim.loop)
			{
				animFrame_ = (animFrame_ + 1) % kMaxAnimFrames_;
			}
			else if (animFrame_ < anim.frameCount - 1)
			{
				animFrame_++;
			}
		}
	}
	isMoving_ = false;
}



void Player::Draw(float facing) const
{
	int left = (int)pos_.x;
	int top = (int)pos_.y * -1 + 704;
	int right = left + (int)size_.x;
	int bottom = top + (int)size_.y;

	int x1 = left;
	int x2 = right;
	if (facing < 0.0f)
	{
		x1 = right; x2 = left;
	}

	const AnimData& anim = kAnims[(int)animaState_];


	// 左から64ずつ切り取る位置
	int srcX = animFrame_ * kSrcWidth_;
	int srcY = anim.row * kSrcHeight_;

	int texture = textureHandleStand_;
	switch (animaState_)
	{
	case AnimaState::walk: texture = textureHandleRun_; break;
	case AnimaState::up: texture = textureHandleUp_; break;
	case AnimaState::down: texture = textureHandleDown_; break;
	default:
		break;
	}
	/*else if (isAttacking_)
	{
	}*/
	
	Novice::DrawQuad(
		x1, top,       // 左上
		x2, top,      // 右上
		x1, bottom,    // 左下
		x2, bottom,   // 右下
		srcX, srcY,      // 画像の切り取り開始位置
		kSrcWidth_, kSrcHeight_, // 切り取る幅・高さ
		texture,
		WHITE
	);

}
