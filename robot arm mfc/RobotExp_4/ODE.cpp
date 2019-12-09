#include "stdafx.h"
#include "ODE.h"
#include "SystemMemory.h"
#include "DataType.h"

#ifndef DRAWSTUFF_TEXTURE_PATH
#define DRAWSTUFF_TEXTURE_PATH "./../ode-0.13/drawstuff/textures"
#endif

#define GRAVITY 9.81
#define MAX_JOINT_NUM 2

#define DEG2RAD 0.0174533
#define RAD2DEG 57.2958

dsFunctions g_Fn;

static dWorldID g_World;
static dSpaceID g_Space;
static dJointGroupID g_Contactgroup;

Object g_oObj[MAX_JOINT_NUM + 1];
static dJointID g_oJoint[MAX_JOINT_NUM + 1];

double g_tar_q[MAX_JOINT_NUM] = { 0.0, 0.0};
double g_cur_q[MAX_JOINT_NUM] = { 0.0, 0.0};

void InitDrawStuff() {

	g_Fn.version = DS_VERSION;
	g_Fn.start = &StartDrawStuff;
	g_Fn.step = &SimLoopDrawStuff;
	g_Fn.command = &CommandDrawStuff;
	g_Fn.stop = StopDrawStuff;
	g_Fn.path_to_textures = DRAWSTUFF_TEXTURE_PATH;
}


void InitODE() {

	dInitODE();
	g_World = dWorldCreate();
	g_Space = dHashSpaceCreate(0);
	g_Contactgroup = dJointGroupCreate(0);
	dWorldSetGravity(g_World, 0, 0, -GRAVITY);
	dWorldSetCFM(g_World, 1e-5);
	//dWorldSetAutoDisableFlag(g_World, 1);
}



void RunODE(size_t width, size_t height) {

	InitDrawStuff();
	InitODE();

	InitRobot();

	// run simulation
	dsSimulationLoop(0, 0, width, height, &g_Fn);
}



void ReleaseODE() {

	dJointGroupDestroy(g_Contactgroup);
	dSpaceDestroy(g_Space);
	dWorldDestroy(g_World);
	dCloseODE();
}



void StartDrawStuff() {

	//TO DO
	float dPos[3] = { 0.0, 0.0, 6.0 };
	float dRot[3] = { 90.0, -90, 0.0 };
	dsSetViewpoint(dPos, dRot);
}


void SimLoopDrawStuff(int pause) 
{

	//TO DO
	DataType_t jointData;
	GET_SYSTEM_MEMORY("JointData", jointData);
	
	g_tar_q[0] = jointData.Q_tar[0];
	g_tar_q[1] = jointData.Q_tar[1];
	
	jointData.Q_cur[0] = g_cur_q[0];
	jointData.Q_cur[1] = g_cur_q[1];

	SET_SYSTEM_MEMORY("JointData", jointData);

	if (g_tar_q[0] >= 360.0 * DEG2RAD) g_tar_q[0] -= 360.0*DEG2RAD;
	if (g_tar_q[0] <= -360.0 * DEG2RAD) g_tar_q[0] += 360.0*DEG2RAD;

	PControl();

	dReal dR, dLength;

	dsSetColor(0, 1, 0);
	dGeomCapsuleGetParams(g_oObj[0].geom, &dR, &dLength);
	dsDrawCapsuleD(dBodyGetPosition(g_oObj[0].body), 
	dBodyGetRotation(g_oObj[0].body), (float)dLength, (float)dR);
	
	dsSetColor(0, 0, 1); 
	dGeomCapsuleGetParams(g_oObj[1].geom, &dR, &dLength);
	dsDrawCapsuleD(dBodyGetPosition(g_oObj[1].body), 
	dBodyGetRotation(g_oObj[1].body), (float)dLength, (float)dR);


	dsSetColor(1, 1, 1);
	dGeomCapsuleGetParams(g_oObj[2].geom, &dR, &dLength);
	dsDrawCapsuleD(dBodyGetPosition(g_oObj[2].body), 
	dBodyGetRotation(g_oObj[2].body), (float)dLength, (float)dR);

	double dt = 0.01;
	dWorldStep(g_World, dt);
}



void CommandDrawStuff(int cmd) {

	//TO DO

}



void StopDrawStuff() {

	//TO DO

}


void InitRobot()
{
	dMass mass;
	dMatrix3 R;

	dReal dX[MAX_JOINT_NUM + 1] =		{ 0.00, 0.00, 0.00 };
	dReal dY[MAX_JOINT_NUM + 1] =		{ 0.00, -0.50, -1.25 };
	dReal dZ[MAX_JOINT_NUM + 1] =		{ 0.25, 0.50, 0.50 };

	dReal dOriX[MAX_JOINT_NUM + 1] =	{ 0.00, 1.00, 1.00 };
	dReal dOriY[MAX_JOINT_NUM + 1] =	{ 0.00, 0.00, 0.00 };
	dReal dOriZ[MAX_JOINT_NUM + 1] =	{ 1.00, 0.00, 0.00 };
	dReal dOriQ[MAX_JOINT_NUM + 1] =	{ 0.00, 90.0 * DEG2RAD, 90.0 * DEG2RAD};

	dReal dLength[MAX_JOINT_NUM + 1] =	{ 0.50, 1.00, 0.50 };
	dReal dWeight[MAX_JOINT_NUM + 1] =	{ 0.50, 1.00, 0.50 };

	dReal dRadius[MAX_JOINT_NUM + 1] =  { 0.125, 0.125, 0.125 };

	for (int i = 0; i < MAX_JOINT_NUM + 1; i++)
	{
		g_oObj[i].body = dBodyCreate(g_World);
		dBodySetPosition(g_oObj[i].body, dX[i], dY[i], dZ[i]);
		dMassSetZero(&mass);
		dMassSetCapsuleTotal(&mass, dWeight[i], 1, dRadius[i], dLength[i]);
		dBodySetMass(g_oObj[i].body, &mass);
		g_oObj[i].geom = dCreateCapsule(g_Space, dRadius[i], dLength[i]);
		dGeomSetBody(g_oObj[i].geom, g_oObj[i].body);
		dRFromAxisAndAngle(R, dOriX[i], dOriY[i], dOriZ[i], dOriQ[i]);
		dBodySetRotation(g_oObj[i].body, R);
	}

	dReal dJointX[MAX_JOINT_NUM + 1] = { 0.00, 0.00, 0.00 };
	dReal dJointY[MAX_JOINT_NUM + 1] = { 0.00, 0.00, -1.00 };
	dReal dJointZ[MAX_JOINT_NUM + 1] = { 0.00, 0.50, 0.50 };

	dReal dJointAX[MAX_JOINT_NUM + 1] = { 0.00, 0.00, 0.00 };
	dReal dJointAY[MAX_JOINT_NUM + 1] = { 0.00, 0.00, 0.00 };
	dReal dJointAZ[MAX_JOINT_NUM + 1] = { 0.00, 1.00, 1.00 };

	g_oJoint[0] = dJointCreateFixed(g_World, 0);
	dJointAttach(g_oJoint[0], g_oObj[0].body, 0);
	dJointSetFixed(g_oJoint[0]);

	for (int i = 1; i < MAX_JOINT_NUM + 1; i++)
	{
		g_oJoint[i] = dJointCreateHinge(g_World, 0);
		dJointAttach(g_oJoint[i], g_oObj[i].body, g_oObj[i-1].body);
		dJointSetHingeAnchor(g_oJoint[i], dJointX[i], dJointY[i], dJointZ[i]);
		dJointSetHingeAxis(g_oJoint[i], dJointAX[i], dJointAY[i], dJointAZ[i]);
	}
}

void PControl()
{
	dReal dKp = 10.0, dMax = 100.0;
	dReal dError_Q[MAX_JOINT_NUM];

	for (int i = 1; i < MAX_JOINT_NUM + 1; i++)
	{
		g_cur_q[i - 1] = dJointGetHingeAngle(g_oJoint[i]);
		if (g_tar_q[i - 1] - g_cur_q[i - 1] > 180.0*DEG2RAD)
		{
			g_cur_q[i - 1] += 359.9*DEG2RAD;
		}

		if (g_tar_q[i - 1] - g_cur_q[i - 1] < -180.0*DEG2RAD)
		{
			g_cur_q[i - 1] -= 359.9*DEG2RAD;
		}

		dError_Q[i - 1] = g_tar_q[i - 1] - g_cur_q[i - 1];
		dJointSetHingeParam(g_oJoint[i], dParamVel, dKp*dError_Q[i - 1]);
		dJointSetHingeParam(g_oJoint[i], dParamFMax, dMax);
	}
}