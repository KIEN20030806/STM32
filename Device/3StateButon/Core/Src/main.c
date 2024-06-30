/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f1xx_hal.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define DEBOUND_TIME 50
#define SINGLE_CLICK_TIME 1000
#define DOUBLE_CLICK_TIME 300

typedef enum
{
	NO_CLICK     = 0x00,
	SINGLE_CLICK = 0x01,
	DOUBLE_CLICK = 0x02,
	LONGCLICK_1S = 0x03
}BUTTON_STATE;
BUTTON_STATE Status;
/* USER CODE END Includes */

typedef struct {
	GPIO_TypeDef* BUTTON_PORT;
	uint16_t BUTTON_Pin;
	BUTTON_STATE State;
	uint16_t isPress;
	uint16_t timePress;
	uint16_t timeDouble;
}BUTTON_Name;
BUTTON_Name Button1;
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
static uint8_t BUTTON_ReadPin(BUTTON_Name* Button)
{
	return HAL_GPIO_ReadPin(Button->BUTTON_PORT, Button->BUTTON_Pin);
}
static void BUTTON_DelayMs(uint16_t Time)
{
	HAL_Delay(Time);
}

// ************************** High Level Layer *****************************************************//
void BUTTON_Init(BUTTON_Name* Button, GPIO_TypeDef* BUTTON_PORT, uint16_t BUTTON_Pin)
{
	Button->BUTTON_PORT = BUTTON_PORT;
	Button->BUTTON_Pin = BUTTON_Pin;
}

BUTTON_STATE BUTTON_Read(BUTTON_Name* Button)
{
	Button->State = NO_CLICK;
	while(BUTTON_ReadPin(Button) == 0)
	{
		Button->timePress++;
		Button->isPress = 1;
		BUTTON_DelayMs(1);
	}
	if(Button->isPress)
	{
		while(BUTTON_ReadPin(Button) == 1)
		{
			Button->timeDouble++;
			BUTTON_DelayMs(1);
			if(Button->timeDouble > DOUBLE_CLICK_TIME)
			{
				if(Button->timePress > DEBOUND_TIME && Button->timePress <= SINGLE_CLICK_TIME)
				{
					Button->isPress = 0;
					Button->timePress = 0;
					Button->timeDouble = 0;
					Button->State = SINGLE_CLICK;
					return Button->State;
				}
				else if(Button->timePress  > SINGLE_CLICK_TIME)
				{
					Button->isPress = 0;
					Button->timePress = 0;
					Button->timeDouble = 0;
					Button->State = LONGCLICK_1S;
					return Button->State;
				}
			}
		}
		while(BUTTON_ReadPin(Button) == 0)
		{
			Button->State = DOUBLE_CLICK;
			Button->isPress = 0;
			Button->timePress = 0;
			Button->timeDouble = 0;
			return Button->State;
		}
	}
	return NO_CLICK;
}
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  /* USER CODE BEGIN 2 */
	BUTTON_Init(&Button1, GPIOA, GPIO_PIN_0);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	Status = BUTTON_Read(&Button1);
	if (Status == DOUBLE_CLICK)
	{
	HAL_GPIO_TogglePin (GPIOC, GPIO_PIN_13);
	HAL_Delay(200);
	HAL_GPIO_TogglePin (GPIOC, GPIO_PIN_13);
	HAL_Delay (200) ;
	HAL_GPIO_TogglePin (GPIOC, GPIO_PIN_13);
	HAL_Delay (200);
	HAL_GPIO_TogglePin (GPIOC, GPIO_PIN_13);
	HAL_Delay (200) ;
	HAL_GPIO_TogglePin (GPIOC, GPIO_PIN_13);
	HAL_Delay(200);
	HAL_GPIO_TogglePin (GPIOC, GPIO_PIN_13);
	HAL_Delay(200);
	HAL_GPIO_TogglePin (GPIOC, GPIO_PIN_13);
	HAL_Delay(200);
	HAL_GPIO_TogglePin (GPIOC, GPIO_PIN_13);
	HAL_Delay(200);
	Status = 0;
	}
	if (Status == SINGLE_CLICK)
	{
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	Status = 0;
	}
   if (Status == LONGCLICK_1S)
	{
	HAL_GPIO_TogglePin (GPIOC, GPIO_PIN_13);
	HAL_Delay(1000);
	HAL_GPIO_TogglePin (GPIOC, GPIO_PIN_13);
	HAL_Delay (1000) ;
	HAL_GPIO_TogglePin (GPIOC, GPIO_PIN_13);
	HAL_Delay (1000);
	HAL_GPIO_TogglePin (GPIOC, GPIO_PIN_13);
	HAL_Delay (1000) ;
	HAL_GPIO_TogglePin (GPIOC, GPIO_PIN_13);
	HAL_Delay(1000);
	HAL_GPIO_TogglePin (GPIOC, GPIO_PIN_13);
	HAL_Delay(1000);
	HAL_GPIO_TogglePin (GPIOC, GPIO_PIN_13);
	HAL_Delay(1000);
	HAL_GPIO_TogglePin (GPIOC, GPIO_PIN_13);
	HAL_Delay(1000);
	Status = 0;
	}
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
