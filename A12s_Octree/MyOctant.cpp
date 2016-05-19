#include "MyOctant.h"
//  MyOctant
uint MyOctant::m_nOctantCount = 0;
uint MyOctant::m_nMaxLevel = 3;
uint MyOctant::m_nIdealBOCount = 5;
void MyOctant::Init(void)
{
	m_nChildren = 0;

	m_fSize = 0.0f;

	m_nID = m_nOctantCount;
	m_nLevel = 0;

	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);

	m_pMeshMngr = MeshManagerSingleton::GetInstance();
	m_pBOMngr = MyBOManager::GetInstance();

	m_pParent = nullptr;
	for (uint n = 0; n < 8; n++ )
	{
		m_pChild[n] = nullptr;
	}
}
void MyOctant::Swap(MyOctant& other)
{
	std::swap(m_nChildren, other.m_nChildren);

	std::swap(m_fSize, other.m_fSize);
	std::swap(m_nID, other.m_nID);

	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);

	std::swap(m_pMeshMngr, other.m_pMeshMngr);
	std::swap(m_pBOMngr, other.m_pBOMngr);

	std::swap(m_nLevel, other.m_nLevel);
	std::swap(m_pParent, other.m_pParent);
	for (uint i = 0; i < 8; i++)
	{
		std::swap(m_pChild[i], other.m_pChild[i]);
	}
}
MyOctant * MyOctant::GetParent(void){ return m_pParent; }
void MyOctant::Release(void)
{
	//this is a special kind of release, it will only happen for the root
	if (m_nLevel == 0)
	{
		KillBranches();
	}
	m_nChildren = 0;
	m_fSize = 0.0f;
	m_lBOs.clear();
}
//The big 3
MyOctant::MyOctant(void)
{
	//Init the default values
	Init();

	std::vector<vector3> lMinMax; //Will hold all of the Min and Max vectors of the Bounding Object
	
	uint nObjects = m_pBOMngr->GetObjectCount();
	for (uint i = 0; i < nObjects; i++)
	{
		MyBOClass* pBO = m_pBOMngr->GetBoundingObject(i);
		lMinMax.push_back(pBO->GetMinG());
		lMinMax.push_back(pBO->GetMaxG());
	}
	MyBOClass* pBO = new MyBOClass(lMinMax);

	vector3 vHalfWidth = pBO->GetHalfWidthG();
	float fMax = vHalfWidth.x;
	for (int i = 1; i < 3; i++)
	{
		if (fMax < vHalfWidth[i])
			fMax = vHalfWidth[i];
	}
	vector3 v3Center = pBO->GetCenterLocal();
	lMinMax.clear();
	SafeDelete(pBO);
	
	m_fSize = fMax * 2.0f;
	m_v3Center = v3Center;
	m_v3Min = m_v3Center - (vector3(fMax));
	m_v3Max = m_v3Center + (vector3(fMax));

	m_nOctantCount++;
}
MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	//Init the default values
	Init();
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;

	m_v3Min = m_v3Center - (vector3(m_fSize) / 2.0f);
	m_v3Max = m_v3Center + (vector3(m_fSize) / 2.0f);
	
	m_nOctantCount++;
}
MyOctant::MyOctant(MyOctant const& other)
{
	m_nChildren = other.m_nChildren;
	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;

	m_pMeshMngr = other.m_pMeshMngr;
	m_pBOMngr = other.m_pBOMngr;
	m_fSize = other.m_fSize;
	m_nID = other.m_nID;
	m_nLevel = other.m_nLevel;
	m_pParent = other.m_pParent;
	for (uint i = 0; i < 8; i++)
	{
		m_pChild[i] = other.m_pChild[i];
	}
}
MyOctant& MyOctant::operator=(MyOctant const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}
	return *this;
}
MyOctant::~MyOctant() { Release(); };
//Accessors
float MyOctant::GetSize(void) { return m_fSize; }
vector3 MyOctant::GetCenterGlobal(void) { return m_v3Center; }
vector3 MyOctant::GetMinGlobal(void) { return m_v3Min; }
vector3 MyOctant::GetMaxGlobal(void) { return m_v3Max; }
//--- Non Standard Singleton Methods
void MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
{
	if (m_nID == a_nIndex)
	{
		m_pMeshMngr->AddCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
			glm::scale(vector3(m_fSize)), a_v3Color, WIRE);

		//Display BO
		int nBOs = static_cast<int>(m_lBOs.size());
		for (int nIndex = 0; nIndex < nBOs; nIndex++)
		{
			m_pMeshMngr->AddInstanceToRenderList(m_lBOs[nIndex]);
		}

		return;
	}
	for (uint nIndex = 0; nIndex < m_nChildren; nIndex++)
	{
		m_pChild[nIndex]->Display(a_nIndex);
	}
}
void MyOctant::Display(vector3 a_v3Color)
{
	for (uint nIndex = 0; nIndex < m_nChildren; nIndex++)
	{
		m_pChild[nIndex]->Display(a_v3Color);
	}
	m_pMeshMngr->AddCubeToRenderList( glm::translate(IDENTITY_M4, m_v3Center) *
		glm::scale(vector3(m_fSize)), a_v3Color, WIRE);
}
void MyOctant::Subdivide(void)
{
	//If this node has reach the maximum depth return without canges
	if (m_nLevel >= m_nMaxLevel)
		return;

	//If this node has been already subdivided return without changes
	if (m_nChildren != 0)
		return;

	//As this will have children it will not be a leaf
	m_nChildren = 8;

	float fSize = m_fSize / 4.0f;
	float fSizeD = fSize * 2.0f;
	vector3 v3Center;
	//Octant 0 Bottom Left Back
	v3Center = m_v3Center;
	v3Center.x -= fSize;
	v3Center.y -= fSize;
	v3Center.z -= fSize;
	m_pChild[0] = new MyOctant(v3Center, fSizeD);
	//Octant 1 Bottom Right Back
	v3Center.x += fSizeD;
	m_pChild[1] = new MyOctant(v3Center, fSizeD);

	//Octant 2 Bottom Right Front
	v3Center.z += fSizeD;
	m_pChild[2] = new MyOctant(v3Center, fSizeD);

	//Octant 3 Bottom Left Front
	v3Center.x -= fSizeD;
	m_pChild[3] = new MyOctant(v3Center, fSizeD);

	//Octant 4 Top Left Front
	v3Center.y += fSizeD;
	m_pChild[4] = new MyOctant(v3Center, fSizeD);

	//Octant 5 Top Left Back
	v3Center.z -= fSizeD;
	m_pChild[5] = new MyOctant(v3Center, fSizeD);

	//Octant 6 Top Right Back
	v3Center.x += fSizeD;
	m_pChild[6] = new MyOctant(v3Center, fSizeD);

	//Octant 7 Top Right Front
	v3Center.z += fSizeD;
	m_pChild[7] = new MyOctant(v3Center, fSizeD);

	for (uint nIndex = 0; nIndex < 8; nIndex++)
	{
		m_pChild[nIndex]->m_pParent = this;
		m_pChild[nIndex]->m_nLevel = m_nLevel +1;
		if (m_pChild[nIndex]->ContainsMoreThan(m_nIdealBOCount))
		{
			m_pChild[nIndex]->Subdivide();
		}
	}
}
MyOctant * MyOctant::GetChild(uint a_nChild)
{
	if (a_nChild > 7) return nullptr;
	return m_pChild[a_nChild];
}
bool MyOctant::IsColliding(uint a_nBOIndex)
{
	//If the index given is larger than the number of elements in the bounding object there is no collision
	if (a_nBOIndex >= m_pBOMngr->GetObjectCount())
		return false;
	//As the Octree will never rotate or scale this collision is as easy as an Axis Alligned Bounding Box
	//Get all vectors in global space (the octant ones are already in Global)
	MyBOClass* pOther = m_pBOMngr->GetBoundingObject(a_nBOIndex);
	vector3 v3MinO = pOther->GetMinG();
	vector3 v3MaxO = pOther->GetMaxG();

	//Check for X
	if (m_v3Max.x < v3MinO.x)
		return false;
	if (m_v3Min.x > v3MaxO.x)
		return false;

	//Check for Y
	if (m_v3Max.y < v3MinO.y)
		return false;
	if (m_v3Min.y > v3MaxO.y)
		return false;

	//Check for Z
	if (m_v3Max.z < v3MinO.z)
		return false;
	if (m_v3Min.z > v3MaxO.z)
		return false;

	return true;
}
bool MyOctant::IsLeaf(void) { return m_nChildren == 0; }
bool MyOctant::ContainsMoreThan(uint a_nBOs)
{
	uint nCount = 0;
	for (uint n = 0; n < m_pBOMngr->GetObjectCount(); n++)
	{
		if (IsColliding(n))
			nCount++;
		if (nCount > a_nBOs)
			return true;
	}
	return false;
}
void MyOctant::KillBranches(void)
{
	/*This method has recursive behaviour that is somewhat hard to explain line by line but 
	it will traverse the whole tree until it reaches a node with no childs and
	once it returns from it to its parent it will kill all of its childs, recursibly until
	it reaches back the node that called it.*/
	for (uint nIndex = 0; nIndex < m_nChildren; nIndex++)
	{
		m_pChild[nIndex]->KillBranches();
		delete m_pChild[nIndex];
		m_pChild[nIndex] = nullptr;
	}
	m_nChildren = 0;
}
void MyOctant::ConstructTree(void)
{
	//If this method is tried to be applied to something else
	//other than the root, don't.
	if (m_nLevel != 0)
		return;

	m_nOctantCount = 1;

	//clear the tree
	KillBranches();

	//If the base tree
	if (ContainsMoreThan(m_nIdealBOCount))
	{
		Subdivide();
	}

	//Add octant ID to BOs
	AssignIDtoBO();
}
void MyOctant::AssignIDtoBO(void)
{
	//traverse until you reach a leaf
	for (uint nChild = 0; nChild < m_nChildren; nChild++)
	{
		m_pChild[nChild]->AssignIDtoBO();
	}
	if(m_nChildren == 0) //if this is a leaf
	{
		uint nBOs = m_pBOMngr->GetObjectCount();
		for (uint nIndex = 0; nIndex < nBOs; nIndex++)
		{
			if (IsColliding(nIndex))
			{
				m_lBOs.push_back(nIndex);
			}
		}
	}
}
void MyOctant::CheckCollisions(void)
{
	m_pBOMngr->ClearCollidingLists();
	TraverseForCollisions();
}
void MyOctant::TraverseForCollisions(void)
{
	//traverse to leafs
	for (uint nChild = 0; nChild < m_nChildren; nChild++)
	{
		if(m_pChild[nChild] != nullptr)
			m_pChild[nChild]->TraverseForCollisions();
	}
	//Check sorted collision
	int nBOs = static_cast<int>(m_lBOs.size());
	for (int nIndexA = 0; nIndexA < nBOs - 1; nIndexA++)
	{
		for (int nIndexB = nIndexA + 1; nIndexB < nBOs; nIndexB++)
		{
			m_pBOMngr->CheckCollisionBetween(m_lBOs[nIndexA], m_lBOs[nIndexB]);
		}
	}
}