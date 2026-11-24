#pragma once
#include "Vector2.h"
#include "BodyPart.h"

enum class MODE {
	Idle,
    Telegraph,
    Attack,
    Colldown
};

class Boss
{
public:
    BodyPart body{ 800.0f, 150.0f, 275.0f, 190.0f}; 
    BodyPart head{ 75.0f, -100.0f, 130.0f, 80.0f}; 
    BodyPart rightArm{ 295.0f, 0.0f, 70.0f, 150.0f}; // 胴体の右
    BodyPart leftArm{ -90.0f, 0.0f, 70.0f, 150.0f }; // 胴体の左
    BodyPart leftLeg{ -250.0f, 220.0f, 127.0f, 60.0f }; // 胴体の左
    BodyPart rightLeg{ 325.0f, 220.0f, 127.0f, 60.0f }; // 胴体の右

    float timer = 0.0f;
    
    Boss();

    void Update(float deltaTime);
    void Draw();
};

