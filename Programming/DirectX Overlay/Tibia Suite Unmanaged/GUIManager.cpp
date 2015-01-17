#include "stdafx.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "GUIManager.h"

const float g_fOne = 1.0f;
const float g_fZero = 0.0f;
const float g_f64 = 64.0f;

const D3DVERTEXELEMENT9 GUIManager::vElem[] = {
	{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	{ 0, 20, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
	D3DDECL_END()
};

void GUIManager::SetDevice(IDirect3DDevice9 *_device){
	try{
		m_stateBlock.Release();
		m_defaultStateBlock.Release();
		vDecl.Release();
		vShader.Release();
		pShader.Release();
		MessageBoxA(0, "Released", "", 0);
	}
	catch (exception &e){
		MessageBoxA(0, e.what(), "", 0);
	}
	device = _device;
	Init();
}

void GUIManager::Init(){
	device->CreateVertexDeclaration(vElem, &vDecl);
	device->CreateVertexShader((DWORD*)g_vs30_VertexMain, &vShader);
	device->CreatePixelShader((DWORD*)g_ps30_PixelMain, &pShader);
	m_stateBlock = _createStateBlock(D3DSBT_ALL);
	m_defaultStateBlock = _createDefaultStateBlock();
}

void GUIManager::BeginUI() {
	m_stateBlock->Capture();
	m_defaultStateBlock->Apply();
	RECT screenRect = { 0 };
	if (GetSurfaceRect(&screenRect)) {
		D3DVIEWPORT9 viewport = { 0 };
		viewport.Width = screenRect.right - screenRect.left;
		viewport.Height = screenRect.bottom - screenRect.top;
		viewport.MinZ = 0.0f;
		viewport.MaxZ = 0.94f;
		device->SetViewport(&viewport);
	}
}

void GUIManager::EndUI() {
	m_stateBlock->Apply();
}

bool GUIManager::GetSurfaceRect(RECT *pRect) {
	bool success = false;
	if (pRect != nullptr) {
		ZeroMemory(pRect, sizeof(RECT));
		D3DSURFACE_DESC surfaceDesc;
		IDirect3DSurface9 *pRenderTarget = nullptr;
		if (device->GetRenderTarget(0, &pRenderTarget) == D3D_OK) {
			HRESULT hResult = pRenderTarget->GetDesc(&surfaceDesc);
			pRenderTarget->Release();
			if (hResult == D3D_OK) {
				pRect->right = surfaceDesc.Width;
				pRect->bottom = surfaceDesc.Height;
				success = true;
			}
		}
	}
	return success;
}

void GUIManager::OnLostDevice(){
	m_stateBlock.Release();
	m_defaultStateBlock.Release();
}

void GUIManager::OnResetDevice(){
	m_stateBlock = _createStateBlock(D3DSBT_ALL);
	m_defaultStateBlock = _createDefaultStateBlock();
}

void GUIManager::OnRender(){
	BeginUI();
	float diffuseColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	DrawRectangle(100.0f, 100.0f, 100.0f, 100.0f, diffuseColor);
	EndUI();
}

void GUIManager::DrawRectangle(float X, float Y, float width, float height, const float diffuseColor[]){

	UIVertex vertices[] = {
		{ X, Y, 0, 0, 0, 1.0f, 1.0f, 1.0f, 1.0f },
		{ X + width, Y, 0, 1, 0, 1.0f, 1.0f, 1.0f, 1.0f },
		{ X + width, Y + height, 0, 1, 1, 1.0f, 1.0f, 1.0f, 1.0f },
		{ X, Y + height, 0, 0, 1, 1.0f, 1.0f, 1.0f, 1.0f }
	};

	device->SetVertexShader(vShader);
	device->SetPixelShader(pShader);

	D3DVIEWPORT9 pViewport = { 0 };
	device->GetViewport(&pViewport);

	float vertexParams[] = {

		X, Y, static_cast<float>(pViewport.Width), static_cast<float>(pViewport.Height),

		diffuseColor[0], diffuseColor[1], diffuseColor[2], diffuseColor[3]

	};

	float pixelParams[] = {

		width, height, 0.0f, 0.0f

	};

	UINT32 paramsSize = sizeof(vertexParams) / sizeof(float);
	UINT32 pixelParamsSize = sizeof(pixelParams) / sizeof(float);

	device->SetVertexShaderConstantF(0, vertexParams, paramsSize / 4);
	device->SetPixelShaderConstantF(0, pixelParams, pixelParamsSize / 4);

	device->SetVertexDeclaration(vDecl);
	device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, D3DFVF_XYZ, vertices, sizeof(UIVertex));

	device->SetPixelShader(nullptr);
	device->SetVertexShader(nullptr);
	device->SetVertexDeclaration(nullptr);

}

const SStateBlockRecord<D3DRENDERSTATETYPE> GUIManager::m_renderStateTable[] =
{
	{ D3DRS_ZENABLE, FALSE }, // TRUE
	{ D3DRS_FILLMODE, D3DFILL_SOLID },
	{ D3DRS_SHADEMODE, D3DSHADE_GOURAUD },
	{ D3DRS_ZWRITEENABLE, FALSE }, // TRUE
	{ D3DRS_ALPHATESTENABLE, TRUE }, // FALSE
	{ D3DRS_LASTPIXEL, TRUE },
	{ D3DRS_SRCBLEND, D3DBLEND_SRCALPHA }, // D3DBLEND_ONE
	{ D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA }, // D3DBLEND_ZERO
	{ D3DRS_CULLMODE, D3DCULL_NONE }, // D3DCULL_CCW
	{ D3DRS_ZFUNC, D3DCMP_LESSEQUAL },
	{ D3DRS_ALPHAREF, 10 }, // 0
	{ D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL }, // D3DCMP_NONE
	{ D3DRS_DITHERENABLE, FALSE },
	{ D3DRS_ALPHABLENDENABLE, TRUE }, // FALSE
	{ D3DRS_FOGENABLE, FALSE },
	{ D3DRS_SPECULARENABLE, FALSE },
	{ D3DRS_FOGCOLOR, 0 },
	{ D3DRS_FOGTABLEMODE, D3DFOG_NONE },
	{ D3DRS_FOGSTART, *(DWORD*)&g_fZero },
	{ D3DRS_FOGEND, *(DWORD*)&g_fZero },
	{ D3DRS_FOGDENSITY, *(DWORD*)&g_fOne },
	{ D3DRS_RANGEFOGENABLE, FALSE },
	{ D3DRS_STENCILENABLE, FALSE },
	{ D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP },
	{ D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP },
	{ D3DRS_STENCILPASS, D3DSTENCILOP_KEEP },
	{ D3DRS_STENCILFUNC, D3DCMP_ALWAYS },
	{ D3DRS_STENCILREF, 0 },
	{ D3DRS_STENCILMASK, 0xFFFFFFFF },
	{ D3DRS_STENCILWRITEMASK, 0xFFFFFFFF },
	{ D3DRS_TEXTUREFACTOR, 0xFFFFFFFF },
	{ D3DRS_WRAP0, 0 },
	{ D3DRS_WRAP1, 0 },
	{ D3DRS_WRAP2, 0 },
	{ D3DRS_WRAP3, 0 },
	{ D3DRS_WRAP4, 0 },
	{ D3DRS_WRAP5, 0 },
	{ D3DRS_WRAP6, 0 },
	{ D3DRS_WRAP7, 0 },
	{ D3DRS_CLIPPING, FALSE }, // TRUE
	{ D3DRS_LIGHTING, FALSE }, // TRUE
	{ D3DRS_AMBIENT, 0 },
	{ D3DRS_FOGVERTEXMODE, D3DFOG_NONE },
	{ D3DRS_COLORVERTEX, TRUE },
	{ D3DRS_LOCALVIEWER, TRUE },
	{ D3DRS_NORMALIZENORMALS, FALSE },
	{ D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1 },
	{ D3DRS_SPECULARMATERIALSOURCE, D3DMCS_COLOR2 },
	{ D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL },
	{ D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL },
	{ D3DRS_VERTEXBLEND, D3DVBF_DISABLE },
	{ D3DRS_CLIPPLANEENABLE, FALSE },
	{ D3DRS_POINTSIZE, *(DWORD*)&g_fOne },
	{ D3DRS_POINTSIZE_MIN, *(DWORD*)&g_fOne },
	{ D3DRS_POINTSPRITEENABLE, FALSE },
	{ D3DRS_POINTSCALEENABLE, FALSE },
	{ D3DRS_POINTSCALE_A, *(DWORD*)&g_fOne },
	{ D3DRS_POINTSCALE_B, *(DWORD*)&g_fZero },
	{ D3DRS_POINTSCALE_C, *(DWORD*)&g_fZero },
	{ D3DRS_MULTISAMPLEANTIALIAS, FALSE }, // TRUE
	{ D3DRS_MULTISAMPLEMASK, 0xFFFFFFFF },
	{ D3DRS_PATCHEDGESTYLE, D3DPATCHEDGE_DISCRETE },
	{ D3DRS_DEBUGMONITORTOKEN, D3DDMT_ENABLE },
	{ D3DRS_POINTSIZE_MAX, *(DWORD*)&g_f64 },
	{ D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE },
	{ D3DRS_COLORWRITEENABLE, 0x0000000F },
	{ D3DRS_TWEENFACTOR, *(DWORD*)&g_fOne },
	{ D3DRS_BLENDOP, D3DBLENDOP_ADD },
	{ D3DRS_POSITIONDEGREE, D3DDEGREE_CUBIC },
	{ D3DRS_NORMALDEGREE, D3DDEGREE_LINEAR },
	{ D3DRS_SCISSORTESTENABLE, FALSE },
	{ D3DRS_SLOPESCALEDEPTHBIAS, 0 },
	{ D3DRS_ANTIALIASEDLINEENABLE, FALSE },
	{ D3DRS_MINTESSELLATIONLEVEL, *(DWORD*)&g_fOne },
	{ D3DRS_MAXTESSELLATIONLEVEL, *(DWORD*)&g_fOne },
	{ D3DRS_ADAPTIVETESS_X, *(DWORD*)&g_fZero },
	{ D3DRS_ADAPTIVETESS_Y, *(DWORD*)&g_fZero },
	{ D3DRS_ADAPTIVETESS_Z, *(DWORD*)&g_fOne },
	{ D3DRS_ADAPTIVETESS_W, *(DWORD*)&g_fZero },
	{ D3DRS_ENABLEADAPTIVETESSELLATION, FALSE },
	{ D3DRS_TWOSIDEDSTENCILMODE, FALSE },
	{ D3DRS_CCW_STENCILFAIL, D3DSTENCILOP_KEEP },
	{ D3DRS_CCW_STENCILZFAIL, D3DSTENCILOP_KEEP },
	{ D3DRS_CCW_STENCILPASS, D3DSTENCILOP_KEEP },
	{ D3DRS_CCW_STENCILFUNC, D3DCMP_ALWAYS },
	{ D3DRS_COLORWRITEENABLE1, 0x0000000F },
	{ D3DRS_COLORWRITEENABLE2, 0x0000000F },
	{ D3DRS_COLORWRITEENABLE3, 0x0000000F },
	{ D3DRS_BLENDFACTOR, 0xFFFFFFFF },
	{ D3DRS_SRGBWRITEENABLE, 0 },
	{ D3DRS_DEPTHBIAS, 0 },
	{ D3DRS_WRAP8, 0 },
	{ D3DRS_WRAP9, 0 },
	{ D3DRS_WRAP10, 0 },
	{ D3DRS_WRAP11, 0 },
	{ D3DRS_WRAP12, 0 },
	{ D3DRS_WRAP13, 0 },
	{ D3DRS_WRAP14, 0 },
	{ D3DRS_WRAP15, 0 },
	{ D3DRS_SEPARATEALPHABLENDENABLE, FALSE },
	{ D3DRS_SRCBLENDALPHA, D3DBLEND_ONE },
	{ D3DRS_DESTBLENDALPHA, D3DBLEND_ZERO },
	{ D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD }
};

const SStateBlockRecord<D3DSAMPLERSTATETYPE> GUIManager::m_samplerStateTable[] =
{
	{ D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP }, // D3DTADDRESS_WRAP
	{ D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP }, // D3DTADDRESS_WRAP
	{ D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP }, // D3DTADDRESS_WRAP
	{ D3DSAMP_BORDERCOLOR, 0x00000000 },
	{ D3DSAMP_MAGFILTER, D3DTEXF_POINT },
	{ D3DSAMP_MINFILTER, D3DTEXF_POINT },
	{ D3DSAMP_MIPFILTER, D3DTEXF_NONE },
	{ D3DSAMP_MIPMAPLODBIAS, 0 },
	{ D3DSAMP_MAXMIPLEVEL, 0 },
	{ D3DSAMP_MAXANISOTROPY, 1 },
	{ D3DSAMP_SRGBTEXTURE, 0 },
	{ D3DSAMP_ELEMENTINDEX, 0 },
	{ D3DSAMP_DMAPOFFSET, 0 },
};

const SStateBlockRecord<D3DTEXTURESTAGESTATETYPE> GUIManager::m_textureStage1StateTable[] =
{
	{ D3DTSS_COLOROP, D3DTOP_MODULATE },
	{ D3DTSS_COLORARG1, D3DTA_TEXTURE },
	{ D3DTSS_COLORARG2, D3DTA_DIFFUSE }, // D3DTA_CURRENT
	{ D3DTSS_ALPHAOP, D3DTOP_MODULATE }, // D3DTOP_SELECTARG1
	{ D3DTSS_ALPHAARG1, D3DTA_TEXTURE },
	{ D3DTSS_ALPHAARG2, D3DTA_DIFFUSE }, // D3DTA_CURRENT
	{ D3DTSS_BUMPENVMAT00, *(DWORD*)&g_fZero },
	{ D3DTSS_BUMPENVMAT01, *(DWORD*)&g_fZero },
	{ D3DTSS_BUMPENVMAT10, *(DWORD*)&g_fZero },
	{ D3DTSS_BUMPENVMAT11, *(DWORD*)&g_fZero },
	{ D3DTSS_TEXCOORDINDEX, 0 },
	{ D3DTSS_BUMPENVLSCALE, *(DWORD*)&g_fZero },
	{ D3DTSS_BUMPENVLOFFSET, *(DWORD*)&g_fZero },
	{ D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE },
	{ D3DTSS_COLORARG0, D3DTA_CURRENT },
	{ D3DTSS_ALPHAARG0, D3DTA_CURRENT },
	{ D3DTSS_RESULTARG, D3DTA_CURRENT },
	{ D3DTSS_CONSTANT, 0 /* ??? */ },
};

const SStateBlockRecord<D3DTEXTURESTAGESTATETYPE> GUIManager::m_textureStageNStateTable[] =
{
	{ D3DTSS_COLOROP, D3DTOP_DISABLE },
	{ D3DTSS_COLORARG1, D3DTA_TEXTURE },
	{ D3DTSS_COLORARG2, D3DTA_CURRENT },
	{ D3DTSS_ALPHAOP, D3DTOP_DISABLE },
	{ D3DTSS_ALPHAARG1, D3DTA_TEXTURE },
	{ D3DTSS_ALPHAARG2, D3DTA_CURRENT },
	{ D3DTSS_BUMPENVMAT00, *(DWORD*)&g_fZero },
	{ D3DTSS_BUMPENVMAT01, *(DWORD*)&g_fZero },
	{ D3DTSS_BUMPENVMAT10, *(DWORD*)&g_fZero },
	{ D3DTSS_BUMPENVMAT11, *(DWORD*)&g_fZero },
	{ D3DTSS_TEXCOORDINDEX, 0 },
	{ D3DTSS_BUMPENVLSCALE, *(DWORD*)&g_fZero },
	{ D3DTSS_BUMPENVLOFFSET, *(DWORD*)&g_fZero },
	{ D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE },
	{ D3DTSS_COLORARG0, D3DTA_CURRENT },
	{ D3DTSS_ALPHAARG0, D3DTA_CURRENT },
	{ D3DTSS_RESULTARG, D3DTA_CURRENT },
	{ D3DTSS_CONSTANT, 0 },
};

CComPtr<IDirect3DStateBlock9> GUIManager::_createDefaultStateBlock() {
	device->BeginStateBlock();

	// Apply default states
	for (auto &cur : m_renderStateTable)
		device->SetRenderState(cur.ordinal, cur.value);

	for (int i = 0; i < 4; ++i) {
		for (auto &cur : m_samplerStateTable)
			device->SetSamplerState(i, cur.ordinal, cur.value);
	}

	for (auto &cur : m_textureStage1StateTable)
		device->SetTextureStageState(0, cur.ordinal, cur.value);

	for (int i = 1; i < 7; ++i) {
		for (auto &cur : m_textureStageNStateTable)
			device->SetTextureStageState(i, cur.ordinal, cur.value);
	}

	device->SetPixelShader(nullptr);
	device->SetVertexShader(nullptr);
	device->SetVertexDeclaration(nullptr);
	CComPtr<IDirect3DStateBlock9> pStateBlock = nullptr;
	device->EndStateBlock(&pStateBlock);
	return pStateBlock;
}

CComPtr<IDirect3DStateBlock9> GUIManager::_createStateBlock(D3DSTATEBLOCKTYPE type) {
	CComPtr<IDirect3DStateBlock9> pStateBlock = nullptr;
	HRESULT hResult = device->CreateStateBlock(type, &pStateBlock);
	return pStateBlock;
}