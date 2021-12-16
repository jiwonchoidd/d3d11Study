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

//���� �´� �ε����� �ٲ��ش�.
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
//ȣ��Ǳ����� ��� �Ž��� ����� �Ǿ� �־���Ѵ�.
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
		//�����ӿ� ��Ų�� �͵鸸 �޾ƿ´�.
		FbxDeformer* pFbxDeformer = pFbxMesh->GetDeformer(iDeformer, FbxDeformer::eSkin);
		//�����Ӹ� ��Ų���� ĳ����
		FbxSkin* pSkin = (FbxSkin*)pFbxDeformer;
		// ������ ��ġ�� ����� iNumCluster �ִ�.
		int iNumCluster = pSkin->GetClusterCount();
		pMesh->m_matBindPoseList.resize(iNumCluster);
		for (int iCluster = 0; iCluster < iNumCluster; iCluster++)
		{
			//�������� ���� �׸��µ� �Ϻ��� ������ ���ԵǴ� ��찡 ������
			//Ŭ������ Ư�� ������ ������ ��ġ�� ����� �ִ� ����ü
			FbxCluster* pCluster = pSkin->GetCluster(iCluster);
			int iNumVertex = pCluster->GetControlPointIndicesCount();

			//Tpose �� ���ư��� ���
			//�ʱ� ��ġ�� ���
			FbxAMatrix matXBindPose, matInitPostion;
			pCluster->GetTransformLinkMatrix(matXBindPose);
			pCluster->GetTransformMatrix(matInitPostion);
			//������ ���� �ٸ� ����� ������ �ֱ� ������ �� ��ǥ��� �ٲ�����Ѵ�.

			FbxAMatrix matBoneBindPos = matInitPostion.Inverse() * matXBindPose;
			KMatrix matBinePos = DxConvertMatrix(ConvertAMatrix(matBoneBindPos));
			// ������ ��ġ�� ����� iClusterSize ������ ������ ��ģ��.
			//������� �ѱ��.
			D3DKMatrixInverse(&matBinePos, NULL, &matBinePos);
			//�̰͸� ������۸� �Ѱ��൵ �Ǵ°�..
			pMesh->m_matBindPoseList[iCluster] = matBinePos;

			FbxNode* pLinkNode = pCluster->GetLink();
			pMesh->m_pFbxNodeList.push_back(pLinkNode);
			//ControlPoint(������) ��������Ʈ
			int* iIndex = pCluster->GetControlPointIndices();
			// ����ġ����Ʈ
			double* pWeight = pCluster->GetControlPointWeights();
			for (int i = 0; i < iNumVertex; i++)
			{
				pSkindata->m_VertexList[iIndex[i]].m_IndexList.push_back(iCluster);
				pSkindata->m_VertexList[iIndex[i]].m_WeightList.push_back(pWeight[i]);
				//iIndex[i] ������  iMatrixIndex����� pWeight[i]=1 ����ġ�� ������ ��ģ��.				
			}
		}
	}
	return true;
}
void    KFbxObj::ParseAnimStack(FbxString* szData)
{
	//�ִϸ��̼ǿ� ���� �����͸� �̾��ִ� getanimationevaluitator;
	m_pFbxScene->GetAnimationEvaluator()->Reset();

	FbxTakeInfo* pTakeInfo = m_pFbxScene->GetTakeInfo(*szData);
	FbxTime FrameTime;
	FrameTime.SetTime(0, 0, 0, 1, 0,
		m_pFbxScene->GetGlobalSettings().GetTimeMode());
	float fFrameTime = FrameTime.GetSecondDouble();
	float fFrameStep = 1;
	m_fSampleTime = fFrameTime * fFrameStep;
	//�о��� info�� ���� ��쿡 ���� Ÿ���� ����
	if (pTakeInfo)
	{
		//first last frame �ð����� ȯ���ؼ� �ð����� �и��ϰ� �� 
		m_fStartTime = (float)pTakeInfo->mLocalTimeSpan.GetStart().GetSecondDouble();
		m_fEndTime = (float)pTakeInfo->mLocalTimeSpan.GetStop().GetSecondDouble();
	}
	else
	{
		//�����̳ʰ� ���� �Ȱǵ������� ����Ʈ ��
		FbxTimeSpan tlTimeSpan;
		m_pFbxScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(tlTimeSpan);
		m_fStartTime = (float)tlTimeSpan.GetStart().GetSecondDouble();
		m_fEndTime = (float)tlTimeSpan.GetStop().GetSecondDouble();
	}
}
// Parse Animation Total Frame, Begin, End
//���ø� ����� �ִϸ��̼� �츮�� ���� ����, �ƽ��� �����
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
	//�ִϸ��̼� ������ ����
	FbxAnimEvaluator* pAnim = m_pFbxScene->GetAnimationEvaluator();
	float fCurrentTime = m_fStartTime;
	while (fCurrentTime < m_fEndTime)
	{
		FbxTime time;
		time.SetSecondDouble(fCurrentTime);
		for (int iMesh = 0; iMesh < m_pMeshList.size(); iMesh++)
		{
			KMesh* pMesh = m_pMeshList[iMesh];
			//todo : �θ������ �����ؾ��ϴ� ��?
			FbxAMatrix matGlobal = pAnim->GetNodeGlobalTransform(pMesh->m_pFbxNode, time);
			KMatrix matGlobaDX = DxConvertMatrix(ConvertAMatrix(matGlobal));
			pMesh->m_AnimationTrack.push_back(matGlobaDX);
		}
		fCurrentTime += m_fSampleTime;
	}
}
