#include "../Header/CManager.h"

CManager::CManager()
{
	m_time = 0;
	m_isInitialized = false;
}

#ifdef OPEN_GL
void CManager::Initialize()
{

	//Create window
	Rect dimensions = { 0, 0, 800, 600 };

	std::vector <const char*> scene;
	scene.push_back("../Dwarf/DwarfWithEffectInstance.x");
	scene.push_back("../Dragon/Dragon.fbx");
	scene.push_back("../Anubis/Anubis.fbx");
	scene.push_back("../Quad/screen.fbx");

	m_window.Initialize(dimensions, GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL, "OpenGL", Color{ 0.0f, 0.3f, 0.6f, 1.0f }, scene);
	m_viewport.Initialize(dimensions);

	m_renderTexture.Initialize(m_window.m_size.size, Color{ 0.8f, 0.1f, 0.0f, 1.0f });
	m_renderTexture.Unbind();

	m_window.m_scene.m_models[0].AddTransform(glm::translate(MATRIX4(1.0f), VECTOR3(0.0f, 0.0f, 0.0f)));
	m_window.m_scene.m_models[1].AddTransform(glm::translate(MATRIX4(1.0f), VECTOR3(150.0f, 0.0f, 0.0f)));
	m_window.m_scene.m_models[2].AddTransform(glm::mat4(1.0f));

	m_window.m_scene.m_models[3].AddTransform(glm::rotate(glm::scale(glm::translate(MATRIX4(1.0f),
	VECTOR3(-150.0f, 0.0f, 0.0f)), VECTOR3(150.0f, 150.0f, 0.0f)), -1.5708f, VECTOR3(1.0f, 0.0f, 0.0f)));

	scene.clear();
	

	//Load Shaders
	m_shaderProgram.Initialize("../Header/Shader/VertexShader.glsl", "../Header/Shader/PixelShader.glsl");

	m_userInterface.Initialize();

	m_isInitialized = true;
}
#endif

#ifdef DIRECT_X
void CManager::Initialize(WNDPROC pWndProc, HINSTANCE hInstance, std::string title, std::string className, Size size, Color color, INT nCmdShow)
{
	//Window
	m_window.Initialize(pWndProc, hInstance, title, className, size, color, nCmdShow);

	//DeviceContext and SwapChain and render target
	m_device.Initialize(m_deviceContext, m_window.m_swapChain);
	m_deviceContext.SetRenderTarget(m_window.m_swapChain);
	m_renderTexture.Initialize(m_device.m_pointer, size, Color{0.8f, 0.1f, 0.0f, 1.0f});

	//Samplers
	m_sampler.Initialize();
	m_device.CreateSamplerState(m_sampler);
	m_deviceContext.SetSampler(m_sampler);

	//Viewports
	m_viewport.Initialize(m_window.m_size);
	m_deviceContext.SetViewPort(m_viewport);

	m_deviceContext.SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//shaders
	if (!FAILED(m_shaderProgram.Initialize("../Header/Shader/Shaders.fx", "vs_4_0", "ps_4_0", "VS", "PS")))
	{
		m_device.CreateVertexShader(m_shaderProgram);
		m_deviceContext.SetVertexShader(m_shaderProgram);
		
		m_device.CreatePixelShader(m_shaderProgram);
		m_deviceContext.SetPixelShader(m_shaderProgram);

		m_device.CreateInputLayout(m_shaderProgram);
		m_deviceContext.SetInputLayout(m_shaderProgram);

		m_device.CreateConstantBuffer(m_shaderProgram.m_viewCB, sizeof(MATRIX4));
		m_device.CreateConstantBuffer(m_shaderProgram.m_projectionCB, sizeof(MATRIX4));
		m_device.CreateConstantBuffer(m_shaderProgram.m_worldCB, sizeof(MATRIX4));
		m_device.CreateConstantBuffer(m_shaderProgram.m_colorCB, sizeof(VECTOR4));

		m_deviceContext.SetVertexConstantBuffer(0, m_shaderProgram.m_viewCB);
		m_deviceContext.SetVertexConstantBuffer(1, m_shaderProgram.m_projectionCB);
		m_deviceContext.SetVertexConstantBuffer(2, m_shaderProgram.m_worldCB);

		m_deviceContext.SetVertexConstantBuffer(3, m_shaderProgram.m_colorCB);
		m_deviceContext.SetPixelConstantBuffer(3, m_shaderProgram.m_colorCB);
	}

	//Scene
	std::vector <const char*> scene;
	scene.push_back("../Dwarf/DwarfWithEffectInstance.x");
	scene.push_back("../Dragon/Dragon.fbx");
	scene.push_back("../Anubis/Anubis.fbx");
	scene.push_back("../Quad/screen.fbx");

	m_window.m_scene.Initialize(scene, m_device);

	m_window.m_scene.m_models[0].AddTransform(glm::mat4(1.0f));

	m_window.m_scene.m_models[1].AddTransform(glm::translate(MATRIX4(1.0f), VECTOR3(150.0f, 0.0f, 0.0f)));

	m_window.m_scene.m_models[2].AddTransform(glm::mat4(1.0f));

	m_window.m_scene.m_models[3].AddTransform(glm::rotate(glm::scale(glm::translate(MATRIX4(1.0f), 
	VECTOR3(-150.0f, 0.0f, 0.0f)), VECTOR3(150.0f, 150.0f, 0.0f)), -1.5708f, VECTOR3(1.0f,0.0f,0.0f)));

	scene.clear();

	//Camera
	m_window.m_camera.Initialize(VECTOR3(0.0f, 400.0f, 0.0f), VECTOR3(0.0f, 0.0f, 1.0f),
	VECTOR3(0.0f, -1.0f, 0.0f), VECTOR3(1.0f, 0.0f, 0.0f), -90.0f, 0.0f, 2.0f, 0.25f);

	m_window.m_camera.AddCamera(VECTOR3(0.0f, 0.0f, -400.0f), VECTOR3(0.0f, 1.0f, 0.0f),
	VECTOR3(0.0f, 0.0f, 1.0f), VECTOR3(1.0f, 0.0f, 0.0f), 90.0f, 0.0f, 2.0f, 0.25f);

	m_window.m_camera.SetViewMatrix((45.0f / 360.0f) * 6.283185307f, m_window.m_size.size, 0.1f, 500.0f);

	m_userInterface.Initialize();
	ImGui_ImplWin32_Init(m_window.m_handle);
	ImGui_ImplDX11_Init(m_device.m_pointer, m_deviceContext.m_pointer);

	m_isInitialized = true;
}
#endif

void CManager::Render()
{
#ifdef OPEN_GL
	m_shaderProgram.Bind();

	m_shaderProgram.UpdateCamera(m_window.m_camera.m_cameras[m_window.m_camera.m_activeCamera]);

	glClearColor(m_window.m_clearColor.r, m_window.m_clearColor.g, m_window.m_clearColor.b, m_window.m_clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	m_window.Render(m_shaderProgram);
	
	//m_renderTexture.Bind(m_window.m_size.size);
	//m_renderTexture.Clear();

	//ImGui
	m_userInterface.Initframe();
	m_userInterface.SetFrame((float)m_window.m_scene.GetNumVertices(), (float)m_window.m_scene.GetNumFaces(), 
	(float)m_window.m_scene.GetNumMeshes(), (float)m_window.m_scene.GetNumModels());
	m_userInterface.RenderFrame();

	//Present
	glutSwapBuffers();
#endif

#ifdef DIRECT_X

	//---------------------------------------------------------------------------------------------------------
	m_renderTexture.SetRenderTarget(m_deviceContext.m_pointer);
	m_renderTexture.ClearRenderTarget(m_deviceContext.m_pointer);

	m_window.m_scene.Render(m_deviceContext, m_shaderProgram, m_window.m_camera.m_cameras[0], m_window.m_camera.m_cameras[1]);
	m_window.m_scene.m_models[3].m_meshes[0].m_texture = m_renderTexture.m_shaderResourceView;
	//---------------------------------------------------------------------------------------------------------
	
	m_deviceContext.SetRenderTarget(m_window.m_swapChain);
	m_deviceContext.ClearColor(m_window.m_swapChain);
	m_deviceContext.ClearDepthStencil(m_window.m_swapChain);

	m_window.m_scene.Render(m_deviceContext, m_shaderProgram, m_window.m_camera.m_cameras[1], m_window.m_camera.m_cameras[1]);

	//ImGui
	m_userInterface.Initframe();
	m_userInterface.SetFrame((float)m_window.m_scene.GetNumVertices(), (float)m_window.m_scene.GetNumFaces(), 
	(float)m_window.m_scene.GetNumMeshes(), (float)m_window.m_scene.GetNumModels());
	m_userInterface.RenderFrame();
	//------

	m_window.m_swapChain.Present(0, 0);
#endif
}

void CManager::Update()
{
#ifdef OPEN_GL
	m_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
#endif

	Resize();

#ifdef DIRECT_X
	m_time = 0.0f;

	if (m_device.m_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		m_time += (float)XM_PI * 0.0125f;
	}
	else
	{
		static DWORD dwTimeStart = 0;
		DWORD dwTimeCur = GetTickCount();

		if (dwTimeStart == 0)
		{
			dwTimeStart = dwTimeCur;
		}

		m_time = (dwTimeCur - dwTimeStart) / 1000.0f;
	}
#endif

	m_inputHandler.Update(m_window.m_camera, m_window.m_scene);
	m_window.m_scene.Update(m_time);
}

void CManager::Resize()
{
	if (m_window.Resize() && m_isInitialized)
	{
		m_viewport.Initialize(m_window.m_size);

#ifdef OPEN_GL
		m_viewport.Resize(m_window.m_size);
		//m_renderTexture.Initialize(m_window.m_size.size, m_renderTexture.m_color);
#endif

#ifdef DIRECT_X
		m_renderTexture.Initialize(m_device.m_pointer, m_window.m_size.size, m_renderTexture.m_color);

		m_window.m_swapChain.Resize(m_window.m_size.size);

		ID3D11Texture2D* pBackBuffer = NULL;
		m_window.m_swapChain.m_pointer->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		m_device.CreateRenderTargetView(m_window.m_swapChain.m_renderTarget, pBackBuffer);
		pBackBuffer->Release();

		m_device.CreateDepthStencilTexture(m_window.m_swapChain.m_depthStencil);
		m_device.CreateDepthStencilView(m_window.m_swapChain.m_depthStencil);
		m_deviceContext.SetRenderTarget(m_window.m_swapChain);
		m_deviceContext.SetViewPort(m_viewport);
#endif
		m_window.m_camera.Resize(m_window.m_size.size);
	}
}