#ifndef UART_H_
#define UART_H_

#ifdef __cplusplus
  extern "C" {
#endif

  #include "main.h"

  void uart_init(void);
  void uart_write(int ch);
  int uart_read(void);

#ifdef __cplusplus
  }
#endif

#endif /* UART_H_ */
