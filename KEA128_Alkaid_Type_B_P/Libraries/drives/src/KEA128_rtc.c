#include "headfile.h"


/* Configure RTC to interrupt every 1 second from OSC (32.768KHz) clock source */
void RTC_init(void)
{
    RTC_MOD = 511; // overflow every 32 times
    RTC_SC = RTC_SC_RTCPS_MASK; // external 32768 clock selected with 1/64 predivider.
    RTC_SC = RTC_SC_RTIF_MASK | RTC_SC_RTIE_MASK; // interrupt cleared and enabled.
}

