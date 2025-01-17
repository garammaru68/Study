#include "SObjectManager.h"
#include "SInput.h"

SObject* SObjectComposite::Clone()
{
	SObjectComposite* pComposite = new SObjectComposite(*this);
	auto pSource = pComposite->m_Components.begin();
	for (auto obj : m_Components)
	{
		SObject* pNewObj = obj->Clone();
		*pSource = pNewObj;
		pSource++;
	}
	return pComposite;
}
SObject* SObjectManager::Clone(wstring szName)
{
	SObject* pNewObj = GetPtr(szName);
	if (pNewObj != nullptr)
	{
		return pNewObj->Clone();
	}
	return pNewObj;
}
void  SObjectManager::GetBitmapLoad(FILE* fp, wstring& ret)
{
	TCHAR szBuffer[256] = { 0, };
	TCHAR szTemp[256] = { 0, };
	//	nullptr,
	_fgetts(szBuffer, _countof(szBuffer), fp);
	_stscanf_s(szBuffer, _T("%s"), szTemp, (unsigned)_countof(szTemp));
	wstring maskbmp = szTemp;
	if (maskbmp != L"nullptr")
	{
		ret = maskbmp;
	}
}
void SObjectManager::CreateEffect(std::vector<SSpriteInfo>&	list)
{
	for (int iEffect = 0; iEffect < list.size(); iEffect++)
	{
		SEffect* pEffect = new SEffect;
		pEffect->Init();
		pEffect->m_szName = list[iEffect].szName;
		pEffect->Load(list[iEffect].colorbitmap.c_str(), list[iEffect].maskbitmap.c_str());
		SPoint p = { 0,0 };
		pEffect->Set(p, list[iEffect].rtArray[0]);
		pEffect->SetSprite(list[iEffect].rtArray);
		Add(pEffect);
	}
}
void  SObjectManager::CreateObject(std::vector<SObjAttribute>&	list)
{
	//for (SObjAttribute& info : list)
	//{
	//	SObject* pObj = new SObject;
	//	pObj->m_szName = info.szName;
	//	pObj->m_Attribute = info;
	//	pObj->Init();
	//	if (info.bColorKey == false)
	//	{
	//		pObj->Load(info.colorbitmap.c_str(),
	//			info.maskbitmap.c_str());
	//	}
	//	else
	//	{
	//		pObj->Load(info.colorbitmap.c_str(),
	//			nullptr,
	//			info.dwColor);
	//	}
	//	// state
	//	pObj->m_StateBitmap.resize(4);
	//	pObj->m_StateBitmap[DEFAULT] =
	//		pObj->m_pColorBmp;
	//	if (!info.pushbitmap.empty())
	//	{
	//		pObj->m_StateBitmap[PUSH] =
	//			g_BitmapMgr.Load(info.pushbitmap.c_str());
	//	}
	//	if (!info.selectbitmap.empty())
	//	{
	//		pObj->m_StateBitmap[SELECT] =
	//			g_BitmapMgr.Load(info.selectbitmap.c_str());
	//	}
	//	if (!info.disbitmap.empty())
	//	{
	//		pObj->m_StateBitmap[DISABLE] =
	//			g_BitmapMgr.Load(info.disbitmap.c_str());
	//	}
	//	pObj->Set(info.pos, info.rtSrc);
	//	Add(pObj);
	//}
}
bool SObjectManager::LoadEffectFile(T_STR szFileName, std::vector<SSpriteInfo>& list)
{
	FILE* fp = nullptr;
	_wfopen_s(&fp, szFileName.c_str(), L"rt");
	if (fp == nullptr) return false;
	TCHAR szBuffer[256] = { 0, };
	TCHAR szTemp[256] = { 0, };
	TCHAR szTempParent[256] = { 0, };
	if (fp != nullptr)
	{
		SObjAttribute objinfo;
		int iMaxCount = 0;
		_fgetts(szBuffer, _countof(szBuffer), fp);
		_stscanf_s(szBuffer, _T("%s %s %d"), szTemp, (unsigned)_countof(szTemp), szTempParent, (unsigned)_countof(szTempParent), &iMaxCount);
		objinfo.szName = szTemp;
		objinfo.szParentName = szTempParent;

		while (1)
		{
			SSpriteInfo tSprite;
			int iNumFrame = 0;
			_fgetts(szBuffer, _countof(szBuffer), fp);
			_stscanf_s(szBuffer, _T("%s %s %d"), szTemp, (unsigned)_countof(szTemp), szTempParent, (unsigned)_countof(szTempParent), &iNumFrame);
			//list[iCnt].resize(iNumFrame);
			tSprite.szName = szTemp;
			tSprite.szParentName = szTempParent;
			if (tSprite.szName == L"END")
			{
				break;
			}
			_fgetts(szBuffer, _countof(szBuffer), fp);
			_stscanf_s(szBuffer, _T("%s"), szTemp, (unsigned)_countof(szTemp));
			tSprite.colorbitmap = szTemp;

			_fgetts(szBuffer, _countof(szBuffer), fp);
			_stscanf_s(szBuffer, _T("%s"), szTemp, (unsigned)_countof(szTemp));
			tSprite.maskbitmap = szTemp;

			RECT rt;
			for (int iFrame = 0; iFrame < iNumFrame; iFrame++)
			{
				_fgetts(szBuffer, _countof(szBuffer), fp);
				_stscanf_s(szBuffer, _T("%s %d %d %d %d"), szTemp, (unsigned)_countof(szTemp),
					&rt.left, &rt.top, &rt.right, &rt.bottom);
				tSprite.rtArray.push_back(rt);
			}
			list.push_back(tSprite);
		}
	}
	fclose(fp);
	return true;

}
bool SObjectManager::LoadObjectFile(T_STR szFileName, std::vector<SObjAttribute>&	list)
{
	FILE* fp = nullptr;
	_wfopen_s(&fp, szFileName.c_str(), L"rt");
	if (fp == nullptr) return false;
	TCHAR szBuffer[256] = { 0, };
	TCHAR szTemp[256] = { 0, };
	TCHAR szTempParent[256] = { 0, };

	if (fp != nullptr)
	{
		SObjAttribute objhead;
		int iMaxCount = 0;
		_fgetts(szBuffer, _countof(szBuffer), fp);
		_stscanf_s(szBuffer, _T("%s %s %d"), szTemp, (unsigned)_countof(szTemp), szTempParent, (unsigned)_countof(szTempParent), &iMaxCount);
		objhead.szName = szTemp;
		objhead.szParentName = szTempParent;

		while (1)
		{
			SObjAttribute objinfo;
			_fgetts(szBuffer, _countof(szBuffer), fp);
			if (_tcscmp(szBuffer, L"\n")) // 한줄 공란이 없을 경우
			{
				_stscanf_s(szBuffer, _T("%d"), &objinfo.iObjType);
				if (objinfo.iObjType <= -1)
				{
					break;
				}
				_stscanf_s(szBuffer, _T("%d %s %s %d %d"), &objinfo.iObjType,
					szTemp, (unsigned)_countof(szTemp),
					szTempParent, (unsigned)_countof(szTempParent),
					&objinfo.iVisible,
					&objinfo.iEnable);
			}
			else
			{
				//0 //BACKGROUND
				_fgetts(szBuffer, _countof(szBuffer), fp);
				_stscanf_s(szBuffer, _T("%d %s %s %d %d"), &objinfo.iObjType,
					szTemp, (unsigned)_countof(szTemp),
					szTempParent, (unsigned)_countof(szTempParent),
					&objinfo.iVisible,
					&objinfo.iEnable);
			}
			if (objinfo.iObjType <= -1)
			{
				break;
			}

			objinfo.szName = szTemp;
			objinfo.szParentName = szTempParent;


			GetBitmapLoad(fp, objinfo.colorbitmap);
			GetBitmapLoad(fp, objinfo.pushbitmap);
			GetBitmapLoad(fp, objinfo.selectbitmap);
			GetBitmapLoad(fp, objinfo.disbitmap);
			GetBitmapLoad(fp, objinfo.maskbitmap);
			//	0, 0,
			_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
			_stscanf_s(szBuffer, _T("%d%d%d%d"), &objinfo.rtDesk.left,
				&objinfo.rtDesk.top,
				&objinfo.rtDesk.right,
				&objinfo.rtDesk.bottom);

			//	-1, -1, -1, -1,
			_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
			_stscanf_s(szBuffer, _T("%d%d%d%d"), &objinfo.rtSrc.left,
				&objinfo.rtSrc.top,
				&objinfo.rtSrc.right,
				&objinfo.rtSrc.bottom);
			objinfo.pos.x = objinfo.rtDesk.left;
			objinfo.pos.y = objinfo.rtDesk.top;

			//	0,255, 0, 255
			int mask = 0;
			int r = 0;
			int g = 0;
			int b = 0;
			_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
			_stscanf_s(szBuffer, _T("%d%d%d%d"), &mask, &r, &g, &b);
			if (mask == 0)
				objinfo.bColorKey = false;
			else
				objinfo.bColorKey = true;
			objinfo.dwColor = RGB(r, g, b);
			list.push_back(objinfo);
		}
	}
	fclose(fp);
	return true;

}
void SObjectManager::AddCollisionExecute(SObject* ownder, CollisionFunction func)
{
	ownder->m_iCollisionObjectID = m_iExecuteCollisionID++;
	m_CollisionObjectList.insert(make_pair(ownder->m_iCollisionObjectID, ownder));
	g_ObjectMgr.m_fnCollisionExecute.insert(make_pair(ownder->m_iCollisionObjectID, func));
}
void SObjectManager::AddSelectExecute(SObject* ownder, SelectFunction func)
{
	ownder->m_iSelecSObjectID = m_iExecuteSelectID++;
	m_SelecSObjectList.insert(make_pair(ownder->m_iSelecSObjectID, ownder));
	g_ObjectMgr.m_fnSelectExecute.insert(make_pair(ownder->m_iSelecSObjectID, func));
}
SObject* SObjectManager::Add(SObject* pAddObject)
{
	if (pAddObject == nullptr) return nullptr;
	SObject* pData = GetPtr(pAddObject->m_szName);
	if (pData != nullptr)
	{
		return pData;
	}
	m_List.insert(make_pair(pAddObject->m_szName, pAddObject));
	return nullptr;
}
bool SObjectManager::Load(const TCHAR* filename)
{
	TCHAR drive[MAX_PATH] = { 0, };
	TCHAR dir[MAX_PATH] = { 0, };
	TCHAR name[MAX_PATH] = { 0, };
	TCHAR ext[MAX_PATH] = { 0, };
	_wsplitpath_s(filename, drive, dir, name, ext);
	std::wstring key = name;
	key = name;
	key += ext;

	FILE* fp = nullptr;
	_wfopen_s(&fp, filename, L"rt");
	TCHAR szBuffer[256] = { 0, };
	TCHAR szTemp[256] = { 0, };
	TCHAR szTempParent[256] = { 0, };
	T_STR szObjType;

	if (fp != nullptr)
	{
		SObjAttribute objinfo;
		int iMaxCount = 0;
		_fgetts(szBuffer, _countof(szBuffer), fp);
		_stscanf_s(szBuffer, _T("%s %s %d"), szTemp, (unsigned)_countof(szTemp), szTempParent, (unsigned)_countof(szTempParent), &iMaxCount);
		objinfo.szName = szTemp;
		objinfo.szParentName = szTempParent;
		szObjType = szTemp;
	}
	fclose(fp);

	if (szObjType == L"#OBJECT")
	{
		LoadObjectFile(filename, m_ObjAttribute);
		CreateObject(m_ObjAttribute);
		return true;
	}
	if (szObjType == L"#SPRITE")
	{
		LoadEffectFile(filename, m_rtSpriteList);
		CreateEffect(m_rtSpriteList);
		return true;
	}
	return false;
}

SObject*    SObjectManager::GetPtr(wstring filename)
{
	m_iter = m_List.find(filename);
	if (m_iter == m_List.end())
	{
		return nullptr;
	}
	return (*m_iter).second;
}

bool		SObjectManager::Init()
{
	return true;
}
bool		SObjectManager::Frame()
{
	for (auto src : m_SelecSObjectList)
	{
		SObject* pSrcObj = (SObject*)src.second;
		POINT ptMouse = g_Input.GetPos();
		if (pSrcObj->m_Attribute.iEnable < 0) continue;
		pSrcObj->m_bSelect = false;
		pSrcObj->m_iImageState = 0;

		pSrcObj->m_dwSelectState = SSelectState::S_DEFAULT;
		if (SCollision::RectInPt(pSrcObj->m_rtCollide, ptMouse))
		{
			pSrcObj->m_iImageState = 1;
			DWORD dwKeyState = g_Input.GetKey(VK_LBUTTON);
			//pSrcObj->m_dwSelectState |= SSelectState::T_HOVER;
			pSrcObj->m_dwSelectState = SSelectState::S_HOVER;
			if (dwKeyState == KEY_PUSH)
			{
				pSrcObj->m_dwSelectState = SSelectState::S_ACTIVE;
				pSrcObj->m_iImageState = 2;
			}
			if (dwKeyState == KEY_HOLD)
			{
				pSrcObj->m_dwSelectState = SSelectState::S_FOCUS;
				pSrcObj->m_iImageState = 2;
			}
			if (dwKeyState == KEY_UP)
			{
				pSrcObj->m_dwSelectState = SSelectState::S_SELECTED;
				pSrcObj->m_bSelect = true;
			}
			SelectFuncIterator calliter = m_fnSelectExecute.find(pSrcObj->m_iCollisionObjectID);
			if (calliter != m_fnSelectExecute.end())
			{
				SelectFunction call = calliter->second;
				call(ptMouse, pSrcObj->m_dwSelectState);
			}
		}
		/*else
		{
			pSrcObj->m_dwSelectState &= ~SSelectState::T_HOVER;
			pSrcObj->m_dwSelectState &= ~SSelectState::T_ACTIVE;
			pSrcObj->m_dwSelectState &= ~SSelectState::T_FOCUS;
			pSrcObj->m_dwSelectState &= ~SSelectState::T_SELECTED;
		}*/
	}
	SOverlapState dwOverlap = SOverlapState::OVERLAP_NONE;
	for (auto src : m_CollisionObjectList)
	{
		SObject* pSrcObj = (SObject*)src.second;
		if (pSrcObj->m_bCollisionEnabled == false) continue;
		if (pSrcObj->m_Attribute.iEnable < 0) continue;
		for (auto desk : m_CollisionObjectList)
		{
			SObject* pDeskObj = (SObject*)desk.second;
			if (pSrcObj == pDeskObj) continue;
			if (pDeskObj->m_bCollisionEnabled == false) continue;
			if (pDeskObj->m_Attribute.iEnable < 0) continue;

			if (SCollision::Rect2Rect(pSrcObj->m_rtCollide, pDeskObj->m_rtCollide))
			{
				if (pSrcObj->m_dwOverlapState == SOverlapState::OVERLAP_BEGIN ||
					pSrcObj->m_dwOverlapState == SOverlapState::OVERLAP_STAY)
				{
					dwOverlap = SOverlapState::OVERLAP_STAY;
					pSrcObj->m_dwOverlapState = SOverlapState::OVERLAP_STAY;
				}

				if (pSrcObj->m_dwOverlapState == SOverlapState::OVERLAP_NONE)
				{
					dwOverlap = SOverlapState::OVERLAP_BEGIN;
					pSrcObj->m_dwOverlapState = SOverlapState::OVERLAP_BEGIN;
				}

				CollisionFuncIterator calliter = m_fnCollisionExecute.find(pSrcObj->m_iCollisionObjectID);
				if (calliter != m_fnCollisionExecute.end())
				{
					CollisionFunction call = calliter->second;
					call(pDeskObj, dwOverlap);
				}
				/*calliter = m_fnCollisionExecute.find(pDeskObj->m_iCollisionObjectID);
				if (calliter != m_fnCollisionExecute.end())
				{
					CollisionFunction call = calliter->second;
					call(pSrcObj, dwOverlap);
				}*/
			}
			else
			{
				if (pSrcObj->m_dwOverlapState == SOverlapState::OVERLAP_BEGIN ||
					pSrcObj->m_dwOverlapState == SOverlapState::OVERLAP_STAY)
				{
					dwOverlap = SOverlapState::OVERLAP_END;
					pSrcObj->m_dwOverlapState = SOverlapState::OVERLAP_END;
					CollisionFuncIterator calliter = m_fnCollisionExecute.find(pSrcObj->m_iCollisionObjectID);
					if (calliter != m_fnCollisionExecute.end())
					{
						CollisionFunction call = calliter->second;
						call(nullptr, dwOverlap);
					}
				}
				else
				{
					pSrcObj->m_dwOverlapState = SOverlapState::OVERLAP_NONE;
				}
			}
		}
	}
	return true;
}
bool		SObjectManager::Render()
{
	return true;
}
bool		SObjectManager::Release()
{
	for (m_iter = m_List.begin();
		m_iter != m_List.end();
		m_iter++)
	{
		(*m_iter).second->Release();
		delete (*m_iter).second;
	}
	m_List.clear();
	return true;
}
void		SObjectManager::DeleteExecute(SObject* owner)
{
	auto obj = m_SelecSObjectList.find(owner->m_iSelecSObjectID);
	if (obj != m_SelecSObjectList.end())
	{
		m_SelecSObjectList.erase(obj);
	}
	obj = m_CollisionObjectList.find(owner->m_iCollisionObjectID);
	if (obj != m_CollisionObjectList.end())
	{
		m_CollisionObjectList.erase(obj);
	}
}
SObjectManager::SObjectManager()
{
	m_szDefaultPath = L"../../data/bitmap/";
	m_iExecuteSelectID = 0;
	m_iExecuteCollisionID = 0;
}

SObjectManager::~SObjectManager()
{
	Release();
}