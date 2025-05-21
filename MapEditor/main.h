#ifndef _MAIN_H_
#define _MAIN_H_
#include"d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"
#include <Windows.h>
#include "renderer.h"
#include "json.hpp"


#define MAX_NUM			(256)
#define CLASS_NAME		"�|���S���`��"			//�E�B���h�E�N���X�̖��O
#define WINDOW_NAME		"�|���S���`��"	//�E�B���h�E�̖��O�i�L���v�V�����ɕ\���j


//���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")		//�`�揈���ɕK�v
#pragma	comment(lib,"d3dx9.lib")	//[d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")	//DirectX�R���|�[�l���g�g�p�ɕK�v
#pragma comment(lib,"winmm.lib")	//�V�X�e�������擾���ɕK�v
#pragma comment(lib,"dinput8.lib")	//���͏����ɕK�v

//�}�N����`
#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define FVF_VERTEX_3D	(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)
//RGBA
#define RED (255)
#define GREEN (255)
#define BLUE (255)
#define A (255)

#define NUMOBJECT (10)
#define NUMPRIORITY (10)
#define MAXPOS_X (1280.0f)
#define MAXPOS_Y (720.0f)
#define MINPOS (0.0f)
#define PLAYER_SPEED (15.0f)
#define BLOCK_X (45.0f)
#define BLOCK_Y (45.0f)
#define BLOCKCOLLISION_X (BLOCK_X-10.0f)
#define BLOCKCOLLISION_X (BLOCK_X-10.0f)
#define RESETPOS ((BLOCK_X*2)+0.5f)
#define CAMERAPOS_V_Y (400.0f)
#define CAMERAPOS_V_Z (1200.0f) 
#define NUM_MODEL (15)
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);



#endif




