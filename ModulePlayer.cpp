#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "ModuleSceneIntro.h"


ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");


	engine_start_fx = App->audio->LoadFx("Game/sounds/sfx/engine_start.wav");
	car_crash_fx = App->audio->LoadFx("Game/sounds/sfx/car_crash.wav");

	App->audio->PlayFx(engine_start_fx);

	VehicleInfo car;

	// Car properties ----------------------------------------

	//----------------
	//car
	//----------------

	//base
	car.chassis_size.Set(2 * CAR_SIZE, 0.1 * CAR_SIZE, 7 * CAR_SIZE);
	car.chassis_offset.Set(0 * CAR_SIZE, 0 * CAR_SIZE, 0.0 * CAR_SIZE);

	//part1 big part
	car.part1_size.Set(2 * CAR_SIZE, 2 * CAR_SIZE, 2 * CAR_SIZE);
	car.part1_offset.Set(0 * CAR_SIZE, 1.05 * CAR_SIZE, -1.1 * CAR_SIZE);

	//part2 small part
	car.part2_size.Set(2 * CAR_SIZE, 1.4 * CAR_SIZE, 1.3 * CAR_SIZE);
	car.part2_offset.Set(0 * CAR_SIZE, 0.75 * CAR_SIZE, 0.56 * CAR_SIZE);

	//part3 front part
	car.part3_size.Set(2 * CAR_SIZE, 1.4 * CAR_SIZE, 0.1 * CAR_SIZE);
	car.part3_offset.Set(0 * CAR_SIZE, 0.75 * CAR_SIZE, 3.45 * CAR_SIZE);

	//motor
		//part4 main engine
	car.part4_size.Set(1.5 * CAR_SIZE, 0.8 * CAR_SIZE, 2 * CAR_SIZE);
	car.part4_offset.Set(0 * CAR_SIZE, 0.45 * CAR_SIZE, 2.3 * CAR_SIZE);

		//part6 small part
	car.part6_size.Set(0.1875 * CAR_SIZE, 0.2 * CAR_SIZE, 0.5 * CAR_SIZE);
	car.part6_offset.Set(0 * CAR_SIZE, 0.97 * CAR_SIZE, 2.3 * CAR_SIZE);

		//part7 lower "cilinder"
	car.part7_size.Set(0.375 * CAR_SIZE, 0.4 * CAR_SIZE, 1 * CAR_SIZE);
	car.part7_offset.Set(0 * CAR_SIZE, 1.25 * CAR_SIZE, 2.3 * CAR_SIZE);

		//part8 upper "cilinder"
	car.part8_size.Set(0.375 * CAR_SIZE, 0.4 * CAR_SIZE, 1 * CAR_SIZE);
	car.part8_offset.Set(0 * CAR_SIZE, 1.8 * CAR_SIZE, 2.3 * CAR_SIZE);

		//4 small parts

			//part9 front left part
	car.part9_size.Set(0.1 * CAR_SIZE, 0.15 * CAR_SIZE, 0.1 * CAR_SIZE);
	car.part9_offset.Set(0.1 * CAR_SIZE, 1.525 * CAR_SIZE, 2.6 * CAR_SIZE);

			//part10 front right part
	car.part10_size.Set(0.1 * CAR_SIZE, 0.15 * CAR_SIZE, 0.1 * CAR_SIZE);
	car.part10_offset.Set(-0.1 * CAR_SIZE, 1.525 * CAR_SIZE, 2.6 * CAR_SIZE);

			//part11 rear left part
	car.part11_size.Set(0.1 * CAR_SIZE, 0.15 * CAR_SIZE, 0.1 * CAR_SIZE);
	car.part11_offset.Set(0.1 * CAR_SIZE, 1.525 * CAR_SIZE, 2 * CAR_SIZE);

			//part12 rear right part
	car.part12_size.Set(0.1 * CAR_SIZE, 0.15 * CAR_SIZE, 0.1 * CAR_SIZE);
	car.part12_offset.Set(-0.1 * CAR_SIZE, 1.525 * CAR_SIZE, 2 * CAR_SIZE);

	//Ligths
	car.radiusL = 0.3f * CAR_SIZE;
	car.Light_offset.Set(0.5 * CAR_SIZE, 0.3 * CAR_SIZE, 3.35 * CAR_SIZE);
	car.Light1_offset.Set(-0.5 * CAR_SIZE, 0.3 * CAR_SIZE, 3.35 * CAR_SIZE);

	//part5 rear part
	car.part5_size.Set(2 * CAR_SIZE, 1.2 * CAR_SIZE, 1.35 * CAR_SIZE);
	car.part5_offset.Set(0 * CAR_SIZE, 0.6 * CAR_SIZE, -2.8 * CAR_SIZE);
	
	//----------------
	//end car
	//----------------

	car.mass = 500.0  * CAR_SIZE;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.5f * CAR_SIZE;
	float wheel_radius = 0.9 * CAR_SIZE;
	float wheel_width = 0.8f * CAR_SIZE;
	float suspensionRestLength = 1.2 * CAR_SIZE;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;

	vec3 direction(0, -1, 0);
	vec3 axis(-1, 0, 0);

	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width + 0.6f * wheel_width, connection_height, half_length - wheel_radius + 0.5);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width ;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width - 0.6f * wheel_width, connection_height, half_length - wheel_radius + 0.5);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width + 0.7f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width - 0.7f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->collision_listeners.add(this);
	ResetCar(0, 40*MAP_SIZE, 0);
	vehicle->CarOrientation(3);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{

	float speed_cam = 0.09;
	if (following_camera)
	{
		vec3 p = vehicle->GetPos();
		vec3 f = vehicle->getForwardVector();

		vec3 dist_to_car = { -10.0f, 5.0f, -10.0f };
		vec3 camera_new_position = { p.x + (f.x * dist_to_car.x), p.y + f.y + dist_to_car.y, p.z + (f.z * dist_to_car.z) };
		vec3 speed_camera = camera_new_position - App->camera->Position;

		App->camera->Look(App->camera->Position + (speed_cam * speed_camera), p);
	}

	turn = acceleration = brake = 0.0f;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		following_camera = !following_camera;
	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		acceleration = MAX_ACCELERATION * CAR_SIZE;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if (turn < TURN_DEGREES)
			turn += TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if (turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		
		if (vehicle->GetKmh() < 0.0f)
		{	
			acceleration = -(MAX_ACCELERATION*CAR_SIZE / 2);
		}
		else
		{
			brake = BRAKE_POWER*CAR_SIZE;
		}
		

	}

	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		
		if (App->scene_intro->respawn > 0)
		{
		
			if (App->scene_intro->checkpoint[0].active == false && App->scene_intro->checkpoint[1].active == false)
			{
			
				ResetCar(0, 20 * MAP_SIZE, 0);
				vehicle->CarOrientation(3);
			}
			if (App->scene_intro->checkpoint[0].active == true && App->scene_intro->checkpoint[1].active == false)
			{
			
				ResetCar(-9.423* MAP_SIZE, (2.539 + 2)* MAP_SIZE, 14.234* MAP_SIZE);
				vehicle->CarOrientation(3);
			}

			if (App->scene_intro->checkpoint[0].active == true && App->scene_intro->checkpoint[1].active == true)
			{
				
				ResetCar(-32.433* MAP_SIZE, (6.739f + 2)* MAP_SIZE, 39.179* MAP_SIZE);
				vehicle->CarOrientation(1);
			}

			App->scene_intro->ResetPendulum();
			App->scene_intro->respawn -= 1;
		}
		
		
			if (App->scene_intro->respawn <=0)
			{
				App->audio->PlayFx(App->scene_intro->you_lose_fx);
				ResetCar(0, 20 * MAP_SIZE, 0);
				vehicle->CarOrientation(3);
				App->scene_intro->respawn = 3;
				App->scene_intro->last_timer = 0;
				App->scene_intro->Reset();
				App->scene_intro->timer.Start();
				App->scene_intro->lose += 1;
			}

	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();


	return UPDATE_CONTINUE;
}

void ModulePlayer::ResetCar(float x, float y, float z)
{
	vehicle->SetPos(x, y, z);
	vehicle->StopMotion();
}
