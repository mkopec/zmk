# Copyright (c) 2025 The ZMK Contributors
# SPDX-License-Identifier: MIT

target_sources_ifdef(CONFIG_ZMK_INDICATOR_LEDS app PRIVATE indicator_leds.c)
target_sources_ifdef(CONFIG_ZMK_UNDERGLOW_CAPS_LOCK_INDICATOR app PRIVATE underglow_indicator.c)
