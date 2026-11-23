#include "Stage.h"
#include<Novice.h>
void Stage::Initialize() {
	//マップチップの初期化
	mapChip.Initialize();

	//最初はチュートリアルを表示
	ChangeStage(StageType::Tutorial);
}

void Stage::ChangeStage(StageType type) {
	currentStage = type;
	switch (currentStage) {
	case StageType::Tutorial:

		//チュートリアル用のCSVを読み込み
		mapChip.LoadMap(
			"./Resources/Maps/Tutorial.csv"
		);
		break;
	case StageType::Boss:
		//ボス戦用のステージCSVを読み込み
		mapChip.LoadMap(
			"./Resources/Maps/Boss.csv"
		);
		break;
	}
}
void Stage::Update(Player& player) {
	player.Update(mapChip);
}
void Stage::Draw() {
	//マップの描画をします
	mapChip.Draw();
}
