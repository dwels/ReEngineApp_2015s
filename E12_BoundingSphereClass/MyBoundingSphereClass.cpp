#include "MyBoundingSphereClass.h"
//  MyBoundingSphereClass
void MyBoundingSphereClass::Init(void)
{
	m_fRadius = 0.0f;
	m_m4ToWorld = IDENTITY_M4;

	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);
}
void MyBoundingSphereClass::Swap(MyBoundingSphereClass& other)
{
	std::swap(m_fRadius, other.m_fRadius);
	std::swap(m_m4ToWorld, other.m_m4ToWorld);

	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);
}
void MyBoundingSphereClass::Release(void)
{
	if (m_pSphere != nullptr)
	{
		delete m_pSphere;
		m_pSphere = nullptr;
	}
}
//The big 3
MyBoundingSphereClass::MyBoundingSphereClass(std::vector<vector3> a_lVectorList)
{
	vertexList = a_lVectorList;
	nVertexCount = vertexList.size();

	if (nVertexCount > 0)
	{
		m_v3Min = vertexList[0];
		m_v3Max = vertexList[0];
	}

	for (uint i = 0; i < nVertexCount; i++)
	{
		if (vertexList[i].x > m_v3Max.x)
			m_v3Max.x = vertexList[i].x;
		else if (vertexList[i].x < m_v3Min.x)
			m_v3Min.x = vertexList[i].x;

		if (vertexList[i].y > m_v3Max.y)
			m_v3Max.y = vertexList[i].y;
		else if (vertexList[i].y < m_v3Min.y)
			m_v3Min.y = vertexList[i].y;

		if (vertexList[i].z > m_v3Max.z)
			m_v3Max.z = vertexList[i].z;
		else if (vertexList[i].z < m_v3Min.z)
			m_v3Min.z = vertexList[i].z;
	}

	m_v3Center = (m_v3Max + m_v3Min) / 2.0f;
	m_fRadius = glm::distance(m_v3Center, m_v3Max);

	m_pSphere = new PrimitiveClass();
	m_pSphere->GenerateSphere(m_fRadius, 10, REGREEN);
}
MyBoundingSphereClass::MyBoundingSphereClass(MyBoundingSphereClass const& other)
{
	m_fRadius = other.m_fRadius;
	m_m4ToWorld = other.m_m4ToWorld;

	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;
}
MyBoundingSphereClass& MyBoundingSphereClass::operator=(MyBoundingSphereClass const& other)
{
	if(this != &other)
	{
		Release();
		Init();
		MyBoundingSphereClass temp(other);
		Swap(temp);
	}
	return *this;
}
MyBoundingSphereClass::~MyBoundingSphereClass(){Release();};
//Accessors
void MyBoundingSphereClass::SetModelMatrix(matrix4 a_m4ToWorld){ m_m4ToWorld = a_m4ToWorld; }
vector3 MyBoundingSphereClass::GetCenter(void){ return vector3(m_m4ToWorld * vector4(m_v3Center, 1.0f)); }
float MyBoundingSphereClass::GetRadius(void) { return m_fRadius; }
//--- Non Standard Singleton Methods
bool MyBoundingSphereClass::IsColliding(MyBoundingSphereClass* const a_pOther)
{
	if (glm::distance(this->GetCenter(), a_pOther->GetCenter()) < m_fRadius + a_pOther->GetRadius())
		return true;
	else
		return false;
}
void MyBoundingSphereClass::UpdatePosition(vector3 _v3Input)
{
	v3Position = _v3Input;
}

matrix4 MyBoundingSphereClass::GetModelMatrix() {
	return m_m4ToWorld;
}

vector3 MyBoundingSphereClass::GetPosition() {
	return v3Position;
}