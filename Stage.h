#pragma once
#include "MapChip.h"

//ステージの種類の管理
enum class StageType {
	Tutorial,//チュートリアル
	Boss//ボス
};
class Stage{

public:
	//初期化
	void Initialize();

	//ステージの切り替え
	void ChangeStage(StageType type);

	//描画
	void Draw();

private:
	//マップチップ
	MapChip mapChip;

	//現在のステージ
	//注意書きがあったので初期値を宣言しています↓
	StageType currentStage = StageType::Tutorial;

};

