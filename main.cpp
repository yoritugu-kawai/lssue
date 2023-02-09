#include <Novice.h>
#include <math.h>
#include <string>
#include <cmath>
#define KOSTMAX 1000
#define KOSTMAX_2 500

const char kWindowTitle[] = "LC1_11_カワイヨリツグ_タイトル";
/*---プレイヤーの構造体---*/
 struct  obj {
	int x;//Xの座標
	int y;//Yの座標

	float radius;//画像の大きさ
	float speed;//プレイヤーのスピード
	float angle;//プレイヤーの角度
};

 /*---演出の構造体---*/
 struct branch {
	 	double frame;//フレーム
	 	double normal;//通常
 };
 struct Sub {
	 double x;//Xの座標
	 double y;//Yの座標
	 double radius;//画像の大きさ

	 double vessel;//器
	
	 float speed;//演出のスピード
	 double tim;//演出のタイマー
	 double afterimage;//残像
	 double safety;//セーフティー
	 branch start;//スタート
	 branch end;//エンド
 };

 /*---結合する構造体---*/
 struct Player {
	 obj body;//本体
	Sub particle[KOSTMAX];//パーティクル
	Sub easing[KOSTMAX];//イージング
	obj item;//アイテム
};

typedef enum {
	SABER,
	MURAMASA,
	EMIYA,
	FIRE
};

/*---画像の構造体---*/
struct Iamge{
	int item;//アイテム
	int body;//本体
	int particle;//パーティクル
	int easing;//イージング
	int staging; //演出
};
struct Character {
	Iamge saber;
	Iamge muramasa;
	Iamge emiya;
};
/*---初期化タイマーの構造体---*/
struct Tim {
	int rest;//リセット
	int change;//フラグ
};

//キャラクター１のイージング関数↓
double easeInSine(double x) {

	double PI = 3.14;

	return 1 - cosf((x * PI) / 2);
}

//シーン切り替えの上限下限関数 ↓
int clamp(int suteji, int min, int max) {
	if (suteji < min) { return min; }
	if (suteji > max) { return max; }
	return suteji;
}
//キャラクター２のイージング関数↓
double easeInSine4(double x) {

	return x * x * x * x * x;
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);
	/*
	↓順番

	 1.シーン切り替え宣言
	 2.初期値
	 3.SABER初期値
	 4.MURAMASA初期値
	 5.EMIYA初期値
	 6.背景画像
	 7.スタート画面の画像
	 8.キャラクター１(saber)画像
	 9.キャラクター２(muramasa)の画像
	 12.キャラクター3(emiya)の画像
	 13.キャラクター１(saber)イージング
	 14.キャラクター１(saber)パーティクル
	 15.キャラクター2(muramasa)イージング
	 16.キャラクター2(muramasa)イージング
	 17.キャラクター3(emiya)の演出
	 18.キャラクター3(emiya)のパーティクル
	 19.リセットとフラグ
	
	*/

	/*---シーン切り替え宣言---*/
	int Scene = 0;

	/*------初期値----- */
	Player player[4];
	Character iamge[10];
	Tim time[4];

	/*------SABER初期値----- */
	player[SABER].body.x = 1180;
	player[SABER].body.y = 300;
	player[SABER].body.radius = 1;

	player[SABER].item.x = 1210;
	player[SABER].item.y = 350;
	player[SABER].item.angle = 3.2f;
	player[SABER].item.radius = 0.3f;

	/*------MURAMASA初期値----- */
	player[MURAMASA].body.x = 1200;
	player[MURAMASA].body.y = 300;
	player[MURAMASA].body.radius = 1;
	
	/*------EMIYA初期値----- */
	player[EMIYA].body.x  = -10;
	player[EMIYA].body.y  = 50;
	player[EMIYA].body.radius = 1;

	/*---背景画像---*/
	int Background = Novice::LoadTexture("./bak./haikei.png");
	
	/*---スタート画面の画像---*/  
	int up = Novice::LoadTexture("./bak./up.png");
	int down = Novice::LoadTexture("./bak./down.png");

	/*-----キャラクター１(saber)画像 ----*/
	int ActionScene_1 = Novice::LoadTexture("./bak./1.png");
	int KeyImage = Novice::LoadTexture("./bak./ki.png");

	 iamge[0].saber.easing = Novice::LoadTexture("./saber./kariba.png");  
	 iamge[1].saber.easing = Novice::LoadTexture("./saber./kariba2.png");
	 iamge[2].saber.easing = Novice::LoadTexture("./saber./kariba2.5.png");
	 iamge[3].saber.easing = Novice::LoadTexture("./saber./b-.png");

	 iamge[0].saber.staging = Novice::LoadTexture("./saber./si.png");
	 iamge[0].saber.item = Novice::LoadTexture("./saber./ken.png");

	 iamge[0].saber.body = Novice::LoadTexture("./saber./saber.png");

	 iamge[0].saber.particle = Novice::LoadTexture("./saber./kaze.png");
	
	/*-----キャラクター２(muramasa)の画像----*/
	int ActionScene_2 = Novice::LoadTexture("./bak./2.png");

	 iamge[0].muramasa.body= Novice::LoadTexture("./muramasa./muramasa.png");

	 iamge[0].muramasa.item= Novice::LoadTexture("./muramasa./katana.png");
	 iamge[1].muramasa.item= Novice::LoadTexture("./muramasa./tumukari.png");
	
     iamge[0].muramasa.easing = Novice::LoadTexture("./muramasa./sen.png");
     iamge[1].muramasa.easing = Novice::LoadTexture("./muramasa./fia.png");
     iamge[2].muramasa.easing = Novice::LoadTexture("./muramasa./fia2.png");
	
	/*-----キャラクター3(emiya)の画像----*/
	int ActionScene_3 = Novice::LoadTexture("./bak./3.png");

	iamge[0].emiya.body = Novice::LoadTexture("./emiya./emiya.png");
	
	iamge[0].emiya.particle = Novice::LoadTexture("./emiya./kaze.png");
	
	iamge[0].emiya.staging = Novice::LoadTexture("./emiya./aiasu.png");
	iamge[1].emiya.staging = Novice::LoadTexture("./emiya./ro.png");

	/*-----キャラクター１(saber)イージング-----*/

	/*===フレーム==*/
	player[SABER].easing[0].start.frame=100;
	player[SABER].easing[0].end.frame=10;
	player[SABER].easing[0].start.normal=900;
	player[SABER].easing[0].end.normal=0;

	/*===座標==*/
	player[SABER].easing[0].tim = 0;
	player[SABER].easing[0].x=0;
	player[SABER].easing[0].y=300;

	/*===残像とフラグ==*/
	player[SABER].easing[0].safety = 0;
	player[SABER].easing[KOSTMAX_2].vessel;
	player[SABER].easing[KOSTMAX_2].afterimage;

	/*===画像切り替え==*/
	double vesselSaber=0;
	int existingSaber[10]; //存在
	existingSaber[0] = 0; //ビーム
	existingSaber[1] = 0; // ビーム切り替え

	/*-----キャラクター１(saber)パーティクル-----*/
	player[SABER].particle[KOSTMAX].x;
	player[SABER].particle[KOSTMAX].y;
	player[SABER].particle[KOSTMAX].radius;
	player[SABER].particle[KOSTMAX].speed;
	player[SABER].particle[KOSTMAX].safety = { false };

	for (int i = 0; i < KOSTMAX; i++) {

		player[SABER].particle[i].x = 0;
		player[SABER].particle[i].y = 0;
		player[SABER].particle[i].radius = 0.3f;
		player[SABER].particle[i].speed= 0;

	}
	existingSaber[2] = 0;//閃光

	/*-----キャラクター2(muramasa)イージング-----*/
	
	double vesselMuramasa = 0;//器
	/*===フレーム==*/
	player[MURAMASA].easing[0].start.frame = 100;
	player[MURAMASA].easing[0].end.frame = 10;
    player[MURAMASA].easing[0].start.normal = 1100;
	player[MURAMASA].easing[0].end.normal = 0;
   
	/*===座標==*/
	player[MURAMASA].easing[0].tim = 0;
	player[MURAMASA].easing[0].x = 0;
	player[MURAMASA].easing[0].y = 300;

	/*===残像とフラグ==*/
	player[MURAMASA].easing[0].safety = 0;
	player[MURAMASA].easing[500].vessel;
	player[MURAMASA].easing[500].afterimage;

	/*-----キャラクター2(muramasa)イージング2-----*/
	
	double vesselFire = 0;//器
	/*===フレーム==*/
	player[FIRE].easing[0].start.frame = 1;
	player[FIRE].easing[0].end.frame = 40;
	player[FIRE].easing[0].start.normal = 100;
	player[FIRE].easing[0].end.normal = 0;

	/*===座標==*/
	player[FIRE].easing[0].tim = 0;
	player[FIRE].easing[0].x = -100;
	player[FIRE].easing[0].y = 0;

	/*===残像とフラグ==*/
	player[FIRE].easing[0].safety = 0;
	player[FIRE].easing[500].vessel;
	player[FIRE].easing[500].afterimage;

	/*---キャラクター3(emiya)の演出----*/

	/*==円のサイズ==*/
	double emiyaSize_1 = 80.0f;
	double emiyaSize_2 = 90.0f;
	double emiyaSize_3 = 100.0f;

	/*==タイマーカウント==*/
	int staging_count = 1;//演出カウント
	int staging_reset = 0;//演出リセット
	int count_overlapped = 1;//カウント重ね

	int staging_beginning = 0;//演出はじめ

	double petal_angle = 0.0f;//花弁角度

	


	
	/*---キャラクター3(emiya)のパーティクル----*/
	player[EMIYA].particle[KOSTMAX].x;
	player[EMIYA].particle[KOSTMAX].y;
	player[EMIYA].particle[KOSTMAX].radius;
	player[EMIYA].particle[KOSTMAX].speed;
	player[EMIYA].particle[KOSTMAX].safety = { false };
	for (int i = 0; i < KOSTMAX; i++) {

		player[EMIYA].particle[i].x = 0;
		player[EMIYA].particle[i].y = 0;
		player[EMIYA].particle[i].radius = 1.0f;
		player[EMIYA].particle[i].speed = 0;

	}

	/*--リセットとフラグ--*/
	time[SABER].rest = 110;
	time[SABER].change = 0;


	time[MURAMASA].rest = 0;
	time[MURAMASA].change = 0;


	time[FIRE].rest = 31;
	time[FIRE].change = 0;

	time[EMIYA].rest = 400;
	time[EMIYA].change = 0;


	int dontTouch = 1;
	int backTim = 0;
	
	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

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
		if(dontTouch==1) {
			if (keys[DIK_UP] && !preKeys[DIK_UP]) {
				Scene -= 1;

			}
			if (keys[DIK_DOWN] && !preKeys[DIK_DOWN]) {

				Scene += 1;

			}
		}
		Scene = clamp(Scene, 0, 3);
		Novice::ScreenPrintf(0., 0, "%d", Scene);
		
		switch (Scene) {
		case 0:
			break;
		case 1: {
			if (time[SABER].rest >= 110) {
				/*---キャラクター１(saber)イージング----*/
				if (keys[DIK_1] && preKeys[DIK_1] == 0) {
					/*==イージングの変数==*/
					player[SABER].easing[0].safety = 1;
					player[SABER].easing[0].tim = 0;
					player[SABER].easing[0].start.frame = 0;
					for (int i = 0; i < 500; i++) {
						player[SABER].easing[i].afterimage = 0;
					}
					/*==角度の変数==*/
					player[SABER].item.angle = +1.0f;
					/*==タイマーのリセット==*/
					time[SABER].rest = 0;
					backTim = 90;
					/*==フラグ==*/
					existingSaber[0] = 1;
					/*==アップ、ダウンキー無効化==*/
					dontTouch = 0;
				}
			}
			/*==タイマースタート==*/
			if (player[SABER].easing[0].safety == 1) {
				player[SABER].easing[0].start.frame++;
				player[SABER].easing[0].tim++;

			}
			if (player[SABER].easing[0].start.frame == player[SABER].easing[0].end.frame) {
				player[SABER].easing[0].safety = 0;
			}

			/*==動き方==*/
			vesselSaber = player[SABER].easing[0].start.frame / player[SABER].easing[0].end.frame;
			player[SABER].easing[0].x = player[SABER].easing[0].start.normal + (player[SABER].easing[0].end.normal - player[SABER].easing[0].start.normal) * easeInSine(vesselSaber);

			/*==移動処理==*/
			if (player[SABER].easing[0].tim >= 1) {

				player[SABER].easing[0].tim = 0;

				for (int i = 0; i < KOSTMAX_2; i++)
				{
					if (player[SABER].easing[i].afterimage == 0)
					{
						player[SABER].easing[i].vessel = player[SABER].easing[0].x;
						player[SABER].easing[i].afterimage = 1;
						break;
					}

				}
			}




			/*---ここまで１つ目----*/

			/*---キャラクター１(saber)イージング２の処理----*/
			time[SABER].change++;
			if (time[SABER].change >= 10) {
				time[SABER].change = 0;
				existingSaber[1] += 1;

				if (existingSaber[1] == 2) {
					existingSaber[1] = 0;

				}

			}


			/*---キャラクター１が後ろに下がる処理---*/   
			backTim--;
			if (backTim == 0) {
				player[SABER].body.x = 1220;
				player[SABER].item.x = 1250;
				existingSaber[2] = 1;
			}
			/*---キャラクター１パーティクル処理---*/
			for (int i = 0; i < KOSTMAX; i++) {
				if (player[SABER].particle[i].safety== false) {
					player[SABER].particle[i].safety = true;
					player[SABER].particle[i].x = rand() % 60 + 1200;
					player[SABER].particle[i].y = rand() % 100 + 270;
					player[SABER].particle[i].speed= 0;
					break;
				}
			}

			for (int i = 0; i < KOSTMAX; i++) {
				if (player[SABER].particle[i].safety == true) {
					player[SABER].particle[i].y -= player[SABER].particle[i].speed;
					player[SABER].particle[i].speed++;
				}
			}

			for (int i = 0; i < KOSTMAX; i++) {
				if( player[SABER].particle[i].y <= 100) {
					player[SABER].particle[i].safety = false;
				}
			}
			/*---初期処理----*/
			time[SABER].rest++;
			if (time[SABER].rest == 110) {
				player[SABER].item.angle = 3.2f;
				existingSaber[0] = 0;
				player[SABER].body.x=1180;
				player[SABER].item.x=1210;

				dontTouch = 1;
				existingSaber[2] = 0;
			}


		}break;

		case 2: {

			/*----イージングと演出スタート---*/
			if (time[MURAMASA].rest <= 0) {
				if (keys[DIK_2] && preKeys[DIK_2] == 0) {
				
					player[MURAMASA].easing[0].safety = 1;
					player[MURAMASA].easing[0].start.frame = 0;
					for (int i = 0; i < 500; i++) {
						player[MURAMASA].easing[i].vessel = 0;
					}
					time[MURAMASA].change = 1;
					time[MURAMASA].rest = 100;
					/*==アップ、ダウンキー無効化==*/
					dontTouch = 0;
				


						
				}
			}
			/*----イージングスタート---*/
			if (player[MURAMASA].easing[0].safety == 1) {
				player[MURAMASA].easing[0].start.frame++;
				player[MURAMASA].easing[0].tim++;
				

			}
			if (player[MURAMASA].easing[0].start.frame == player[MURAMASA].easing[0].end.frame) {
				player[MURAMASA].easing[0].safety = 0;
			}

			vesselMuramasa = player[MURAMASA].easing[0].start.frame / player[MURAMASA].easing[0].end.frame;
			player[MURAMASA].easing[0].x = player[MURAMASA].easing[0].start.normal + (player[MURAMASA].easing[0].end.normal - player[MURAMASA].easing[0].start.normal) * easeInSine(vesselMuramasa);

			/*---イージング移動処理---*/
			if (player[MURAMASA].easing[0].tim >= 1)
			{
				player[MURAMASA].easing[0].tim = 0;
				for (int i = 0; i < 500; i++)
				{
					if (player[MURAMASA].easing[i].vessel == 0)
					{
						player[MURAMASA].easing[i].afterimage = player[MURAMASA].easing[0].x;
						player[MURAMASA].easing[i].vessel= 1;
						break;
					}

				}

			}
		
				/*---イージングと処理　演出二段目--*/

			if (time[FIRE].rest <= 0) {
				time[FIRE].change = 1;
				player[FIRE].easing[0].safety = 1;
				player[FIRE].easing[0].start.frame = 0;

				for (int i = 0; i < 500; i++) {
					player[FIRE].easing[i].vessel = 0;
				}

				if (player[FIRE].easing[0].safety == 1) {
					player[FIRE].easing[0].start.frame++;
					player[FIRE].easing[0].tim++;

				}
				if (player[FIRE].easing[0].start.frame == player[FIRE].easing[0].end.frame) {
					player[FIRE].easing[0].safety = 0;
				}


			}

			vesselFire = player[FIRE].easing[0].start.frame / player[FIRE].easing[0].end.frame;
			player[FIRE].easing[0].y = player[FIRE].easing[0].start.normal + (player[FIRE].easing[0].end.normal - player[FIRE].easing[0].start.normal) * easeInSine4(vesselFire);

			
			/*---イージング移動処理　演出二段目---*/

			time[FIRE].rest--;

			if (player[FIRE].easing[0].tim == 5)
			{
				player[FIRE].easing[0].tim = 0;
				for (int i = 0; i < 500; i++)
				{
					if (player[FIRE].easing[i].vessel == 0)
					{
						player[FIRE].easing[i].afterimage = player[FIRE].easing[0].y;
						player[FIRE].easing[i].vessel = 1;
						break;
					}

				}

			}
			

			/*------リセット------*/
			time[MURAMASA].rest--;
			if (time[MURAMASA].rest <= 0) {
				time[MURAMASA].change = 0;
				time[FIRE].rest = 32;
				dontTouch = 1;
				time[FIRE].change =0;
			}


		}break;

		case 3: {
			/*---演出スタート--*/
			if (time[EMIYA].rest >= 400) {
				if (keys[DIK_3] && preKeys[DIK_3] == 0) {
					time[EMIYA].change = 1;
					staging_beginning = 1;
					time[EMIYA].rest = 0;
					count_overlapped = 1;
					staging_reset = 0;
					/*==アップ、ダウンキー無効化==*/
					dontTouch = 0;

				}
			}
		
			/*---花弁の回転---*/ 
			petal_angle += 5.0f;



			/*---円の回転処理---*/
			if (staging_beginning == 1) {
				if (staging_count == 1) {
					emiyaSize_1 += 3.0f;
					emiyaSize_2 += 2.0f;
					emiyaSize_3 += 1.0f;
					staging_reset += 1;
				}

				if (staging_count == 2) {
					staging_count = 0;
				}

				if (staging_reset == 150) {
					count_overlapped = 0;
					staging_count = 0;
				}
				if (count_overlapped == 1) {
					staging_count++;
				}
			}

			/*---パーティクル処理---*/  
			for (int i = 0; i < KOSTMAX; i++) {
				if (player[EMIYA].particle[KOSTMAX].safety == false) {
					player[EMIYA].particle[KOSTMAX].safety = true;
					player[EMIYA].particle[i].x = rand() % 6000 + 120;
					player[EMIYA].particle[i].y = rand() % 1000 + 700;
					player[EMIYA].particle[KOSTMAX].speed = 0;
					break;
				}
			}

			for (int i = 0; i < KOSTMAX; i++) {
				if (player[EMIYA].particle[KOSTMAX].safety == true) {
					player[EMIYA].particle[i].y -= player[EMIYA].particle[KOSTMAX].speed;

					player[EMIYA].particle[KOSTMAX].speed++;
				}
			}

			for (int i = 0; i < KOSTMAX; i++) {
				if (player[EMIYA].particle[i].y <= 10) {
					player[EMIYA].particle[KOSTMAX].safety = false;
				}
			}
		
			
			/*---リセット----*/
			time[EMIYA].rest++;
			if (time[EMIYA].rest == 400) {
				emiyaSize_1 = 80.0f;
				emiyaSize_2 = 90.0f;
				emiyaSize_3 = 100.0f;
				time[EMIYA].change = 0;
				staging_beginning = 0;
				dontTouch = 1;
			
			}
			

		}break;

		}
		
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		Novice::DrawSprite(0, 0, Background, 1, 1, 0.0f, 0xffffffff);
		switch (Scene) {
		case 0:
			//画面切り替えの画像↓
			Novice::DrawSprite(500, 200, up, 1, 1, 0.0f, 0xffffffdd);
			Novice::DrawSprite(500, 400, down, 1, 1, 0.0f, 0xffffffdd);
			break;
		case 1: {
			/* --操作画像--*/ 
			Novice::DrawSprite(0, 0, ActionScene_1, 1, 1, 0.0f, 0xffffffdd);
			Novice::DrawSprite(200, 0, KeyImage, 1, 1, 0.0f, 0xffffffdd);

			
			if (existingSaber[0] == 1) {
				/*---イージング--*/
				for (int i = 0; i < KOSTMAX_2; i++) {
					if (player[SABER].easing[i].afterimage == 1) {
						Novice::DrawSprite(player[SABER].easing[i].vessel, player[SABER].easing[0].y, iamge[0].saber.easing, 2, 1, 0.0f, 0xffffffff);
						if (existingSaber[1] == 0) {
							Novice::DrawSprite(player[SABER].easing[i].vessel, player[SABER].easing[0].y, iamge[1].saber.easing, 2, 1, 0.0f, 0xffffffdd);
						}
						if (existingSaber[1] == 1) {
							Novice::DrawSprite(player[SABER].easing[i].vessel-100, player[SABER].easing[0].y-30, iamge[2].saber.easing, 2, 2, 0.0f, 0xffffffdd);

						}


					}
				}
				/*---斬撃--*/
				Novice::DrawSprite(1125, 300, iamge[3].saber.easing, 1, 1, 0.0f, 0xffffffdd);
			}
			/*---パーティクル--*/
			if (existingSaber[0] == 0) {
				for (int i = 0; i < KOSTMAX; i++) {
					if (player[SABER].particle[i].safety == true) {
						Novice::DrawSprite(player[SABER].particle[i].x, player[SABER].particle[i].y, iamge[0].saber.particle, player[SABER].particle[i].radius, player[SABER].particle[i].radius, 0.0f, 0xffffff64);
					}
				}
			}
			/*---キャラクター１の画像とアイテム--*/
			Novice::DrawSprite(player[SABER].body.x, player[SABER].body.y, iamge[0].saber.body, player[SABER].body.radius, player[SABER].body.radius, 0.0f, 0xffffffff);
			Novice::DrawSprite(player[SABER].item.x, player[SABER].item.y, iamge[0].saber.item, player[SABER].item.radius, player[SABER].item.radius, player[SABER].item.angle, 0xffffffdd);
			/*---閃光--*/
			if (existingSaber[2] == 1) {
				Novice::DrawSprite(0, 0, iamge[0].saber.staging, 1, 1, 0.0f, 0xffffffdd);
			}


		}
			
			break;
		case 2: {
			/* --操作画像--*/
			Novice::DrawSprite(0, 0, ActionScene_2, 1, 1, 0.0f, 0xffffffdd);
			Novice::DrawSprite(200, 0, KeyImage, 1, 1, 0.0f, 0xffffffdd);
			
			/*---イージング最初の線の画像-----*/
			if (time[MURAMASA].change == 1) {
				for (int i = 0; i < 500; i++) {
					if (player[MURAMASA].easing[i].vessel == 1) {
						Novice::DrawSprite(player[MURAMASA].easing[i].afterimage, player[MURAMASA].easing[0].y, iamge[0].muramasa.easing, 2, 1, 0.0f, 0xffffffff);
					}
				}
			}
			/*---イージング二段目の炎の画像-----*/
			if (time[FIRE].change == 1) {
				for (int i = 0; i < 500; i++) {
					if (player[FIRE].easing[i].vessel == 1) {
						Novice::DrawSprite(player[FIRE].easing[0].x , player[FIRE].easing[i].afterimage + 120, iamge[1].muramasa.easing, 1.2, 1, 0.0f, 0xffffffff);
						Novice::DrawSprite(player[FIRE].easing[0].x , player[FIRE].easing[i].afterimage, iamge[2].muramasa.easing, 1.2f, 1, 0.0f, 0xffffffff);
					}
				}

			
			}
			/*----武器-----*/
			if (time[MURAMASA].change == 0) {
				Novice::DrawSprite(1225, 350, iamge[0].muramasa.item, 0.3f, 0.3f, 3.2f, 0xffffffdd);
			}
			if (time[MURAMASA].change == 1) {
				Novice::DrawSprite(1230, 330, iamge[1].muramasa.item, 0.3f, 0.3f, 1.2f, 0xffffffdd);
				

			}


				/*--キャラクター2--*/
            Novice::DrawSprite(player[MURAMASA].body.x, player[MURAMASA].body.y, iamge[0].muramasa.body, player[MURAMASA].body.radius, player[MURAMASA].body.radius, 0.0f, 0xffffffff);
			
		}
			
			
			break;
		case 3: {
			/* --操作画像--*/
			Novice::DrawSprite(0, 0, ActionScene_3, 1, 1, 0.0f, 0xffffffdd);
			Novice::DrawSprite(200, 0, KeyImage, 1, 1, 0.0f, 0xffffffdd);
			
			if (time[EMIYA].change == 1) {
				/*----円の画像-----*/
				Novice::DrawEllipse(630, 340, emiyaSize_1, emiyaSize_1, 0.0f, 0xd77bba44, kFillModeSolid);
				Novice::DrawEllipse(630, 340, emiyaSize_2, emiyaSize_2, 0.0f, 0xd77bba44, kFillModeSolid);
				Novice::DrawEllipse(630, 340, emiyaSize_3, emiyaSize_3, 0.0f, 0xd77bba44, kFillModeSolid);

				//パーティクル画像↓
				for (int i = 0; i < KOSTMAX; i++) {
					if (player[EMIYA].particle[KOSTMAX].safety == true) {
						Novice::DrawSprite(player[EMIYA].particle[i].x, player[EMIYA].particle[i].y, iamge[0].emiya.particle, player[EMIYA].particle[KOSTMAX].radius, player[EMIYA].particle[KOSTMAX].radius, petal_angle, 0xffffffff);
					}
				}
				//花弁画像↓
				Novice::DrawSprite(640, 360, iamge[0].emiya.staging, 1, 1, petal_angle, WHITE);

				//中心の画像↓
				Novice::DrawSprite(-10, 50, iamge[1].emiya.staging, 1, 1, 0.0f, WHITE);
			}
			
			
			
			
			
			/*--キャラクター3--*/
           	Novice::DrawSprite(player[EMIYA].body.x, player[EMIYA].body.y, iamge[0].emiya.body, player[EMIYA].body.radius, player[EMIYA].body.radius, 0.0f, WHITE);
			
		}
		
			break; 
		
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
