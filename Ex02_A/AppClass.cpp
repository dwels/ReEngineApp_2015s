#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("FINAL"); // Window Name
	m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f); //Clear Color
}

void AppClass::InitVariables(void)
{
	//Set the camera at a position other than the default
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 5.0f, 25.0f), vector3(0.0f, 0.0f, 0.0f), REAXISY);

	//Load models onto the Mesh manager
	m_pMeshMngr->LoadModel("Sorted\\PolyOut.obj", "PolyOut");
	m_pMeshMngr->LoadModel("Sorted\\QuestionBlock.obj", "QuestionBlock");
	m_pMeshMngr->LoadModel("Sorted\\Polycube.obj", "Polycube");
	m_pMeshMngr->LoadModel("Portal\\CompanionCube.bto", "CompanionCube");
	m_pMeshMngr->LoadModel("Sorted\\Lego.obj", "Lego");
	m_pMeshMngr->LoadModel("Sorted\\IronMan.obj", "IronMan");
	m_pMeshMngr->LoadModel("Sorted\\A.obj", "A");
	m_pMeshMngr->LoadModel("Portal\\Wheatley.bto", "Wheatley");

	m_pMeshMngr->LoadModel("Zelda\\HylianShield.bto", "HylianShield");
	m_pMeshMngr->LoadModel("Zelda\\MasterSwordInSheath.bto", "MasterSwordInSheath");

	m_pMeshMngr->LoadModel("Minecraft\\Cow.obj", "Cow");
	m_pMeshMngr->LoadModel("Minecraft\\Creeper.obj", "Creeper");
	m_pMeshMngr->LoadModel("Minecraft\\Cube.obj", "Cube");
	m_pMeshMngr->LoadModel("Minecraft\\Steve.obj", "Steve");
	m_pMeshMngr->LoadModel("Minecraft\\Zombie.obj", "Zombie");
	m_pMeshMngr->LoadModel("Minecraft\\Pig.obj", "Pig");

	m_pMeshMngr->LoadModel("Planets\\00_Sun.obj", "00_Sun");
	m_pMeshMngr->LoadModel("Planets\\01_Mercury.obj", "01_Mercury");
	m_pMeshMngr->LoadModel("Planets\\02_Venus.obj", "02_Venus");
	m_pMeshMngr->LoadModel("Planets\\03_Earth.obj", "03_Earth");
	m_pMeshMngr->LoadModel("Planets\\03A_Moon.obj", "03A_Moon");
	m_pMeshMngr->LoadModel("Planets\\04_Mars.obj", "04_Mars");
	m_pMeshMngr->LoadModel("Planets\\05_Jupiter.obj", "05_Jupiter");
	m_pMeshMngr->LoadModel("Planets\\06_Saturn.obj", "06_Saturn");
	m_pMeshMngr->LoadModel("Planets\\07_Uranus.obj", "07_Uranus");
	m_pMeshMngr->LoadModel("Planets\\08_Neptune.obj", "08_Neptune");
	m_pMeshMngr->LoadModel("Planets\\09_Pluto.obj", "09_Pluto");
	
	m_pMeshMngr->LoadModel("Sorted\\WarpPipe.ato", "WarpPipe");

	//Create BOs for all models
	for (uint n = 0; n < m_pMeshMngr->GetInstanceCount() - 1; n++)
	{
		m_lBO.push_back(new MyBOClass(m_pMeshMngr->GetVertexList(n)));
	}

	//Number of objects shot
	m_nShoot = 0;

	//Origin of shoot
	m_v3Origin = vector3(0.0f, 5.0f, 0.0f);

	//List of WarpPipe Positions
	m_lSteps.push_back(vector3(-5.0f, 8.0f, 5.0f));
	m_lSteps.push_back(vector3(-5.0f, 8.0f, 0.0f));
	m_lSteps.push_back(vector3( 0.0f, 8.0f, 0.0f));
	m_lSteps.push_back(vector3( 5.0f, 8.0f,-5.0f));
	m_lSteps.push_back(vector3( 5.0f, 8.0f, 5.0f));
	m_lSteps.push_back(vector3( 0.0f, 8.0f, 0.0f));
	m_lSteps.push_back(vector3(-5.0f, 8.0f,-5.0f));
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

	//Generate a clock
	static uint nClock = m_pSystem->GenClock();
	//Create a count for time differences
	static double dDeltaTime = 0.0f;
	dDeltaTime = m_pSystem->LapClock(nClock);
	//Count the time increments
	static double dTimeTotal = 0.0f;
	dTimeTotal += dDeltaTime;
	//Create a Percentage of progress in this time
	float fPercentage = static_cast<float>(MapValue(dTimeTotal, 0.0, 2.0, 0.0, 1.0));

	//Check the route this object is
	static uint nRoute = 0;
	if(nRoute < m_lSteps.size() - 1)
		m_v3Origin = glm::lerp(m_lSteps[nRoute], m_lSteps[nRoute+1], fPercentage);
	else
		m_v3Origin = glm::lerp(m_lSteps[nRoute], m_lSteps[0], fPercentage);
	
	//If done with the route
	if (fPercentage >= 1)
	{
		dTimeTotal = 0.0f;
		nRoute++;
		if (nRoute >= m_lSteps.size())
			nRoute = 0;
	}

	//Add the Pipe to the render list
	m_pMeshMngr->SetModelMatrix(
		glm::translate(m_v3Origin + vector3(0.0f, 3.0f, 0.0f)) * 
		glm::rotate(IDENTITY_M4, 180.0f, vector3(1.0f, 0.0f, 0.0f)) *
		glm::scale(vector3(1.5f)), "WarpPipe");
	m_pMeshMngr->AddInstanceToRenderList("WarpPipe");

	//Add the shoot models
	for (uint n = 0; n < m_nShoot; n++)
	{
		m_lPosition[n] += vector3(0.0f, -0.1f, 0.0f);
		m_lRotations[n] += m_lIncrements[n];
		
		matrix4 mTemp = 
			glm::rotate(glm::translate(m_lPosition[n]), m_lRotations[n].x, REAXISX) *
			glm::rotate(IDENTITY_M4, m_lRotations[n].y, REAXISY) *
			glm::rotate(IDENTITY_M4, m_lRotations[n].z, REAXISZ);

		//Update the models and BOs
		m_pMeshMngr->SetModelMatrix(mTemp, n);
		m_lBO[n]->SetModelMatrix(mTemp);
		m_pMeshMngr->AddInstanceToRenderList(n);
		m_lBO[n]->DisplayOriented(REYELLOW);
		m_lBO[n]->DisplayReAlligned(REPURPLE);
	}

	//Add a plane into the world
	m_pMeshMngr->AddPlaneToRenderList(
		glm::translate(vector3(0.0f, -5.0f, 0.0f)) *
		glm::rotate(IDENTITY_M4, 90.0f, REAXISX) *
		glm::scale(vector3(50.0f)), REBLACK);

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window

	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList();//Resets the render list
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	//Release the BOs
	for (uint n = 0; n < m_lBO.size(); n++)
	{
		SafeDelete(m_lBO[n]);
	}
	m_lBO.clear();
	super::Release(); //release the memory of the inherited fields
}