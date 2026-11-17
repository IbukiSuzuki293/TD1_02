#pragma once
#include <vector>
#include "Vector2.h"
class MapChip{
public:
	//初期化
	void Initialize();
	//CSVの読み込み
	void LoadMap(const char* fileName);
	//マップ描画
	void Draw();

private:
	//マップデータ
	std::vector<std::vector<int>>mapData;

	//画像ハンドル
	int textureHandle = -1;

	//マップサイズ
	int mapWidth = 0;
	int mapHeight = 0;
	//マップチップのサイズ
	static const int CHIP_SIZE = 32;

	//Tilesetの横方向のチップ数
	static const int TILE_COLUMNS = 8;
	//マップの描画開始位置
	Vector2 mapPosition = { 0.0f,0.0f };
};

