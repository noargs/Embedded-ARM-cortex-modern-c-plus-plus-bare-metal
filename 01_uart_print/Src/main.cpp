#include "main.h"
#include "uart.h"
#include "debug.hpp"



int main(void)
{
  uart_init();

  while(1)
  {
	printf("Hello from embedded c++ \r\n");
	for(int i=0; i<700000;i++);
	debug_log("Hello there!");
	for(int i=0; i<700000;i++);
	debug_log2("Hello again!");


	// [[OUTPUT]]

	// Hello from embedded c++
	// [DEBUG] <main:15>: Hello there!
	// [DEBUG LEVEL2] <main:17>: Hello again!
  }
}
