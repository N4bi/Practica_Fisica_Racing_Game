#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet\src\btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{

}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Black;

	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	//----------------
	//Car
	//----------------

	//Base
	Cube chassis = CreateCarPartsBox(info.chassis_size, info.chassis_offset);
	chassis.color = Red;
	chassis.Render();

	//Part1 big part
	Cube Part1 = CreateCarPartsBox(info.part1_size, info.part1_offset);
	Part1.color = Red;
	Part1.Render();
	

	//Part2 small part
	Cube Part2 = CreateCarPartsBox(info.part2_size, info.part2_offset);
	Part2.color = Red;
	Part2.Render();

	//Part3 front part
	Cube Part3 = CreateCarPartsBox(info.part3_size, info.part3_offset);
	Part3.color = Red;
	Part3.Render();

	//motor

		//Part4 Main Engine
		Cube Part4 = CreateCarPartsBox(info.part4_size, info.part4_offset);
		Part4.Render();

		//Part6 small part
		Cube Part6 = CreateCarPartsBox(info.part6_size, info.part6_offset);
		Part6.Render();

		//Part7 lower "cilinder"
		Cube Part7 = CreateCarPartsBox(info.part7_size, info.part7_offset);
		Part7.Render();

		//Part8 upper "cilinder"
		Cube Part8 = CreateCarPartsBox(info.part8_size, info.part8_offset);
		Part8.Render();

		//4 small parts
			
			//Part9 front left part
			Cube Part9 = CreateCarPartsBox(info.part9_size, info.part9_offset);
			Part9.color.r = 255.0f;
			Part9.Render();

			//Part10 front right part
			Cube Part10 = CreateCarPartsBox(info.part10_size, info.part10_offset);
			Part10.color.r = 255.0f;
			Part10.Render();

			//Part11 rear left part
			Cube Part11 = CreateCarPartsBox(info.part11_size, info.part11_offset);
			Part11.color.r = 255.0f;
			Part11.Render();

			//Part12 rear right part
			Cube Part12 = CreateCarPartsBox(info.part12_size, info.part12_offset);
			Part12.color.r = 255.0f;
			Part12.Render();

	//Lights
	Sphere Light = CreateCarPartsSphere(info.radiusL, info.Light_offset);
	Sphere Light1 = CreateCarPartsSphere(info.radiusL, info.Light1_offset);
	Light.color.r = 255.0f;
	Light.color.g = 255.0f;
	Light1.color.r = 255.0f;
	Light1.color.g = 255.0f;
	Light.Render();
	Light1.Render();

	//Part5 rear part
	Cube Part5 = CreateCarPartsBox(info.part5_size, info.part5_offset);
	Part5.color = Red;
	Part5.Render();

}

Cube PhysVehicle3D::CreateCarPartsBox(vec3 size, vec3 offset)
{
	Cube chassis(size.x, size.y, size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset_1(offset.x, offset.y, offset.z);
	offset_1 = offset_1.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset_1.getX();
	chassis.transform.M[13] += offset_1.getY();
	chassis.transform.M[14] += offset_1.getZ();

	return chassis;
}

Sphere PhysVehicle3D::CreateCarPartsSphere(float radius, vec3 offset)
{
	Sphere c(radius);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&c.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset_1(offset.x, offset.y, offset.z);
	offset_1 = offset_1.rotate(q.getAxis(), q.getAngle());

	c.transform.M[12] += offset_1.getX();
	c.transform.M[13] += offset_1.getY();
	c.transform.M[14] += offset_1.getZ();


	return c;
}
// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}

vec3 PhysVehicle3D::getForwardVector() const
{
	btVector3 vector = vehicle->getForwardVector();
	vec3 new_vector = { vector.getX(), vector.getY(), vector.getZ() };
	return new_vector;
}


void PhysVehicle3D::CarOrientation(float angle_r)
{
	float matrix[16];
	memset(matrix, 0.0f, sizeof(matrix));

	// Keep position
	vec3 p = GetPos();
	matrix[12] = p.x;
	matrix[13] = p.y;
	matrix[14] = p.z;
	matrix[15] = 1;

	// Rotate the body by angle_r
	matrix[0] = cos(angle_r);
	matrix[2] = -sin(angle_r);
	matrix[5] = 1;
	matrix[8] = sin(angle_r);
	matrix[10] = cos(angle_r);

	SetTransform(matrix);

}