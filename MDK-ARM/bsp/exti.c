#include "exti.h"
#include "gpio.h"
#include "robomaster_control.h"

void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    //__HAL_RCC_GPIOA_CLK_ENABLE();               //����GPIOAʱ��  ��������

	  GPIO_InitStruct.Pin=GPIO_PIN_0;                //PA0
    GPIO_InitStruct.Mode=GPIO_MODE_IT_RISING;      //�����ش���
    GPIO_InitStruct.Pull=GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	  HAL_NVIC_SetPriority(EXTI0_IRQn,2,0);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ0
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);             //ʹ���ж���0
    
}

//�жϷ�����
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);//�����жϴ����ú���
}


//�жϷ����������Ҫ��������
//��HAL�������е��ⲿ�жϷ�����������ô˺���
//GPIO_Pin:�ж����ź�
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	HAL_Delay(100);     //����
    switch(GPIO_Pin)
    {
        case GPIO_PIN_0:
            if(remote_control.switch_left!=3&&remote_control.switch_right==2) 
            {
	
            }
            break;
		}
}
