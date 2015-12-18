#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "PhysVehicle3D.h"
#include "Timer.h"



ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{

	LOG("Loading Intro assets");
	bool ret = true;
	win = 0;
	lose = 0;
	respawn = 3;

	//--SOUNDS
		
		//--MUSIC
	App->audio->PlayMusic("Game/sounds/music/undertale.ogg");


		//--SFX
	checkpoint_fx = App->audio->LoadFx("Game/sounds/sfx/Sonic_Checkpoint.wav");
	you_win_fx = App->audio->LoadFx("Game/sounds/sfx/youwin.wav");
	you_lose_fx = App->audio->LoadFx("Game/sounds/sfx/youlose.wav");
	count_go_fx = App->audio->LoadFx("Game/sounds/sfx/count_go.wav");
	car_crash_fx = App->audio->LoadFx("Game/sounds/sfx/car_crash.wav");
	App->audio->PlayFx(count_go_fx);


	//--TIMER
	minutes = 0;
	last_timer = 0;
	last_timer_min = 0;


	//--PLANE
	p.Scale(1, 1, 1);
	p.SetPos(0, 0, 0);
	plane = App->physics->AddBody(p);
	plane->SetAsSensor(true);
	plane->collision_listeners.add(this);

	//-- BALL
	ball = App->physics->CreateSphere(2.0f, (-25.916f - 20.0f), 40.0f, -20.321);
	ball.color.r = 255.0f;
	ball.color.g = 255.0f;
	ball_body = App->physics->AddBody(ball, 0.1f);
	ball_body->SetLinearVelocity(0.0f, -10.0f, 0.0f);
	ball_body->collision_listeners.add(this);


	//-- CIRCUIT
	road[0] = App->physics->CreateCube(6.0f, 1.235f, 13.0f, 0.0f, 6.865f+0.01f, 0.0f);
	road_body[0] = App->physics->AddBody(road[0], 0);

	road[1] = App->physics->CreateCube(6.0f, 1.235f, 13.0f, 3.788f, 6.865f, -9.296f,-43.98f,vec3(0.0f,1.0f,0.0f));
	road_body[1] = App->physics->AddBody(road[1], 0);

	road[2] = App->physics->CreateCube(6.0f, 1.235f, 13.0f,12.009f ,6.865f+0.01 , -11.656f, -103.908f, vec3(0.0f, 1.0f, 0.0f));
	road_body[2] = App->physics->AddBody(road[2], 0);

	road[3] = App->physics->CreateCube(6.0f, 1.235f, 13.0f, 17.310f, 6.865f, -5.961f, -171.714f, vec3(0.0f, 1, 0.0f));
	road_body[3] = App->physics->AddBody(road[3], 0);

	road[4] = App->physics->CreateCube(6.0f, 1.230f, 13.0f, 18.475f, 4.98f, 4.490f, -176.993f, vec3(0, 1, 0));
	road_body[4] = App->physics->AddBody(road[4], 0);

	road[5] = App->physics->CreateCube(5.608f, 1.230f, 8.680f, 19.166f, 2.539f, 13.258f, -175.675f, vec3(1.714f, 175.675f, 2.782f));
	road_body[5] = App->physics->AddBody(road[5], 0);

	road[6] = App->physics->CreateCube(24.059f, 1.230f , 8.680f , 4.711f , 2.539f , 14.234 , -180.288f, vec3(0, 1, 0));
	road_body[6] = App->physics->AddBody(road[6], 0);

	road[7] = App->physics->CreateCube(5.193f , 1.230f , 8.680f , -9.423 , 2.539 , 14.234 , -175.675, vec3(0, 1, 0));
	road_body[7] = App->physics->AddBody(road[7], 0);

	road[8] = App->physics->CreateCube(5.193f , 1.230f , 8.680f , -10.083 , 2.539 , 5.811 , -175.675f, vec3(0, 175.675f, 0));
	road_body[8] = App->physics->AddBody(road[8], 0);

	road[9] = App->physics->CreateCube(5.0f , 1.230f , 9.0f , -10.488 , 4.929 , -0.696f , -30, vec3(-1, 0, 0));
	road_body[9] = App->physics->AddBody(road[9], 0);

	road[10] = App->physics->CreateCube(6.0f, 1.230f, 23.382, -10.356f, 6.735f, -16.303f);
	road_body[10] = App->physics->AddBody(road[10], 0);

	road[11] = App->physics->CreateCube(6.0f , 1.235f , 31.947f , -23.312f , 6.735f+0.01 , -25.287f , -90, vec3(0, 1, 0));
	road_body[11] = App->physics->AddBody(road[11], 0);

	road[12] = App->physics->CreateCube(6.0f, 1.230f, 34.502f, -36.263f, 6.735f, -10.801);
	road_body[12] = App->physics->AddBody(road[12], 0);

	road[13] = App->physics->CreateCube(4.073f , 0.362f , 4.957f , -36.314 , 7.735f , 4.570 , -10.712, vec3(1, 0, 0));
	road_body[13] = App->physics->AddBody(road[13], 0);

	road[14] = App->physics->CreateCube(6.0f , 1.230f , 23.796f , -36.242f , 6.735f , 30.328 );
	road_body[14] = App->physics->AddBody(road[14], 0);

	road[15] = App->physics->CreateCube(4.073f , 0.362f , 4.957f , -36.314 , 7.735f , 20.623 , -10.712, vec3(-1, 0, 0));
	road_body[15] = App->physics->AddBody(road[15], 0);
	 
	road[16] = App->physics->CreateCube(6.0f , 1.230f , 13.0f , -32.433 , 6.739f , 39.179 , -90, vec3(0, 1, 0));
	road_body[16] = App->physics->AddBody(road[16], 0);


	//-- FIRST RAMP
	road[17] = App->physics->CreateCube(7.488f, 1.006, 15.0f, -22.946, 8.739f, 39.679, 35.0f, vec3(0, 0.0f, 1.0f));
	road_body[17] = App->physics->AddBody(road[17], 0);

	road[18] = App->physics->CreateCube(7.488f, 1.006, 15.0f, -17.5, 12.0f, 39.679, 30.0f, vec3(0, 0.0f, 1.0f));
	road_body[18] = App->physics->AddBody(road[18], 0);

	road[19] = App->physics->CreateCube(7.488f, 1.006, 15.0f, (-17.5 + 6), (12.0f + 3), 39.679, 25.0f, vec3(0, 0.0f, 1.0f));
	road_body[19] = App->physics->AddBody(road[19], 0);

	road[20] = App->physics->CreateCube(7.488f, 1.006, 15.0f, (-17.5 + 12), (12.0f + 5.5f), 39.679, 20.0f, vec3(0, 0.0f, 1.0f));
	road_body[20] = App->physics->AddBody(road[20], 0);

	road[21] = App->physics->CreateCube(7.488f, 1.006, 15.0f, (-17.5 + 18), (12.0f + 7.5f), 39.679, 15.0f, vec3(0, 0.0f, 1.0f));
	road_body[21] = App->physics->AddBody(road[21], 0);

	road[22] = App->physics->CreateCube(7.488f, 1.006, 15.0f, (-17.5 + 24), (12.0f + 8.8f), 39.679, 10.0f, vec3(0, 0.0f, 1.0f));
	road_body[22] = App->physics->AddBody(road[22], 0);

	road[23] = App->physics->CreateCube(7.488f, 1.006, 15.0f, (-17.5 + 30), (12.0f + 9.5f), 39.679, 5.0f, vec3(0, 0.0f, 1.0f));
	road_body[23] = App->physics->AddBody(road[23], 0);

	road[24] = App->physics->CreateCube(7.488f, 1.006, 15.0f, (-17.5 + 37), (12.0f + 9.8f), 39.679);
	road_body[24] = App->physics->AddBody(road[24], 0);
	//---------------------------------------

	road[25] = App->physics->CreateCube(7.488f , 1.006 , 15.0f , (-17.5 + 44) , (12.0f + 9.8f) , 39.679 );
	road_body[25] = App->physics->AddBody(road[25], 0);

	road[26] = App->physics->CreateCube(7.488f, 1.006, 15.0f, (-17.5 + 44), (12.0f + 9.8f), (39.679 - 15.0f));
	road_body[26] = App->physics->AddBody(road[26], 0);

	road[27] = App->physics->CreateCube(7.488f, 1.006, 15.0f, (-17.5 + 44), (12.0f + 9.8f), (39.679 - 30.0f));
	road_body[27] = App->physics->AddBody(road[27], 0);

	road[28] = App->physics->CreateCube(7.488f, 1.006, 15.0f,(-17.5 + 44), (12.0f + 9.8f), (39.679 - 45.0f));
	road_body[28] = App->physics->AddBody(road[28], 0);

	road[29] = App->physics->CreateCube(7.488f, 1.006, 15.0f, (-17.5 + 44), (12.0f + 9.8f), (39.679 - 60.0f));
	road_body[29] = App->physics->AddBody(road[29], 0);

	//-- SECOND RAMP
	road[30] = App->physics->CreateCube(7.488f, 1.006, 15.0f, (-17.5 + 44 - 7.488f), (12.0f + 9.8f + 1.006f), (39.679 - 60.0f), 15.0f, vec3(0.0f, 0.0f, -1.0f));
	road_body[30] = App->physics->AddBody(road[30], 0);

	road[31] = App->physics->CreateCube(7.488f, 1.006, 15.0f, (-17.5 + 44 - 14.976f), (12.0f + 9.8f + 3.018f), (39.679 - 60.0f), 15.0f, vec3(0.0f, 0.0f, -1.0f));
	road_body[31] = App->physics->AddBody(road[31], 0);

	road[32] = App->physics->CreateCube(7.488f, 1.006, 15.0f, (-17.5 + 44 - 22.464f), (12.0f + 9.8f + 5.03f), (39.679 - 60.0f), 15.0f, vec3(0.0f, 0.0f, -1.0f));
	road_body[32] = App->physics->AddBody(road[32], 0);

	road[33] = App->physics->CreateCube(7.488f, 1.006, 15.0f, (-17.5 + 44 - 29.952f), (12.0f + 9.8f + 7.042), (39.679 - 60.0f), 15.0f, vec3(0.0f, 0.0f, -1.0f));
	road_body[33] = App->physics->AddBody(road[33], 0);

	road[34] = App->physics->CreateCube(7.488f, 1.006, 15.0f, (-17.5 + 44 - 37.44f), (12.0f + 9.8f + 9.054), (39.679 - 60.0f), 15.0f, vec3(0.0f, 0.0f, -1.0f));
	road_body[34] = App->physics->AddBody(road[34], 0);

	road[35] = App->physics->CreateCube(7.488f, 1.006, 15.0f, (-17.5 + 44 - 44.928f), (12.0f + 9.8f + 10.06f), (39.679 - 60.0f));
	road_body[35] = App->physics->AddBody(road[35], 0);
	//-------------------------

	// FIELD
	road[36] = App->physics->CreateCube(80.0f, 1.006, 80.0f, (-17.5 + 44 - 52.416 - (73.0f / 2)), (12.0f + 9.8f + 10.06f), (39.679 - 60.0f));
	road[36].color.g = 89.0f;
	road_body[36] = App->physics->AddBody(road[36], 0);


	road[37] = App->physics->CreateCube(40.0f, 1.006, 80.0f, (-17.5 + 44 - 52.416 - 77.0f), (12.0f + 9.8f + 10.06f + (40.0f / 2)), (39.679 - 60.0f), -90.0f, vec3(0.0, 0.0f, 1.0f));
	road[37].color.b = 255.0f;
	road_body[37] = App->physics->AddBody(road[37], 0);

	road[38] = App->physics->CreateCube(80.0f, 1.006, 40.0f, (-17.5 + 44 - 52.416 - (73.0f / 2)), (12.0f + 9.8f + 10.06f + (40.0f / 2)), (39.679 - 60.0f + (80.0f / 2)), 90.0f, vec3(1.0f, 0.0f, 0.0f));
	road[38].color.b = 255.0f;
	road_body[38] = App->physics->AddBody(road[38], 0);

	road[39] = App->physics->CreateCube(80.0f, 1.006, 40.0f, (-17.5 + 44 - 52.416 - (73.0f / 2)), (12.0f + 9.8f + 10.06f + (40.0f / 2)), (39.679 - 60.0f - (80.0f / 2)), 90.0f, vec3(1.0f, 0.0f, 0.0f));
	road[39].color.b = 255.0f;
	road_body[39] = App->physics->AddBody(road[39], 0);

	goal = App->physics->CreateCylinder(5.0f, (-17.5 + 44 - 52.416 - 75.0f), (12.0f + 9.8f + 15.0f), (39.679 - 60.0f));
	goal.color.r = 255.0f;
	goal_body = App->physics->AddBody(goal, 0);
	goal_body->SetAsSensor(true);
	goal_body->collision_listeners.add(this);
	//-------------

	//--OBSTACLES

		//-- PENDULUMS

	pendulum[0] = App->physics->CreateCube(2  , 2  , 2  , (-17.5 + 44) , (12.0f + 35.8) , (39.679 - 15.0f) );
	pendulum_body[0] = App->physics->AddBody(pendulum[0], 0);

	pendulum[1] = App->physics->CreateCube(4  , 4  , 4  , (-17.5 + 44) , (12.0f + 9.8f) , (39.679 - 15.0f) );
	pendulum_body[1] = App->physics->AddBody(pendulum[1], 500);
	pendulum_body[1]->SetLinearVelocity(100.f, 0.0f, 0.0f);
	
	App->physics->AddConstraintHinge(*pendulum_body[0], *pendulum_body[1], vec3(0, -2  , 0), vec3(0, 21  , 0), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, -1.0f));


	pendulum[2] = App->physics->CreateCube(2, 2, 2, (-17.5 + 44) , (12.0f + 35.8) , (39.679 - 30.0f) );
	pendulum_body[2] = App->physics->AddBody(pendulum[2], 0);

	pendulum[3] = App->physics->CreateCube(4, 4, 4, (-17.5 + 44) , (12.0f + 9.8) , (39.679 - 30.0f) );
	pendulum_body[3] = App->physics->AddBody(pendulum[3], 500);
	pendulum_body[3]->SetLinearVelocity(-8.5f, 0.0f, 0.0f);
	
	App->physics->AddConstraintHinge(*pendulum_body[2], *pendulum_body[3], vec3(0, -2  , 0), vec3(0, 21  , 0), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, -1.0f));

	pendulum[4] = App->physics->CreateCube(2, 2, 2, (-17.5 + 44) , (12.0f + 35.8f) , (39.679 - 45.0f) );
	pendulum_body[4] = App->physics->AddBody(pendulum[4], 0);

	pendulum[5] = App->physics->CreateCube(4, 4, 4, (-17.5 + 44) , (12.0f + 9.8f) , (39.679 - 45.0f) );
	pendulum_body[5] = App->physics->AddBody(pendulum[5], 500);
	pendulum_body[5]->SetLinearVelocity(8.5f, 0.0f, 0.0f);
		
	App->physics->AddConstraintHinge(*pendulum_body[4], *pendulum_body[5], vec3(0, -2  , 0), vec3(0, 21  , 0), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, -1.0f));

		//--------------------------------


		//-- CYLINDER OBSTACLES
	cylinder_obstacle[0] = App->physics->CreateCylinder(1.0f, (-23.312f - 5) , (6.735f+0.1f) , -25.287f , 5.8f, 90.0f, vec3(0, 1, 0));
	cylinder_obstacle_body[0] = App->physics->AddBody(cylinder_obstacle[0], 0);

	cylinder_obstacle[1] = App->physics->CreateCylinder(1.0f, (-23.312f) , (6.735f + 0.1f) , -25.287f , 5.8f, 90.0f, vec3(0, 1, 0));
	cylinder_obstacle_body[1] = App->physics->AddBody(cylinder_obstacle[1], 0);

	cylinder_obstacle[2] = App->physics->CreateCylinder(1.0f, (-23.312f + 5) , (6.735f + 0.1f) , -25.287f , 5.8f, 90.0f, vec3(0, 1, 0));
	cylinder_obstacle_body[2] = App->physics->AddBody(cylinder_obstacle[2], 0);

	cylinder_obstacle[3] = App->physics->CreateCylinder(1.0f, (-23.312f + 10) , (6.735f + 0.1f) , -25.287f , 5.8f, 90.0f, vec3(0, 1, 0));
	cylinder_obstacle_body[3] = App->physics->AddBody(cylinder_obstacle[3], 0);

	cylinder_obstacle[4] = App->physics->CreateCylinder(1.0f, (-23.312f - 10) , (6.735f + 0.1f) , -25.287f , 5.8f, 90.0f, vec3(0, 1, 0));
	cylinder_obstacle_body[4] = App->physics->AddBody(cylinder_obstacle[4], 0);

		//--------------------------------------


		//-- CUBE OBSTACLE
	cube_obstacle[0] = App->physics->CreateCube(4, 10, 4, (4.711f - 6.014f), (2.539f + 5 + 1), (14.234 - 2));
	cube_obstacle_body[0] = App->physics->AddBody(cube_obstacle[0], 0);


	cube_obstacle[1] = App->physics->CreateCube(4, 10, 4, (4.711f + 6.014f), (2.539f + 5 + 1), (14.234 + 2));
	cube_obstacle_body[1] = App->physics->AddBody(cube_obstacle[1], 0);

		//-------------------------------------------------	


		//-- CHECKPOINTS
	checkpoint[0].sphere = App->physics->CreateSphere(1.0f, -9.423, (2.539 + 2), 14.234);
	checkpoint[0].body = App->physics->AddBody(checkpoint[0].sphere, 0);
	checkpoint[0].body->SetAsSensor(true);
	checkpoint[0].body->collision_listeners.add(this);

	checkpoint[1].sphere = App->physics->CreateSphere(1.0f, -32.433, (6.739f + 2), 39.179);
	checkpoint[1].body = App->physics->AddBody(checkpoint[1].sphere, 0);
	checkpoint[1].body->SetAsSensor(true);
	checkpoint[1].body->collision_listeners.add(this);

		//---------------------------------------------------



		//-- BALL OBSTACLE
	ball_obstacle = App->physics->CreateSphere(5.0f, (-17.5 + 20), (12.0f + 8.8f + 10.0f), 39.679);
	ball_obstacle.color.r = 255.0f;
	ball_obstacle.color.g = 255.0f;
	ball_obstacle_body = App->physics->AddBody(ball_obstacle, 1000.0f);
	ball_obstacle_body->SetLinearVelocity(-70.0f, -20.0f, 0.0f);
	ball_obstacle_body->collision_listeners.add(this);

		//--------------------------------------------------


	App->camera->Move(vec3(10.0f, 20.0f, 10.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

//--RENDER ALL

	p.axis = true;
	p.Render();

	for (int i = 0; i < 40; ++i)
	{
		road[i].color.b = 128.0f;
		road[i].Render();
	}

	for (int i = 0; i < 6; ++i)
	{

		pendulum[i].color.r = 255.0f;
		pendulum[i].Render();
		pendulum_body[i]->GetTransform(&pendulum[i].transform);
	}

	for (int i = 0; i < 5; ++i)
	{

		cylinder_obstacle[i].color.r = 255.0f;
		cylinder_obstacle[i].Render();
	}

	for (int i = 0; i < 2; ++i)
	{


		cube_obstacle[i].Render();
		cube_obstacle[i].color.r = 255.0f;
	}

	for (int i = 0; i < 2; ++i)
	{

		checkpoint[i].sphere.Render();
		checkpoint[i].body->GetTransform(&checkpoint[i].sphere.transform);

		if (checkpoint[i].active == true)
		{
			if (checkpoint[i].isTouched != true)
			{
				checkpoint[i].isTouched = true;
				App->audio->PlayFx(checkpoint_fx);
			}
			checkpoint[i].sphere.color.r = 0.0f;
			checkpoint[i].sphere.color.g = 255.0f;
		}
		else
		{
			checkpoint[i].sphere.color.r = 255.0f;
			checkpoint[i].sphere.color.g = 0.0f;
		}

	}

	ball_obstacle_body->GetTransform(&ball_obstacle.transform);
	ball_obstacle.Render();

	ball_body->GetTransform(&ball.transform);
	ball.Render();
	goal.Render();

//---------------------------------

//--TITLE
	
	if ((int)timer.ReadSec() == 60)
	{
		minutes++;
		timer.Start();
	}

	char title[200];

	sprintf_s(title, " KM/h: %.1f     Timer: %.1d : %.1d   Last timer: %d : %d    Wins: %d Loses: %d    Lifes: %d", App->player->vehicle->GetKmh(),minutes,(int)timer.ReadSec(),last_timer_min, last_timer,win,lose,respawn);
	App->window->SetTitle(title);
//----------------------------------
	
	if (isColliding == true)
	{
		if (checkpoint[0].active == false && checkpoint[1].active == false)
		{
			if (respawn > 0)
			{
				isColliding = false;
				App->audio->PlayFx(car_crash_fx);
				ResetPendulum();
				App->player->ResetCar(0, 20 * MAP_SIZE, 0);
				App->player->vehicle->CarOrientation(3);
				respawn -= 1;
			}

			if (respawn <= 0)
			{
				isColliding = false;
				App->audio->PlayFx(you_lose_fx);
				respawn = 3;
				last_timer = 0;
				last_timer_min = 0;
				Reset();
				timer.Start();
				lose += 1;
			}

		}

		if (checkpoint[0].active == true && checkpoint[1].active == false)
		{
			if (respawn > 0)
			{
				isColliding = false;
				App->audio->PlayFx(car_crash_fx);
				ResetPendulum();
				App->player->ResetCar(-9.423* MAP_SIZE, (2.539 + 2)* MAP_SIZE, 14.234* MAP_SIZE);
				App->player->vehicle->CarOrientation(3);
				respawn -= 1;
			}
		
			if (respawn <= 0)
			{
				isColliding = false;
				App->audio->PlayFx(you_lose_fx);
				respawn = 3;
				last_timer = 0;
				last_timer_min = 0;
				Reset();
				timer.Start();
				lose += 1;
			}
		}

		if (checkpoint[0].active == true && checkpoint[1].active == true)
		{
			if (respawn > 0)
			{
				isColliding = false;
				App->audio->PlayFx(car_crash_fx);
				ResetPendulum();
				App->player->ResetCar(-32.433* MAP_SIZE, (6.739f + 2)* MAP_SIZE, 39.179* MAP_SIZE);
				App->player->vehicle->CarOrientation(1);
				respawn -= 1;
			}

			if (respawn <= 0)
			{
				isColliding = false;
				App->audio->PlayFx(you_lose_fx);
				respawn = 3;
				last_timer = 0;
				last_timer_min = 0;
				Reset();
				timer.Start();
				lose += 1;
			}

		}

	}

	return UPDATE_CONTINUE;

}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{

	if (body1 == checkpoint[0].body)
	{
		checkpoint[0].active = true;
	}

	if (body1 == checkpoint[1].body && body2 != ball_obstacle_body)
	{
		checkpoint[1].active = true;
	}

	if (body1 == ball_body && body2 == goal_body)
	{
			App->audio->PlayFx(you_win_fx);
			respawn = 3;
			last_timer = 0;
			last_timer_min = 0;
			Reset();
			timer.Start();
			win += 1;
	}

	if (body1 == plane && body2 != ball_obstacle_body)
	{
		

		if (body2 == ball_body)
		{
			ball_body->SetPos((-25.916f - 20.0f)* MAP_SIZE, 40.0f* MAP_SIZE, -20.321* MAP_SIZE);
			ball_body->StopMotion();
		}

		if (isColliding != true)
		{
			isColliding = true;
		}
	
	}
	
	if (body1 == plane && body2 == ball_obstacle_body)
	{
		ball_obstacle_body->SetPos((-17.5 + 20)* MAP_SIZE, (12.0f + 8.8f + 10.0f)* MAP_SIZE, 39.679* MAP_SIZE);
		ball_obstacle_body->StopMotion();
		ball_obstacle_body->SetLinearVelocity(-70.0f, -40.0f, 0.0f);

	}

}

//-- RESET METHODS
void ModuleSceneIntro::Reset()
{
	
	ball_body->SetPos((-25.916f - 20.0f)*MAP_SIZE, (40.0f - 3.6f-2.0f)* MAP_SIZE, -20.321* MAP_SIZE);
	ball_body->StopMotion();
	ball_body->SetLinearVelocity(0.0f, -500.0f, 0.0f);
	App->player->ResetCar(0, 20 * MAP_SIZE, 0);
	App->player->vehicle->CarOrientation(3);
	ResetPendulum();	
	timer.Stop();
	last_timer = timer.ReadSec();
	last_timer_min = minutes;
	minutes = 0;

	checkpoint[0].active = false;
	checkpoint[1].active = false;
	checkpoint[0].isTouched = false;
	checkpoint[1].isTouched = false;
	
}

void ModuleSceneIntro::ResetPendulum()
{
	pendulum_body[1]->SetLinearVelocity(100.0f, 0.0f, 0.0f);
	pendulum_body[3]->SetLinearVelocity(-150.0f, 0.0f, 0.0f);
	pendulum_body[5]->SetLinearVelocity(200.0f, 0.0f, 0.0f);
}



