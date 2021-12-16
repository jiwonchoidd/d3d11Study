#include "KFbxObj.h"

void KFbxObj::SelectedAnim(float start, float end)
{
	m_fElpaseTime += 1.0f * g_fSecPerFrame;
	m_iAnimIndex = (m_fElpaseTime + start) * 30.0f;
	if (m_fElpaseTime > end)
	{
		m_fElpaseTime = 0.0f;
	}
}

//값에 맞는 인덱스로 바꿔준다.
KMesh* KFbxObj::GetFindIndex(FbxNode* pNode)
{
	for (int iNode = 0; iNode < m_pMeshList.size(); iNode++)
	{
		KMesh* pMesh = m_pMeshList[iNode];
		if (pMesh->m_pFbxNode == pNode)
		{
			return pMesh;
		}
	}
	return nullptr;
}
//호출되기전에 모든 매쉬가 등록이 되어 있어야한다.
bool KFbxObj::ParseMeshSkinning(FbxMesh* pFbxMesh, KMesh* pMesh, KSkinData* pSkindata)
{
	int iNumDeformer = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (iNumDeformer == 0)
	{
		return false;
	}
	int iNumVertexCount = pFbxMesh->GetControlPointsCount();
	pSkindata->m_VertexList.resize(iNumVertexCount);

	for (int iDeformer = 0; iDeformer < iNumDeformer; iDeformer++)
	{
		//디포머에 스킨인 것들만 받아온다.
		FbxDeformer* pFbxDeformer = pFbxMesh->GetDeformer(iDeformer, FbxDeformer::eSkin);
		//디포머를 스킨으로 캐스팅
		FbxSkin* pSkin = (FbxSkin*)pFbxDeformer;
		// 영향을 미치는 행렬이 iNumCluster 있다.
		int iNumCluster = pSkin->GetClusterCount();
		pMesh->m_matBindPoseList.resize(iNumCluster);
		for (int iCluster = 0; iCluster < iNumCluster; iCluster++)
		{
			//정점들이 원을 그리는데 일부의 정점만 포함되는 경우가 있을때
			//클러스터 특정 정점에 영향을 미치는 행렬이 있는 구조체
			FbxCluster* pCluster = pSkin->GetCluster(iCluster);
			int iNumVertex = pCluster->GetControlPointIndicesCount();

			//Tpose 로 돌아가는 행렬
			//초기 위치값 행렬
			FbxAMatrix matXBindPose, matInitPostion;
			pCluster->GetTransformLinkMatrix(matXBindPose);
			pCluster->GetTransformMatrix(matInitPostion);
			//정점당 서로 다른 행렬을 가지고 있기 때문에 본 좌표계로 바꿔줘야한다.

			FbxAMatrix matBoneBindPos = matInitPostion.Inverse() * matXBindPose;
			KMatrix matBinePos = DxConvertMatrix(ConvertAMatrix(matBoneBindPos));
			// 영향을 미치는 행렬이 iClusterSize 정점에 영향을 미친다.
			//역행렬을 넘긴다.
			D3DKMatrixInverse(&matBinePos, NULL, &matBinePos);
			//이것만 상수버퍼를 넘겨줘도 되는것..
			pMesh->m_matBindPoseList[iCluster] = matBinePos;

			FbxNode* pLinkNode = pCluster->GetLink();
			pMesh->m_pFbxNodeList.push_back(pLinkNode);
			//ControlPoint(제어점) 정점리스트
			int* iIndex = pCluster->GetControlPointIndices();
			// 가중치리스트
			double* pWeight = pCluster->GetControlPointWeights();
			for (int i = 0; i < iNumVertex; i++)
			{
				pSkindata->m_VertexList[iIndex[i]].m_IndexList.push_back(iCluster);
				pSkindata->m_VertexList[iIndex[i]].m_WeightList.push_back(pWeight[i]);
				//iIndex[i] 정점은  iMatrixIndex행렬이 pWeight[i]=1 가중치로 영향을 미친다.				
			}
		}
	}
	return true;
}
void    KFbxObj::ParseAnimStack(FbxString* szData)
{
	//애니메이션에 대한 데이터를 뽑아주는 getanimationevaluitator;
	m_pFbxScene->GetAnimationEvaluator()->Reset();

	FbxTakeInfo* pTakeInfo = m_pFbxScene->GetTakeInfo(*szData);
	FbxTime FrameTime;
	FrameTime.SetTime(0, 0, 0, 1, 0,
		m_pFbxScene->GetGlobalSettings().GetTimeMode());
	float fFrameTime = FrameTime.GetSecondDouble();
	float fFrameStep = 1;
	m_fSampleTime = fFrameTime * fFrameStep;
	//읽어진 info가 있을 경우에 로컬 타임을 넣음
	if (pTakeInfo)
	{
		//first last frame 시간으로 환산해서 시간으로 분리하게 함 
		m_fStartTime = (float)pTakeInfo->mLocalTimeSpan.GetStart().GetSecondDouble();
		m_fEndTime = (float)pTakeInfo->mLocalTimeSpan.GetStop().GetSecondDouble();
	}
	else
	{
		//디자이너가 설정 안건드렸을경우 디폴트 값
		FbxTimeSpan tlTimeSpan;
		m_pFbxScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(tlTimeSpan);
		m_fStartTime = (float)tlTimeSpan.GetStart().GetSecondDouble();
		m_fEndTime = (float)tlTimeSpan.GetStop().GetSecondDouble();
	}
}
// Parse Animation Total Frame, Begin, End
//샘플링 방식의 애니메이션 우리는 선형 보간, 맥스는 곡선보간
void	KFbxObj::ParseAnimation()
{
	FbxArray<FbxString*> AnimStackNameArray;
	m_pFbxScene->FillAnimStackNameArray(AnimStackNameArray);
	int iAnimStackCount = AnimStackNameArray.GetCount();
	for (int iStack = 0; iStack < iAnimStackCount; iStack++)
	{
		ParseAnimStack(AnimStackNameArray.GetAt(iStack));
	}
}
// Get Animation Data All Mesh Parse except Camera, Light 
void	KFbxObj::ParseAnimationNode()
{
	//애니메이션 데이터 저장
	FbxAnimEvaluator* pAnim = m_pFbxScene->GetAnimationEvaluator();
	float fCurrentTime = m_fStartTime;
	while (fCurrentTime < m_fEndTime)
	{
		FbxTime time;
		time.SetSecondDouble(fCurrentTime);
		for (int iMesh = 0; iMesh < m_pMeshList.size(); iMesh++)
		{
			KMesh* pMesh = m_pMeshList[iMesh];
			//todo : 부모행렬을 제외해야하는 것?
			FbxAMatrix matGlobal = pAnim->GetNodeGlobalTransform(pMesh->m_pFbxNode, time);
			KMatrix matGlobaDX = DxConvertMatrix(ConvertAMatrix(matGlobal));
			pMesh->m_AnimationTrack.push_back(matGlobaDX);
		}
		fCurrentTime += m_fSampleTime;
	}
}
