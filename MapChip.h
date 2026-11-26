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
	//マップの当たり判定
	bool IsSolid(int x, int y);
private:
	bool csvOpen = false;//読み込めているかのテスト用
	//マップデータ
	std::vector<std::vector<int>>mapData;

	//画像ハンドル
	int textureHandle = -1;

	//マップサイズ
	int mapWidth = 0;
	int mapHeight = 0;
	//マップチップのサイズ(基本64)
	static const int CHIP_SIZE = 64;
	//タイルセットの全体の大きさ
	static const int TILE_IMAGE_WIDTH = 256;//(タイルが増えたら基本ここを変える)
	static const int TILE_IMAGE_HEIGHT = 64;
	//TileSetの種類の数を下に記入(先頭の空白も数に入れる)
	static const int TILE_COLUMNS = 4;
	//マップの描画開始位置
	Vector2 mapPosition = { 0.0f,0.0f };
};

