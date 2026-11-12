#include <Novice.h>
#include "Player.h"
#include "Attack.h"

const char kWindowTitle[] = "LC1B_12_スズキ_イブキ_タイトル";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Player player;

	Attack attack;
	float facing = 1.0f;

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

		// 更新処理
		if (keys[DIK_A]) { player.Move(Vector2{ -5.0f, 0.0f }); facing = -1.0f; }
		if (keys[DIK_D]) { player.Move(Vector2{ 5.0f, 0.0f }); facing = 1.0f; }

		// 武器切り替え(押した瞬間だけ反応させる)
		if (!preKeys[DIK_Q] && keys[DIK_Q]) attack.PrevWeapon();
		if (!preKeys[DIK_E] && keys[DIK_E]) attack.NextWeapon();

		// 攻撃
		if (!preKeys[DIK_SPACE] && keys[DIK_SPACE]) attack.Start();
		attack.Update();

		// 当たり判定(Bossができたらここで矩形同士の判定をする)
		if (attack.IsHitboxActive() && !attack.HasHit()) {
			Vector2 hbPos, hbSize;
			attack.GetHitbox(player.GetPos(), facing, hbPos, hbSize);
			// if (ボスと矩形が重なっている) {
			//     boss.TakeDamage(attack.GetDamage());
			//     attack.MarkHit(); // 1振り1ヒット
			// }
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		player.Draw();
		// 描画(デバッグ用に当たり判定を赤い箱で表示)
		if (attack.IsHitboxActive()) {
			Vector2 hbPos, hbSize;
			attack.GetHitbox(player.GetPos(), facing, hbPos, hbSize);
			Novice::DrawBox((int)hbPos.x, (int)hbPos.y, (int)hbSize.x, (int)hbSize.y, 0.0f, RED, kFillModeSolid);
		}

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
