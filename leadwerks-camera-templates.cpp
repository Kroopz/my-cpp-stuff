//FPS SPECTATOR CAMERA WITH WASD + MOUSE CONTROLS
//everything past //Action goes inside while (true) loop

	//camera
  	Camera* cam = Camera::Create();
	float camMoveSpeed = 0.1;
	float camPitch = 80;
	float camYaw = 0;
	float camRoll = 0;
	float camPanX = 0;
	float camPanY = 10;
	float camFw = -5;
	cam->Move(camPanX, camPanY, camFw);
	cam->SetRotation(camPitch, camYaw, camRoll);
	cam->SetCollisionType(Collision::None);

	//mouse
	bool mRelease = false; //used to check whether cursor is snapped to center
	float xSens = 0.4;
	float ySens = 0.4;
	int mousX = 0;
	int mousY = 0;
	int mousZ = 0;
	int mousPosX = 0;
	int mousPosY = 0;
	int mCenterPosX = Math::Round(window->GetWidth() / 2);
	int mCenterPosY = Math::Round(window->GetHeight() / 2);
	window->SetMousePosition(mCenterPosX, mCenterPosY);

  	//Action
  	//while (true) {...
    	//Hit Alt to release cursor. Minimizing window or losing focus will also release, but world will not render then (see Time::Update section)
    	if (window->KeyHit(Key::Alt) && window->Active() == true) { mRelease = !mRelease; }

    	//cam movement
    	//get current mouse position
		mousPosX = Math::Round(window->GetMousePosition().x); 
		mousPosY = Math::Round(window->GetMousePosition().y);
    
    	//snap mouse to center
    	if (mRelease == false && window->Active() == true)
	    { window->SetMousePosition(mCenterPosX, mCenterPosY); }

    	//calculate mouse move
		mousX = (mousPosX - mCenterPosX);
		mousY = (mousPosY - mCenterPosY);
    
    	//calculate angle move
		camPitch = Math::Clamp((camPitch + mousY * ySens), -90, 90);
		camYaw = Math::Clamp((camYaw + mousX * xSens), -1, 361);
		if (camYaw < 0) { camYaw = 360; } else if (camYaw > 360) { camYaw = 0; } //clamp camera rotation to avoid leaks
    
    	//cam rotation
		cam->SetRotation(camPitch, camYaw, camRoll);
		
    	//get current cam position
		camFw = cam->GetPosition().z;
		camPanX = cam->GetPosition().x;
		camPanY = cam->GetPosition().y;
		
    	//calculate cam movement using camMoveSpeed
		if (window->KeyHit(Key::R)) { cam->SetPosition(0, 10, -5, true); } //reset cam position [REMOVE FOR RELEASE] 
		if (window->KeyDown(Key::W)) { cam->Move(0, 0, camMoveSpeed, false); }
		if (window->KeyDown(Key::S)) { cam->Move(0, 0, -camMoveSpeed, false); }
		if (window->KeyDown(Key::A)) { cam->Move(-camMoveSpeed, 0, 0, false); }
		if (window->KeyDown(Key::D)) { cam->Move(camMoveSpeed, 0, 0, false); }
    
    	//pause on focus loss (minimized, or mouse released)
		if (window->Minimized() == false && window->Active() == true )
		{
			Leadwerks::Time::Update();
			world->Update();
			world->Render();
		} else { 
		//game will repeatedly clear buttons until focus is regained
		window->FlushKeys();
		window->FlushMouse();
		}

    	//Context->Sync(false);
  //...}
