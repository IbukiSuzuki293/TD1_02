#pragma once
#include <Novice.h>
#include "Vector2.h"

class Player
{
public:
	Player();
	void Move(const Vector2& delta);
	void Draw() const;
	Vector2 GetPos() const;
	
private:
	Vector2 pos_{0.0f, 0.0f};
};


