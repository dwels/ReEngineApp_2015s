#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("309.02 - Fall 2015 Final"); // Window Name

	m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);
}
void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(REAXISZ * 130.0f, REAXISY * 0.0f, REAXISY);

	m_pBOMngr = MyBOManager::GetInstance();
	m_nInstances = 8;
	for (int i = 0; i < m_nInstances; i++)
	{
		String sInstance = "Cube_" + std::to_string(i);
		matrix4 m4Positions = glm::translate(vector3(glm::sphericalRand(35.0f)));
		m_pMeshMngr->LoadModel("Portal\\CompanionCube.bto", sInstance, false, m4Positions);
		m_pBOMngr->AddObject(sInstance);
	}
	m_pOctree = new MyOctant();
	m_pOctree->ConstructTree();
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
	
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pOctree->ConstructTree();
	m_pOctree->Display();
	m_pMeshMngr->AddInstanceToRenderList("ALL");
			
	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	printf("FPS: %d            \r", nFPS);//print the Frames per Second
	//Print info on the screen
	m_pMeshMngr->Print("Number of blocks: ");
	m_pMeshMngr->PrintLine(std::to_string(m_nInstances), REBLUE);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}
void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}
void AppClass::Release(void)
{
	SafeDelete(m_pOctree);
	m_pBOMngr->ReleaseInstance();
	super::Release(); //release the memory of the inherited fields
}