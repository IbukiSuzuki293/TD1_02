#include "MapChip.h"
#include <Novice.h>
#include<fstream>
#include<sstream>
#include<string>

//タイルセットの読み込み
void MapChip::Initialize() {
	textureHandle =
		Novice::LoadTexture(
			"./Resources/Images/TileSet1.png");
}

void MapChip::LoadMap(const char* fileName) {
	mapData.clear();
	mapWidth = 0;
	mapHeight = 0;
	csvOpen = false;

	//前のステージが残らないようにリセット
	std::ifstream file(fileName);

	if (!file.is_open()) {
		
		return;
	}

	std::string line;

	//1行づつ文字列を読み込みを行うファイルの最後まで繰り返す
	while (std::getline(file, line)) {
		std::stringstream ss(line);

		std::string cell;

		std::vector<int> row;

		//カンマ区切りで読み込み
		while (std::getline(ss, cell, ',')) {
			int id = std::stoi(cell);

			row.push_back(id);
		}
			mapData.push_back(row);
		
	}
	file.close();
	//マップサイズを取得
	mapHeight = static_cast<int>(mapData.size());

	if (mapHeight > 0) {
		mapWidth = static_cast<int>(mapData[0].size());
	}
}

void MapChip::Draw() {
	for (int y = 0;y < mapHeight;y++) {
		for (int x = 0; x < mapWidth;x++) {
			//タイルの番号から空白かの判断
			int id = mapData[y][x];
			//空白マス
			if (id == -1) {
				continue;
			}
			//tileSet内の画像位置を計算
			int sourceX =
				(id % TILE_COLUMNS) * CHIP_SIZE;
			int sourceY =
				(id / TILE_COLUMNS) * CHIP_SIZE;
			//画面上の描画位置を計算
			int drawX =
				static_cast<int>(mapPosition.x + x * CHIP_SIZE);
			int drawY =
				static_cast<int>(mapPosition.y + y * CHIP_SIZE);

			//マップチップを描画
			Novice::DrawSpriteRect(
				drawX,drawY,sourceX,sourceY,
				CHIP_SIZE,CHIP_SIZE,
				textureHandle,
				(float)CHIP_SIZE / TILE_IMAGE_WIDTH,
				(float)CHIP_SIZE / TILE_IMAGE_HEIGHT,
				0.0f, 0xFFFFFFFF
			);
			
		}
		
	}
}