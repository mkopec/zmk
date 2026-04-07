/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_behavior_disp_toggle

#include <zephyr/device.h>
#include <zephyr/devicetree.h>

#include <drivers/behavior.h>
#include <zmk/behavior.h>
#include <zmk/ble.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

/*
 * Advertisement manufacturer data format:
 *   Byte 0-1: Company ID 0xFFFF (unassigned, LE)
 *   Byte 2:   0x5A ('Z') — ZMK marker
 *   Byte 3:   0x44 ('D') — display command
 *   Byte 4:   0x01 = DP-1, 0x02 = HDMI-A-1
 *   Byte 5:   sequence number (incremented each press)
 */

#define DISP_MFR_COMPANY_ID_LO 0xFF
#define DISP_MFR_COMPANY_ID_HI 0xFF
#define DISP_MFR_MAGIC_ZMK     0x5A /* 'Z' */
#define DISP_MFR_MAGIC_DISP    0x44 /* 'D' */
#define DISP_CMD_DP1           0x01
#define DISP_CMD_HDMI          0x02

#define DISP_BROADCAST_DURATION_MS 500

static uint8_t disp_seq;
static bool next_is_dp1 = true; /* first press → DP-1 */

static int on_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                     struct zmk_behavior_binding_event event) {
    uint8_t cmd = next_is_dp1 ? DISP_CMD_DP1 : DISP_CMD_HDMI;
    uint8_t mfr[] = {
        DISP_MFR_COMPANY_ID_LO,
        DISP_MFR_COMPANY_ID_HI,
        DISP_MFR_MAGIC_ZMK,
        DISP_MFR_MAGIC_DISP,
        cmd,
        disp_seq++,
    };

    LOG_DBG("Broadcasting display toggle: cmd=0x%02x seq=%u", cmd, disp_seq - 1);
    next_is_dp1 = !next_is_dp1;

    return zmk_ble_broadcast_adv(mfr, sizeof(mfr), DISP_BROADCAST_DURATION_MS);
}

static const struct behavior_driver_api behavior_disp_toggle_driver_api = {
    .binding_pressed = on_keymap_binding_pressed,
};

BEHAVIOR_DT_INST_DEFINE(0, NULL, NULL, NULL, NULL, POST_KERNEL,
                        CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                        &behavior_disp_toggle_driver_api);

#endif /* DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT) */
