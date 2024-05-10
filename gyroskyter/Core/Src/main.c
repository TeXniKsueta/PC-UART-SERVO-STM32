/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "MCU6050.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
int _write(int32_t file, uint8_t ptr, int32_t len)
    {
        for (int i = 0; i < len; i++)
        {
            ITM_SendChar(ptr++);
        }
        return len;
    }
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */



/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//void MPU_6050_Init(void){
//	int check=0;
//	int data=0;
//
//	HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_WHO_AM_I, 1, &check, 1, 1000);
//	if (check==104) {
//	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_PWR_MGMT_1, 1, &data, 1, 1000);
//	data = 0x07;
//	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_SMPLRT_DIV, 1, &data, 1, 1000);
//
//	data = 0x00;
//	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_ACCEL_CONFIG, 1, &data, 1, 1000);
//	data = 0x00;
//	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_GYRO_CONFIG, 1, &data, 1, 1000);
//	}
//}



int16_t Accel_X_RAW = 0;
int16_t Accel_Y_RAW = 0;
int16_t Accel_Z_RAW = 0;
uint8_t Data1;
int16_t Gyro_X_RAW = 0;
int16_t Gyro_Y_RAW = 0;
int16_t Gyro_Z_RAW = 0;
int16_t  Accel_X_Filtr = 0;

uint32_t  timme = 0;
uint32_t byte_counter = 0;
uint32_t check_byte = 0;

const int NUM_READ = 30;
int filter()
{

	long sum = 0;
	for (int i = 0; i < NUM_READ; i++)  // согласно количеству усреднений
	    sum += Accel_X_RAW;                  // суммируем значения с любого датчика в переменную sum
	  return ((int)sum / NUM_READ);

}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint8_t rx_buff[10] = {0};
	int new_int = 0;




	uint8_t str[100];




  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
//  uint8_t Rec_Data[6]={0,0,0,0,0,0};
//  MPU_6050_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
  void servo_deg(uint8_t deg){
  		deg >=360? deg=360: deg;
  		deg ==0? deg = 0:deg;
  		TIM2->CCR1 = 7500 - (deg*35);

  }



  goto mark;

  mark: {
	  byte_counter = 0;
	  new_int = 0;
	  check_byte =0;
  	for(int i = 0; i <=1; i ++) rx_buff[i] = 0x0;
  	*rx_buff = 0;
  	HAL_UART_Transmit(&huart1,"Enter ugol\r",11,60);
  	HAL_UART_Receive_IT(&huart1,rx_buff,100);
  	//while ((USART1->SR & USART_SR_RXNE)==0) ;
  	 while (HAL_UART_Receive_IT(&huart1,rx_buff,100)!=HAL_OK)
  	  	  {
  			 __NOP();
  		  }

	   	new_int = atoi(rx_buff);
//	   	HAL_Delay(100);
    if (new_int>=0 && new_int<271)// допустимый угол поворота зависит от конструкции сервопривода

  	 {
    //	HAL_Delay(100);
  	servo_deg(new_int);
//	HAL_Delay(100);
  //	servo_deg(0);

  	//HAL_UART_Transmit(&huart1,"Povernul na ugol",17,60);
  	sprintf(str, "Povernul na ugol %d\r", new_int);

  	HAL_UART_Transmit(&huart1,str, strlen((char *)str),60);
  	 }
  	  	  else
  	  	  {
  	  		HAL_UART_Transmit(&huart1,"Wrong number\r", sizeof("Wrong number\r"),60);
  	  	  }

  	goto mark;
  }
  while (1)
  {
//HAL_Delay(1000);
//servo_deg(90);
//HAL_Delay(1000);
//servo_deg(180);
//HAL_Delay(1000);
//servo_deg(180);
//	  HAL_I2C_Mem_Read(&hi2c1,MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_ACCEL_XOUT_H, 1, Rec_Data, 6, 1000);
//	                        Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);
//	                        Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
//	                        Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);
//
//	                      HAL_Delay(10);
//
//
//	                        HAL_I2C_Mem_Read(&hi2c1,MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_GYRO_XOUT_H, 1, Rec_Data, 6, 1000);
//	                        Gyro_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);
//	                        Gyro_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
//	                        Gyro_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);
//
//
//
//
//
//							 if((HAL_GetTick() - timme) > 1000){
//								// что-то делаем
//								timme = HAL_GetTick();
//
//								Accel_X_Filtr = filter();
//
//
//								 if (Accel_X_Filtr > 4000){
//
//
//									 servo_deg(Accel_X_Filtr / 90);
//
//								 }
//
//								 if (Accel_X_Filtr > -300 && Accel_X_Filtr < 300)
//									 servo_deg(90);
//
//								 if (Accel_X_Filtr < -4000 )
//									 servo_deg((180 - Accel_X_Filtr / -90 ));
//
//								 HAL_Delay(100);
//								 TIM2->CCR1=(-Accel_X_Filtr/8)+3300;

//							}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
