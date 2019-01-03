/**
  ******************************************************************************
  * @file    pid.c
  * @author  Ginger
  * @version V1.0.0
  * @date    2015/11/14
  * @brief   对每一个pid结构体都要先进行函数的连接，再进行初始化
  ******************************************************************************
  * @attention 应该是用二阶差分(d)云台会更加稳定
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "pid.h"
#include "stm32f4xx.h"

#define ABS(x)		((x>0)? x: -x) 

PID_TypeDef pid_pitch,pid_pithch_speed,pid_roll,pid_roll_speed,pid_yaw_speed;
extern int isMove;

/*参数初始化--------------------------------------------------------------*/
static void pid_param_init(
	PID_TypeDef * pid, 
	PID_ID   id,
	uint16_t maxout,
	uint16_t intergral_limit,
	float deadband,
	uint16_t period,
	int16_t  max_err,
	int16_t  target,

	float 	kp, 
	float 	ki, 
	float 	kd)
{
	pid->id = id;		
	
	pid->ControlPeriod = period;             //没用到
	pid->DeadBand = deadband;
	pid->IntegralLimit = intergral_limit;
	pid->MaxOutput = maxout;
	pid->Max_Err = max_err;
	pid->target = target;
	
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	
	pid->output = 0;
}

/*中途更改参数设定--------------------------------------------------------------*/
static void pid_reset(PID_TypeDef * pid, float kp, float ki, float kd)
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
}

/*pid计算-----------------------------------------------------------------------*/

	
float pid_calculate(PID_TypeDef* pid, float measure)//, int16_t target)
{
//	uint32_t time,lasttime;
	
	pid->lasttime = pid->thistime;
	pid->thistime = HAL_GetTick();
	pid->dtime = pid->thistime-pid->lasttime;
	pid->measure = measure;
  //	pid->target = target;
		
	pid->last_err  = pid->err;
	pid->last_output = pid->output;
	
	pid->err = pid->target - pid->measure;
	
	//是否进入死区
	if((ABS(pid->err) > pid->DeadBand))
	{
		pid->pout = pid->kp * pid->err;
		pid->iout += (pid->ki * pid->err);
		

		pid->dout =  pid->kd * (pid->err - pid->last_err); 
		
		//积分是否超出限制
		if(pid->iout > pid->IntegralLimit)
			pid->iout = pid->IntegralLimit;
		if(pid->iout < - pid->IntegralLimit)
			pid->iout = - pid->IntegralLimit;
		
		//pid输出和
		pid->output = pid->pout + pid->iout + pid->dout;
		

		//pid->output = pid->output*0.7f + pid->last_output*0.3f;  //滤波？
		if(pid->output>pid->MaxOutput)         
		{
			pid->output = pid->MaxOutput;
		}
		if(pid->output < -(pid->MaxOutput))
		{
			pid->output = -(pid->MaxOutput);
		}
	
	}


	return pid->output;
}








/*pid结构体初始化，每一个pid参数需要调用一次-----------------------------------------------------*/
void pid_init(PID_TypeDef* pid)
{
	pid->f_param_init = pid_param_init;
	pid->f_pid_reset = pid_reset;
	pid->f_cal_pid = pid_calculate;
}

void all_pid_init()
{
	/*< 初始化PID参数 >*/
	//底盘电机初始化
  for(int i=0; i<4; i++)
  {	

    pid_init(&motor_pid[i]);
    motor_pid[i].f_param_init(&motor_pid[i],
																	PID_Speed,					
																	2000,							//maxOutput												//输出限幅
																	1000,								//integralLimit										//积分限幅
																	10,									//deadband												//死区（绝对值）
																	0,									//controlPeriod										//控制周期
																	5000,								//max_err													//最大误差
																	0,									//target
																	2,								//kp
																	0.05,							//ki	
																	0.01);							//kd
    																	
  }
	
	
	//抬升电机初始化
  for(int i=4; i<6; i++)
  {	

    pid_init(&motor_pid[i]);
    motor_pid[i].f_param_init(&motor_pid[i],
																	PID_Speed,					
																	4000,							//maxOutput												//输出限幅
																	1000,								//integralLimit										//积分限幅
																	10,									//deadband												//死区（绝对值）
																	0,									//controlPeriod										//控制周期
																	5000,								//max_err													//最大误差
																	0,									//target
																	4.5,								//kp
																	0.08,							//ki	
																	0.06);							//kd
    																	
  }
	
	 pid_init(&motor_pid[6]);
    motor_pid[6].f_param_init(&motor_pid[6],
																	PID_Speed,					
																	4000,							//maxOutput												//输出限幅
																	1000,								//integralLimit										//积分限幅
																	10,									//deadband												//死区（绝对值）
																	0,									//controlPeriod										//控制周期
																	5000,								//max_err													//最大误差
																	0,									//target
																	1.8,								//kp
																	0.08,							//ki	
																	0.01);							//kd
	
	pid_init(&motor_pid[7]);
    motor_pid[7].f_param_init(&motor_pid[7],
																	PID_Speed,					
																	4000,							//maxOutput												//输出限幅
																	1000,								//integralLimit										//积分限幅
																	10,									//deadband												//死区（绝对值）
																	0,									//controlPeriod										//控制周期
																	5000,								//max_err													//最大误差
																	0,									//target
																	4.5,								//kp
																	0.08,							//ki	
																	0.06);							//kd

	
//	#ifdef QUADROTOR_YUNTAI
//	//pitch轴电机角度环pid初始化  
//	 pid_init(&motor_pid[4]);
//    motor_pid[4].f_param_init(&motor_pid[4],
//																	PID_Speed,					
//																	29000,							//maxOutput												//输出限幅
//																	2000,								//integralLimit										//积分限幅
//																	10,									//deadband												//死区（绝对值）
//																	0,									//controlPeriod										//控制周期
//																	5000,								//max_err													//最大误差
//																	0,									//target
//																	50,								//kp
//																	10,							//ki	
//																	65);							//kd
//	//yaw轴电机角度环pid初始化    
//	 pid_init(&motor_pid[5]);
//    motor_pid[5].f_param_init(&motor_pid[5],
//																	PID_Speed,					
//																	29000,							//maxOutput												//输出限幅
//																	2000,								//integralLimit										//积分限幅
//																	10,									//deadband												//死区（绝对值）
//																	0,									//controlPeriod										//控制周期
//																	5000,								//max_err													//最大误差
//																	0,									//target
//																	50,								//kp
//																	10,							//ki	
//																	65);							//kd

//	#endif
//	#ifdef INFANTRY_YUNTAI
//	//YAW轴电机角度环pid初始化  
//	 pid_init(&motor_pid[4]);
//    motor_pid[4].f_param_init(&motor_pid[4],
//																	PID_Speed,					
//																	16384,							//maxOutput												//输出限幅
//																	2000,								//integralLimit										//积分限幅
//																	10,									//deadband												//死区（绝对值）
//																	0,									//controlPeriod										//控制周期
//																	5000,								//max_err													//最大误差
//																	0,									//target
//																	2,								//kp
//																	0,							//ki	
//																	0);							//kd
//	//PITCH轴电机角度环pid初始化    
//	 pid_init(&motor_pid[5]);
//    motor_pid[5].f_param_init(&motor_pid[5],
//																	PID_Speed,					
//																	16384,							//maxOutput												//输出限幅
//																	2000,								//integralLimit										//积分限幅
//																	10,									//deadband												//死区（绝对值）
//																	0,									//controlPeriod										//控制周期
//																	5000,								//max_err													//最大误差
//																	0,									//target
//																	1,							//kp
//																	0,							//ki	
//																	0);							//kd
//	
//	//PITCH轴电机角速度环pid初始化    
//	 pid_init(&pid_pithch_speed);
//    pid_pithch_speed.f_param_init(&pid_pithch_speed,
//																	PID_Speed,					
//																	6000,							//maxOutput												//输出限幅
//																	1500,								//integralLimit										//积分限幅
//																	12,									//deadband												//死区（绝对值）
//																	0,									//controlPeriod										//控制周期
//																	300,								//max_err													//最大误差
//																	0,									//target
//																	1,								//kp    3.2
//																	0.05,							//ki	    0.02
//																	0.2);							//kd			0.1
//	#endif
//	
////拨单电机pid初始化
//    pid_init(&motor_pid[6]);
//    motor_pid[6].f_param_init(&motor_pid[6],
//																	PID_Speed,					
//																	10000,							//maxOutput												//输出限幅
//																	2000,								//integralLimit										//积分限幅
//																	10,									//deadband												//死区（绝对值）
//																	0,									//controlPeriod										//控制周期
//																	5000,								//max_err													//最大误差
//																	0,									//target
//																	2,								//kp
//																	0.05,							//ki	
//																	3);							//kd
}
