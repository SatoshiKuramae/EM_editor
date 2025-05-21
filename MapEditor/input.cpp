//==============================================================
//
// manager.cpp
//
//==============================================================
#include "input.h"

//静的メンバ初期化
LPDIRECTINPUT8 CInput::m_pInput = nullptr;

//コンストラクタ(CInput)
CInput::CInput()
{

}

//デストラクタ(CInput)
CInput::~CInput()
{

}

//初期化
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == nullptr)
	{
		if (FAILED(DirectInput8Create(hInstance, 
			DIRECTINPUT_VERSION, IID_IDirectInput8, 
			(void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

//破棄
void CInput::Uninit()
{
	//入力デバイス（キーボード）の破棄
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();		//キーボードへのアクセス権を放棄
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
	//DirectInputオブジェクトの破棄
	if (m_pInput != nullptr)
	{
		m_pInput->Release();
		m_pInput = nullptr;
	}
}

//CInoutKeyboard実装

//コンストラクタ(CInputKeyboard)
CInputKeyboard::CInputKeyboard()
{

}

//デストラクタ(CInputKeyboard)
CInputKeyboard::~CInputKeyboard()
{

}

//初期化
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	//入力デバイス（キーボード）の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	//データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	//協調モードを設定
	if (FAILED((m_pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	//キーボードへのアクセス権を獲得
	m_pDevice->Unacquire();
	return S_OK;

}

//破棄
void CInputKeyboard::Uninit()
{
	CInput::Uninit();
}

//更新処理
void CInputKeyboard::Update()
{
	BYTE aKeyState[MAX_KEY];
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(m_aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			m_aKeyState[nCntKey] = aKeyState[nCntKey];		//キーボードのプレス情報を保存
		}
	}
	else
	{
		m_pDevice->Acquire();	//キーボードへのアクセス権を獲得
	}
}

//各種入力状態の関数
bool CInputKeyboard::GetKeyboardPress(int nKey)
{
	return ((m_aKeyState[nKey] & 0x80) != 0) ? true : false;
}

bool CInputKeyboard::GetKeyboardTrigger(int nKey)
{
	return ((m_aKeyStateTrigger[nKey] & 0x80) != 0) ? true : false;
}
//================================================================================================================================
//パッド入力処理
//================================================================================================================================
//初期化
HRESULT CInputGamepad::InitJoypad()
{
	//メモリのクリア
	memset(&m_joyKeyState, 0, sizeof(XINPUT_STATE));

	//メモリのクリア
	memset(&m_JoypadMotor, 0, sizeof(XINPUT_VIBRATION));

	//XInputのステートを設定(有効にする)
	XInputEnable(true);

	return S_OK;
}

//終了処理
void CInputGamepad::UninitJoypad()
{
	//XInputのステートを設定(無効にする)
	XInputEnable(false);
}

// スティックの入力を方向パッドフラグに変換
WORD CInputGamepad::GetJoypadStick(SHORT sThumbX, SHORT sThumbY, SHORT sDeadZone)
{
	WORD wButtons = 0;

	if (sThumbY >= sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_UP;
	}
	else if (sThumbY <= -sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_DOWN;
	}

	if (sThumbX <= -sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_LEFT;
	}
	else if (sThumbX >= sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_RIGHT;
	}

	return wButtons;
}

//ジョイパッドの更新処理
void CInputGamepad::UpdateJoypad()
{
	XINPUT_STATE joykeyState; //ジョイパッドの入力情報

	//ジョイパッドの状態を取得
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		WORD Button = joykeyState.Gamepad.wButtons;
		WORD OldButton = m_joyKeyState.Gamepad.wButtons;
		m_joyKeyStateTrigger.Gamepad.wButtons = Button & ~OldButton;    // トリガー処理
		m_ajoyKeyStateRelease.Gamepad.wButtons = ~Button & OldButton;    // リリース処理
		joykeyState.Gamepad.wButtons |= GetJoypadStick(m_joyKeyState.Gamepad.sThumbLX, m_joyKeyState.Gamepad.sThumbLY, JOYPAD_DEADZONE);

		// 現在の時間を取得する
		m_aJoypadCurrentTime.Gamepad.wButtons = timeGetTime();

		if (joykeyState.Gamepad.wButtons && ((m_aJoypadCurrentTime.Gamepad.wButtons - m_aJoypadExecLastTime.Gamepad.wButtons) > SPEED_KEY))
		{
			// 最後に真を返した時間を保存
			m_aJoypadExecLastTime.Gamepad.wButtons = m_aJoypadCurrentTime.Gamepad.wButtons;

			// キーボードのリピート情報を保存
			/*m_joyKeyStateRepeat = joykeyState;*/
		}
		else
		{
			// キーボードのリピート情報を保存
		/*	m_joyKeyStateRepeat.Gamepad.wButtons = 0;*/
		}

		m_joyKeyState = joykeyState;                // プレス処理
	}
}

//ジョイパッドのプレス情報を取得
bool CInputGamepad::GetJoypadPress(JOYKEY key)
{
	return (m_joyKeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//ジョイパッドのトリガー情報を取得
bool CInputGamepad::GetJoypadTrigger(JOYKEY key)
{
	return (m_joyKeyStateTrigger.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//スティックの更新処理
void CInputGamepad::UpdateStick(void)
{
	//キーボードの状態を取得
	if (XInputGetState(0, &m_XInput) == ERROR_SUCCESS)
	{
		for (int nCntStick = 0; nCntStick < STICKTYPE_MAX; nCntStick++)
		{
			float fX, fY; //スティックのX軸、Y軸

			switch (nCntStick)
			{
			case STICKTYPE_LEFT:
				fX = (GetXInputState()->Gamepad.sThumbLX);
				fY = (GetXInputState()->Gamepad.sThumbLY);
				break;
			case STICKTYPE_RIGHT:
				fX = (GetXInputState()->Gamepad.sThumbRX);
				fY = (GetXInputState()->Gamepad.sThumbRY);
				break;
			}

			//角度を取得
			m_stick.afAngle[nCntStick] = FindAngle(D3DXVECTOR3(fX, fY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f)) * -1;

			//スティックの倒し具合を取得
			m_stick.afTplDiameter[nCntStick] = fabsf(fX);

			if (m_stick.afTplDiameter[nCntStick] < fabsf(fY))
			{
				m_stick.afTplDiameter[nCntStick] = fabsf(fY);
			}

			m_stick.afTplDiameter[nCntStick] /= 32768.0f; //倒している状態の初期値

			//方向入力フラグを初期化
			for (int nCntAngle = 0; nCntAngle < STICKANGLE_MAX; nCntAngle++)
			{
				m_bAngle[nCntStick][nCntAngle] = false;
			}
			if (m_stick.afTplDiameter[nCntStick] > 0.1f)
			{
				//角度が四分割で上に位置する時、上フラグを真にする
				if ((m_stick.afAngle[nCntStick] < D3DX_PI * -0.75) || (m_stick.afAngle[nCntStick] > D3DX_PI * 0.75))
				{
					m_bAngle[nCntStick][STICKANGLE_UP] = true;
				}


				//角度が四分割で下に位置する時、下フラグを真にする
				else if ((m_stick.afAngle[nCntStick] > D3DX_PI * -0.25) && (m_stick.afAngle[nCntStick] < D3DX_PI * 0.25))
				{
					m_bAngle[nCntStick][STICKANGLE_DOWN] = true;
				}

				//角度が四分割で左に位置する時、左フラグを真にする
				else if ((m_stick.afAngle[nCntStick] > D3DX_PI * -0.75) && (m_stick.afAngle[nCntStick] < D3DX_PI * -0.25))
				{
					m_bAngle[nCntStick][STICKANGLE_LEFT] = true;
				}

				//角度が四分割で右に位置する時、右フラグを真にする
				else if ((m_stick.afAngle[nCntStick] > D3DX_PI * 0.25) && (m_stick.afAngle[nCntStick] < D3DX_PI * 0.75))
				{
					m_bAngle[nCntStick][STICKANGLE_RIGHT] = true;
				}
			}
			//角度に応じた入力処理
			for (int nCntAngle = 0; nCntAngle < STICKANGLE_MAX; nCntAngle++)
			{
				//スティックのトリガー情報を保存
				m_stick.abAngleTrigger[nCntStick][nCntAngle] = (m_stick.abAnglePress[nCntStick][nCntAngle] ^ m_bAngle[nCntStick][nCntAngle]) & m_bAngle[nCntStick][nCntAngle];

				//スティックのリリース情報を保存
				m_stick.abAngleRelease[nCntStick][nCntAngle] = (m_stick.abAnglePress[nCntStick][nCntAngle] ^ m_bAngle[nCntStick][nCntAngle]) & ~m_bAngle[nCntStick][nCntAngle];

				//現在の時間を取得
				m_aStickCurrentTime[nCntStick][nCntAngle] = timeGetTime();

				if (m_bAngle[nCntStick][nCntAngle] && ((m_aStickCurrentTime[nCntStick][nCntAngle] - m_aStickExecLastTime[nCntStick][nCntAngle]) > SPEED_KEY))
				{
					//最後に真を返した時間を保存
					m_aStickExecLastTime[nCntStick][nCntAngle] = m_aStickCurrentTime[nCntStick][nCntAngle];

					//スティックのリピート情報を保存
					m_stick.abAngleRepeat[nCntStick][nCntAngle] = m_bAngle[nCntStick][nCntAngle];
				}
				else
				{
					//スティックのリピート情報を保存
					m_stick.abAngleRepeat[nCntStick][nCntAngle] = 0;
				}

				//スティックのプレス情報を保存
				m_stick.abAnglePress[nCntStick][nCntAngle] = m_bAngle[nCntStick][nCntAngle];
			}
		}
	}
}

//2点の角度を求める
float CInputGamepad::FindAngle(D3DXVECTOR3 pos, D3DXVECTOR3 TargetPos)
{
	float fAngle; //角度

	fAngle = atan2f(TargetPos.y - pos.y, TargetPos.x - pos.x);

	fAngle -= (D3DX_PI * 0.5f);
	fAngle *= -1.0f;

	return fAngle;
}

// コントローラーの入力情報の取得
XINPUT_STATE* CInputGamepad::GetXInputState(void)
{
	return &m_XInput;
}

// スティックの入力情報を取得
CInputGamepad::STICKINPUT CInputGamepad::GetStick(void)
{
	return m_stick;
}

//コンストラクタ
CInputGamepad::CInputGamepad()
{

}
//デストラクタ
CInputGamepad::~CInputGamepad()
{

}