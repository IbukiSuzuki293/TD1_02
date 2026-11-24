#pragma once
#include <Novice.h>
#include <vector>
#include <string>
#include <math.h>
#include <cmath>
#include "Vector2.h"


class BodyPart {
public:
	//親からの相対座標角度
	Vector2 local = {};
	Vector2 world = {};

	float width = 0.0f;
	float height = 0.0f;

	BodyPart* parent = nullptr;
	std::vector<BodyPart*> minipart;

	BodyPart(float x, float y, float w, float h);
	void AddChild(BodyPart* mini);
	void Update(float deltaTime);
	void Draw();
	
};
