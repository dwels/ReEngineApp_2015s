#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("E15: Entity Manager Expanded"); // Window Name

	m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);
}
void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(REAXISZ * 130.0f, REAXISY * 0.0f, REAXISY);

	m_pBOMngr = MyBOManager::GetInstance();

	m_nOctant = -1;
#ifdef DEBUG
	m_nInstances = 1000;
#else 
	m_nInstances = 5000;
#endif
	int nSquare = static_cast<int>(std::sqrt(m_nInstances));
	m_nInstances = nSquare * nSquare;
	for (int i = 0; i < nSquare; i++)
	{
		for (int j = 0; j < nSquare; j++)
		{
			String sInstance = "Cube_" + std::to_string(i) + "_" + std::to_string(j);
			matrix4 m4Positions = glm::translate(static_cast<float>(i - nSquare / 2.0f), static_cast<float>(j), 0.0f);
			m4Positions = glm::translate(vector3(glm::sphericalRand(35.0f)));
			m_pMeshMngr->LoadModel("Minecraft\\Cube.bto", sInstance, false, m4Positions);
			m_pBOMngr->AddObject(sInstance);
		}
	}
	m_pOctree = new MyOctant();
	m_pOctree->ConstructTree();

	m_bOctreeCalc = false;
	m_bDisplayOct = false;
	m_bDisplayColl = true;
	m_bDisplayGeom = true;
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

	//m_pOctree->ConstructTree();
	//m_pEntityMngr->Update();
	m_pMeshMngr->PrintLine("O|P Move up down", REWHITE);
	m_pMeshMngr->Print("Octant: ", REWHITE);
	m_pMeshMngr->PrintLine(std::to_string(m_nOctant), REYELLOW);
	if (m_nOctant < 0)
	{
		m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);
		m_pMeshMngr->Print("<K> Check Collisions: ", REWHITE);
		if (m_bOctreeCalc)
		{
			m_pMeshMngr->PrintLine("Brute force", RERED);
			m_pBOMngr->Update();
		}
		else
		{
			m_pMeshMngr->PrintLine("Spatial Optimization", REGREEN);
			m_pOctree->CheckCollisions();
		}
		m_pMeshMngr->Print("<H> Display Octree: ", REWHITE);
		if (m_bDisplayOct)
		{
			m_pMeshMngr->PrintLine("ON", REGREEN);
			m_pOctree->Display();
		}
		else
		{
			m_pMeshMngr->PrintLine("OFF", RERED);
		}
		m_pMeshMngr->Print("<J> Display Collisions: ", REWHITE);
		if (m_bDisplayColl)
		{
			m_pMeshMngr->PrintLine("ON", REGREEN);
			m_pBOMngr->DisplaySphere("ALL", vector3(-2.0f,-2.0f,-2.0f));
		}
		else
		{
			m_pMeshMngr->PrintLine("OFF", RERED);
		}
		m_pMeshMngr->Print("<G> Display Geometry: ", REWHITE);
		if (m_bDisplayGeom)
		{
			m_pMeshMngr->PrintLine("ON", REGREEN);
			m_pMeshMngr->AddInstanceToRenderList("ALL");
		}
		else
		{
			m_pMeshMngr->PrintLine("OFF", RERED);
		}
	}
	else
	{
		m_v4ClearColor = vector4(0.0f, 0.0f, 0.0f, 0.0f);
		//m_pOctree->Display(REWHITE);
		m_pOctree->Display(m_nOctant);
	}
	
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
	//Render the grid based on the camera's mode:
	//m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
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