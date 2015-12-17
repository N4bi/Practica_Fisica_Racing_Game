
#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define CAR_SIZE 0.8
#define MAX_ACCELERATION 6000.0f * CAR_SIZE
#define TURN_DEGREES 30.5f * DEGTORAD
#define BRAKE_POWER 1500.0f*CAR_SIZE


class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void ResetCar(float x, float y, float z);



public:

	PhysVehicle3D*			 vehicle;
	float					 turn;
	float					 acceleration;
	float					 brake;
	bool					 following_camera;

	int						 engine_start_fx;
	int						 car_crash_fx;
	
	

	




};