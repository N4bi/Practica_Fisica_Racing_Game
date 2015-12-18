#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"


#define MAP_SIZE 2

struct PhysBody3D;
struct PhysMotor3D;

struct CHECKPOINT
{
	Sphere sphere;
	PhysBody3D* body;

	bool active = false;
	bool isTouched = false;

};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	void Reset();
	void ResetPendulum();
	

public:

	//--PLANE
	Plane			 p;

	//--BALL
	Sphere			 ball;

	//--ROAD
	Cube			 road[40];


	//--GOAL
	Cylinder		 goal;

	//--OBSTACLES & CHECKPOINTS
	Cube			 pendulum[6];
	Cylinder		 cylinder_obstacle[5];
	Cube			 cube_obstacle[2];
	CHECKPOINT		 checkpoint[2];
	Sphere			 ball_obstacle;
	Cube			 cube_wall;

	//------------------------------------------------

	//--BODIES
	PhysBody3D*		 plane;
	PhysBody3D*		 road_body[40];
	PhysBody3D*		 goal_body;
	PhysBody3D*		 pendulum_body[6];
	PhysBody3D*		 cylinder_obstacle_body[5];
	PhysBody3D*	     cube_obstacle_body[2];
	PhysBody3D*		 ball_obstacle_body;
	PhysBody3D*		 ball_body;
	PhysBody3D*		 cube_wall_body;
	PhysBody3D*		 road_test_body;

	//--SFX
	uint			 checkpoint_fx;
	uint			 you_win_fx;
	uint			 you_lose_fx;
	uint			 count_go_fx;
	uint			 car_crash_fx;

	//--TIMER
	Timer			 timer;
	Uint32			 timer_b;
	Uint32			 minutes = 0;
	Uint32			 last_timer_min = 0;
	Uint32			 last_timer = 0;

	//-----------------------------
	bool			 isColliding = false;
	int				 win = 0;
	int				 lose = 0;
	int				 respawn = 3;

};
