#include "debug.hpp"
#include "uart.h"

//#define DEBUG_ITM

#ifdef DEBUG_ITM
extern "C" int __io_putchar(int ch)
{
  ITM_SendChar(ch);
  return ch;
}
#else
extern "C" int __io_putchar(int ch)
{
  uart_write(ch);
  return ch;
}
#endif

void log_error(char* p)
{
  std::printf("[ERROR] ");
  std::printf("%s", p);
  std::printf("\r\n");
}

void log_info(char* p)
{
  std::printf("[INFO] ");
  std::printf("%s", p);
  std::printf("\r\n");
}


