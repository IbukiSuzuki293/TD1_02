#include <Novice.h>
#include "Player.h"
#include "Boss.h"
#include "Stage.h"
#include "Attack.h"

const char kWindowTitle[] = "LC1B_12_スズキ_イブキ_タイトル";




// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 704);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Player player;
	//ステージ生成
	Stage stage;

	//ステージ初期化
	stage.Initialize();

	Attack attack;
	float facing = 1.0f;

	Boss boss;
	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();
		

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		
		//ボスの倍速　この数値を増やすとボスが機敏になります
		boss.Update(0.02f);

		bool isSpecialStunned = attack.IsSpecialAttacking();
		bool isAttacking = attack.IsAttacking();


		if (!player.IsDodging() && !isSpecialStunned && !isAttacking) {
			if (keys[DIK_A]) { player.Move(Vector2{ -5.0f, 0.0f }); facing = -1.0f; }
			if (keys[DIK_D]) { player.Move(Vector2{ 5.0f, 0.0f }); facing = 1.0f; }
		}

		// 回避(押した瞬間だけ反応させる)
		if (!preKeys[DIK_K] && keys[DIK_K] && !isSpecialStunned && !isAttacking) {
			player.Dodge(facing);
		}
		// ジャンプ(押した瞬間だけ反応させる)
		if (!player.IsDodging() && !isSpecialStunned && !isAttacking && !preKeys[DIK_W] && keys[DIK_W]) {
			player.Jump();
		}
		player.Update();

		// 武器切り替え(押した瞬間だけ反応させる)
		if (!preKeys[DIK_J] && keys[DIK_J]) attack.NextWeapon();

		// 攻撃
		if (!player.IsDodging() && !preKeys[DIK_SPACE] && keys[DIK_SPACE]) attack.Start();

		if (!player.IsDodging() && !preKeys[DIK_P] && keys[DIK_P])
		{
			attack.StartSpecial();
		}

		attack.Update(player.GetPos(), player.GetSize(), facing);

		// 当たり判定(Bossができたらここで矩形同士の判定をする)
		if (attack.IsHitboxActive() && !attack.HasHit()) {
			Vector2 hbPos, hbSize;
			attack.GetHitbox(player.GetPos(), player.GetSize(), facing, hbPos, hbSize);
			// if (ボスと矩形が重なっている) {
			// }
		}

		// 2. 矢の当たり判定
		Arrow* arrows = attack.GetArrows();
		for (int i = 0; i < Attack::kMaxArrows; i++) {
			if (!arrows[i].isActive) continue;
			// if (ボスと矩形が重なっている) {
			// }
		}


		///
		/// ↑更新処理ここまで
		///
		
		///
		/// ↓描画処理ここから
		///
		player.Draw();

		boss.Draw();
		
		//ステージの描画
		
		player.Draw(facing);
		if (attack.IsHitboxActive()) {
			Vector2 hbPos, hbSize;
			attack.GetHitbox(player.GetPos(), player.GetSize(), facing, hbPos, hbSize);

			unsigned int attackColor = RED;
			if (attack.GetWeapon() == WeaponType::Sword) {
				switch (attack.GetComboCount()) {
				case 1: attackColor = RED;   break; 
				case 2: attackColor = GREEN; break; 
				case 3: attackColor = BLUE;  break; 
				}
			}
			else if (attack.GetWeapon() == WeaponType::PileBunker) {
				attackColor = 0xFF00FFFF;           
			}

			Novice::DrawBox((int)hbPos.x, (int)hbPos.y * -1 + 800, (int)hbSize.x, (int)hbSize.y, 0.0f, attackColor, kFillModeSolid);
		}

		const Arrow* constArrows = attack.GetArrows();
		for (int i = 0; i < Attack::kMaxArrows; i++)
		{
			if (!constArrows[i].isActive) continue;
			Novice::DrawBox(
				(int)constArrows[i].pos.x,
				(int)constArrows[i].pos.y * -1 + 800,
				(int)constArrows[i].size.x,
				(int)constArrows[i].size.y,
				0.0f,
				0xFFFF00FF,
				kFillModeSolid
			);
		}

		stage.Draw();

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
