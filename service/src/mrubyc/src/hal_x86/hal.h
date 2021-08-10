/*! @file
  @brief
  Hardware abstraction layer
        for x86

  <pre>
  Copyright (C) 2018 Kyushu Institute of Technology.
  Copyright (C) 2018 Shimane IT Open-Innovation Center.

  This file is distributed under BSD 3-Clause License.
  </pre>
*/

#ifndef MRBC_SRC_HAL_H_
#define MRBC_SRC_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif
/***** Constant values ******************************************************/
/***** Macros ***************************************************************/
#if !defined(MRBC_TICK_UNIT)
#define MRBC_TICK_UNIT_1_MS   1
#define MRBC_TICK_UNIT_2_MS   2
#define MRBC_TICK_UNIT_4_MS   4
#define MRBC_TICK_UNIT_10_MS 10
// You may be able to reduce power consumption if you configure
// MRBC_TICK_UNIT_2_MS or larger.
#define MRBC_TICK_UNIT MRBC_TICK_UNIT_1_MS
// Substantial timeslice value (millisecond) will be
// MRBC_TICK_UNIT * MRBC_TIMESLICE_TICK_COUNT (+ Jitter).
// MRBC_TIMESLICE_TICK_COUNT must be natural number
// (recommended value is from 1 to 10).
#define MRBC_TIMESLICE_TICK_COUNT 10
#endif
#if !defined(MRBC_NO_TIMER)	// use hardware timer.
#define sti() __asm__ ("sti"::)
#define cli() __asm__ ("cli"::)
#define hlt() __asm__ ("hlt"::)
# define hal_init()        ((void)0) // this wont work now...
# define hal_enable_irq()  sti()
# define hal_disable_irq() cli()
# define hal_idle_cpu()    hlt()

#else // MRBC_NO_TIMER
static void sleep_ms_fake(int useconds) {
#define MULT_FACTOR 100
		unsigned long int i,j;
		unsigned long int max = useconds * MULT_FACTOR;
#undef MULT_FACTOR
		for (j=0;j<max;j++)
			for (i=0;i<15;i++) asm("nop");
}
# define hal_init()        ((void)0)
# define hal_enable_irq()  ((void)0)
# define hal_disable_irq() ((void)0)
# define hal_idle_cpu()    ((sleep_ms_fake(1)), mrbc_tick())

#endif


/***** Typedefs *************************************************************/
/***** Global variables *****************************************************/
/***** Function prototypes **************************************************/
int hal_write(int fd, const void *buf, int nbytes);
int hal_flush(int fd);


/***** Inline functions *****************************************************/


#ifdef __cplusplus
}
#endif
#endif // ifndef MRBC_SRC_HAL_H_
