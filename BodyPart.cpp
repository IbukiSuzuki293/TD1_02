#include "BodyPart.h"
#include <vector>
#include <string>
#include <cmath>
#include "Vector2.h"

//胴体 300  200
//左右腕　70 150
//左右足　150 70
//頭　130 80

BodyPart::BodyPart(float lx, float ly, float w, float h) {
	local.x = lx;
	local.y = ly;
    width = w;
    height = h;
}

void BodyPart::AddChild(BodyPart* mini) {
	mini->parent = this;
	minipart.push_back(mini);
}

void BodyPart::Update(float deltaTime) {
    if (parent != nullptr) {
        
        float cosA = cosf(parent->world.angle);
        float sinA = sinf(parent->world.angle);

        world.x = parent->world.x + (local.x * cosA - local.y * sinA);
        world.y = parent->world.y + (local.x * sinA + local.y * cosA);
        world.angle = parent->world.angle + local.angle;
    }
    else {
        
        world = local;
    }

    
    for (BodyPart* child : minipart) {
        child->Update(deltaTime);
    }
}

void BodyPart::Draw() {
    Novice::DrawBox(
        static_cast<int>(world.x), 
        static_cast<int>(world.y), 
        static_cast<int>(width), 
        static_cast<int>(height), 
        world.angle, WHITE, kFillModeSolid
    );
    for (BodyPart* child : minipart) {
        child->Draw();
    }
}
