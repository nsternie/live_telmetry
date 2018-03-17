/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim10;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM10_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM6_Init(void);
static void MX_TIM7_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_ADC1_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_TIM10_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();

  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(GYRO1_CS_GPIO_Port, GYRO1_CS_Pin, 1);
  HAL_GPIO_WritePin(GYRO2_CS_GPIO_Port, GYRO2_CS_Pin, 1);
  HAL_GPIO_WritePin(GYRO3_CS_GPIO_Port, GYRO3_CS_Pin, 1);
  HAL_GPIO_WritePin(GYRO4_CS_GPIO_Port, GYRO4_CS_Pin, 1);
  HAL_GPIO_WritePin(GYRO5_CS_GPIO_Port, GYRO5_CS_Pin, 1);
  HAL_GPIO_WritePin(GYRO6_CS_GPIO_Port, GYRO6_CS_Pin, 1);
  HAL_GPIO_WritePin(MS5607_CS_GPIO_Port, MS5607_CS_Pin, 1);
  HAL_GPIO_WritePin(ADXL_CS_GPIO_Port, ADXL_CS_Pin, 1);
  HAL_GPIO_WritePin(GPS_nRST_GPIO_Port, GPS_nRST_Pin, 1);
  HAL_GPIO_WritePin(MEM_CS_GPIO_Port, MEM_CS_Pin, 1);
  HAL_GPIO_WritePin(CS_Radio_GPIO_Port, CS_Radio_Pin, 1);

  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 1);
  HAL_GPIO_WritePin(nRST_RADIO_GPIO_Port, nRST_RADIO_Pin, 1);
  HAL_Delay(1000);

  volatile uint8_t TXData[30] = {0};
  volatile uint8_t RXData[30] = {0};

  //Set into standby
  TXData[0] = 0x80;
  TXData[1] = 0x00;
  HAL_GPIO_WritePin(CS_Radio_GPIO_Port, CS_Radio_Pin, 0);
  HAL_SPI_TransmitReceive(&hspi1, TXData, RXData, 2, 0xff);
  HAL_GPIO_WritePin(CS_Radio_GPIO_Port, CS_Radio_Pin, 1);
  HAL_Delay(10);

  //Set pkt type to LORA
  TXData[0] = 0x8A;
  TXData[1] = 0x01;
  HAL_GPIO_WritePin(CS_Radio_GPIO_Port, CS_Radio_Pin, 0);
  HAL_SPI_TransmitReceive(&hspi1, TXData, RXData, 2, 0xff);
  HAL_GPIO_WritePin(CS_Radio_GPIO_Port, CS_Radio_Pin, 1);
  HAL_Delay(10);

  //Set RF frequency to ~2.4GHz
  TXData[0] = 0x86;
  TXData[1] = 0xB8;
  TXData[2] = 0x9D;
  TXData[3] = 0x80;
  HAL_GPIO_WritePin(CS_Radio_GPIO_Port, CS_Radio_Pin, 0);
  HAL_SPI_TransmitReceive(&hspi1, TXData, RXData, 4, 0xff);
  HAL_GPIO_WritePin(CS_Radio_GPIO_Port, CS_Radio_Pin, 1);
  HAL_Delay(10);

  //Set TX/RX buffer base addresses
  TXData[0] = 0x8F;
  TXData[1] = 0x00;	//txBase
  TXData[2] = 0x80;	//rxBase
  HAL_GPIO_WritePin(CS_Radio_GPIO_Port, CS_Radio_Pin, 0);
  HAL_SPI_TransmitReceive(&hspi1, TXData, RXData, 3, 0xff);
  HAL_GPIO_WritePin(CS_Radio_GPIO_Port, CS_Radio_Pin, 1);
  HAL_Delay(10);

  //Set modulation params
  TXData[0] = 0x8B;
  TXData[1] = 0xB0; //SF = 11
  TXData[2] = 0x0A; //BW = 1600
  TXData[3] = 0x05; //LI 4/5
  HAL_GPIO_WritePin(CS_Radio_GPIO_Port, CS_Radio_Pin, 0);
  HAL_SPI_TransmitReceive(&hspi1, TXData, RXData, 4, 0xff);
  HAL_GPIO_WritePin(CS_Radio_GPIO_Port, CS_Radio_Pin, 1);
  HAL_Delay(10);

  //Write 0x32 to reg 0x925 as per data sheet note on pg 115
  //Not 100% sure this is correct
  TXData[0] = 0x18;
  TXData[1] = 0x09;
  TXData[2] = 0x25;
  TXData[3] = 0x32;
  HAL_GPIO_WritePin(CS_Radio_GPIO_Port, CS_Radio_Pin, 0);
  HAL_SPI_TransmitReceive(&hspi1, TXData, RXData, 4, 0xff);
  HAL_GPIO_WritePin(CS_Radio_GPIO_Port, CS_Radio_Pin, 1);
  HAL_Delay(10);

  //Set packet params
  TXData[0] = 0x8C;
  TXData[1] = 0x0C; //16 (12???) symbol preamble
  TXData[2] = 0x00; //explicit header (variable len pkt)
  TXData[3] = 0x16; //Payload (22 bytes)
  TXData[4] = 0x20; //CRC enabled
  TXData[5] = 0x40; //Standard IQ
  HAL_GPIO_WritePin(CS_Radio_GPIO_Port, CS_Radio_Pin, 0);
  HAL_SPI_TransmitReceive(&hspi1, TXData, RXData, 6, 0xff);
  HAL_GPIO_WritePin(CS_Radio_GPIO_Port, CS_Radio_Pin, 1);
  HAL_Delay(10);

  //Set TX power & ramp speed
  TXData[0] = 0x8E;
  TXData[1] = 0x1F;
  TXData[2] = 0xE0;
  HAL_GPIO_WritePin(CS_Radio_GPIO_Port, CS_Radio_Pin, 0);
  HAL_SPI_TransmitReceive(&hspi1, TXData, RXData, 3, 0xff);
  HAL_GPIO_WritePin(CS_Radio_GPIO_Port, CS_Radio_Pin, 1);
  HAL_Delay(10);

  //Set IRQs
  TXData[0] = 0x8D;
  TXData[1] = 0x00;
  TXData[2] = 0x03; //TXDone and RXDone active
  TXData[3] = 0x00;
  TXData[4] = 0x01; //TXDone on DIO1
  TXData[5] = 0x00;
  TXData[6] = 0x02; //RXDone on DIO2
  TXData[7] = 0x00;
  TXData[8] = 0x00;
  HAL_GPIO_WritePin(CS_Radio_GPIO_Port, CS_Radio_Pin, 0);
  HAL_SPI_TransmitReceive(&hspi1, TXData, RXData, 9, 0xff);
  HAL_GPIO_WritePin(CS_Radio_GPIO_Port, CS_Radio_Pin, 1);
  HAL_Delay(10);

  //Read back some params to make sure comms are okay
  //Read back packet type, should be 0x01;
  TXData[0] = 0x03;
  TXData[1] = 0x00;
  TXData[2] = 0x00;
  HAL_GPIO_WritePin(CS_Radio_GPIO_Port, CS_Radio_Pin, 0);
  HAL_SPI_TransmitReceive(&hspi1, TXData, RXData, 3, 0xff);
  HAL_GPIO_WritePin(CS_Radio_GPIO_Port, CS_Radio_Pin, 1);
  HAL_Delay(10);

  while(1){
	  TXData[0] = 0x1A;
	  TXData[1] = 0x00; //TXBase
	  TXData[2] = 0x01;
	  TXData[3] = 0x0F;
	  TXData[4] = 0x02;
	  TXData[5] = 0x0E;
	  TXData[6] = 0x03;
	  TXData[7] = 0x0D;
	  HAL_GPIO_WritePin(CS_Radio_GPIO_Port, CS_Radio_Pin, 0);
	  HAL_SPI_TransmitReceive(&hspi1, TXData, RXData, 24, 0xff);
	  HAL_GPIO_WritePin(CS_Radio_GPIO_Port, CS_Radio_Pin, 1);
	  HAL_Delay(1); //Replace with a check of the busy pin in future for faster operation
	  TXData[0] = 0x83;
	  TXData[1] = 0x00;
	  TXData[2] = 0x00;
	  TXData[3] = 0x00;
	  HAL_GPIO_WritePin(CS_Radio_GPIO_Port, CS_Radio_Pin, 0);
	  HAL_SPI_TransmitReceive(&hspi1, TXData, RXData, 24, 0xff);
	  HAL_GPIO_WritePin(CS_Radio_GPIO_Port, CS_Radio_Pin, 1);

	  HAL_Delay(500);
  }
  //Write function to clear IRQs
  //Write function to get IRQ status
  //Move functions to seperate file

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Activate the Over-Drive mode 
    */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* ADC1 init function */
static void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* SPI2 init function */
static void MX_SPI2_Init(void)
{

  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM6 init function */
static void MX_TIM6_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 17999;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 99;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM7 init function */
static void MX_TIM7_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;

  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 1799;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 499;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM10 init function */
static void MX_TIM10_Init(void)
{

  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 1799;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 999;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 921600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART3 init function */
static void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, MEM_CS_Pin|CS_Radio_Pin|GYRO5_CS_Pin|GYRO4_CS_Pin 
                          |ADXL_CS_Pin|LED1_Pin|LED2_Pin|GPS_nRST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, PYRO_2_FIRE_Pin|GYRO1_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, PYRO_1_FIRE_Pin|MS5607_CS_Pin|GYRO3_CS_Pin|GYRO2_CS_Pin 
                          |nRST_RADIO_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GYRO6_CS_GPIO_Port, GYRO6_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : GYRO2_INT_Pin DIO1_Radio_Pin DIO2_Radio_Pin DIO3_Radio_Pin 
                           GYRO4_INT_Pin */
  GPIO_InitStruct.Pin = GYRO2_INT_Pin|DIO1_Radio_Pin|DIO2_Radio_Pin|DIO3_Radio_Pin 
                          |GYRO4_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : MEM_CS_Pin CS_Radio_Pin LED1_Pin LED2_Pin 
                           GPS_nRST_Pin */
  GPIO_InitStruct.Pin = MEM_CS_Pin|CS_Radio_Pin|LED1_Pin|LED2_Pin 
                          |GPS_nRST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : BUSY_Radio_Pin */
  GPIO_InitStruct.Pin = BUSY_Radio_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BUSY_Radio_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PYRO_2_FIRE_Pin */
  GPIO_InitStruct.Pin = PYRO_2_FIRE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(PYRO_2_FIRE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PYRO_1_FIRE_Pin nRST_RADIO_Pin */
  GPIO_InitStruct.Pin = PYRO_1_FIRE_Pin|nRST_RADIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : MS5607_CS_Pin GYRO3_CS_Pin GYRO2_CS_Pin */
  GPIO_InitStruct.Pin = MS5607_CS_Pin|GYRO3_CS_Pin|GYRO2_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : GYRO5_CS_Pin GYRO4_CS_Pin ADXL_CS_Pin */
  GPIO_InitStruct.Pin = GYRO5_CS_Pin|GYRO4_CS_Pin|ADXL_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : GYRO1_INT_Pin GYRO5_INT_Pin ADXL_DRDY_Pin */
  GPIO_InitStruct.Pin = GYRO1_INT_Pin|GYRO5_INT_Pin|ADXL_DRDY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : GYRO1_CS_Pin */
  GPIO_InitStruct.Pin = GYRO1_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GYRO1_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : GYRO6_CS_Pin */
  GPIO_InitStruct.Pin = GYRO6_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GYRO6_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : GYRO6_INT_Pin GYRO3_INT_Pin */
  GPIO_InitStruct.Pin = GYRO6_INT_Pin|GYRO3_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI1_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/