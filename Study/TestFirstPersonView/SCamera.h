#pragma once
#include "SStd.h"
#include "SShape.h"
class SCamera
{
public:
	float		m_pSpeed;
	Vector3		m_vCameraPos = { 0, 0, 0 };
	Vector3		m_vCameraTarget = { 0,0,0 };
	Matrix		m_matView;
	Matrix		m_matProj;
	Vector3		m_vLook;
	Vector3		m_vUp;
	Vector3		m_vRight;
	float		m_fDistance;
	bool		m_bDrag;
	POINT		m_ptClick;
	RECT		m_rtOffset;
	RECT		m_rtPreOffset;
	int			m_fWheelDelta;
public:
	virtual  int WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam);
	virtual void SetPos(Vector3 p);
	virtual void SetTarget(Vector3 p);
	virtual void Update(Vector4 d);
	virtual bool		CreateViewMatrix(
		Vector3 p,
		Vector3 t,
		Vector3 u = { 0,1,0 });
	virtual bool		CreateProjMatrix(
		float fN,
		float fF,
		float fFov,
		float fAspect
	);
	void FrontMovement(float fDir = 1.0f);
	void RightMovement(float fDir = 1.0f);
	void UpMovement(float fDir = 1.0f);
	void FrontBase(float fDir);
	void RightBase(float fDir);
	void UpBase(float fDir);
	virtual void UpdateVector();
	virtual bool Init();
	virtual bool Frame();
public:
	SCamera();
	virtual ~SCamera();
};