#pragma once

#include "PhysBody3D.h"
#include "glmath.h"
#include "Primitive.h"

class btRaycastVehicle;
struct PhysBody3D;

struct Wheel
{
	vec3 connection; // origin of the ray. Must come from within the chassis
	vec3 direction;
	vec3 axis;
	float suspensionRestLength; // max length for suspension in meters
	float radius;
	float width;
	bool front; // is front wheel ?
	bool drive; // does this wheel received engine power ?
	bool brake; // does breakes affect this wheel ?
	bool steering; // does this wheel turns ?
};

struct VehicleInfo
{
	~VehicleInfo();

	//----------------
	//car
	//----------------

	//base
	vec3 chassis_size;
	vec3 chassis_offset;

	//part1 big part
	vec3 part1_size;
	vec3 part1_offset;

	//part2 small part
	vec3 part2_size;
	vec3 part2_offset;

	//part3 front part
	vec3 part3_size;
	vec3 part3_offset;

	//motor

		//part4 main engine
		vec3 part4_size;
		vec3 part4_offset;

		//part6 small part
		vec3 part6_size;
		vec3 part6_offset;

		//part7 lower "cilinder"
		vec3 part7_size;
		vec3 part7_offset;

		//part8 upper "cilinder"
		vec3 part8_size;
		vec3 part8_offset;

			//4 small unions

			//part9 front left part
			vec3 part9_size;
			vec3 part9_offset;

			//part10 front right part
			vec3 part10_size;
			vec3 part10_offset;

			//part11 rear left part
			vec3 part11_size;
			vec3 part11_offset;

			//part12 rear right part
			vec3 part12_size;
			vec3 part12_offset;

	//Ligths
	float radiusL = 0.5f;
	vec3 Light_offset;
	vec3 Light1_offset;

	//part5 rear part
	vec3 part5_size;
	vec3 part5_offset;
	
	//----------------
	//end car
	//----------------

	float mass;
	float suspensionStiffness; // default to 5.88 / 10.0 offroad / 50.0 sports car / 200.0 F1 car
	float suspensionCompression; // default to 0.83
	float suspensionDamping; // default to 0.88 / 0..1 0 bounces / 1 rigid / recommended 0.1...0.3
	float maxSuspensionTravelCm; // default to 500 cm suspension can be compressed
	float frictionSlip; // defaults to 10.5 / friction with the ground. 0.8 should be good but high values feels better (kart 1000.0)
	float maxSuspensionForce; // defaults to 6000 / max force to the chassis

	Wheel* wheels;
	int num_wheels;
};


struct PhysVehicle3D : public PhysBody3D
{
public:
	PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info);
	~PhysVehicle3D();

	void Render();
	void ApplyEngineForce(float force);
	void Brake(float force);
	void Turn(float degrees);
	float GetKmh() const;
	vec3 getForwardVector() const;
	void CarOrientation(float angle_r);
	Cube CreateCarPartsBox(vec3 size, vec3 offset);
	Sphere CreateCarPartsSphere(float radius, vec3 offset);

public:

	VehicleInfo info;
	btRaycastVehicle* vehicle;
};