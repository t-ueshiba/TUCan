#ifndef __BODYINFO_H__
#define __BODYINFO_H__

#define RLEG_JOINT0	0
#define RLEG_JOINT1	1
#define RLEG_JOINT2	2
#define RLEG_JOINT3	3
#define RLEG_JOINT4	4
#define RLEG_JOINT5	5

#define LLEG_JOINT0	6
#define LLEG_JOINT1	7
#define LLEG_JOINT2	8
#define LLEG_JOINT3 9	
#define LLEG_JOINT4	10
#define LLEG_JOINT5	11

#define CHEST_JOINT0	12
#define CHEST_JOINT1	13

#define HEAD_JOINT0	14
#define HEAD_JOINT1	15

#define RARM_JOINT0	16
#define RARM_JOINT1	17
#define RARM_JOINT2	18
#define RARM_JOINT3	19
#define RARM_JOINT4	20
#define RARM_JOINT5	21
#define RARM_JOINT6	22

#define LARM_JOINT0	23
#define LARM_JOINT1	24
#define LARM_JOINT2	25
#define LARM_JOINT3	26
#define LARM_JOINT4	27
#define LARM_JOINT5	28
#define LARM_JOINT6	29

#define DOF		30

#define FORCE_SENSOR_NO	4
#define GYRO_SENSOR_NO	1
#define G_SENSOR_NO	1
#define TEMP_SENSOR_NO	3

#define RLEG		0
#define	LLEG		1
#define RARM		2
#define	LARM		3
#define	TORSO		4
#define	HEAD		5
#define RHAND		6
#define	LHAND		7

enum { POWER_RELAY, BATTERY_MON, _DIN_NO };

#define DIN_NO	2

#define	E_INVALID_ID	-3

//#include "misc.h"

#define URL "HRP2YH/model/HRP2YHmain.wrl"

#define MODEL_NAME	"HRP2YH"
#define WAIST_JOINT	"WAIST"
#define RLEG_END	"RLEG_JOINT5"
#define LLEG_END	"LLEG_JOINT5"
#define RARM_END	"RARM_JOINT5"
#define LARM_END	"LARM_JOINT5"
#define CHEST_JOINT	"CHEST_JOINT1"
#define RARM_HAND	"RARM_JOINT6"
#define LARM_HAND	"LARM_JOINT6"

#define SOLE_ROLL	(0)
#define SOLE_PITCH	(0)

#define ZMP_LIMIT	{{{0.04,-0.05},{0.085,-0.055}},\
			       {{0.05,-0.04},{0.085,-0.055}}}

#define FOOT_SIZE	{{0.13, 0.1},{0.055, 0.075}}

#define ANKLE_HEIGHT	(0.105)
#define FSENSOR_OFFSET	(0.0)

#define FITTING_FZ_THD		(25) //[N]

#define TOUCH_THD		(25) //[N]

#define LEG_LINK_LEN1	(0.3) // [m]
#define LEG_LINK_LEN2	(0.3) // [m]
#define WAIST_HEIGHT	(LEG_LINK_LEN1+LEG_LINK_LEN2+ANKLE_HEIGHT)

#define HALF_SITTING_HIP_ANGLE		(deg2rad(-26))
#define HALF_SITTING_KNEE_ANGLE		(deg2rad(50))
#define HALF_SITTING_ANKLE_ANGLE	(-HALF_SITTING_HIP_ANGLE-HALF_SITTING_KNEE_ANGLE)

#define INITIAL_ZMP_REF_X	(-LEG_LINK_LEN1*sin(HALF_SITTING_HIP_ANGLE)+LEG_LINK_LEN2*sin(HALF_SITTING_ANKLE_ANGLE)) // [m]
#define INITIAL_ZMP_REF_Z	(-1*(LEG_LINK_LEN1*cos(HALF_SITTING_HIP_ANGLE)+LEG_LINK_LEN2*cos(HALF_SITTING_ANKLE_ANGLE)+ANKLE_HEIGHT)) // [m]

#define TAU_THD		(3)
#define RWEIGHT		(530) // [N]

#define MIN_FZ		(25) // [N]


#define R_HIP_Y      RLEG_JOINT0
#define R_HIP_R      RLEG_JOINT1
#define R_HIP_P      RLEG_JOINT2
#define R_KNEE_P     RLEG_JOINT3
#define R_ANKLE_P    RLEG_JOINT4
#define R_ANKLE_R    RLEG_JOINT5

#define L_HIP_Y      LLEG_JOINT0
#define L_HIP_R      LLEG_JOINT1
#define L_HIP_P      LLEG_JOINT2
#define L_KNEE_P     LLEG_JOINT3
#define L_ANKLE_P    LLEG_JOINT4
#define L_ANKLE_R    LLEG_JOINT5

#define CHEST_Y	     CHEST_JOINT0
#define CHEST_P      CHEST_JOINT1

#define HEAD_Y       HEAD_JOINT0
#define HEAD_P	     HEAD_JOINT1

#define R_SHOULDER_P RARM_JOINT0
#define R_SHOULDER_R RARM_JOINT1
#define R_SHOULDER_Y RARM_JOINT2
#define R_ELBOW_P    RARM_JOINT3
#define R_WRIST_Y    RARM_JOINT4
#define R_WRIST_P    RARM_JOINT5
#define R_HAND_P     RARM_JOINT6

#define L_SHOULDER_P LARM_JOINT0
#define L_SHOULDER_R LARM_JOINT1
#define L_SHOULDER_Y LARM_JOINT2
#define L_ELBOW_P    LARM_JOINT3
#define L_WRIST_Y    LARM_JOINT4
#define L_WRIST_P    LARM_JOINT5
#define L_HAND_P     LARM_JOINT6

#endif
