#include "Boss.h"
#include <math.h>

Boss::Boss() {
    // 胴体を親として各パーツを接続
    body.AddChild(&head);
    body.AddChild(&rightArm);
    body.AddChild(&leftArm);
    body.AddChild(&leftLeg);
    body.AddChild(&rightLeg);
}

void Boss::Update(float deltaTime) {
    timer += deltaTime;

    //胴体
    //body.local.x += 10.0f * deltaTime;
    body.local.y += sinf(timer * 1.5f) * 1.5f;
    //頭
    head.local.y = -90.0f + sinf(timer * 2.0f) * 3.0f;

    //右腕
    rightArm.local.y = sinf(timer * 5.0f) * 3.0f;
    //左腕
    leftArm.local.y = sinf(timer * 5.0f) * 3.0f;

    //左足
    leftLeg.local.x = -10.0f;
    //右足
    rightLeg.local.x = 160.0f;

    //全パーツの位置更新
    body.Update(deltaTime);
}

void Boss::Draw() {
    body.Draw();
}
