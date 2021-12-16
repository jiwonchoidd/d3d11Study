#define _CRT_SECURE_NO_WARNINGS
#include "KFbxObj.h"
#include <algorithm>
#include "KInput.h"
bool Compare(const pair<float, int>& a, const pair<float, int>& b)
{
	return a.first > b.first;
}
//nullptr를 넣으면 MainPS 아닐 경우에는 인자로 받아온 PS
void KFbxObj::ChangePixelShader(ID3D11PixelShader* ps,
	KMatrix* matNormal,
	KVector3 vLight)
{
	for (int iMesh = 0; iMesh < m_pMeshList.size(); iMesh++)
	{
		KMesh* pMesh = m_pMeshList[iMesh];
		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (int iSubMesh = 0; iSubMesh < m_pMeshList[iMesh]->m_pSubMesh.size(); iSubMesh++)
			{
				KMesh* pSubMesh = m_pMeshList[iMesh]->m_pSubMesh[iSubMesh];
				if (ps == nullptr)
				{
					pSubMesh->m_pMainPS = pSubMesh->m_pPS;
				}
				else
				{
					pSubMesh->m_pMainPS = ps;
				}
				if (matNormal != nullptr)
				{
					pSubMesh->m_cbData.matNormal = *matNormal;
				}
				pSubMesh->m_cbData.vLightDir.x = vLight.x;
				pSubMesh->m_cbData.vLightDir.y = vLight.y;
				pSubMesh->m_cbData.vLightDir.z = vLight.z;
			}
		}
		else
		{
			if (ps == nullptr)
			{
				pMesh->m_pMainPS = pMesh->m_pPS;
			}
			else
			{
				pMesh->m_pMainPS = ps;
			}
			if (matNormal != nullptr)
			{
				pMesh->m_cbData.matNormal = *matNormal;
			}
			pMesh->m_cbData.vLightDir.x = vLight.x;
			pMesh->m_cbData.vLightDir.y = vLight.y;
			pMesh->m_cbData.vLightDir.z = vLight.z;
		}
	}
}
bool KFbxObj::Frame()
{

	#pragma region 캐릭터 이동 & 회전 & 애니메이션
	if (m_bAnimPlay)
	{
		m_fElpaseTime += 1.0f * g_fSecPerFrame;
		m_iAnimIndex = m_fElpaseTime * 30.0f;
		if (m_fEndTime < m_fElpaseTime)
		{
			m_iAnimIndex = 0;
			m_fElpaseTime = 0;
			//m_bAnimPlay = false;
		}
	}
	#pragma endregion

	return true;
}
//재귀호출.
void      KFbxObj::ParseNode(FbxNode* pNode, KMesh* pParentMesh)
{
	// 카메라나 라이트 등 매쉬가 아니라면 리턴
	if (pNode->GetCamera() || pNode->GetLight())
	{
		return;
	}
	//전체 다 매쉬로 매쉬에 노드를 물려준다.
	KMesh* pMesh = new KMesh;
	pMesh->m_pFbxNode = pNode;
	pMesh->m_szName = TBASIS::mtw(pNode->GetName());
	KMatrix matParent;
	if (pParentMesh != nullptr)
	{
		pMesh->m_szParentName = pParentMesh->m_szName;
		matParent = pParentMesh->m_matWorld;
	}
	//재귀함수로 부모 매쉬를 넣어준다.
	pMesh->m_pParent = pParentMesh;
	//각 매쉬의 월드행렬은 부모를 더해지기 때문에 부모 매개변수로 넣음
	pMesh->m_matWorld = ParseTransform(pNode, matParent);

	if (pNode->GetMesh())
	{
		ParseMesh(pNode, pMesh);
		pMesh->m_ClassType = CLASS_GEOM;
	}
	else
	{
		pMesh->m_ClassType = CLASS_BONE;
	}
	m_pMeshList.push_back(pMesh);

	int iNumChild = pNode->GetChildCount();
	for (int iNode = 0; iNode < iNumChild; iNode++)
	{
		FbxNode* pChildNode = pNode->GetChild(iNode);
		ParseNode(pChildNode, pMesh);
	}
}
KMatrix   KFbxObj::ParseTransform(FbxNode* pNode, KMatrix& matParent)
{
	// TODO : 월드행렬
	//월드행렬에 지오매트리까지 곱해야 월드 행렬 나중에 애니메이션를 고려해서
	//나중에 지오매트릭행렬을 곱한다.
	//로컬좌표 곱하기 부모 좌표
	FbxVector4 rotLcl = pNode->LclRotation.Get();
	FbxVector4 transLcl = pNode->LclTranslation.Get();
	FbxVector4 scaleLcl = pNode->LclScaling.Get();
	FbxMatrix matTransform(transLcl, rotLcl, scaleLcl);
	KMatrix matLocal = DxConvertMatrix(ConvertMatrix(matTransform));
	KMatrix matWorld = matLocal * matParent;
	return matWorld;
}

void	KFbxObj::SetMatrix(
	KMatrix* pMatWorld,
	KMatrix* pMatView, KMatrix* pMatProj)
{
	if (pMatWorld != nullptr)
	{
		m_cbData.matWorld = *pMatWorld;
	}
	if (pMatView != nullptr)
	{
		m_cbData.matView = *pMatView;
	}
	if (pMatProj != nullptr)
	{
		m_cbData.matProj = *pMatProj;
	}
}

KMatrix     KFbxObj::DxConvertMatrix(KMatrix m)
{
	KMatrix mat;
	mat._11 = m._11; mat._12 = m._13; mat._13 = m._12;
	mat._21 = m._31; mat._22 = m._33; mat._23 = m._32;
	mat._31 = m._21; mat._32 = m._23; mat._33 = m._22;
	mat._41 = m._41; mat._42 = m._43; mat._43 = m._42;
	mat._14 = mat._24 = mat._34 = 0.0f;
	mat._44 = 1.0f;
	return mat;
}
//복사, 변환
KMatrix     KFbxObj::ConvertMatrix(FbxMatrix& m)
{
	KMatrix mat;
	float* pMatArray = reinterpret_cast<float*>(&mat);
	double* pSrcArray = reinterpret_cast<double*>(&m);
	for (int i = 0; i < 16; i++)
	{
		pMatArray[i] = pSrcArray[i];
	}
	return mat;
}
//매개변수만 다름
KMatrix     KFbxObj::ConvertAMatrix(FbxAMatrix& m)
{
	KMatrix mat;
	float* pMatArray = reinterpret_cast<float*>(&mat);
	double* pSrcArray = reinterpret_cast<double*>(&m);
	for (int i = 0; i < 16; i++)
	{
		pMatArray[i] = pSrcArray[i];
	}
	return mat;
}
void		KFbxObj::LoadMaterial(KMtrl* pMtrl)
{
	//서브 메터리얼일때와 루트 메터리얼 하나 있을때 구분함
	if (pMtrl == nullptr) return;
	if (pMtrl->m_pSubMtrl.size() > 0)
	{
		for (int iSub = 0; iSub < pMtrl->m_pSubMtrl.size(); iSub++)
		{
			//서브 매트리얼 크기 만큼 포문 돌림
			KMtrl* pSubMtrl = pMtrl->m_pSubMtrl[iSub];
			FbxSurfaceMaterial* pFbxMaterial = pSubMtrl->m_pFbxSurfaceMtrl;
			FbxProperty prop = pFbxMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if (prop.IsValid())
			{
				//텍스쳐 수 가져오는 함수
				int iTexCount = prop.GetSrcObjectCount<FbxTexture>();
				for (int iTex = 0; iTex < iTexCount; iTex++)
				{
					FbxTexture* pTex = prop.GetSrcObject<FbxFileTexture>(iTex);
					//텍스쳐가 있을수도 없을수도
					if (pTex == nullptr) continue;
					FbxFileTexture* fileTexture = prop.GetSrcObject<FbxFileTexture>(iTex);

					std::string szFileName;
					char Drive[MAX_PATH] = { 0, };
					char Dir[MAX_PATH] = { 0, };
					char FName[MAX_PATH] = { 0, };
					char Ext[MAX_PATH] = { 0, };

					//tga파일 지원하지 않아서 확장자 호환
					if (fileTexture->GetFileName())
					{
						_splitpath(fileTexture->GetFileName(), Drive, Dir, FName, Ext);
						Ext[4] = 0;
						szFileName = FName;
						if (_stricmp(Ext, ".tga") == 0)
						{
							szFileName += ".dds";
						}
						else
						{
							szFileName += Ext;
						}
					}

					pSubMtrl->m_Texture.m_szFileName = TBASIS::g_szDataPath;
					pSubMtrl->m_Texture.m_szFileName += L"object/";
					pSubMtrl->m_Texture.m_szFileName += TBASIS::mtw(szFileName);
					pSubMtrl->m_Texture.LoadTexture(
						pSubMtrl->m_Texture.m_szFileName);
				}
			}
		}
	}
	else
	{
		//매터리얼의 첫번째 주소값을 받아옴 
		//FbxSurfaceMaterial* pFbxSurfaceMtrl = pMtrl->m_pFbxSurfaceMtrl;
		FbxSurfaceMaterial* pFbxMaterial = pMtrl->m_pFbxNode->GetMaterial(0);
		//디퓨즈인 속성을 prop에 저장
		FbxProperty prop = pFbxMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
		if (prop.IsValid())
		{
			int iTexCount = prop.GetSrcObjectCount<FbxTexture>();
			for (int iTex = 0; iTex < iTexCount; iTex++)
			{
				FbxTexture* pTex = prop.GetSrcObject<FbxFileTexture>(iTex);
				if (pTex == nullptr) continue;
				FbxFileTexture* fileTexture = prop.GetSrcObject<FbxFileTexture>(iTex);

				std::string szFileName;
				char Drive[MAX_PATH] = { 0, };
				char Dir[MAX_PATH] = { 0, };
				char FName[MAX_PATH] = { 0, };
				char Ext[MAX_PATH] = { 0, };
				if (fileTexture->GetFileName())
				{
					_splitpath(fileTexture->GetFileName(), Drive, Dir, FName, Ext);
					Ext[4] = 0;
					szFileName = FName;
					if (_stricmp(Ext, ".tga") == 0)
					{
						szFileName += ".dds";
					}
					else
					{
						szFileName += Ext;
					}
				}

				pMtrl->m_Texture.m_szFileName = TBASIS::g_szDataPath;
				pMtrl->m_Texture.m_szFileName += L"object/";
				pMtrl->m_Texture.m_szFileName += TBASIS::mtw(szFileName);
				pMtrl->m_Texture.LoadTexture(pMtrl->m_Texture.m_szFileName);
			}
		}
	}
}
//todo: 수정
bool    KFbxObj::Render(ID3D11DeviceContext* pContext)
{
	// 서브메터리얼 존재할때 각각 렌더하거나 루트메터리얼 하나일때 렌더하는 방식
	for (int iObj = 0; iObj < m_pMeshList.size(); iObj++)
	{
		KMesh* pMesh = m_pMeshList[iObj];
		//본타입 같이 지오매트릭 타입이 아니라면 렌더 대상이 아니다.
		if (pMesh->m_ClassType != CLASS_GEOM) continue;
		//전치행렬
		D3DKMatrixTranspose(&pMesh->m_matAnimMatrix.matAnimation[0],
						&pMesh->m_AnimationTrack[m_iAnimIndex]);

		for (int iBone = 0; iBone < pMesh->m_matBindPoseList.size(); iBone++)
		{
			//뼈의 위치 역행렬
			KMatrix matAnim = pMesh->m_matBindPoseList[iBone] *
				pMesh->m_pMeshLinkList[iBone]->m_AnimationTrack[m_iAnimIndex];
			D3DKMatrixTranspose(&pMesh->m_matAnimMatrix.matAnimation[iBone],
				&matAnim);
		}
		KMtrl* pMtrl = nullptr;
		pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 서브 메터리얼의 존재한다면..
		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (int iSub = 0; iSub < pMesh->m_pSubMesh.size(); iSub++)
			{
				if (pMesh->m_pSubMesh[iSub]->m_pVertexList.size() <= 0) continue;
				KMtrl* pSubMtrl =
					m_pFbxMaterialList[pMesh->m_iMtrlRef]->m_pSubMtrl[iSub];
				if (pSubMtrl->m_Texture.m_pTextureSRV != nullptr)
				{
					//pContext->PSSetSamplers(0, 1, &pSubMtrl->m_Texture.m_pSampler);
					pContext->PSSetShaderResources(0, 1, &pSubMtrl->m_Texture.m_pTextureSRV);
				}

				// 캐릭터는 대부분 메터리얼 하나일것 스키닝도 서브메터리얼 사용 가능함. 
				D3DKMatrixTranspose(&pMesh->m_pSubMesh[iSub]->m_matAnimMatrix.matAnimation[0],
					&pMesh->m_AnimationTrack[m_iAnimIndex]);
				for (int iBone = 0; iBone < pMesh->m_matBindPoseList.size(); iBone++)
				{
					pMesh->m_pSubMesh[iSub]->m_matAnimMatrix.matAnimation[iBone]
						= pMesh->m_matAnimMatrix.matAnimation[iBone];
				}
				pMesh->m_pSubMesh[iSub]->SetMatrix(&m_matWorld,
					&m_cbData.matView, &m_cbData.matProj);
				pMesh->m_pSubMesh[iSub]->Render(pContext);
			}
		}
		else
		{
			if (pMesh->m_iMtrlRef >= 0)
			{
				pMtrl = m_pFbxMaterialList[pMesh->m_iMtrlRef];
			}
			if (pMtrl != nullptr)
			{
				if (pMtrl->m_Texture.m_pTextureSRV != nullptr)
				{
					//pContext->PSSetSamplers(0, 1, &pMtrl->m_Texture.m_pSampler);
					pContext->PSSetShaderResources(0, 1, &pMtrl->m_Texture.m_pTextureSRV);
				}
			}
			pMesh->SetMatrix(&m_cbData.matWorld, &m_cbData.matView, &m_cbData.matProj);
			pMesh->Render(pContext);
		}
	}
	return true;
}
int     KFbxObj::GetRootMtrl(FbxSurfaceMaterial* pFbxMaterial)
{
	for (int iMtrl = 0; iMtrl < m_pFbxMaterialList.size(); iMtrl++)
	{
		if (m_pFbxMaterialList[iMtrl]->m_pFbxSurfaceMtrl == pFbxMaterial)
		{
			return iMtrl;
		}
	}
	return -1;
}
//매쉬 해석 버텍스를 가진 기하 타입을 pnct_vertex를 채워줘서 vertexlist를 pushback
void	KFbxObj::ParseMesh(FbxNode* pNode, KMesh* pMesh)
{
	//무조건 0번째 주소값을 가져와서 비교한다.
	FbxSurfaceMaterial* pFbxMaterial = pNode->GetMaterial(0);
	pMesh->m_iMtrlRef = GetRootMtrl(pFbxMaterial);

	std::string name = pNode->GetName();
	FbxMesh* pFbxMesh = pNode->GetMesh();
	std::vector< std::string> fbxFileTexList;
	if (pFbxMesh != nullptr)
	{
		int iNumCP = pFbxMesh->GetControlPointsCount();
		// 스키닝 오브젝트 여부?
		// pFbxMesh에 영향을 미치는 행렬(노드)에 전체 개수?
		// 행렬[0]=FbxNode ~ 행렬[3] = 4개
		// 정점[0]->인덱스[1]		// 		
		KSkinData skindata;
		bool bSkinnedMesh = ParseMeshSkinning(pFbxMesh, pMesh, &skindata);
		if (bSkinnedMesh)
		{
			_ASSERT(skindata.m_VertexList.size() == iNumCP);
			for (int i = 0; i < skindata.m_VertexList.size(); i++)
			{
				std::vector<std::pair<float, int>> list;
				for (int j = 0; j <
					skindata.m_VertexList[i].m_IndexList.size();
					j++)
				{
					//Pair 정렬을 위한 리스트
					list.push_back(std::make_pair(
						skindata.m_VertexList[i].m_WeightList[j],
						skindata.m_VertexList[i].m_IndexList[j]));
				}
				std::sort(list.begin(), list.end(), Compare);
				for (int k = 0; k < list.size(); k++)
				{
					skindata.m_VertexList[i].m_WeightList[k] = list[k].first;
					skindata.m_VertexList[i].m_IndexList[k] = list[k].second;
				}
			}
		}
		//정점 성분 레이어 수만큼 각각의 성분이 있는지
		pMesh->m_iNumLayer = pFbxMesh->GetLayerCount();
		pMesh->m_LayerList.resize(pMesh->m_iNumLayer);

		// todo : 정점성분 레이어 리스트
		for (int iLayer = 0; iLayer < pMesh->m_iNumLayer; iLayer++)
		{
			//하나의 레이어의 작업이다. 
			FbxLayer* pLayer = pFbxMesh->GetLayer(iLayer);
			//color, normal, UV, material
			if (pLayer->GetVertexColors() != nullptr)
			{
				pMesh->m_LayerList[iLayer].pColor = pLayer->GetVertexColors();
			}
			if (pLayer->GetNormals() != nullptr)
			{
				pMesh->m_LayerList[iLayer].pNormal = pLayer->GetNormals();
			}
			if (pLayer->GetUVs() != nullptr)
			{
				pMesh->m_LayerList[iLayer].pUV = pLayer->GetUVs();
			}
			if (pLayer->GetMaterials() != nullptr)
			{
				pMesh->m_LayerList[iLayer].pMaterial = pLayer->GetMaterials();
			}
		}
		// 월드행렬이 아니다.상속관계에서 되물림되지 않는 행렬 기하행렬. 
		// 피봇은  소스의 원점
		///*GetTransform(); 변환을 해주는 변환 행렬
		//애니메이션이 들어가기전까지는 단위행렬이다.
		//로컬 행렬을 월드 행렬로 적용을했더니 결과가 월드 정점이 나왔다.
		//우리가 읽어들인 정점은 리스트에서 그냥 그대로 가져온 정점
		//월드좌표에다가 월드 행렬을 곱해지는 결과가 나옴 */
	// TODO : 기하행렬
		FbxAMatrix matGeom;
		FbxVector4 rot = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
		FbxVector4 trans = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		FbxVector4 scale = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
		matGeom.SetT(trans);
		matGeom.SetR(rot);
		matGeom.SetS(scale);

		int m_iNumPolygon = pFbxMesh->GetPolygonCount();
		// 정점리스트 주소
		FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints();

		int iBasePolyIndex = 0;
		int iNumFbxMaterial = pNode->GetMaterialCount();
		if (iNumFbxMaterial > 1)
		{
			pMesh->m_pSubMesh.resize(iNumFbxMaterial);
			for (int iSub = 0; iSub < iNumFbxMaterial; iSub++)
			{
				pMesh->m_pSubMesh[iSub] = new KMesh;
			}
		}

		FbxLayerElementMaterial* fbxSubMaterial =
			pMesh->m_LayerList[0].pMaterial;
		FbxLayerElementUV* VertexUVList =
			pMesh->m_LayerList[0].pUV;
		FbxLayerElementVertexColor* VertexColorList =
			pMesh->m_LayerList[0].pColor;
		FbxLayerElementNormal* VertexNormalList =
			pMesh->m_LayerList[0].pNormal;

		for (int iPoly = 0; iPoly < m_iNumPolygon; iPoly++)
		{
			int iSubMtrlIndex = 0;
			if (fbxSubMaterial != nullptr)
			{
				switch (fbxSubMaterial->GetMappingMode())
				{
				case FbxLayerElement::eByPolygon:
				{
					switch (fbxSubMaterial->GetReferenceMode())
					{
					case FbxLayerElement::eDirect:
					{
						iSubMtrlIndex = iPoly;
					}break;
					case FbxLayerElement::eIndex:
					case FbxLayerElement::eIndexToDirect:
					{
						iSubMtrlIndex =
							fbxSubMaterial->GetIndexArray().GetAt(iPoly);
					}break;
					}
				}break;
				default:
				{
					iSubMtrlIndex = 0;
				}break;
				}
			}
			if (iSubMtrlIndex < 0 || iSubMtrlIndex >= iNumFbxMaterial)
			{
				int kk = 0;
			}
			// 삼각형, 사각형
			int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
			int m_iNumTriangle = iPolySize - 2;
			int iVertexIndex[3];
			for (int iTriangle = 0; iTriangle < m_iNumTriangle; iTriangle++)
			{
				// 위치 인덱스 yz 좌표 바꿔서 넣어야함
				iVertexIndex[0] = pFbxMesh->GetPolygonVertex(iPoly, 0);
				iVertexIndex[1] = pFbxMesh->GetPolygonVertex(iPoly, iTriangle + 2);
				iVertexIndex[2] = pFbxMesh->GetPolygonVertex(iPoly, iTriangle + 1);

				for (int iIndex = 0; iIndex < 3; iIndex++)
				{
					//TODO : UV때문에 안나왔다고??? 나중에 확인해봐야겠다...
					PNCT_VERTEX vertex;
					PNCTIW_VERTEX iwVertex;
					//안전빵 제로 메모리..
					ZeroMemory(&iwVertex, sizeof(PNCTIW_VERTEX));
					//이것은 로컬 위치라고 생각하면됨
					FbxVector4 pos = pVertexPositions[iVertexIndex[iIndex]];
					//열우선방식의 행열 곱하기 정점.
					//나중에 애니메이션를 위해 기하좌표는 나중에 곱해줌
					FbxVector4 vPos = matGeom.MultT(pos);
					vertex.pos.x = vPos.mData[0];
					vertex.pos.y = vPos.mData[2];
					vertex.pos.z = vPos.mData[1];
					if (VertexUVList != nullptr)
					{
						// UV 인덱스
						// UV 인덱스 yz 좌표 바꿔서 넣어야함
						int uvIndex[3];
						uvIndex[0] = pFbxMesh->GetTextureUVIndex(iPoly, 0);
						uvIndex[1] = pFbxMesh->GetTextureUVIndex(iPoly, iTriangle + 2);
						uvIndex[2] = pFbxMesh->GetTextureUVIndex(iPoly, iTriangle + 1);
						FbxVector2 uv = ReadTextureCoord(
							pFbxMesh, 1, VertexUVList,
							iVertexIndex[iIndex], uvIndex[iIndex]);
						vertex.tex.x = uv.mData[0];
						vertex.tex.y = 1.0f - uv.mData[1];
					}
					//컬러 노말은 인덱스가 같아 똑같은 수식이다.
					if (VertexColorList != nullptr)
					{
						int iColorIndex[3];
						iColorIndex[0] = iBasePolyIndex + 0;
						iColorIndex[1] = iBasePolyIndex + iTriangle + 2;
						iColorIndex[2] = iBasePolyIndex + iTriangle + 1;

						FbxColor color = ReadColor(
							pFbxMesh, 1, VertexColorList,
							iVertexIndex[iIndex], iColorIndex[iIndex]);
						vertex.color.x = color.mRed;
						vertex.color.y = color.mGreen;
						vertex.color.z = color.mBlue;
						vertex.color.w = 1.0f;

					}
					if (VertexNormalList != nullptr)
					{
						int iNormalIndex[3];
						iNormalIndex[0] = iBasePolyIndex + 0;
						iNormalIndex[1] = iBasePolyIndex + iTriangle + 2;
						iNormalIndex[2] = iBasePolyIndex + iTriangle + 1;
						FbxVector4 normal = ReadNormal(
							pFbxMesh, 1, VertexNormalList,
							iVertexIndex[iIndex], iNormalIndex[iIndex]);
						vertex.normal.x = normal.mData[0];
						vertex.normal.y = normal.mData[2];
						vertex.normal.z = normal.mData[1];
					}
					// 인덱스 및 가중치 저장
					int iRealIndex = iVertexIndex[iIndex];
					if (bSkinnedMesh)
					{
						int iNum =
							skindata.m_VertexList[iRealIndex].m_IndexList.size();
						for (int i = 0; i < min(iNum, 4); i++)
						{
							iwVertex.index[i] =
								skindata.m_VertexList[iRealIndex].m_IndexList[i];
							iwVertex.weight[i] =
								skindata.m_VertexList[iRealIndex].m_WeightList[i];
						}
					}
					// 비 스키닝 오브젝트를 -> 스키닝화 처리
					else
					{
						//0번의 인덱스는 자기자신의 월드행렬이 곱해진다.
						iwVertex.index[0] = 0;
						iwVertex.weight[0] = 1.0f;
						for (int i = 1; i < 4; i++)
						{
							iwVertex.index[i] = 0;
							iwVertex.weight[i] = 0.0f;
						}
					}
					if (iNumFbxMaterial > 1)
					{
						pMesh->m_pSubMesh[iSubMtrlIndex]->m_pVertexList.push_back(vertex);
						pMesh->m_pSubMesh[iSubMtrlIndex]->m_iwList.push_back(iwVertex);
					}
					else
					{
						pMesh->m_pVertexList.push_back(vertex);
						pMesh->m_iwList.push_back(iwVertex);
					}
				}
			}
			iBasePolyIndex += iPolySize;
		}
	}
}
void	KFbxObj::PreProcess(FbxNode* pNode)
{
	if (pNode && (pNode->GetCamera() || pNode->GetLight())) { return; }
	m_pFbxNodeList.push_back(pNode);
	// pNode 정보 얻기
	int iNumFbxMaterial = pNode->GetMaterialCount();
	FbxSurfaceMaterial* pFbxMaterial = pNode->GetMaterial(0);
	//서브매터리얼이 있다면
	if (GetRootMtrl(pFbxMaterial) == -1)
	{
		if (iNumFbxMaterial > 1)
		{
			KMtrl* pMtrl = new KMtrl(pNode, pFbxMaterial);
			//서브메터리얼 수만큼 메테리얼 넣어준다 
			for (int iSub = 0; iSub < iNumFbxMaterial; iSub++)
			{
				FbxSurfaceMaterial* pFbxSubMaterial = pNode->GetMaterial(iSub);
				_ASSERT(pFbxSubMaterial != nullptr);
				KMtrl* pSubMtrl = new KMtrl(pNode, pFbxSubMaterial);
				pMtrl->m_pSubMtrl.push_back(pSubMtrl);
			}
			m_pFbxMaterialList.push_back(pMtrl);
		}
		else
		{
			if (pFbxMaterial != nullptr)
			{
				KMtrl* pMtrl = new KMtrl(pNode, pFbxMaterial);
				m_pFbxMaterialList.push_back(pMtrl);
			}
		}
	}
	int iNumChild = pNode->GetChildCount();
	for (int iNode = 0; iNode < iNumChild; iNode++)
	{
		FbxNode* pChildNode = pNode->GetChild(iNode);
		PreProcess(pChildNode);
	}
}
//파일이름, 쉐이더 인자로 받아 오브젝트 import
bool	KFbxObj::LoadObject(std::string filename, std::string shaderName)
{
	//fbx 로더 설정 fbx매니저 static으로 구성되어있음
	m_pFbxManager = FbxManager::Create();
	m_pFbxImporter = FbxImporter::Create(m_pFbxManager, "");
	m_pFbxScene = FbxScene::Create(m_pFbxManager, "");
	bool bRet = m_pFbxImporter->Initialize(filename.c_str());//디폴트값이 있음 안해도됨
	bRet = m_pFbxImporter->Import(m_pFbxScene);//씬에다가 저장해라
	FbxAxisSystem::MayaZUp.ConvertScene(m_pFbxScene);//축 시스템은 마야Z축버젼으로 설정함

	FbxNode* m_pRootNode = m_pFbxScene->GetRootNode();// 루트노드 가져와서
	PreProcess(m_pRootNode);// 이 안에 원하는게 다있기 때문에 전체 노드트리 순환
		//PreProcess로 FBXNODELIST에 모든 FBX 정보 받아옴
	//preprecess로 채워준 매터리얼 리스트로 매터리얼 로드함
	for (int iMtrl = 0; iMtrl < m_pFbxMaterialList.size(); iMtrl++)
	{
		KMtrl* pMtrl = m_pFbxMaterialList[iMtrl];
		LoadMaterial(pMtrl);
	}

	ParseAnimation();//애니메이션 끝 시작 몇 프레임인지 계산
	ParseNode(m_pRootNode, nullptr);//노드 해석 오브젝트 상속구조를 파악해 meshlist에 넣어준다.
	ParseAnimationNode();//모든 오브젝트를 돌면서 애니메이션 데이터 가져온다.
	//ParseAnimationNode(pNode, pMesh);
	for (int iMesh = 0; iMesh < m_pMeshList.size(); iMesh++)
	{
		KMesh* pMesh = m_pMeshList[iMesh];
		for (int iBone = 0; iBone < pMesh->m_pFbxNodeList.size(); iBone++)
		{
			KMesh* pLinkMesh = GetFindIndex(pMesh->m_pFbxNodeList[iBone]);
			pMesh->m_pMeshLinkList.push_back(pLinkMesh);
		}
		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (int iSubMesh = 0; iSubMesh < m_pMeshList[iMesh]->m_pSubMesh.size(); iSubMesh++)
			{
				KMesh* pSubMesh = m_pMeshList[iMesh]->m_pSubMesh[iSubMesh];
				// todo : 쉐이더 등등 중복처리 미작업
				pSubMesh->CreateModel(TBASIS::mtw(shaderName),
					TBASIS::mtw(shaderName), L"");
			}
		}
		else
		{
			pMesh->CreateModel(TBASIS::mtw(shaderName),
				TBASIS::mtw(shaderName),L"");
		}
	}
	m_pFbxScene->Destroy();
	m_pFbxImporter->Destroy();
	m_pFbxManager->Destroy();
	return bRet;
}
bool    KFbxObj::Release()
{
	for (int iObj = 0; iObj < m_pMeshList.size(); iObj++)
	{
		m_pMeshList[iObj]->Release();
		SAFE_DEL(m_pMeshList[iObj]);
	}
	for (int iObj = 0; iObj < m_pFbxMaterialList.size(); iObj++)
	{
		m_pFbxMaterialList[iObj]->Release();
		SAFE_DEL(m_pFbxMaterialList[iObj]);
	}
	return true;
}
