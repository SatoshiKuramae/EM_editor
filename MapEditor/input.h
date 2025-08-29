//============================================================
//
//input.h
//
// AUther : SatoshiKuramae
//============================================================
#ifndef _INPUT_H_
#define _INPUT_H_
#include "main.h"
#include "dinput.h"
#include "d3dx9.h" //描画処理に必要"
#include "Xinput.h"//ジョイパッド
#include "xaudio2.h"
constexpr int MAX_KEY(256);
constexpr int JOYPAD_DEADZONE(1);
constexpr int SPEED_KEY(1);
//ライブラリのリンク
#pragma comment(lib, "d3d9.lib")		//描画処理に必要
#pragma	comment(lib, "d3dx9.lib")	//[d3d9.lib]の拡張ライブラリ
#pragma comment(lib, "dxguid.lib")	//DirectXコンポーネント使用に必要
#pragma comment(lib, "winmm.lib")	//システム時刻取得時に必要
#pragma comment(lib, "dinput8.lib")	//入力処理に必要
#pragma comment(lib,"xinput.lib") //ジョイパッド

//入力基底クラス
class CInput
{
public:
	CInput();
	~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit();
	virtual void Update() = 0;

protected:
	//全入力処理で共有
	static LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 m_pDevice;

};

//キーボード入力
class CInputKeyboard :public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();

	bool GetKeyboardPress(int nKey);
	bool GetKeyboardTrigger(int nKey);

protected:
	BYTE m_aKeyState[MAX_KEY];
	BYTE m_aKeyStateTrigger[MAX_KEY];

};

//パッド入力
class CInputGamepad :public CInput
{
public:
	//キーの種類
	typedef enum
	{
		JOYKEY_UP = 0,
		JOYKEY_DOWN,
		JOYKEY_LEFT,
		JOYKEY_RIGHT,
		JOYKEY_START,
		JOYKEY_BACK,
		JOYKEY_L3,
		JOYKEY_R3,
		JOYKEY_LB,
		JOYKEY_RB,
		JOYKEY_LT,
		JOYKEY_RT,
		JOYKEY_A,
		JOYKEY_B,
		JOYKEY_X,
		JOYKEY_Y,
		JOYKEY_MAX
	}JOYKEY;

	//スティックの番号
	typedef enum
	{
		STICKTYPE_LEFT = 0, // 左スティック
		STICKTYPE_RIGHT, // 右スティック
		STICKTYPE_MAX
	}STICKTYPE;

	//スティックの方向
	typedef enum
	{
		STICKANGLE_UP = 0, // 上
		STICKANGLE_DOWN,   // 下
		STICKANGLE_LEFT,   // 左
		STICKANGLE_RIGHT,  // 右
		STICKANGLE_MAX,
	}STICKANGLE;

	//スティックの入力情報
	typedef struct
	{
		float afTplDiameter[STICKTYPE_MAX];                      // スティックの倒し具合
		float afAngle[STICKTYPE_MAX];                            // スティックの角度
		bool abAnglePress[STICKTYPE_MAX][STICKANGLE_MAX];        // スティックの方向プレス情報
		bool abAngleTrigger[STICKTYPE_MAX][STICKANGLE_MAX];      // スティックの方向トリガー情報
		bool abAngleRepeat[STICKTYPE_MAX][STICKANGLE_MAX];       // スティックの方向リピート情報
		bool abAngleRelease[STICKTYPE_MAX][STICKANGLE_MAX];      // スティックの方向リリース情報
	}STICKINPUT;

	CInputGamepad();
	~CInputGamepad();

	HRESULT InitJoypad(void);
	void UninitJoypad(void);
	void UpdateJoypad(void);
	bool GetJoypadPress(JOYKEY key);
	bool GetJoypadTrigger(JOYKEY key);
	bool GetJoypadRepeat(JOYKEY key);
	XINPUT_STATE* GetXInputState(void);
	void UpdateStick(void);
	STICKINPUT GetStick(void);
	float FindAngle(D3DXVECTOR3 pos, D3DXVECTOR3 TargetPos);
	WORD GetJoypadStick(SHORT sThumbX, SHORT sThumbY, SHORT sDeadZone);
private:
	LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 m_pDevKeyboard;
	BYTE m_aKeyState[MAX_KEY];
	BYTE m_aKeyStateTrigger[MAX_KEY];

	XINPUT_STATE m_joyKeyState;
	XINPUT_STATE m_joyKeyStateTrigger;
	XINPUT_STATE m_ajoyKeyStateRelease;
	XINPUT_STATE m_aJoypadCurrentTime;
	XINPUT_STATE m_aJoypadExecLastTime;
	XINPUT_STATE m_JoypadInput;
	D3DXVECTOR3 m_joyStickPos;
	bool m_bAngle[STICKTYPE_MAX][STICKANGLE_MAX];
	DWORD m_aStickCurrentTime[STICKTYPE_MAX][STICKANGLE_MAX];
	DWORD m_aStickExecLastTime[STICKTYPE_MAX][STICKANGLE_MAX];
	STICKINPUT m_stick;
	XINPUT_STATE m_XInput;
	XINPUT_VIBRATION m_JoypadMotor;
};
#endif
