#pragma once

template <typename T>
struct SStateBlockRecord {
	T ordinal;
	DWORD value;
};

class GUIManager final : public Singleton<GUIManager> {

	friend class Singleton<GUIManager>;

private:

	struct UIVertex {
		float x, y, z, u, v;
		D3DCOLORVALUE color;
	};

	static const D3DVERTEXELEMENT9 vElem[];

	static const SStateBlockRecord<D3DRENDERSTATETYPE> m_renderStateTable[];
	static const SStateBlockRecord<D3DSAMPLERSTATETYPE> m_samplerStateTable[];
	static const SStateBlockRecord<D3DTEXTURESTAGESTATETYPE> m_textureStage1StateTable[];
	static const SStateBlockRecord<D3DTEXTURESTAGESTATETYPE> m_textureStageNStateTable[];

	CComPtr<IDirect3DDevice9> device;
	CComPtr<IDirect3DVertexDeclaration9> vDecl;
	CComPtr<IDirect3DVertexShader9> vShader;
	CComPtr<IDirect3DPixelShader9> pShader;

	CComPtr<IDirect3DStateBlock9> m_stateBlock;
	CComPtr<IDirect3DStateBlock9> m_defaultStateBlock;

	GUIManager() : Singleton<GUIManager>() {}

	void Init();
	void BeginUI();
	void EndUI();
	bool GetSurfaceRect(RECT *pRect);

	CComPtr<IDirect3DStateBlock9> _createStateBlock(D3DSTATEBLOCKTYPE type);
	CComPtr<IDirect3DStateBlock9> _createDefaultStateBlock();

public:

	void SetDevice(IDirect3DDevice9 *_device);
	IDirect3DDevice9* GetDevice() { return device; }
	bool IsReady() { return device != nullptr; }
	void OnLostDevice();
	void OnResetDevice();
	void OnRender();

	void DrawRectangle(float X, float Y, float width, float height, const float diffuseColor[]);

};