#include "robomaster_task.h"


/**
  * @brief  TIM2�жϴ�������Ҫ���еĳ���5MSִ��һ��
  * @param  ��
  * @retval 
  * @usage  ���ڵ��̵����������imu���е��̿��ƣ����ڿ�������è��        
  *               
  */
int	cnt=0, //�ж��жϳ����Ƿ�����
		cnt1=0,//�жϵ��̿����Ƿ�����
		cnt2=0;//1���ʱ

int flag=0,Ts=0;//Ts�������Զ�������

void task() 
{	
	cnt++;
	cnt1++;
	cnt2++;
	
	if(cnt==100)    //0.5s����һ�Σ�ʹ��7������2HZƵ����˸���Ӷ��ж��жϳ�����������
	{
		cnt=0;
		HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_1);
	}
	
		if(cnt2==200)    //1s����һ�Σ�1s��ʱ��־λ+1
	{ 
		cnt2=0;
		Ts++;		
	}
	
	if(remote_control.switch_left!=3&&remote_control.switch_right==2&&remote_control.ch4>500)//�Զ��ϵ���־
	{
		flag=1;//��ǰʱ��
	}
//	if(remote_control.switch_left!=3&&remote_control.switch_right!=2&&)//�Զ��ϵ���־
//	{
////		flag2=flag;//��һʱ��
//		flag=0;//��ǰʱ��
//	}
	//MPU9250_GET_DATA();//�������mpu9250imuԭʼ����
	//mpu_get_data();//��ð���imuԭʼ����
	//imu_ahrs_update();//������Ԫ����imu��̬
	//imu_attitude_update();//����imu��pitch,roll,yaw�Ƕ�
	chassis_control();		//���̵���Ŀ���
	footstep_control();
	//pan_tilt_control();
	//shoot_control();
	
}
