#include "stdafx.h"
#include "GUIManager.h"
#include "d3dHook.h"

APIHook<d3dHook::IDirect3D9__CreateDevice> *d3dHook::hkCreateDevice = nullptr;
APIHook<d3dHook::IDirect3DDevice9__Present> *d3dHook::hkPresent = nullptr;
APIHook<d3dHook::IDirect3DDevice9__Reset> *d3dHook::hkReset = nullptr;

HWND d3dHook::CreateDummyWindow(){
	wClass = { 0 };
	hWnd = { 0 };
	wClass.lpszClassName = L"d3d9_init";
	wClass.hInstance = GetModuleHandleW(NULL);
	wClass.lpfnWndProc = DefWindowProcW;
	wClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	if (RegisterClass(&wClass)){
		MessageBoxA(0, "Class Registered", "", 0);
		if (hWnd = CreateWindowW(wClass.lpszClassName, nullptr, 0, 0, 0, WIDTH, HEIGHT, nullptr, nullptr, wClass.hInstance, nullptr))
			MessageBoxA(0, "Window Created", "", 0);
		ShowWindow(hWnd, SW_HIDE);
		UpdateWindow(hWnd);
	}
	return hWnd;
}

void d3dHook::DestroyDummyWindow(){
	if (hWnd){
		DestroyWindow(hWnd);
		UnregisterClassW(wClass.lpszClassName, wClass.hInstance);
	}
}

bool d3dHook::Attach(){
	bool success = false;
	CreateDummyWindow();
	IDirect3D9 *d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d || !hWnd) return success;
	MessageBoxA(0, "IDirect3D9 Created", "", 0);
	IDirect3DDevice9 *device9 = nullptr;
	D3DPRESENT_PARAMETERS params = { 0 };
	params.BackBufferWidth = WIDTH;
	params.BackBufferHeight = HEIGHT;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.BackBufferCount = 1;
	params.hDeviceWindow = hWnd;
	params.Windowed = TRUE;
	params.BackBufferFormat = D3DFMT_UNKNOWN;
	DWORD flags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	if (SUCCEEDED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 0, flags, &params, &device9))){
		MessageBoxA(0, "IDirect3DDevice9 Created", "", 0);
		DWORD_PTR **d3dVTable = reinterpret_cast<DWORD_PTR**>(d3d);
		DWORD_PTR **device9VTable = reinterpret_cast<DWORD_PTR**>(device9);
		hkCreateDevice = new APIHook<IDirect3D9__CreateDevice>(reinterpret_cast<BYTE*>((*d3dVTable)[16]), (BYTE*)d3dHook::CreateDeviceCallBack);
		hkPresent = new APIHook<IDirect3DDevice9__Present>(reinterpret_cast<BYTE*>((*device9VTable)[17]), (BYTE*)d3dHook::PresentCallBack);
		hkReset = new APIHook<IDirect3DDevice9__Reset>(reinterpret_cast<BYTE*>((*device9VTable)[16]), (BYTE*)d3dHook::ResetCallBack);
		device9->Release();
		d3d->Release();
	}
	DestroyDummyWindow();
	return success;
}

bool d3dHook::Detach(){
	bool success = false;

	return success;
}

HRESULT CALLBACK d3dHook::CreateDeviceCallBack(LPDIRECT3DDEVICE9 Device, UINT Adapter, D3DDEVTYPE DeviceType,
	HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DDevice9 **ppReturnedDeviceInterface){
	HRESULT ret = hkCreateDevice->UnHook()(Device, Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
	GUIManager::getInstance()->SetDevice(*ppReturnedDeviceInterface);
	hkCreateDevice->Hook();
	return ret;
}

HRESULT CALLBACK d3dHook::PresentCallBack(LPDIRECT3DDEVICE9 Device, const RECT *pSourceRect,
	const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion){
	if (GUIManager::getInstance()->IsReady())
		GUIManager::getInstance()->OnRender();
	else
		GUIManager::getInstance()->SetDevice(Device);
	HRESULT ret = hkPresent->UnHook()(Device, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	hkPresent->Hook();
	return ret;
}

HRESULT CALLBACK d3dHook::ResetCallBack(LPDIRECT3DDEVICE9 Device, D3DPRESENT_PARAMETERS* pPresentationParameters){
	if (GUIManager::getInstance()->IsReady())
		GUIManager::getInstance()->OnLostDevice();
	HRESULT ret = hkReset->UnHook()(Device, pPresentationParameters);
	if (GUIManager::getInstance()->IsReady())
		GUIManager::getInstance()->OnResetDevice();
	hkReset->Hook();
	return ret;
}