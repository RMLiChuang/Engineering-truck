#include "exti.h"
#include "gpio.h"
#include "robomaster_control.h"

void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    //__HAL_RCC_GPIOA_CLK_ENABLE();               //开启GPIOA时钟  开启过了

	  GPIO_InitStruct.Pin=GPIO_PIN_0;                //PA0
    GPIO_InitStruct.Mode=GPIO_MODE_IT_RISING;      //上升沿触发
    GPIO_InitStruct.Pull=GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	  HAL_NVIC_SetPriority(EXTI0_IRQn,2,0);       //抢占优先级为2，子优先级为0
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);             //使能中断线0
    
}

//中断服务函数
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);//调用中断处理公用函数
}


//中断服务程序中需要做的事情
//在HAL库中所有的外部中断服务函数都会调用此函数
//GPIO_Pin:中断引脚号
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	HAL_Delay(100);     //消抖
    switch(GPIO_Pin)
    {
        case GPIO_PIN_0:
            if(remote_control.switch_left!=3&&remote_control.switch_right==2) 
            {
	
            }
            break;
		}
}
