#pragma once
#include "Header.h"

class CRenderToTexture
{
public:

	Color							m_color;
#ifdef DIRECT_X

	ID3D11Texture2D*				m_renderTargetTexture;
	D3D11_TEXTURE2D_DESC			m_textureDesc;

	ID3D11RenderTargetView*			m_renderTargetView;
	D3D11_RENDER_TARGET_VIEW_DESC	m_renderTargetViewDesc;

	ID3D11ShaderResourceView*		m_shaderResourceView;
	D3D11_SHADER_RESOURCE_VIEW_DESC m_shaderResourceViewDesc;

	bool Initialize(ID3D11Device* device, Size size, Color color);

	void SetRenderTarget(ID3D11DeviceContext* deviceContext);
	void ClearRenderTarget(ID3D11DeviceContext* deviceContext);

#endif

#ifdef OPEN_GL
	unsigned int					m_frameBufferId;
	unsigned int					m_textureId;
	unsigned int					m_dethId;

	Size							m_size;

	bool Initialize(Size size, Color color);
	void Bind(Size size);
	void Unbind();
	void Clear();
#endif

	void Erase();

	CRenderToTexture();
	~CRenderToTexture();
};