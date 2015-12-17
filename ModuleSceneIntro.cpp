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
	ball.radius = 2.0f* MAP_SIZE;
	ball.color.r = 255.0f;
	ball.color.g = 255.0f;
 
	ball.SetPos((-25.916f - 20.0f)* MAP_SIZE, 40.0f* MAP_SIZE, -20.321* MAP_SIZE);
	ball_body = App->physics->AddBody(ball, 0.1f);
	ball_body->SetLinearVelocity(0.0f, -10.0f, 0.0f);
	ball_body->collision_listeners.add(this);


	//-- CIRCUIT
	road[0].size.Set(6.0f*MAP_SIZE, 1.235f*MAP_SIZE, 13.0f * MAP_SIZE);
	road[0].SetPos(0, 6.865f*MAP_SIZE, 0);
	road_body[0] = App->physics->AddBody(road[0], 0);

	road[1].size.Set(6.0f*MAP_SIZE, 1.230f * MAP_SIZE, 13.0f * MAP_SIZE);
	road[1].SetRotation(-43.98f, vec3(0.0f, 1, 0.0f));
	road[1].SetPos(3.788f* MAP_SIZE, 6.865f * MAP_SIZE, -9.296f * MAP_SIZE);
	road_body[1] = App->physics->AddBody(road[1], 0);

	road[2].size.Set(6.0f* MAP_SIZE, 1.235f* MAP_SIZE, 13.0f* MAP_SIZE);
	road[2].SetRotation(-103.908f, vec3(0.0f, 1, 0.0f));
	road[2].SetPos(12.009f* MAP_SIZE, 6.865f* MAP_SIZE, -11.656f* MAP_SIZE);
	road_body[2] = App->physics->AddBody(road[2], 0);

	road[3].size.Set(6.0f* MAP_SIZE, 1.230f* MAP_SIZE, 13.0f* MAP_SIZE);
	road[3].SetRotation(-171.714f, vec3(0.0f, 1, 0.0f));
	road[3].SetPos(17.310f* MAP_SIZE, 6.865f* MAP_SIZE, -5.961f* MAP_SIZE);
	road_body[3] = App->physics->AddBody(road[3], 0);

	road[4].size.Set(6.0f* MAP_SIZE, 1.230f* MAP_SIZE, 13.0f* MAP_SIZE);
	road[4].SetRotation(-176.993f, vec3(0, 1, 0));
	road[4].SetPos(18.475f* MAP_SIZE, 4.98f* MAP_SIZE, 4.490f* MAP_SIZE);
	road_body[4] = App->physics->AddBody(road[4], 0);

	road[5].size.Set(5.608f* MAP_SIZE, 1.230f* MAP_SIZE, 8.680f* MAP_SIZE);
	road[5].SetRotation(-175.675f, vec3(1.714f, 175.675f, 2.782f));
	road[5].SetPos(19.166f* MAP_SIZE, 2.539f* MAP_SIZE, 13.258f* MAP_SIZE);
	road_body[5] = App->physics->AddBody(road[5], 0);

	road[6].size.Set(24.059f* MAP_SIZE, 1.230f* MAP_SIZE, 8.680f* MAP_SIZE);
	road[6].SetRotation(-180.288f, vec3(0, 1, 0));
	road[6].SetPos(4.711f* MAP_SIZE, 2.539f* MAP_SIZE, 14.234* MAP_SIZE);
	road_body[6] = App->physics->AddBody(road[6], 0);

	road[7].size.Set(5.193f* MAP_SIZE, 1.230f* MAP_SIZE, 8.680f* MAP_SIZE);
	road[7].SetRotation(-175.675, vec3(0, 1, 0));
	road[7].SetPos(-9.423* MAP_SIZE, 2.539* MAP_SIZE, 14.234* MAP_SIZE);
	road_body[7] = App->physics->AddBody(road[7], 0);

	road[8].size.Set(5.193f* MAP_SIZE, 1.230f* MAP_SIZE, 8.680f* MAP_SIZE);
	road[8].SetRotation(-175.675f, vec3(0, 175.675f, 0));
	road[8].SetPos(-10.083* MAP_SIZE, 2.539* MAP_SIZE, 5.811* MAP_SIZE);
	road_body[8] = App->physics->AddBody(road[8], 0);

	road[9].size.Set(5.0f* MAP_SIZE, 1.230f* MAP_SIZE, 9.0f* MAP_SIZE);
	road[9].SetRotation(-30, vec3(-30, 0, 0));
	road[9].SetPos(-10.488* MAP_SIZE, 4.929* MAP_SIZE, -0.696f* MAP_SIZE);
	road_body[9] = App->physics->AddBody(road[9], 0);

	road[10].size.Set(6.0f* MAP_SIZE, 1.230f* MAP_SIZE, 23.382* MAP_SIZE);
	road[10].SetPos(-10.356f* MAP_SIZE, 6.735f* MAP_SIZE, -16.303f* MAP_SIZE);
	road_body[10] = App->physics->AddBody(road[10], 0);

	road[11].size.Set(6.0f* MAP_SIZE, 1.235f* MAP_SIZE, 31.947f* MAP_SIZE);
	road[11].SetRotation(-90, vec3(0, 90, 0));
	road[11].SetPos(-23.312f* MAP_SIZE, 6.735f* MAP_SIZE, -25.287f* MAP_SIZE);
	road_body[11] = App->physics->AddBody(road[11], 0);

	road[12].size.Set(6.0f* MAP_SIZE, 1.230f* MAP_SIZE, 34.502f* MAP_SIZE);
	road[12].SetPos(-36.263f* MAP_SIZE, 6.735f* MAP_SIZE, -10.801* MAP_SIZE);
	road_body[12] = App->physics->AddBody(road[12], 0);

	road[13].size.Set(4.073f* MAP_SIZE, 0.362f* MAP_SIZE, 4.957f* MAP_SIZE);
	road[13].SetRotation(-10.712, vec3(1, 0, 0));
	road[13].SetPos(-36.314* MAP_SIZE, 7.735f* MAP_SIZE, 4.570* MAP_SIZE);
	road_body[13] = App->physics->AddBody(road[13], 0);

	road[14].size.Set(6.0f* MAP_SIZE, 1.230f* MAP_SIZE, 23.796f* MAP_SIZE);
	road[14].SetPos(-36.242f* MAP_SIZE, 6.735f* MAP_SIZE, 30.328* MAP_SIZE);
	road_body[14] = App->physics->AddBody(road[14], 0);

	road[15].size.Set(4.073f* MAP_SIZE, 0.362f* MAP_SIZE, 4.957f* MAP_SIZE);
	road[15].SetRotation(-10.712, vec3(-1, 0, 0));
	road[15].SetPos(-36.314* MAP_SIZE, 7.735f* MAP_SIZE, 20.623* MAP_SIZE);
	road_body[15] = App->physics->AddBody(road[15], 0);
	 
	road[16].size.Set(6.0f* MAP_SIZE, 1.230f* MAP_SIZE, 13.0f* MAP_SIZE);
	road[16].SetRotation(-90, vec3(0, 90, 0));
	road[16].SetPos(-32.433* MAP_SIZE, 6.739f* MAP_SIZE, 39.179* MAP_SIZE);
	road_body[16] = App->physics->AddBody(road[16], 0);


	//-- FIRST RAMP
	road[17].size.Set(7.488f* MAP_SIZE, 1.006* MAP_SIZE, 15.0f* MAP_SIZE);
	road[17].SetRotation(35.0f, vec3(0, 0.0f, 1.0f));
	road[17].SetPos(-22.946* MAP_SIZE, 8.739f* MAP_SIZE, 39.679* MAP_SIZE);
	road_body[17] = App->physics->AddBody(road[17], 0);

	road[18].size.Set(7.488f* MAP_SIZE, 1.006* MAP_SIZE, 15.0f* MAP_SIZE);
	road[18].SetRotation(30.0f, vec3(0, 0.0f, 1.0f));
	road[18].SetPos(-17.5* MAP_SIZE, 12.0f* MAP_SIZE, 39.679* MAP_SIZE);
	road_body[18] = App->physics->AddBody(road[18], 0);

	road[19].size.Set(7.488f* MAP_SIZE, 1.006* MAP_SIZE, 15.0f* MAP_SIZE);
	road[19].SetRotation(25.0f, vec3(0, 0.0f, 1.0f));
	road[19].SetPos((-17.5 + 6)* MAP_SIZE, (12.0f + 3)* MAP_SIZE, 39.679* MAP_SIZE);
	road_body[19] = App->physics->AddBody(road[19], 0);

	road[20].size.Set(7.488f* MAP_SIZE, 1.006* MAP_SIZE, 15.0f* MAP_SIZE);
	road[20].SetRotation(20.0f, vec3(0, 0.0f, 1.0f));
	road[20].SetPos((-17.5 + 12)* MAP_SIZE, (12.0f + 5.5f)* MAP_SIZE, 39.679* MAP_SIZE);
	road_body[20] = App->physics->AddBody(road[20], 0);

	road[21].size.Set(7.488f* MAP_SIZE, 1.006* MAP_SIZE, 15.0f* MAP_SIZE);
	road[21].SetRotation(15.0f, vec3(0, 0.0f, 1.0f));
	road[21].SetPos((-17.5 + 18)* MAP_SIZE, (12.0f + 7.5f)* MAP_SIZE, 39.679* MAP_SIZE);
	road_body[21] = App->physics->AddBody(road[21], 0);

	road[22].size.Set(7.488f* MAP_SIZE, 1.006* MAP_SIZE, 15.0f* MAP_SIZE);
	road[22].SetRotation(10.0f, vec3(0, 0.0f, 1.0f));
	road[22].SetPos((-17.5 + 24)* MAP_SIZE, (12.0f + 8.8f)* MAP_SIZE, 39.679* MAP_SIZE);
	road_body[22] = App->physics->AddBody(road[22], 0);

	road[23].size.Set(7.488f* MAP_SIZE, 1.006* MAP_SIZE, 15.0f* MAP_SIZE);
	road[23].SetRotation(5.0f, vec3(0, 0.0f, 1.0f));
	road[23].SetPos((-17.5 + 30)* MAP_SIZE, (12.0f + 9.5f)* MAP_SIZE, 39.679* MAP_SIZE);
	road_body[23] = App->physics->AddBody(road[23], 0);

	road[24].size.Set(7.488f* MAP_SIZE, 1.006* MAP_SIZE, 15.0f* MAP_SIZE);
	road[24].SetRotation(0.0f, vec3(0, 0.0f, 1.0f));
	road[24].SetPos((-17.5 + 37)* MAP_SIZE, (12.0f + 9.8f)* MAP_SIZE, 39.679* MAP_SIZE);
	road_body[24] = App->physics->AddBody(road[24], 0);
	//---------------------------------------

	road[25].size.Set(7.488f* MAP_SIZE, 1.006* MAP_SIZE, 15.0f* MAP_SIZE);
	road[25].SetPos((-17.5 + 44)* MAP_SIZE, (12.0f + 9.8f)* MAP_SIZE, 39.679* MAP_SIZE);
	road_body[25] = App->physics->AddBody(road[25], 0);

	road[26].size.Set(7.488f* MAP_SIZE, 1.006* MAP_SIZE, 15.0f* MAP_SIZE);
	road[26].SetPos((-17.5 + 44)* MAP_SIZE, (12.0f + 9.8f)* MAP_SIZE, (39.679 - 15.0f)* MAP_SIZE);
	road_body[26] = App->physics->AddBody(road[26], 0);

	road[27].size.Set(7.488f* MAP_SIZE, 1.006* MAP_SIZE, 15.0f* MAP_SIZE);
	road[27].SetPos((-17.5 + 44)* MAP_SIZE, (12.0f + 9.8f)* MAP_SIZE, (39.679 - 30.0f)* MAP_SIZE);
	road_body[27] = App->physics->AddBody(road[27], 0);

	road[28].size.Set(7.488f* MAP_SIZE, 1.006* MAP_SIZE, 15.0f* MAP_SIZE);
	road[28].SetPos((-17.5 + 44)* MAP_SIZE, (12.0f + 9.8f)* MAP_SIZE, (39.679 - 45.0f)* MAP_SIZE);
	road_body[28] = App->physics->AddBody(road[28], 0);

	road[29].size.Set(7.488f* MAP_SIZE, 1.006* MAP_SIZE, 15.0f* MAP_SIZE);
	road[29].SetPos((-17.5 + 44)* MAP_SIZE, (12.0f + 9.8f)* MAP_SIZE, (39.679 - 60.0f)* MAP_SIZE);
	road_body[29] = App->physics->AddBody(road[29], 0);

	//-- SECOND RAMP
	road[30].size.Set(7.488f* MAP_SIZE, 1.006* MAP_SIZE, 15.0f* MAP_SIZE);
	road[30].SetRotation(15.0f, vec3(0.0f, 0.0f, -1.0f));
	road[30].SetPos((-17.5 + 44 - 7.488f)* MAP_SIZE, (12.0f + 9.8f + 1.006f)* MAP_SIZE, (39.679 - 60.0f)* MAP_SIZE);
	road_body[30] = App->physics->AddBody(road[30], 0);

	road[31].size.Set(7.488f* MAP_SIZE, 1.006* MAP_SIZE, 15.0f* MAP_SIZE);
	road[31].SetRotation(15.0f, vec3(0.0f, 0.0f, -1.0f));
	road[31].SetPos((-17.5 + 44 - 14.976f)* MAP_SIZE, (12.0f + 9.8f + 3.018f)* MAP_SIZE, (39.679 - 60.0f)* MAP_SIZE);
	road_body[31] = App->physics->AddBody(road[31], 0);

	road[32].size.Set(7.488f* MAP_SIZE, 1.006* MAP_SIZE, 15.0f* MAP_SIZE);
	road[32].SetRotation(15.0f, vec3(0.0f, 0.0f, -1.0f));
	road[32].SetPos((-17.5 + 44 - 22.464f)* MAP_SIZE, (12.0f + 9.8f + 5.03f)* MAP_SIZE, (39.679 - 60.0f)* MAP_SIZE);
	road_body[32] = App->physics->AddBody(road[32], 0);

	road[33].size.Set(7.488f* MAP_SIZE, 1.006* MAP_SIZE, 15.0f* MAP_SIZE);
	road[33].SetRotation(15.0f, vec3(0.0f, 0.0f, -1.0f));
	road[33].SetPos((-17.5 + 44 - 29.952f)* MAP_SIZE, (12.0f + 9.8f + 7.042)* MAP_SIZE, (39.679 - 60.0f)* MAP_SIZE);
	road_body[33] = App->physics->AddBody(road[33], 0);

	road[34].size.Set(7.488f* MAP_SIZE, 1.006* MAP_SIZE, 15.0f* MAP_SIZE);
	road[34].SetRotation(15.0f, vec3(0.0f, 0.0f, -1.0f));
	road[34].SetPos((-17.5 + 44 - 37.44f)* MAP_SIZE, (12.0f + 9.8f + 9.054)* MAP_SIZE, (39.679 - 60.0f)* MAP_SIZE);
	road_body[34] = App->physics->AddBody(road[34], 0);

	road[35].size.Set(7.488f* MAP_SIZE, 1.006* MAP_SIZE, 15.0f* MAP_SIZE);
	road[35].SetPos((-17.5 + 44 - 44.928f)* MAP_SIZE, (12.0f + 9.8f + 10.06f)* MAP_SIZE, (39.679 - 60.0f)* MAP_SIZE);
	road_body[35] = App->physics->AddBody(road[35], 0);
	//-------------------------

	// FIELD
	road[36].size.Set(80.0f* MAP_SIZE, 1.006* MAP_SIZE, 80.0f* MAP_SIZE);
	road[36].color.g = 89.0f;
	road[36].SetPos((-17.5 + 44 - 52.416 - (73.0f / 2))* MAP_SIZE, (12.0f + 9.8f + 10.06f)* MAP_SIZE, (39.679 - 60.0f)* MAP_SIZE);
	road_body[36] = App->physics->AddBody(road[36], 0);

	road[37].size.Set(40.0f* MAP_SIZE, 1.006* MAP_SIZE, 80.0f* MAP_SIZE);
	road[37].color.b = 255.0f;
	road[37].SetRotation(-90.0f, vec3(0.0, 0.0f, 1.0f));
	road[37].SetPos((-17.5 + 44 - 52.416 - 77.0f)* MAP_SIZE, (12.0f + 9.8f + 10.06f + (40.0f / 2))* MAP_SIZE, (39.679 - 60.0f)* MAP_SIZE);
	road_body[37] = App->physics->AddBody(road[37], 0);

	road[38].size.Set(80.0f* MAP_SIZE, 1.006* MAP_SIZE, 40.0f* MAP_SIZE);
	road[38].color.b = 255.0f;
	road[38].SetRotation(90.0f, vec3(1.0f, 0.0f, 0.0f));
	road[38].SetPos((-17.5 + 44 - 52.416 - (73.0f / 2))* MAP_SIZE, (12.0f + 9.8f + 10.06f + (40.0f / 2))* MAP_SIZE, (39.679 - 60.0f + (80.0f / 2))* MAP_SIZE);
	road_body[38] = App->physics->AddBody(road[38], 0);

	road[39].size.Set(80.0f* MAP_SIZE, 1.006* MAP_SIZE, 40.0f* MAP_SIZE);
	road[39].color.b = 255.0f;
	road[39].SetRotation(90.0f, vec3(1.0f, 0.0f, 0.0f));
	road[39].SetPos((-17.5 + 44 - 52.416 - (73.0f / 2))* MAP_SIZE, (12.0f + 9.8f + 10.06f + (40.0f / 2))* MAP_SIZE, (39.679 - 60.0f - (80.0f / 2))* MAP_SIZE);
	road_body[39] = App->physics->AddBody(road[39], 0);

	goal.radius = 5.0f* MAP_SIZE;
	goal.SetPos((-17.5 + 44 - 52.416 - 75.0f)* MAP_SIZE, (12.0f + 9.8f + 15.0f)* MAP_SIZE, (39.679 - 60.0f)* MAP_SIZE);
	goal.color.r = 255.0f;
	goal_body = App->physics->AddBody(goal, 0);
	goal_body->SetAsSensor(true);
	goal_body->collision_listeners.add(this);

	//-------------

	//--OBSTACLES

		//-- PENDULUMS
	pendulum[0].size.Set(2 * MAP_SIZE, 2 * MAP_SIZE, 2 * MAP_SIZE);
	pendulum[0].SetPos((-17.5 + 44)* MAP_SIZE, (12.0f + 35.8)* MAP_SIZE, (39.679 - 15.0f)* MAP_SIZE);
	pendulum_body[0] = App->physics->AddBody(pendulum[0], 0);

	pendulum[1].size.Set(4 * MAP_SIZE, 4 * MAP_SIZE, 4 * MAP_SIZE);
	pendulum[1].SetPos((-17.5 + 44)* MAP_SIZE, (12.0f + 9.8f)* MAP_SIZE, (39.679 - 15.0f)* MAP_SIZE);
	pendulum_body[1] = App->physics->AddBody(pendulum[1], 500);
	pendulum_body[1]->SetLinearVelocity(100.f, 0.0f, 0.0f);
	
	
	App->physics->AddConstraintHinge(*pendulum_body[0], *pendulum_body[1], vec3(0, -2 * MAP_SIZE, 0), vec3(0, 21 * MAP_SIZE, 0), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, -1.0f));

	pendulum[2].size.Set(2 * MAP_SIZE, 2 * MAP_SIZE, 2 * MAP_SIZE);
	pendulum[2].SetPos((-17.5 + 44)* MAP_SIZE, (12.0f + 35.8)* MAP_SIZE, (39.679 - 30.0f)* MAP_SIZE);
	pendulum_body[2] = App->physics->AddBody(pendulum[2], 0);

	pendulum[3].size.Set(4 * MAP_SIZE, 4 * MAP_SIZE, 4 * MAP_SIZE);
	pendulum[3].SetPos((-17.5 + 44)* MAP_SIZE, (12.0f + 9.8)* MAP_SIZE, (39.679 - 30.0f)* MAP_SIZE);
	pendulum_body[3] = App->physics->AddBody(pendulum[3], 500);
	pendulum_body[3]->SetLinearVelocity(-8.5f, 0.0f, 0.0f);
	
	
	App->physics->AddConstraintHinge(*pendulum_body[2], *pendulum_body[3], vec3(0, -2 * MAP_SIZE, 0), vec3(0, 21 * MAP_SIZE, 0), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, -1.0f));

	pendulum[4].size.Set(2 * MAP_SIZE, 2 * MAP_SIZE, 2 * MAP_SIZE);
	pendulum[4].SetPos((-17.5 + 44)* MAP_SIZE, (12.0f + 35.8f)* MAP_SIZE, (39.679 - 45.0f)* MAP_SIZE);
	pendulum_body[4] = App->physics->AddBody(pendulum[4], 0);

	pendulum[5].size.Set(4 * MAP_SIZE, 4 * MAP_SIZE, 4 * MAP_SIZE);
	pendulum[5].SetPos((-17.5 + 44)* MAP_SIZE, (12.0f + 9.8f)* MAP_SIZE, (39.679 - 45.0f)* MAP_SIZE);
	pendulum_body[5] = App->physics->AddBody(pendulum[5], 500);
	pendulum_body[5]->SetLinearVelocity(8.5f, 0.0f, 0.0f);
	
	
	App->physics->AddConstraintHinge(*pendulum_body[4], *pendulum_body[5], vec3(0, -2 * MAP_SIZE, 0), vec3(0, 21 * MAP_SIZE, 0), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, -1.0f));

		//--------------------------------


		//-- CYLINDER OBSTACLES
	cylinder_obstacle[0].radius = 1.0f* MAP_SIZE;
	cylinder_obstacle[0].height = 5.8f* MAP_SIZE;
	cylinder_obstacle[0].SetPos((-23.312f - 5)* MAP_SIZE, (6.735f)* MAP_SIZE, -25.287f* MAP_SIZE);
	cylinder_obstacle[0].SetRotation(90.0f, vec3(0, 1, 0));
	cylinder_obstacle_body[0] = App->physics->AddBody(cylinder_obstacle[0], 0);

	cylinder_obstacle[1].radius = 1.0f* MAP_SIZE;
	cylinder_obstacle[1].height = 5.8f* MAP_SIZE;
	cylinder_obstacle[1].SetPos(-23.312f* MAP_SIZE, (6.735f+0.1)* MAP_SIZE, -25.287f* MAP_SIZE);
	cylinder_obstacle[1].SetRotation(90.0f, vec3(0, 1, 0));
	cylinder_obstacle_body[1] = App->physics->AddBody(cylinder_obstacle[1], 0);

	cylinder_obstacle[2].radius = 1.0f* MAP_SIZE;
	cylinder_obstacle[2].height = 5.8f* MAP_SIZE;
	cylinder_obstacle[2].SetPos((-23.312f + 5)* MAP_SIZE, (6.735f+0.1)* MAP_SIZE, -25.287f* MAP_SIZE);
	cylinder_obstacle[2].SetRotation(90.0f, vec3(0, 1, 0));
	cylinder_obstacle_body[2] = App->physics->AddBody(cylinder_obstacle[2], 0);

	cylinder_obstacle[3].radius = 1.0f* MAP_SIZE;
	cylinder_obstacle[3].height = 5.8f* MAP_SIZE;
	cylinder_obstacle[3].SetPos((-23.312f + 10)* MAP_SIZE, (6.735f+0.1)* MAP_SIZE, -25.287f* MAP_SIZE);
	cylinder_obstacle[3].SetRotation(90.0f, vec3(0, 1, 0));
	cylinder_obstacle_body[3] = App->physics->AddBody(cylinder_obstacle[3], 0);

	cylinder_obstacle[4].radius = 1.0f* MAP_SIZE;
	cylinder_obstacle[4].height = 5.8f* MAP_SIZE;
	cylinder_obstacle[4].SetPos((-23.312f - 10)* MAP_SIZE,( 6.735f+0.1)* MAP_SIZE, -25.287f* MAP_SIZE);
	cylinder_obstacle[4].SetRotation(90.0f, vec3(0, 1, 0));
	cylinder_obstacle_body[4] = App->physics->AddBody(cylinder_obstacle[4], 0);

		//--------------------------------------


		//-- CUBE OBSTACLE
	cube_obstacle[0].size.Set(4 * MAP_SIZE, 10 * MAP_SIZE, 4 * MAP_SIZE);
	cube_obstacle[0].SetPos((4.711f - 6.014f)* MAP_SIZE, (2.539f + 5 + 1)* MAP_SIZE, (14.234 - 2)* MAP_SIZE);
	cube_obstacle_body[0] = App->physics->AddBody(cube_obstacle[0], 0);

	cube_obstacle[1].size.Set(4 * MAP_SIZE, 10 * MAP_SIZE, 4 * MAP_SIZE);
	cube_obstacle[1].SetPos((4.711f + 6.014f)* MAP_SIZE, (2.539f + 5 + 1)* MAP_SIZE, (14.234 + 2)* MAP_SIZE);
	cube_obstacle_body[1] = App->physics->AddBody(cube_obstacle[1], 0);

		//-------------------------------------------------	


		//-- CHECKPOINTS
	checkpoint[0].sphere.radius = 1.0f* MAP_SIZE;
	checkpoint[0].sphere.SetPos(-9.423* MAP_SIZE, (2.539 + 2)* MAP_SIZE, 14.234* MAP_SIZE);
	checkpoint[0].body = App->physics->AddBody(checkpoint[0].sphere, 0);
	checkpoint[0].body->SetAsSensor(true);
	checkpoint[0].body->collision_listeners.add(this);


	checkpoint[1].sphere.radius = 1.0f* MAP_SIZE;
	checkpoint[1].sphere.SetPos(-32.433* MAP_SIZE, (6.739f + 2)* MAP_SIZE, 39.179* MAP_SIZE);
	checkpoint[1].body = App->physics->AddBody(checkpoint[1].sphere, 0);
	checkpoint[1].body->SetAsSensor(true);
	checkpoint[1].body->collision_listeners.add(this);

		//---------------------------------------------------



		//-- BALL OBSTACLE
	ball_obstacle.radius = 5.0f*MAP_SIZE;
	ball_obstacle.color.r = 255.0f;
	ball_obstacle.color.g = 255.0f;
	ball_obstacle.SetPos((-17.5 + 20)* MAP_SIZE, (12.0f + 8.8f + 10.0f)* MAP_SIZE, 39.679* MAP_SIZE);
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
	pendulum_body[1]->SetLinearVelocity(300.0f, 0.0f, 0.0f);
	pendulum_body[3]->SetLinearVelocity(-300.0f, 0.0f, 0.0f);
	pendulum_body[5]->SetLinearVelocity(300.0f, 0.0f, 0.0f);
}


