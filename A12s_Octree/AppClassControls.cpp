#include "AppClass.h"
void AppClass::ProcessKeyboard(void)
{
	bool bModifier = false;
	float fSpeed = 0.01f;

#pragma region ON PRESS/RELEASE DEFINITION
	static bool	bLastF1 = false, bLastF2 = false, bLastF3 = false, bLastF4 = false, bLastF5 = false,
				bLastF6 = false, bLastF7 = false, bLastF8 = false, bLastF9 = false, bLastF10 = false,
				bLastEscape = false, bLastF = false, bLastReturn = false, bLastSpace = false, 
				bLastO = false, bLastP = false, bLastG = false, bLastH = false, bLastJ = false, bLastK = false;
#define ON_KEY_PRESS_RELEASE(key, pressed_action, released_action){  \
			bool pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::key);			\
			if(pressed){											\
				if(!bLast##key) pressed_action;}/*Just pressed? */\
			else if(bLast##key) released_action;/*Just released?*/\
			bLast##key = pressed; } //remember the state
#pragma endregion
#pragma region Modifiers
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
		bModifier = true;
#pragma endregion
#pragma region Camera Positioning
	if(bModifier)
		fSpeed *= 50.0f;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		m_pCameraMngr->MoveForward(fSpeed);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		m_pCameraMngr->MoveForward(-fSpeed);
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		m_pCameraMngr->MoveSideways(-fSpeed);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		m_pCameraMngr->MoveSideways(fSpeed);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		m_pCameraMngr->MoveVertical(-fSpeed);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		m_pCameraMngr->MoveVertical(fSpeed);
#pragma endregion
#pragma region Other Actions
	ON_KEY_PRESS_RELEASE(Escape, NULL, PostMessage(m_pWindow->GetHandler(), WM_QUIT, NULL, NULL));
	ON_KEY_PRESS_RELEASE(F1, NULL, m_pCameraMngr->SetCameraMode(CAMPERSP));
	ON_KEY_PRESS_RELEASE(F2, NULL, m_pCameraMngr->SetCameraMode(CAMROTHOZ));
	ON_KEY_PRESS_RELEASE(F3, NULL, m_pCameraMngr->SetCameraMode(CAMROTHOY));
	ON_KEY_PRESS_RELEASE(F4, NULL, m_pCameraMngr->SetCameraMode(CAMROTHOX));
	ON_KEY_PRESS_RELEASE(O, NULL, m_nOctant--);
	ON_KEY_PRESS_RELEASE(P, NULL, m_nOctant++);
	ON_KEY_PRESS_RELEASE(G, NULL, m_bDisplayGeom = !m_bDisplayGeom);
	ON_KEY_PRESS_RELEASE(H, NULL, m_bDisplayOct = !m_bDisplayOct);
	ON_KEY_PRESS_RELEASE(J, NULL, m_bDisplayColl = !m_bDisplayColl);
	ON_KEY_PRESS_RELEASE(K, NULL, m_bOctreeCalc = !m_bOctreeCalc);
#pragma endregion
}
void AppClass::ProcessMouse(void)
{
	m_bArcBall = false;
	m_bFPC = false;
#pragma region ON_MOUSE_PRESS_RELEASE
	static bool	bLastLeft = false, bLastMiddle = false, bLastRight = false;
#define ON_MOUSE_PRESS_RELEASE(key, pressed_action, released_action){  \
			bool pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::key);			\
			if(pressed){											\
				if(!bLast##key) pressed_action;}/*Just pressed? */\
			else if(bLast##key) released_action;/*Just released?*/\
			bLast##key = pressed; } //remember the state
#pragma endregion

	if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
		m_bArcBall = true;
	
	if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
		m_bFPC = true;
}
void AppClass::ProcessJoystick(void)
{
#pragma region ON PRESS/RELEASE DEFINITION
	static bool	bLast0 = false, bLast1 = false, bLast2 = false, bLast3 = false, bLast4 = false,
		bLast5 = false;
#define ON_BUTTON_PRESS_RELEASE(key, pressed_action, released_action){  \
			bool pressed = sf::Joystick::isButtonPressed(nPad, key);			\
			if(pressed){											\
				if(!bLast##key) pressed_action;}/*Just pressed? */\
			else if(bLast##key) released_action;/*Just released?*/\
			bLast##key = pressed; } //remember the state
#pragma endregion

	float fDirection = 1.0f;
	int nPad = 0;
	sf::Joystick::update();
	if (sf::Joystick::isConnected(nPad))
	{
#pragma region OTHER ACTIONS
		static bool bLastPressed0 = false;
		bool pressed = sf::Joystick::isButtonPressed(nPad, 0);
		if (pressed)
		{
			if (!bLastPressed0)
				m_bDisplayGeom = !m_bDisplayGeom;
		}
		//else if(bLastPressed0)
		//released_action;
		bLastPressed0 = pressed;

		ON_BUTTON_PRESS_RELEASE(1, m_bDisplayOct = !m_bDisplayOct, NULL);
		ON_BUTTON_PRESS_RELEASE(2, m_bDisplayColl = !m_bDisplayColl, NULL);
		ON_BUTTON_PRESS_RELEASE(3, m_bOctreeCalc = !m_bOctreeCalc, NULL);
		ON_BUTTON_PRESS_RELEASE(4, m_nOctant--, NULL);
		ON_BUTTON_PRESS_RELEASE(5, m_nOctant++, NULL);
#pragma endregion
#pragma region Camera Position / Orientation
		if (sf::Joystick::isButtonPressed(nPad, 4) && sf::Joystick::isButtonPressed(nPad, 5))
			PostMessage(m_pWindow->GetHandler(), WM_QUIT, NULL, NULL);

		float fAngleX = 0.0f;
		float fAngleY = 0.0f;
		float fSpeed = 0.001f;

		if (sf::Joystick::isButtonPressed(nPad, 8) && sf::Joystick::isButtonPressed(nPad, 9))
		{
			fAngleX = 0.0f;
			fAngleY = 0.0f;
			m_pCameraMngr->SetPositionTargetAndView(REAXISZ * 130.0f, REAXISY * 0.0f, REAXISY);
		}

		float fDelta = sf::Joystick::getAxisPosition(nPad, sf::Joystick::Y);
		if (fDelta > 20 || fDelta < -20)
		{
			fDelta *= 0.01f;
			m_pCameraMngr->MoveForward(-fDelta);
		}
		fDelta = sf::Joystick::getAxisPosition(nPad, sf::Joystick::X);
		if (fDelta > 20 || fDelta < -20)
		{
			fDelta *= 0.01f;
			m_pCameraMngr->MoveSideways(fDelta);
		}
		fDelta = sf::Joystick::getAxisPosition(nPad, sf::Joystick::Z);
		if (fDelta > 20 || fDelta < -20)
		{
			fDelta *= 0.01f;
			m_pCameraMngr->MoveVertical(fDelta);
		}
		fDelta = -sf::Joystick::getAxisPosition(nPad, sf::Joystick::R);
		if (fDelta > 25 || fDelta < -25)
		{
			fAngleX -= fDelta * fSpeed;
			fAngleX *= -20.0f;
		}
		fDelta = sf::Joystick::getAxisPosition(nPad, sf::Joystick::U);
		if (fDelta > 25 || fDelta < -25)
		{
			fAngleY -= fDelta * fSpeed;
			fAngleY *= 20.0f;
		}

		m_pCameraMngr->ChangeYaw(fAngleY);
		m_pCameraMngr->ChangePitch(fAngleX);
		m_pCameraMngr->CalculateView();
#pragma endregion
	}
}