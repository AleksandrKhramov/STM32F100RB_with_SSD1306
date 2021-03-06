/*
GPIO_CRL/H_CNFX
or
GPIO_CRL/H_MODEX
4 - 0100 - X_0
8 - 1000 - X_1
C - 1100 - X
*/
#include "main.h"

int main(void)
{
	RCC_Init();
	GPIO_Init();
	SPI1_Init();	
	SSD1306_Init();				
	char LastNumber = 0;
	bool Mode_Rectangle = false;
	uint8_t SelNum = 0;
	bool Pressed = false;
	uint8_t Iteration = 0;
	//xTaskCreate(vTaskLed1, "LED1", 32, NULL, 1, NULL);
	//xTaskCreate(vTaskButton, "ButtonClick", 32, NULL, 1, NULL);
	
	//vTaskStartScheduler();
	
	while(1)
	{
		if((GPIOA->IDR & GPIO_IDR_IDR0) != 0)
		{
			if(!Pressed)
			{
				Iteration = 8;
				
				if(SelNum < 4)
					++SelNum;
				else
					SelNum = 0;
			}
			
			Pressed = true;
		}	
		else
			Pressed = false;

		GPIOC->BSRR |= GPIO_BSRR_BS8;//Set 9-th pin to 1	
		delay_ms(50);

		if(Iteration > 7)
		{
			Iteration = 0;
			disp1color_FillScreenbuff(0);
				
						
			disp1color_printf(0, 1, FONTID_6X8M,  "               �: 32.53%d %cC\n\r",  LastNumber, 0x80);
			
			disp1color_DrawLine(0, 10, 127, 10);
			disp1color_printf(0, 13, FONTID_6X8M, "          R1: 0.001044 ��\n\r"  
													"          R2: 0.010014 ��\n\r"
													"          R3: 0.101004 ��\n\r"
													"          R4: 1.000004 ��\n\r"
													"          R5: 10.01044 ��");
				
			
			disp1color_DrawRectangle(SEL_RECT_L, SEL_RECT_T + SelNum*9, SEL_RECT_L + 4, SEL_RECT_T + 2 + SelNum*9);

			if(Mode_Rectangle)
				disp1color_DrawRectangle(MODE_RECT_L - 1, MODE_RECT_T - 1, MODE_RECT_L + 3, MODE_RECT_T + 3);	
			else
				disp1color_DrawRectangle(MODE_RECT_L, MODE_RECT_T, MODE_RECT_L + 2, MODE_RECT_T + 2);


			disp1color_printf(118, 1, FONTID_6X8M, "%c", 0x81);

			Mode_Rectangle = !Mode_Rectangle;

			disp1color_UpdateFromBuff();
				
			if(++LastNumber >= 10)
				LastNumber = 0;

			
			/*
			disp1color_printf(0, 1, FONTID_6X8M,  "             �����  �  ������\n\r");
			disp1color_printf(118, 1, FONTID_6X8M, "%c", 0x81);

			//disp1color_printf(0, 1, FONTID_6X8M,  "             �����  ��  �����\n\r");
			//disp1color_printf(118, 1, FONTID_6X8M, "x");

			disp1color_DrawLine(0, 10, 127, 10);
			disp1color_printf(28, 23, FONTID_10X16F, "32.537 %cC", 0x80);
			disp1color_DrawLine(0, 54, 127, 54);

			disp1color_printf(43, 57, FONTID_6X8M, "R1: 0.001044 ��");  

			disp1color_DrawLine(34, 59, 36, 57);
			disp1color_DrawLine(36, 57, 38, 59);
			disp1color_DrawLine(37, 59, 35, 59);

			disp1color_DrawLine(34, 61, 36, 63);
			disp1color_DrawLine(36, 63, 38, 61);
			disp1color_DrawLine(37, 61, 35, 61);

			if(Mode_Rectangle)
				disp1color_DrawRectangle(MODE_RECT_L - 1, MODE_RECT_T - 1, MODE_RECT_L + 3, MODE_RECT_T + 3);	
			else
				disp1color_DrawRectangle(MODE_RECT_L, MODE_RECT_T, MODE_RECT_L + 2, MODE_RECT_T + 2);
			disp1color_UpdateFromBuff();

			Mode_Rectangle = !Mode_Rectangle;*/

		}	
		else
			++Iteration;

		GPIOC->BSRR |= GPIO_BSRR_BR8;//Set 9-th pin to 0
			delay_ms(50);
	}
}
//*************************************************************************************************
void RCC_Init(void)
{
 //Description on 13:30 of third lesson
	
 //Clock control register settings
	
	RCC->CR |= ((uint32_t)RCC_CR_HSEON); 												// Enable HSE   							/*!< External High Speed clock enable */
	//Set and cleared by software.
	//Cleared by hardware to stop the HSE oscillator when entering in Stop or Standby mode. This
	//bit cannot be reset if the HSE oscillator is used directly or indirectly as the system clock.
	
	while (!(RCC->CR & RCC_CR_HSERDY));													// Ready start HSE						/*!< External High Speed clock ready flag */	
	//Set by hardware to indicate that the HSE oscillator is stable. This bit needs 6 cycles of the
	//HSE oscillator clock to go to zero after HSEON is reset.
	
	FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;					// Cloclk Flash memory

 //Clock configuration register
	
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;														// AHB = SYSCLK/1
	//AHB prescaler
  //Set and cleared by software to control the division factor of the AHB clock.
	
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;														// APB1 = HCLK/1	
	//APB low-speed prescaler (APB1)
	//Set and cleared by software to control the division factor of the APB low-speed clock (PCLK1).
	//Warning: the software has to set correctly these bits to not exceed 36 MHz on this domain.
	
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;														// APB2 = HCLK/1
	//APB high-speed prescaler (APB2)
	//Set and cleared by software to control the division factor of the APB high-speed clock (PCLK2).
	
	//Description of next three registers are below themselves 
	RCC->CFGR &= ~RCC_CFGR_PLLMULL;               							// clear PLLMULL bits
	RCC->CFGR &= ~RCC_CFGR_PLLSRC;															// clearn PLLSRC bits
	RCC->CFGR &= ~RCC_CFGR_PLLXTPRE;														// clearn PLLXTPRE bits
	
	RCC->CFGR |= RCC_CFGR_PLLSRC_PREDIV1; 											// source HSE									/*!< PREDIV1 clock selected as PLL entry clock source */
	//PLL entry clock source
	//Set and cleared by software to select PLL clock source. This bit can be written only when
	//PLL is disabled.
	
	RCC->CFGR |= RCC_CFGR_PLLXTPRE_PREDIV1_Div2; 								// source HSE/2 = 4 MHz				/*!< PREDIV1 clock divided by 2 for PLL entry */
	//LSB of division factor PREDIV1
	//Set and cleared by software to select the least significant bit of the PREDIV1 division factor. 
	
	RCC->CFGR |= RCC_CFGR_PLLMULL6; 														// PLL x6: clock = 4 MHz * 6 = 24 MHz		/*!< PLL input clock*6 */
	//PLL multiplication factor
	//These bits are written by software to define the PLL multiplication factor. These bits can be
	//written only when PLL is disabled.

 //Clock configuration register	
	RCC->CR |= RCC_CR_PLLON;                      							// enable PLL
	//PLL enable
	//Set and cleared by software to enable PLL.
	//Cleared by hardware when entering Stop or Standby mode. This bit can not be reset if the
	//PLL clock is used as system clock or is selected to become the system clock.
	
	while((RCC->CR & RCC_CR_PLLRDY) == 0);      								// wait till PLL is ready
	//PLL clock ready flag
	//Set by hardware to indicate that the PLL is locked.

 //Clock configuration register		
	RCC->CFGR &= ~RCC_CFGR_SW;                   							 	// clear SW bits
  RCC->CFGR |= RCC_CFGR_SW_PLL;                 							// select source SYSCLK = PLL
	//System clock switch
	//Set and cleared by software to select SYSCLK source.
	//Set by hardware to force HSI selection when leaving Stop and Standby mode or in case of
	//failure of the HSE oscillator used directly or indirectly as system clock (if the Clock Security
	//System is enabled).

	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_1);  			// wait till PLL is used
	//System clock switch status
	//Set and cleared by hardware to indicate which clock source is used as system clock.
	
}
//-----------------------------------------------------------------------------------------
void GPIO_Init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;	//SET Clock System
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;                       	// enable clock for port A
	
	
	GPIOC->CRH &= ~GPIO_CRH_CNF8;				//
	GPIOC->CRH |= GPIO_CRH_MODE8_0;			//
	
	GPIOC->CRH &= ~GPIO_CRH_CNF9;				//
	GPIOC->CRH |= GPIO_CRH_MODE9_0;			//
	
	GPIOC->CRL &= ~GPIO_CRL_CNF0;				//
	GPIOC->CRL &= ~GPIO_CRL_MODE0;
}
//------------------------------------------------------------------------------------------
void SPI1_Init(void)
{
	//Enable clock for SPI1 и GPIOA
  	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; 
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

	/*********************************************/
	/*** Setting pins GPIOA for work with SPI1 ***/
	/*********************************************/
	//PA7 - MOSI
	//PA6 - MISO
	//PA5 - SCK
	
	//First, clear all comfiguration bits
  	GPIOA->CRL &= ~GPIO_CRL_CNF5;
	GPIOA->CRL &= ~GPIO_CRL_CNF6;
	GPIOA->CRL &= ~GPIO_CRL_CNF7;
	
	GPIOA->CRL &= ~GPIO_CRL_MODE5;
	GPIOA->CRL &= ~GPIO_CRL_MODE5;
	GPIOA->CRL &= ~GPIO_CRL_CNF5;
	
	
	//Setting
	//SCK: MODE5 = 0x03 (11b); CNF5 = 0x02 (10b)
	GPIOA->CRL |= GPIO_CRL_CNF5_1; 
	GPIOA->CRL |= GPIO_CRL_MODE5;
  
	//MISO: MODE6 = 0x00 (00b); CNF6 = 0x01 (01b)
	GPIOA->CRL |= GPIO_CRL_CNF6_0; 
	GPIOA->CRL &= ~GPIO_CRL_MODE6;
  
	//MOSI: MODE7 = 0x03 (11b); CNF7 = 0x02 (10b)
	GPIOA->CRL |= GPIO_CRL_CNF7_1; 
	GPIOA->CRL |= GPIO_CRL_MODE7;
	
  
	/**********************/
	/***  Setting SPI1  ***/
	/**********************/
  
	SPI1->CR1 |= SPI_CR1_BR_2;        			//Baud rate: F_PCLK/32
	SPI1->CR1 &= ~SPI_CR1_CPOL;					//Clock polarity SPI: 0
	SPI1->CR1 &= ~SPI_CR1_CPHA;					//Clock phase SPI: 0
	SPI1->CR1 &= ~SPI_CR1_DFF;  				//Data frame format is 8 bit 
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST;    			//MSB first
	SPI1->CR1 |= SPI_CR1_SSM;          			//Software NSS management SS
	SPI1->CR1 |= SPI_CR1_SSI;          			//SS in high level
  
	SPI1->CR1 |= SPI_CR1_MSTR;         			//Master mode
	
	SPI1->CR1 |= SPI_CR1_SPE; 					//Enable SPI	
}
//-----------------------------------------------------------------------------------------
void SPI1_Write(uint8_t *pBuff, uint16_t BuffLen)
{
	for(uint16_t i = 0; i < BuffLen; ++i)
	{
		//Expect while buffer ready
		while(!(SPI1->SR & SPI_SR_TXE)) ;	
		//Send data
		SPI1->DR = pBuff[i];

		DelayMicro(7);
	}	
}
//------------------------------------------------------------------------------------------
void SSD1306_GPIO_init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	//----------------------------
	//SSD1306_RESET
	GPIOA->CRL &= ~GPIO_CRL_CNF3;
	GPIOA->CRL &= ~GPIO_CRL_MODE3;

	GPIOA->CRL &= ~GPIO_CRL_CNF3; 
	GPIOA->CRL |= GPIO_CRL_MODE3_0;

	//SSD1306_DC
	GPIOA->CRL &= ~GPIO_CRL_CNF1;
	GPIOA->CRL &= ~GPIO_CRL_MODE1;

	GPIOA->CRL &= ~GPIO_CRL_CNF1; 
	GPIOA->CRL |= GPIO_CRL_MODE1_0;

	//SSD1306_CS
	GPIOA->CRL &= ~GPIO_CRL_CNF2;
	GPIOA->CRL &= ~GPIO_CRL_MODE2;
	
	GPIOA->CRL &= ~GPIO_CRL_CNF2; 
	GPIOA->CRL |= GPIO_CRL_MODE2_0;
	//----------------------------
}
//*************************************************************************************************
/*void vTaskLed1(void *argument)
{
	while(1)
	{
		GPIOC->BSRR |= GPIO_BSRR_BS9;//Set 9-th pin to 1	
		vTaskDelay(1000);							//task sleep
		GPIOC->BSRR |= GPIO_BSRR_BR9;//Set 9-th pin to 0
		vTaskDelay(1000);
	}
}
//------------------------------------------------------------------------------------------
void vTaskButton(void *argument)
{
	while(1)
	{		
		if((GPIOA->IDR & GPIO_IDR_IDR0) != 0)
		{
			//FONTID_6X8M
			//FONTID_10X16F
			//SSD1306_SetInverseOn();
			
			disp1color_FillScreenbuff(0);
			GPIOC->BSRR |= GPIO_BSRR_BS8;//Set 9-th pin to 1	
			//vTaskDelay(300);
			disp1color_printf(5, 10, FONTID_6X8M, "���-����������");
			disp1color_UpdateFromBuff();
			GPIOC->BSRR |= GPIO_BSRR_BR8;//Set 9-th pin to 0
		}
		//vTaskDelay(300);
	}
}*/
//------------------------------------------------------------------------------------------
//*************************************************************************************************
//------------------------------------------------------------------------------------------
void DelayMicro(uint32_t time)
{
	for(int i = 0; i < 6; ++i)
		delay(time);
}
//------------------------------------------------------------------------------------------
void delay_ms(uint32_t time)
{
	DelayMicro(1000*time);
}
//------------------------------------------------------------------------------------------
void delay(uint32_t time){
		
	uint32_t i;
	for(i = 0; i < time; i++){
	}
}
//------------------------------------------------------------------------------------------

