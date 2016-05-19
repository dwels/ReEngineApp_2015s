#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("DSA2 Final: Your Name goes here"); // Window Name
	m_pSystem->SetWindowWidth(600);
	m_pSystem->SetWindowHeight(600);
}

void AppClass::InitVariables(void)
{
	//Set the camera position
	m_pCameraMngr->SetPositionTargetAndView(
		vector3(0.0f, 0.0f, 15.0f),//Camera position
		vector3(0.0f, 0.0f, 0.0f),//What Im looking at
		REAXISY);//What is up
	//Load a model onto the Mesh manager
	m_pMeshMngr->LoadModel("Minecraft\\Creeper.obj", "Creeper");
	m_pBO = new MyBOClass(m_pMeshMngr->GetVertexList("Creeper"));
}
void AppClass::UpdateOrientation(void)
{
	static vector3 v3Orientation;
	
	m_m4Orientation = glm::rotate(IDENTITY_M4, v3Orientation.x, REAXISX);
	m_m4Orientation = glm::rotate(m_m4Orientation, v3Orientation.y, REAXISY);
	m_m4Orientation = glm::rotate(m_m4Orientation, v3Orientation.z, REAXISZ);
	
	m_m4Orientation = m_m4Orientation * ToMatrix4(quaternion(vector3(glm::radians(1.0f), glm::radians(1.0f), glm::radians(1.0f))));
	v3Orientation += vector3(1.0f);
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	static int nClock = m_pSystem->GenClock();
	static double dTime = 0.0;
	dTime += m_pSystem->LapClock(nClock);
	std::vector<vector3> lSteps;
	lSteps.push_back(vector3(-2.5f, -2.5f, 0.0f));
	lSteps.push_back(vector3(2.5f, -2.5f, 0.0f));
	lSteps.push_back(vector3(2.5f, 2.5f, 0.0f));
	lSteps.push_back(vector3(-2.5f, 2.5f, 0.0f));
	
	static uint nRoute = 0;
	vector3 v3Start;
	vector3 v3End;
	if (nRoute < lSteps.size() - 1)
	{
		v3Start = lSteps[nRoute];
		v3End = lSteps[nRoute +1];
	}
	else
	{ 
		v3Start = lSteps[nRoute];
		v3End = lSteps[0];
	}
	float fProgress = static_cast<float>(MapValue(dTime, 0.0, 2.0, 0.0, 1.0));
	vector3 vPosition = glm::lerp(v3Start, v3End, fProgress);
	if (fProgress >= 1.0f)
	{
		nRoute++;
		nRoute %= lSteps.size();
		dTime = 0.0;
	}

	UpdateOrientation();

	matrix4 m4Position = glm::translate(vPosition) * m_m4Orientation;
	
	//Set the model matrix for the first model to be the arcball
	m_pMeshMngr->SetModelMatrix(m4Position, "Creeper");
	m_pBO->SetModelMatrix(m4Position);

	//Adds all loaded instances to the render list
	m_pBO->DisplayOriented(RERED);
	m_pBO->DisplayReAlligned(REYELLOW);
	m_pMeshMngr->AddSkyboxToRenderList();
	m_pMeshMngr->AddInstanceToRenderList("Creeper");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	SafeDelete(m_pBO);
	super::Release(); //release the memory of the inherited fields
}