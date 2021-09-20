

#include QMK_KEYBOARD_H
#include "rp.h"
#include "spis.h"

#include <string.h>

#include "quantum.h"

#include "hardware/clocks.h"

extern bool   ch559_start;
extern bool   ch559_update_mode;
matrix_row_t* matrix_mouse_dest;

static void spis_handler(uint8_t const* received, uint32_t receive_len,
                         uint8_t const* next_send) {
    // skip ping packet
    int idx = 0;
    while (received[idx] == 0xc0) {
        idx++;
    }

    for (; idx < receive_len; idx++) {
        uart_recv_callback(received[idx]);
    }
    spis_start();
}

void matrix_init_custom(void) {
    spis_init(spis_handler);
    spis_start();

    setPinOutput(KQ_PIN_CHRST);
    // Assert reset
    writePinHigh(KQ_PIN_CHRST);

    setPinOutput(KQ_PIN_CHBOOT);
    writePinLow(KQ_PIN_CHBOOT);

    // setPinInputHigh(BOOTPIN);

    // setPinOutput(KQ_PIN_LED0);

    // Deassrt reset
    writePinLow(KQ_PIN_CHRST);

    clock_gpio_init(21, CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_XOSC_CLKSRC, 1);

    ch559_start = false;
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    if (ch559_update_mode) {
        return 0;
    }

    static uint32_t reset_timer = 0;
    if ((!ch559_start) && (!ch559_update_mode) &&
        timer_elapsed32(reset_timer) > 1000) {
        uart_buf_init();
        int res = send_reset_cmd();
        if (res == 0) {
            reset_timer = timer_read32();
        }
        // do not return to receive startup response
    }

    matrix_mouse_dest = &current_matrix[MATRIX_MSBTN_ROW];
    return process_packet(current_matrix);
}
