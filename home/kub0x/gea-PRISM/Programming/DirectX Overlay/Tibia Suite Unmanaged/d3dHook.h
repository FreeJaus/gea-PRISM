#pragma once

#include "APIHook.h"

class d3dHook final : public Singleton<d3dHook> {

	friend class Singleton <d3dHook> ;

private:

	const int WIDTH = 10;
	const int HEIGHT = 10;

	WNDCLASS wClass;
	HWND hWnd;

	typedef HRESULT (CALLBACK* IDirect3D9__CreateDevice)(LPDIRECT3DDEVICE9 Device, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DDevice9 **ppReturnedDeviceInterface);
	typedef HRESULT (CALLBACK* IDirect3DDevice9__Present)(LPDIRECT3DDEVICE9 Device, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion);
	typedef HRESULT (CALLBACK* IDirect3DDevice9__Reset)(LPDIRECT3DDEVICE9 Device, D3DPRESENT_PARAMETERS* dxPresentParams);
	
	static APIHook<IDirect3D9__CreateDevice> *hkCreateDevice;
	static APIHook<IDirect3DDevice9__Present> *hkPresent;
	static APIHook<IDirect3DDevice9__Reset> *hkReset;

	d3dHook() : Singleton<d3dHook>() {}

	HWND CreateDummyWindow();
	void DestroyDummyWindow();

	/* DirectX 9 Functions CallBacks */

	static HRESULT CALLBACK CreateDeviceCallBack(LPDIRECT3DDEVICE9 Device, UINT Adapter, D3DDEVTYPE DeviceType,
		HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DDevice9 **ppReturnedDeviceInterface);

	static HRESULT CALLBACK PresentCallBack(LPDIRECT3DDEVICE9 Device, const RECT *pSourceRect,
		const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion);

	static HRESULT CALLBACK ResetCallBack(LPDIRECT3DDEVICE9 Device, D3DPRESENT_PARAMETERS* pPresentationParameters);
	
public:

	bool Attach();
	bool Detach();

};