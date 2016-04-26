#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Elsberry Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");

	fDuration = 1.0f;

	charMatrix = IDENTITY_M4;

	std::vector<vector3> points = {		//size 11
		vector3(-4.0f, -2.0f, 5.0f),	//0 time is 5
		vector3(1.0f, -2.0f, 5.0f),		//1 time is 10
		vector3(-3.0f, -1.0f, 3.0f),	//2 time is 15
		vector3(2.0f, -1.0f, 3.0f),		//3 time is 20
		vector3(-2.0f, 0.0f, 0.0f),		//4 ..25
		vector3(3.0f, 0.0f, 0.0f),		//5	..30
		vector3(-1.0f, 1.0f, -3.0f),	//6	..35
		vector3(4.0f, 1.0f, -3.0f),		//7 ..40
		vector3(0.0f, 2.0f, -5.0f),		//8 ..45
		vector3(5.0f, 2.0f, -5.0f),		//9 ..50
		vector3(1.0f, 3.0f, -5.0f)		//10 ..55
	};

	Points = points;

	pntNum = 11;

	sphere = new PrimitiveClass[pntNum];
	sphereMat = new matrix4[pntNum];

	for (int i = 0; i < pntNum; i++)
	{
		sphere[i].GenerateSphere(0.1f, 5, RERED);
		sphereMat[i] = glm::translate(points[i]);
	}
}

void AppClass::Update(void)
{
#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	double fTimeSpan = m_pSystem->LapClock(); //Delta time (between frame calls)

	//cumulative time
	static double fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fTimeSpan; 
	if (fRunTime > 55.0f) fRunTime = 0.0f;
#pragma endregion

#pragma region Your Code goes here
	m_pMeshMngr->SetModelMatrix(charMatrix, "WallEye");

	if (fRunTime <= 5) 
		charMatrix = glm::translate(IDENTITY_M4, glm::lerp(Points[0], Points[1], static_cast<float>(fRunTime / 5)));

	if (fRunTime <= 10 && fRunTime > 5)
		charMatrix = glm::translate(IDENTITY_M4, glm::lerp(Points[1], Points[2], static_cast<float>((fRunTime -5) / 5)));

	if (fRunTime <= 15 && fRunTime > 10)
		charMatrix = glm::translate(IDENTITY_M4, glm::lerp(Points[2], Points[3], static_cast<float>((fRunTime - 10) / 5)));

	if (fRunTime <= 20 && fRunTime > 15)
		charMatrix = glm::translate(IDENTITY_M4, glm::lerp(Points[3], Points[4], static_cast<float>((fRunTime - 15) / 5)));
	
	if (fRunTime <= 25 && fRunTime > 20)
		charMatrix = glm::translate(IDENTITY_M4, glm::lerp(Points[4], Points[5], static_cast<float>((fRunTime - 20) / 5)));

	if (fRunTime <= 30 && fRunTime > 25)
		charMatrix = glm::translate(IDENTITY_M4, glm::lerp(Points[5], Points[6], static_cast<float>((fRunTime - 25) / 5)));

	if (fRunTime <= 35 && fRunTime > 30)
		charMatrix = glm::translate(IDENTITY_M4, glm::lerp(Points[6], Points[7], static_cast<float>((fRunTime - 30) / 5)));

	if (fRunTime <= 40 && fRunTime > 35)
		charMatrix = glm::translate(IDENTITY_M4, glm::lerp(Points[7], Points[8], static_cast<float>((fRunTime - 35) / 5)));

	if (fRunTime <= 45 && fRunTime > 40)
		charMatrix = glm::translate(IDENTITY_M4, glm::lerp(Points[8], Points[9], static_cast<float>((fRunTime - 40) / 5)));

	if (fRunTime <= 50 && fRunTime > 45)
		charMatrix = glm::translate(IDENTITY_M4, glm::lerp(Points[9], Points[10], static_cast<float>((fRunTime - 45) / 5)));

	if (fRunTime <= 55 && fRunTime > 50)
		charMatrix = glm::translate(IDENTITY_M4, glm::lerp(Points[10], Points[0], static_cast<float>((fRunTime - 50) / 5)));

#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default: //Perspective
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY); //renders the XY grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
	
	matrix4 mProjection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 mView = m_pCameraMngr->GetViewMatrix();

	for (int i = 0; i < pntNum; i++)
	{
		sphere[i].Render(mProjection, mView, sphereMat[i]);
	}

	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	if (sphere != nullptr)
	{
		delete[] sphere; //new[] delete[], new()->delete
		sphere = nullptr;
	}
	if (sphereMat != nullptr)
	{
		delete[] sphereMat; //new[] delete[], new()->delete
		sphereMat = nullptr;
	}
	super::Release(); //release the memory of the inherited fields
}