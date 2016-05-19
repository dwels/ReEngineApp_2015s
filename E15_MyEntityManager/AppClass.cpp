#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("E15: Entity Manager"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(REAXISZ * 30.0f, ZERO_V3, REAXISY);

	//Load Models
	m_pMeshMngr->LoadModel("Minecraft\\Steve.obj", "Steve");
	m_pMeshMngr->LoadModel("Minecraft\\Creeper.obj", "Creeper");
	m_pMeshMngr->LoadModel("Minecraft\\Creeper.obj", "Creeper1");
	m_pMeshMngr->LoadModel("Minecraft\\Creeper.obj", "Creeper2");
	m_pMeshMngr->LoadModel("Minecraft\\Cow.obj", "Cow0");
	m_pMeshMngr->LoadModel("Minecraft\\Cow.obj", "Cow1");

	String sEntity;
	m_pEntityMngr = MyEntityManager::GetInstance();

	//Friction goes between 0 and 1
	float fFriction = 0.10f;
	m_fForce = 10.0f; //Units per second

	sEntity = "Steve";
	m_pEntityMngr->AddEntity(sEntity, 1.5f);
	m_pEntityMngr->SetFriction(fFriction, sEntity);
	m_pEntityMngr->SetPosition(REAXISX * -5.0f, sEntity);

	sEntity = "Creeper";
	m_pEntityMngr->AddEntity(sEntity, 0.8f);
	m_pEntityMngr->SetFriction(fFriction, sEntity);
	m_pEntityMngr->SetPosition(REAXISX * 2.5f, sEntity);

	sEntity = "Creeper1";
	m_pEntityMngr->AddEntity(sEntity, 0.8f);
	m_pEntityMngr->SetFriction(fFriction, sEntity);
	m_pEntityMngr->SetPosition(REAXISX * 0.0f, sEntity);

	sEntity = "Creeper2";
	m_pEntityMngr->AddEntity(sEntity, 0.8f);
	m_pEntityMngr->SetFriction(fFriction, sEntity);
	m_pEntityMngr->SetPosition(REAXISX * -2.5f, sEntity);

	sEntity = "Cow0";
	m_pEntityMngr->AddEntity(sEntity, 9000.1f);
	m_pEntityMngr->SetFriction(fFriction, sEntity);
	m_pEntityMngr->SetPosition(REAXISX * -15.0f, sEntity);

	sEntity = "Cow1";
	m_pEntityMngr->AddEntity(sEntity, 9000.1f);
	m_pEntityMngr->SetFriction(fFriction, sEntity);
	m_pEntityMngr->SetPosition(REAXISX * 15.0f, sEntity);
	
	int nEntities = 33;
	int nPos = nEntities / 2;
	for (int n = 0; n < nEntities; n++)
	{
		sEntity = "Cube";
		sEntity = sEntity + std::to_string(n);
		m_pMeshMngr->LoadModel("Minecraft\\Cube.obj", sEntity);
		m_pEntityMngr->AddEntity(sEntity, 9000.0f);
		m_pEntityMngr->SetPosition(vector3(n - nPos,-1.0f, -0.5f), sEntity);
		m_pEntityMngr->SetGravityAffected(false, sEntity);
	}
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
	
	m_pEntityMngr->Update();

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	printf("FPS: %d            \r", nFPS);//print the Frames per Second
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("Press ", REWHITE);
	m_pMeshMngr->Print("ENTER ", REYELLOW);
	m_pMeshMngr->PrintLine("to apply force from the sides", REWHITE);

	m_pMeshMngr->Print("Press ", REWHITE);
	m_pMeshMngr->Print("SPACE ", REYELLOW);
	m_pMeshMngr->PrintLine("to apply force from the below", REWHITE);
	
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	m_pEntityMngr->Display(ER_MESH | ER_OB); //Display all objects in Entity manager
	
	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	m_pEntityMngr->ReleaseInstance();
	super::Release(); //release the memory of the inherited fields
}