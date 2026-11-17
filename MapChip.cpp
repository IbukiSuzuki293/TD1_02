#include "MapChip.h"
#include <Novice.h>
#include<fstream>
#include<sstream>
#include<string>

void MapChip::Initialize() {
	textureHandle =
		Novice::LoadTexture(
			"Resources/Images/TileSet.png");
}

void MapChip::LoadMap(const char* fileName) {
	mapData.clear();

	std::ifstream file(fileName);

	if (!file.is_open()) {
		return;
	}

	std::string line;
	//1行づつ読み込みを行う
	while (std::getline(file, line)) {
		std::stringstream ss(line);

		std::string cell;

		std::vector<int> row;

		//カンマ区切りで読み込み
		while (std::getline(ss, cell, ',')) {
			int id = std::stoi(cell);

			//Tiledの空白マスは0
			if (id == 0) {
				row.push_back(-1);
			}
			else {
				//GIDからタイル番号に変換
				row.push_back(-1);
			}
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

			int id = mapData[y][x];
			//空白なら描画しない
			if (id < 0) {
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
				1.0f,0.0f,0.0f,0xFFFFFFFF
			);
		}
		
	}
}