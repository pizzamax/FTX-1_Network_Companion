/*
 * SPDX-License-Identifier: PolyForm-Noncommercial-1.0.0
 */

/*
 * Interface to the CAT control code (cat.c): a snapshot of the radio's state (frequency, mode, S meter,
 * AF gain, squelch, notch, ...) and the functions that change it.
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Radio state read over CAT (the FTX-1's CP210x USB serial port)
 */
typedef struct {
    bool connected;         /*!< A CAT port is open and answering */
    bool busy;              /*!< Squelch is open on the receiver reported by RI (BUSY) */
    bool tx;                /*!< The radio is transmitting (RI P4 = 1) */
    uint8_t s_main;         /*!< S meter, MAIN side, 0-255 */
    uint8_t s_sub;          /*!< S meter, SUB side, 0-255 */
    uint32_t freq_main;     /*!< Frequency in Hz (MAIN/VFO-A) */
    uint32_t freq_sub;      /*!< Frequency in Hz (SUB/VFO-B) */
    char mode_main[16];     /*!< Mode name, e.g. "USB", "LSB", "FM", "AM" (MAIN) */
    char mode_sub[16];      /*!< Mode name, e.g. "USB", "LSB", "FM", "AM" (SUB) */
    char mode_main_raw;     /*!< Raw mode character ('1'-'9', 'A'-'F') */
    char mode_sub_raw;      /*!< Raw mode character ('1'-'9', 'A'-'F') */
    uint8_t vol_main;       /*!< AF Gain / Volume, MAIN side, 0-255 */
    uint8_t vol_sub;        /*!< AF Gain / Volume, SUB side, 0-255 */
    uint8_t sq_main;        /*!< Hardware Squelch level, MAIN side, 0-255 */
    uint8_t sq_sub;         /*!< Hardware Squelch level, SUB side, 0-255 */
    bool notch_main;        /*!< Manual notch ON, MAIN side */
    bool notch_sub;         /*!< Manual notch ON, SUB side */
    uint16_t notch_hz_main; /*!< Manual notch frequency in Hz (10-3200), MAIN side; 0 = not read yet */
    uint16_t notch_hz_sub;  /*!< Manual notch frequency in Hz (10-3200), SUB side; 0 = not read yet */
    uint32_t age_ms;        /*!< Time since the last answer */
    uint8_t devices;        /*!< USB devices seen on the last scan (hubs included) */
    uint8_t cp210x;         /*!< Of those, CP210x (VID 0x10C4) devices opened */
    uint8_t answered;       /*!< Of those, devices that answered ID; */
    char last_error[80];    /*!< Last scan problem, "" if none */
} cat_state_t;

/**
 * @brief Start the CAT client. Requires usb_audio_init() (it installs the USB Host Library) first.
 *
 * When the CP210x serial port is attached, it is opened 8N1 and the radio is asked "ID;". The CAT-1 RATE is
 * found by trying the rate that worked last time, 115200, 38400 (the radio's factory setting), 19200, 9600 and 4800;
 * the radio is then read step by step and polled.
 */
esp_err_t cat_init(void);

/**
 * @brief Copy the latest radio state
 */
void cat_get_state(cat_state_t *state);

/*
 * There is deliberately no function that sends a free-form CAT string. Every command is built here from
 * typed arguments, and cat.c only lets an allow-list of commands (frequency, mode, AF gain, squelch,
 * notch, VFO swap/copy, power) reach the radio. Nothing in this firmware can make the radio transmit.
 */

/** @brief Swap the MAIN and SUB VFOs (CAT "SV") */
esp_err_t cat_swap_vfo(void);

/** @brief Copy MAIN to SUB (CAT "AB") */
esp_err_t cat_copy_main_to_sub(void);

/** @brief Copy SUB to MAIN (CAT "BA") */
esp_err_t cat_copy_sub_to_main(void);

/**
 * @brief Set frequency in Hz
 * @param sub true for SUB (VFO-B), false for MAIN (VFO-A)
 * @param freq_hz Frequency in Hz, 30000 - 470000000 (e.g. 14074000); ESP_ERR_INVALID_ARG outside it
 */
esp_err_t cat_set_freq(bool sub, uint32_t freq_hz);

/**
 * @brief Set operating mode
 * @param sub true for SUB (VFO-B), false for MAIN (VFO-A)
 * @param mode_char CAT mode character ('1'-'9', 'A'-'F')
 */
esp_err_t cat_set_mode_char(bool sub, char mode_char);

/**
 * @brief Set operating mode by name (e.g. "USB", "LSB", "CW", "FM", "AM", "DATA-USB", etc.)
 * @param sub true for SUB (VFO-B), false for MAIN (VFO-A)
 * @param mode_name Mode name
 */
esp_err_t cat_set_mode(bool sub, const char *mode_name);

/**
 * @brief Set AF Gain / Volume (0-255)
 * @param sub true for SUB (VFO-B), false for MAIN (VFO-A)
 * @param volume Volume level (0-255)
 */
esp_err_t cat_set_volume(bool sub, uint8_t volume);

/**
 * @brief Set hardware Squelch level (0-255)
 * @param sub true for SUB (VFO-B), false for MAIN (VFO-A)
 * @param squelch Squelch level (0-255)
 */
esp_err_t cat_set_squelch(bool sub, uint8_t squelch);

/**
 * @brief Manual notch ON/OFF (CAT "BP", P2=0)
 * @param sub true for SUB, false for MAIN
 */
esp_err_t cat_set_notch(bool sub, bool on);

/**
 * @brief Manual notch frequency (CAT "BP", P2=1)
 * @param sub true for SUB, false for MAIN
 * @param hz Frequency in Hz, 10-3200 (the radio takes steps of 10 Hz)
 */
esp_err_t cat_set_notch_freq(bool sub, uint16_t hz);

/**
 * @brief Set Radio Power State (CAT "PS")
 * @param on true for ON (PS1;), false for OFF (PS0;)
 * @note Turning power OFF disconnects USB on most rigs, so waking up over USB CAT is generally not supported.
 */
esp_err_t cat_set_power(bool on);

#ifdef __cplusplus
}
#endif
