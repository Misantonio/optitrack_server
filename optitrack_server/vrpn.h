#include "vrpn_Connection.h"
#include "vrpn_Tracker.h"
#include "vrpn_Button.h"
#include "vrpn_Analog.h"

#define _USE_MATH_DEFINES

vrpn_Connection *conexion;
char conexionNom[128];
char conexionNom2[128];
BOOLEAN conectado_vrpn = FALSE;
double xl, yl, zl, ql, q1l, q2l, q3l;
double roll, pitch, yawl;
double x2,y2,z2,q02,q12,q22,q32;
double roll2,pitch2,yaw2;

#define DEFAULT_VRPN_PORT 3883

void VRPN_CALLBACK handle_pos1(void *, const vrpn_TRACKERCB t)
{
	xl = t.pos[0];
	yl = t.pos[2];
	zl = t.pos[1];
	ql = t.quat[3];
	q1l= t.quat[0];
	q2l= t.quat[2];
	q3l= t.quat[1];

	yawl = -atan2( ((2*ql*ql)+(2*q1l*q1l)-1) , ((2*q1l*q2l)+(2*ql*q3l)) ); 
	roll = atan(((2*q2l*q3l)+(2*ql*q1l))/((2*ql*ql)+(2*q3l*q3l)-1));
	pitch = (-asin((2*q1l*q3l)-(2*ql*q2l)));
 
}

void VRPN_CALLBACK handle_pos2(void *, const vrpn_TRACKERCB t)
{
	x2 = t.pos[0];
	y2 = t.pos[1];
	z2 = t.pos[2];

	q02 = t.quat[3];
	q12 = t.quat[0];
	q22 = t.quat[2];
	q32 = t.quat[1];

	//yaw2=atan2(z2,x2);
	roll2 = atan(((2*q22*q32)+(2*q02*q12))/((2*q02*q02)+(2*q32*q32)-1));
	pitch2 = (-asin((2*q12*q32)-(2*q02*q22)));
	yaw2 = -atan2(((2*q12*q22)+(2*q02*q32)),((2*q02*q02)+(2*q12*q12)-1));
}