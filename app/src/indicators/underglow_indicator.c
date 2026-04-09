/*
 * Copyright (c) 2026 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>

#include <zmk/event_manager.h>
#include <zmk/events/hid_indicators_changed.h>
#include <zmk/hid_indicators.h>
#include <zmk/rgb_underglow.h>
#include <dt-bindings/zmk/hid_indicators.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static int underglow_indicator_listener(const zmk_event_t *eh) {
    zmk_hid_indicators_t indicators = zmk_hid_indicators_get_current_profile();

    if (indicators & HID_INDICATOR_CAPS_LOCK) {
        zmk_rgb_underglow_set_override((struct zmk_led_hsb){
            .h = CONFIG_ZMK_UNDERGLOW_CAPS_LOCK_INDICATOR_HUE,
            .s = CONFIG_ZMK_UNDERGLOW_CAPS_LOCK_INDICATOR_SAT,
            .b = CONFIG_ZMK_UNDERGLOW_CAPS_LOCK_INDICATOR_BRT,
        });
    } else {
        zmk_rgb_underglow_clear_override();
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(underglow_indicator, underglow_indicator_listener);
ZMK_SUBSCRIPTION(underglow_indicator, zmk_hid_indicators_changed);
