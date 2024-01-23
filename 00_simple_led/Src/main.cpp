#include "mcal_reg.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

// PA5 - Nucleo board LED
// GPIOA - AHB1
// GPIOA - 0x4002 0000
// MODER - 0x00
// ODR - 0x14
// AHB1ENR - AHB1 + RCC + 0x30

#define RCC_AHBIENR        (*((volatile unsigned int *)0x40023830))
//#define GPIOA_MODER        (*((volatile unsigned int *)0x40020000))
//#define GPIOA_ODR          (*((volatile unsigned int *)0x40020014))

class Led
{
public:
  typedef std::uint32_t port_type;
  typedef std::uint32_t bval_type;

  Led(const port_type p, bval_type b): port(p), bval(b)
  {
	// Disable pin
	*reinterpret_cast<volatile port_type*>(port) &= ~(1U<<bval);  // uint32_t volatile * const port &= (1U<<bval)

	// Set pin mode to output
	const port_type gpio_mode_reg = port - 0x14;
	*reinterpret_cast<volatile port_type*>(gpio_mode_reg) |= (1U << (bval*2));
  }

  void toggle()const
  {
	*reinterpret_cast<volatile port_type*>(port) ^= (1U<<bval);
  }

  void m_delay(std::uint32_t count)
  {
	for (std::uint32_t i=0; i< (count * 1000); i++);
  }

private:
  const port_type port;
  const bval_type bval;
};

int main(void)
{
  RCC_AHBIENR |= (1U<<0);   // Enable the clock access to GPIOA
  Led led5(mcal::reg::gpioa, mcal::reg::gpio_pin5);

  Led led6(mcal::reg::gpioa, mcal::reg::gpio_pin6);

  while(1)
  {
	led5.toggle();
	led6.toggle();
	led5.m_delay(1000);
  }
    /* Loop forever */
	for(;;);
}
