#include "../Header/CRenderToTexture.h"

CRenderToTexture::CRenderToTexture()
{
#ifdef DIRECT_X
	m_renderTargetTexture = 0;
	m_renderTargetView = 0;
	m_shaderResourceView = 0;
#endif

#ifdef OPEN_GL
	m_frameBufferId = 0;
	m_textureId = 0;
#endif
}

CRenderToTexture::~CRenderToTexture()
{
	Erase();
}

void CRenderToTexture::Erase()
{
#ifdef DIRECT_X
	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = nullptr;
	}

	if (m_renderTargetTexture)
	{
		m_renderTargetTexture->Release();
		m_renderTargetTexture = nullptr;
	}
#endif
}

#ifdef OPEN_GL
bool CRenderToTexture::Initialize(Size size, Color color)
{
	if (m_frameBufferId == 0 && m_dethId == 0 && m_textureId == 0)
	{
		glGenFramebuffers(1, &m_frameBufferId);
		glGenRenderbuffers(1, &m_dethId);
		glGenTextures(1, &m_textureId);

		m_color = color;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.width, size.height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindRenderbuffer(GL_RENDERBUFFER, m_dethId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.width, size.height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_dethId);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_textureId, 0);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	else
	{
		return true;
	}
}

void CRenderToTexture::Bind(Size size)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
	glViewport(0, 0, size.width, size.height);
}

void CRenderToTexture::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CRenderToTexture::Clear()
{
	glClearColor(m_color.r, m_color.g, m_color.b, m_color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

#endif

#ifdef DIRECT_X
bool CRenderToTexture::Initialize(ID3D11Device* device, Size size, Color color)
{
	m_color = color;

	HRESULT result;

	// Initialize the render target texture description.
	ZeroMemory(&m_textureDesc, sizeof(m_textureDesc));

	// Setup the render target texture description.
	m_textureDesc.Width = size.width;
	m_textureDesc.Height = size.height;
	m_textureDesc.MipLevels = 1;
	m_textureDesc.ArraySize = 1;
	m_textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	m_textureDesc.SampleDesc.Count = 1;
	m_textureDesc.Usage = D3D11_USAGE_DEFAULT;
	m_textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_textureDesc.CPUAccessFlags = 0;
	m_textureDesc.MiscFlags = 0;

	// Create the render target texture.
	result = device->CreateTexture2D(&m_textureDesc, NULL, &m_renderTargetTexture);

	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the render target view.
	m_renderTargetViewDesc.Format = m_textureDesc.Format;
	m_renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	m_renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	result = device->CreateRenderTargetView(m_renderTargetTexture, &m_renderTargetViewDesc, &m_renderTargetView);

	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the shader resource view.
	m_shaderResourceViewDesc.Format = m_textureDesc.Format;
	m_shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	m_shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	m_shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	result = device->CreateShaderResourceView(m_renderTargetTexture, &m_shaderResourceViewDesc, &m_shaderResourceView);
	
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void CRenderToTexture::SetRenderTarget(ID3D11DeviceContext* deviceContext)
{
	deviceContext->OMSetRenderTargets(1, &m_renderTargetView, nullptr);
}

void CRenderToTexture::ClearRenderTarget(ID3D11DeviceContext* deviceContext)
{
	deviceContext->ClearRenderTargetView(m_renderTargetView, &m_color.r);
}
#endif