//==============================================================
//
// manager.cpp
//
//==============================================================
#include "input.h"

//�ÓI�����o������
LPDIRECTINPUT8 CInput::m_pInput = nullptr;

//�R���X�g���N�^(CInput)
CInput::CInput()
{

}

//�f�X�g���N�^(CInput)
CInput::~CInput()
{

}

//������
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

//�j��
void CInput::Uninit()
{
	//���̓f�o�C�X�i�L�[�{�[�h�j�̔j��
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();		//�L�[�{�[�h�ւ̃A�N�Z�X�������
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
	//DirectInput�I�u�W�F�N�g�̔j��
	if (m_pInput != nullptr)
	{
		m_pInput->Release();
		m_pInput = nullptr;
	}
}

//CInoutKeyboard����

//�R���X�g���N�^(CInputKeyboard)
CInputKeyboard::CInputKeyboard()
{

}

//�f�X�g���N�^(CInputKeyboard)
CInputKeyboard::~CInputKeyboard()
{

}

//������
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	//���̓f�o�C�X�i�L�[�{�[�h�j�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	//�������[�h��ݒ�
	if (FAILED((m_pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	m_pDevice->Unacquire();
	return S_OK;

}

//�j��
void CInputKeyboard::Uninit()
{
	CInput::Uninit();
}

//�X�V����
void CInputKeyboard::Update()
{
	BYTE aKeyState[MAX_KEY];
	int nCntKey;

	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(m_aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			m_aKeyState[nCntKey] = aKeyState[nCntKey];		//�L�[�{�[�h�̃v���X����ۑ�
		}
	}
	else
	{
		m_pDevice->Acquire();	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}

//�e����͏�Ԃ̊֐�
bool CInputKeyboard::GetKeyboardPress(int nKey)
{
	return ((m_aKeyState[nKey] & 0x80) != 0) ? true : false;
}

bool CInputKeyboard::GetKeyboardTrigger(int nKey)
{
	return ((m_aKeyStateTrigger[nKey] & 0x80) != 0) ? true : false;
}
//================================================================================================================================
//�p�b�h���͏���
//================================================================================================================================
//������
HRESULT CInputGamepad::InitJoypad()
{
	//�������̃N���A
	memset(&m_joyKeyState, 0, sizeof(XINPUT_STATE));

	//�������̃N���A
	memset(&m_JoypadMotor, 0, sizeof(XINPUT_VIBRATION));

	//XInput�̃X�e�[�g��ݒ�(�L���ɂ���)
	XInputEnable(true);

	return S_OK;
}

//�I������
void CInputGamepad::UninitJoypad()
{
	//XInput�̃X�e�[�g��ݒ�(�����ɂ���)
	XInputEnable(false);
}

// �X�e�B�b�N�̓��͂�����p�b�h�t���O�ɕϊ�
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

//�W���C�p�b�h�̍X�V����
void CInputGamepad::UpdateJoypad()
{
	XINPUT_STATE joykeyState; //�W���C�p�b�h�̓��͏��

	//�W���C�p�b�h�̏�Ԃ��擾
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		WORD Button = joykeyState.Gamepad.wButtons;
		WORD OldButton = m_joyKeyState.Gamepad.wButtons;
		m_joyKeyStateTrigger.Gamepad.wButtons = Button & ~OldButton;    // �g���K�[����
		m_ajoyKeyStateRelease.Gamepad.wButtons = ~Button & OldButton;    // �����[�X����
		joykeyState.Gamepad.wButtons |= GetJoypadStick(m_joyKeyState.Gamepad.sThumbLX, m_joyKeyState.Gamepad.sThumbLY, JOYPAD_DEADZONE);

		// ���݂̎��Ԃ��擾����
		m_aJoypadCurrentTime.Gamepad.wButtons = timeGetTime();

		if (joykeyState.Gamepad.wButtons && ((m_aJoypadCurrentTime.Gamepad.wButtons - m_aJoypadExecLastTime.Gamepad.wButtons) > SPEED_KEY))
		{
			// �Ō�ɐ^��Ԃ������Ԃ�ۑ�
			m_aJoypadExecLastTime.Gamepad.wButtons = m_aJoypadCurrentTime.Gamepad.wButtons;

			// �L�[�{�[�h�̃��s�[�g����ۑ�
			/*m_joyKeyStateRepeat = joykeyState;*/
		}
		else
		{
			// �L�[�{�[�h�̃��s�[�g����ۑ�
		/*	m_joyKeyStateRepeat.Gamepad.wButtons = 0;*/
		}

		m_joyKeyState = joykeyState;                // �v���X����
	}
}

//�W���C�p�b�h�̃v���X�����擾
bool CInputGamepad::GetJoypadPress(JOYKEY key)
{
	return (m_joyKeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//�W���C�p�b�h�̃g���K�[�����擾
bool CInputGamepad::GetJoypadTrigger(JOYKEY key)
{
	return (m_joyKeyStateTrigger.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//�X�e�B�b�N�̍X�V����
void CInputGamepad::UpdateStick(void)
{
	//�L�[�{�[�h�̏�Ԃ��擾
	if (XInputGetState(0, &m_XInput) == ERROR_SUCCESS)
	{
		for (int nCntStick = 0; nCntStick < STICKTYPE_MAX; nCntStick++)
		{
			float fX, fY; //�X�e�B�b�N��X���AY��

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

			//�p�x���擾
			m_stick.afAngle[nCntStick] = FindAngle(D3DXVECTOR3(fX, fY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f)) * -1;

			//�X�e�B�b�N�̓|������擾
			m_stick.afTplDiameter[nCntStick] = fabsf(fX);

			if (m_stick.afTplDiameter[nCntStick] < fabsf(fY))
			{
				m_stick.afTplDiameter[nCntStick] = fabsf(fY);
			}

			m_stick.afTplDiameter[nCntStick] /= 32768.0f; //�|���Ă����Ԃ̏����l

			//�������̓t���O��������
			for (int nCntAngle = 0; nCntAngle < STICKANGLE_MAX; nCntAngle++)
			{
				m_bAngle[nCntStick][nCntAngle] = false;
			}
			if (m_stick.afTplDiameter[nCntStick] > 0.1f)
			{
				//�p�x���l�����ŏ�Ɉʒu���鎞�A��t���O��^�ɂ���
				if ((m_stick.afAngle[nCntStick] < D3DX_PI * -0.75) || (m_stick.afAngle[nCntStick] > D3DX_PI * 0.75))
				{
					m_bAngle[nCntStick][STICKANGLE_UP] = true;
				}


				//�p�x���l�����ŉ��Ɉʒu���鎞�A���t���O��^�ɂ���
				else if ((m_stick.afAngle[nCntStick] > D3DX_PI * -0.25) && (m_stick.afAngle[nCntStick] < D3DX_PI * 0.25))
				{
					m_bAngle[nCntStick][STICKANGLE_DOWN] = true;
				}

				//�p�x���l�����ō��Ɉʒu���鎞�A���t���O��^�ɂ���
				else if ((m_stick.afAngle[nCntStick] > D3DX_PI * -0.75) && (m_stick.afAngle[nCntStick] < D3DX_PI * -0.25))
				{
					m_bAngle[nCntStick][STICKANGLE_LEFT] = true;
				}

				//�p�x���l�����ŉE�Ɉʒu���鎞�A�E�t���O��^�ɂ���
				else if ((m_stick.afAngle[nCntStick] > D3DX_PI * 0.25) && (m_stick.afAngle[nCntStick] < D3DX_PI * 0.75))
				{
					m_bAngle[nCntStick][STICKANGLE_RIGHT] = true;
				}
			}
			//�p�x�ɉ��������͏���
			for (int nCntAngle = 0; nCntAngle < STICKANGLE_MAX; nCntAngle++)
			{
				//�X�e�B�b�N�̃g���K�[����ۑ�
				m_stick.abAngleTrigger[nCntStick][nCntAngle] = (m_stick.abAnglePress[nCntStick][nCntAngle] ^ m_bAngle[nCntStick][nCntAngle]) & m_bAngle[nCntStick][nCntAngle];

				//�X�e�B�b�N�̃����[�X����ۑ�
				m_stick.abAngleRelease[nCntStick][nCntAngle] = (m_stick.abAnglePress[nCntStick][nCntAngle] ^ m_bAngle[nCntStick][nCntAngle]) & ~m_bAngle[nCntStick][nCntAngle];

				//���݂̎��Ԃ��擾
				m_aStickCurrentTime[nCntStick][nCntAngle] = timeGetTime();

				if (m_bAngle[nCntStick][nCntAngle] && ((m_aStickCurrentTime[nCntStick][nCntAngle] - m_aStickExecLastTime[nCntStick][nCntAngle]) > SPEED_KEY))
				{
					//�Ō�ɐ^��Ԃ������Ԃ�ۑ�
					m_aStickExecLastTime[nCntStick][nCntAngle] = m_aStickCurrentTime[nCntStick][nCntAngle];

					//�X�e�B�b�N�̃��s�[�g����ۑ�
					m_stick.abAngleRepeat[nCntStick][nCntAngle] = m_bAngle[nCntStick][nCntAngle];
				}
				else
				{
					//�X�e�B�b�N�̃��s�[�g����ۑ�
					m_stick.abAngleRepeat[nCntStick][nCntAngle] = 0;
				}

				//�X�e�B�b�N�̃v���X����ۑ�
				m_stick.abAnglePress[nCntStick][nCntAngle] = m_bAngle[nCntStick][nCntAngle];
			}
		}
	}
}

//2�_�̊p�x�����߂�
float CInputGamepad::FindAngle(D3DXVECTOR3 pos, D3DXVECTOR3 TargetPos)
{
	float fAngle; //�p�x

	fAngle = atan2f(TargetPos.y - pos.y, TargetPos.x - pos.x);

	fAngle -= (D3DX_PI * 0.5f);
	fAngle *= -1.0f;

	return fAngle;
}

// �R���g���[���[�̓��͏��̎擾
XINPUT_STATE* CInputGamepad::GetXInputState(void)
{
	return &m_XInput;
}

// �X�e�B�b�N�̓��͏����擾
CInputGamepad::STICKINPUT CInputGamepad::GetStick(void)
{
	return m_stick;
}

//�R���X�g���N�^
CInputGamepad::CInputGamepad()
{

}
//�f�X�g���N�^
CInputGamepad::~CInputGamepad()
{

}