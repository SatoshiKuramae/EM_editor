//===============================================================================
//
//main.h
//
//Author Kuramaesatoshi
//===============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_
#include"d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"
#include <Windows.h>
#include "renderer.h"
#include "json.hpp"


#define MAX_NUM			(256)
#define CLASS_NAME		"EditorTool"			//ウィンドウクラスの名前
#define WINDOW_NAME		"EditorTool"	//ウィンドウの名前（キャプションに表示）


//ライブラリのリンク
#pragma comment(lib,"d3d9.lib")		//描画処理に必要
#pragma	comment(lib,"d3dx9.lib")	//[d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")	//DirectXコンポーネント使用に必要
#pragma comment(lib,"winmm.lib")	//システム時刻取得時に必要
#pragma comment(lib,"dinput8.lib")	//入力処理に必要

//マクロ定義
constexpr double SCREEN_WIDTH(1280);
constexpr double SCREEN_HEIGHT(720);
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define FVF_VERTEX_3D	(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)
//RGBA
constexpr int RED(255);
constexpr int GREEN(255);
constexpr int BLUE(255);
constexpr int A(255);

#define NUMOBJECT (128)
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




