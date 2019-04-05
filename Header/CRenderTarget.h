#pragma once
#include "Header.h"
#include "CTexture.h"

class CRenderTarget
{
public:
#ifdef OPEN_GL
	std::vector <CTexture*> m_view;
#endif

#ifdef DIRECT_X
	ID3D11RenderTargetView* m_view;
#endif 

	CRenderTarget();
	~CRenderTarget();

#ifdef OPEN_GL
	void Initialize(SwapChainData renderTargetData);
	void Erase();
#endif
};