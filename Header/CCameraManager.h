#pragma once
#include "Header.h"
#include "CCamera.h"

class CCameraManager
{
public:

	unsigned int			m_activeCamera;
	std::vector <CCamera>	m_cameras;

	CCameraManager() = default;
	~CCameraManager() = default;

	void Initialize(VECTOR3 position, VECTOR3 up, VECTOR3 front, VECTOR3 right, float yaw, float pitch, float speed, float rotateSpeed);
	void AddCamera(VECTOR3 position, VECTOR3 up, VECTOR3 front, VECTOR3 right, float yaw, float pitch, float speed, float rotateSpeed);

	void Erase();

	void ChangeCamera();
	void SetViewMatrix(float fieldOfView, Size aspectRatio, float nearZ, float farZ);
	void Resize(Size aspectRatio);
	void Zoom();

	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
	void MoveUp();

	void Rotate(float x, float y);

	CCamera* GetActiveCamera();
};