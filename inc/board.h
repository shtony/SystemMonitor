#ifndef _BOARD_H
#define _BOARD_H

#include <stdint.h>

void	peripherals_init(void);
void	main_loop(void);

#if defined(__GNUC__)
__attribute__((__always_inline__))
#endif /* __GNUC__ */
static inline uint32_t
get_systick(void)
{
	extern uint32_t _SysTick;
	return _SysTick;
}

#endif /* _BOARD_H */
