#include "xgpio.h"
#include "xil_io.h"
#include "xparameters.h"
#include <stdint.h>

#define BTNC 0
#define BTNU 1

static uint8_t seg_table[16] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
    0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71
};

int main(void)
{
    uint8_t sw_val = Xil_In8(XPAR_AXI_GPIO_0_BASEADDR);
    uint8_t digit  = sw_val & 0x0F;

    /* 将拨码低 4 位映射到七段码 */
    Xil_Out8(XPAR_AXI_GPIO_1_BASEADDR, seg_table[digit]);
    return 0;
}
