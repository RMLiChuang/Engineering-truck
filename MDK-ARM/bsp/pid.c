/**
  ******************************************************************************
  * @file    pid.c
  * @author  Ginger
  * @version V1.0.0
  * @date    2015/11/14
  * @brief   ��ÿһ��pid�ṹ�嶼Ҫ�Ƚ��к��������ӣ��ٽ��г�ʼ��
  ******************************************************************************
  * @attention Ӧ�����ö��ײ��(d)��̨������ȶ�
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "pid.h"
#include "stm32f4xx.h"

#define ABS(x)		((x>0)? x: -x) 

PID_TypeDef pid_pitch,pid_pithch_speed,pid_roll,pid_roll_speed,pid_yaw_speed;
extern int isMove;

/*������ʼ��--------------------------------------------------------------*/
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
	
	pid->ControlPeriod = period;             //û�õ�
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

/*��;���Ĳ����趨--------------------------------------------------------------*/
static void pid_reset(PID_TypeDef * pid, float kp, float ki, float kd)
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
}

/*pid����-----------------------------------------------------------------------*/

	
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
	
	//�Ƿ��������
	if((ABS(pid->err) > pid->DeadBand))
	{
		pid->pout = pid->kp * pid->err;
		pid->iout += (pid->ki * pid->err);
		

		pid->dout =  pid->kd * (pid->err - pid->last_err); 
		
		//�����Ƿ񳬳�����
		if(pid->iout > pid->IntegralLimit)
			pid->iout = pid->IntegralLimit;
		if(pid->iout < - pid->IntegralLimit)
			pid->iout = - pid->IntegralLimit;
		
		//pid�����
		pid->output = pid->pout + pid->iout + pid->dout;
		

		//pid->output = pid->output*0.7f + pid->last_output*0.3f;  //�˲���
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








/*pid�ṹ���ʼ����ÿһ��pid������Ҫ����һ��-----------------------------------------------------*/
void pid_init(PID_TypeDef* pid)
{
	pid->f_param_init = pid_param_init;
	pid->f_pid_reset = pid_reset;
	pid->f_cal_pid = pid_calculate;
}

void all_pid_init()
{
	/*< ��ʼ��PID���� >*/
	//���̵����ʼ��
  for(int i=0; i<4; i++)
  {	

    pid_init(&motor_pid[i]);
    motor_pid[i].f_param_init(&motor_pid[i],
																	PID_Speed,					
																	2000,							//maxOutput												//����޷�
																	1000,								//integralLimit										//�����޷�
																	10,									//deadband												//����������ֵ��
																	0,									//controlPeriod										//��������
																	5000,								//max_err													//������
																	0,									//target
																	2,								//kp
																	0.05,							//ki	
																	0.01);							//kd
    																	
  }
	
	
	//̧�������ʼ��
  for(int i=4; i<6; i++)
  {	

    pid_init(&motor_pid[i]);
    motor_pid[i].f_param_init(&motor_pid[i],
																	PID_Speed,					
																	3500,							//maxOutput												//����޷�
																	1000,								//integralLimit										//�����޷�
																	10,									//deadband												//����������ֵ��
																	0,									//controlPeriod										//��������
																	5000,								//max_err													//������
																	0,									//target
																	4.5,								//kp
																	0.08,							//ki	
																	0.06);							//kd
    																	
  }
	
	 pid_init(&motor_pid[6]);
    motor_pid[6].f_param_init(&motor_pid[6],
																	PID_Speed,					
																	3500,							//maxOutput												//����޷�
																	1000,								//integralLimit										//�����޷�
																	10,									//deadband												//����������ֵ��
																	0,									//controlPeriod										//��������
																	5000,								//max_err													//������
																	0,									//target
																	1.8,								//kp
																	0.08,							//ki	
																	0.01);							//kd

	
//	#ifdef QUADROTOR_YUNTAI
//	//pitch�����ǶȻ�pid��ʼ��  
//	 pid_init(&motor_pid[4]);
//    motor_pid[4].f_param_init(&motor_pid[4],
//																	PID_Speed,					
//																	29000,							//maxOutput												//����޷�
//																	2000,								//integralLimit										//�����޷�
//																	10,									//deadband												//����������ֵ��
//																	0,									//controlPeriod										//��������
//																	5000,								//max_err													//������
//																	0,									//target
//																	50,								//kp
//																	10,							//ki	
//																	65);							//kd
//	//yaw�����ǶȻ�pid��ʼ��    
//	 pid_init(&motor_pid[5]);
//    motor_pid[5].f_param_init(&motor_pid[5],
//																	PID_Speed,					
//																	29000,							//maxOutput												//����޷�
//																	2000,								//integralLimit										//�����޷�
//																	10,									//deadband												//����������ֵ��
//																	0,									//controlPeriod										//��������
//																	5000,								//max_err													//������
//																	0,									//target
//																	50,								//kp
//																	10,							//ki	
//																	65);							//kd

//	#endif
//	#ifdef INFANTRY_YUNTAI
//	//YAW�����ǶȻ�pid��ʼ��  
//	 pid_init(&motor_pid[4]);
//    motor_pid[4].f_param_init(&motor_pid[4],
//																	PID_Speed,					
//																	16384,							//maxOutput												//����޷�
//																	2000,								//integralLimit										//�����޷�
//																	10,									//deadband												//����������ֵ��
//																	0,									//controlPeriod										//��������
//																	5000,								//max_err													//������
//																	0,									//target
//																	2,								//kp
//																	0,							//ki	
//																	0);							//kd
//	//PITCH�����ǶȻ�pid��ʼ��    
//	 pid_init(&motor_pid[5]);
//    motor_pid[5].f_param_init(&motor_pid[5],
//																	PID_Speed,					
//																	16384,							//maxOutput												//����޷�
//																	2000,								//integralLimit										//�����޷�
//																	10,									//deadband												//����������ֵ��
//																	0,									//controlPeriod										//��������
//																	5000,								//max_err													//������
//																	0,									//target
//																	1,							//kp
//																	0,							//ki	
//																	0);							//kd
//	
//	//PITCH�������ٶȻ�pid��ʼ��    
//	 pid_init(&pid_pithch_speed);
//    pid_pithch_speed.f_param_init(&pid_pithch_speed,
//																	PID_Speed,					
//																	6000,							//maxOutput												//����޷�
//																	1500,								//integralLimit										//�����޷�
//																	12,									//deadband												//����������ֵ��
//																	0,									//controlPeriod										//��������
//																	300,								//max_err													//������
//																	0,									//target
//																	1,								//kp    3.2
//																	0.05,							//ki	    0.02
//																	0.2);							//kd			0.1
//	#endif
//	
////�������pid��ʼ��
//    pid_init(&motor_pid[6]);
//    motor_pid[6].f_param_init(&motor_pid[6],
//																	PID_Speed,					
//																	10000,							//maxOutput												//����޷�
//																	2000,								//integralLimit										//�����޷�
//																	10,									//deadband												//����������ֵ��
//																	0,									//controlPeriod										//��������
//																	5000,								//max_err													//������
//																	0,									//target
//																	2,								//kp
//																	0.05,							//ki	
//																	3);							//kd
}