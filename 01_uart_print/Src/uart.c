#include "uart.h"

#define UART_BAUDRATE             115200 //9600
#define SYS_FREQ                  16000000
#define APB1_CLK                  SYS_FREQ

static uint16_t compute_uart_bd(uint32_t pclk, uint32_t desired_baud);
//void uart_init(void)
//{
//  RCC->AHB1ENR |= 0x1;           // Enable GPIOA clock access (RCC_AHB1ENR_GPIOAEN)
//  RCC->APB1ENR |= 0x20000;       // Enable USART2 clock access (RCC_APB1ENR_USART2EN)
//
//  // Configure PA2, PA3 for USART2 Tx, Rx
//  GPIOA->AFR[0] |= 0x7700;       // 7 = 0111, alt7 for USART2
//  GPIOA->MODER |= 0x00A0;        // Enable Alternate function for PA2, PA3
//
//  USART2->BRR = 0x0683;          // 9600 baud @ 16Mhz
//  USART2->CR1 = 0x000c;          // Enable Tx, Rx, 8-bit data
//  USART2->CR2 = 0x000;           // Set 1 stop bit
//  USART2->CR3 = 0x000;           // No flow control
//  USART2->CR1 |= 0x2000;         // Enable USART2
//}

// ABP1
// PA2 ==> USART2_TX (AF07)
// PA3 ==> USART2_RX (AF07)
void uart_init(void)
{
  //1. enable clock access to UART pins' GPIO port A (0b1 |=  GPIOAEN[BIT0]) [RM page: 117]
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

  //2. PA2 mode to Alternate function mode (0b10 |= MODER2[5:4]) [RM page: 157]
  GPIOA->MODER &= ~(1U << 4);
  GPIOA->MODER |= (1U << 5);

  //3. PA3 mode to Alternate function mode (0b10 |= MODER3[7:6]) [RM page: 157]
  GPIOA->MODER &= ~(1U << 6);
  GPIOA->MODER |= (1U << 7);

  //4. PA2 Alternate function type to USART2_TX (0b0111:AF07 |= AFRL2[11:8]) [RM page: 161]
  // GPIOx_AFRL register
  GPIOA->AFR[0] |= (1U << 8);
  GPIOA->AFR[0] |= (1U << 9);
  GPIOA->AFR[0] |= (1U << 10);
  GPIOA->AFR[0] &= ~(1U << 11);

  //5. PA3 Alternate function type to USART2_RX (0b0111:AF07 |= AFRL2[15:12]) [RM page: 161]
  // GPIOx_AFRL register
  GPIOA->AFR[0] |= (1U << 12);
  GPIOA->AFR[0] |= (1U << 13);
  GPIOA->AFR[0] |= (1U << 14);
  GPIOA->AFR[0] &= ~(1U << 15);


  //                  [ Configure UART module ]

  //1. Enable clock access to UART module (0b1 |= USART2EN[BIT17]) [RM page: 119]
  RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

  //2. Disable UART module, USART_CR1
  USART2->CR1 &= ~USART_CR1_UE;

  //3. Set UART Baud rate
  USART2->BRR = compute_uart_bd(APB1_CLK, UART_BAUDRATE);

  //4. Set tranfer direction, want either or both Tx and Rx
//  USART2->CR1 = USART_CR1_TE | USART_CR1_RE;

//    USART2->BRR = 0x0683;          // 9600 baud @ 16Mhz
    USART2->CR1 = 0x000c;          // Enable Tx, Rx, 8-bit data
    USART2->CR2 = 0x000;           // Set 1 stop bit
    USART2->CR3 = 0x000;           // No flow control

  //5. Enable UART2 interrupt in NVIC

  //6. Enable the UART module
  USART2->CR1 |= USART_CR1_UE;
}

void uart_write(int ch)
{
  while(!(USART2->SR & USART_SR_TXE));  // Wait until Tx (Transmit data register) gets empty (0x0080)
  USART2->DR = (ch & 0xFF);
}

int uart_read(void)
{
  while (!(USART2->SR & 0x0020));
  return USART2->DR;
}

static uint16_t compute_uart_bd(uint32_t pclk, uint32_t desired_baud)
{
  return ((pclk +(desired_baud/2U))/desired_baud);
}


// Bind USART2 to the C++ console (only applicable for Keil IDE)
// ** Must use uart.cpp extension **
//namespace std
//{
//  struct __FILE
//  {
//	int handle;
//  };
//
//  FILE __stdout;
//  FILE __stdin;
//  FILE __stderr;
//
//  int fgetc(FILE* f)
//  {
//	int c;
//	c = uart_read();
//
//	if (c=='\r')
//	{
//	  uart_write(c);
//	  c = '\n';
//	}
//
//	uart_write(c);
//
//	return c;
//  }
//
//  int fputc(int c, FILE* stream)
//  {
//	return uart_write(c);
//  }
//
//  int ferror(FILE* stream)
//  {
//	return 1;
//  }
//
//  long int ftell(FILE* stream)
//  {
//	return 1;
//  }
//
//  int fclose(FILE* f)
//  {
//	return 1;
//  }
//
//  int fseek(FILE* f, long nPos, int nMode)
//  {
//	return 0;
//  }
//
//  int fflush(FILE* f)
//  {
//	return 1;
//  }
//}
