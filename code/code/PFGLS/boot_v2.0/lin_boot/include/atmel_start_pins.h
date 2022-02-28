/*
 * Code generated from START.
 *
 * This file will be overwritten when reconfiguring your START project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <port.h>

/**
 * \brief Set PC0 pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void PC0_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTC_set_pin_pull_mode(0, pull_mode);
}

/**
 * \brief Set PC0 data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void PC0_set_dir(const enum port_dir dir)
{
	PORTC_set_pin_dir(0, dir);
}

/**
 * \brief Set PC0 input/sense configuration
 *
 * Enable/disable PC0 digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Iterrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void PC0_set_isc(const PORT_ISC_t isc)
{
	PORTC_pin_set_isc(0, isc);
}

/**
 * \brief Set PC0 inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on PC0 is inverted
 *                     false = I/O on PC0 is not inverted
 */
static inline void PC0_set_inverted(const bool inverted)
{
	PORTC_pin_set_inverted(0, inverted);
}

/**
 * \brief Set PC0 level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void PC0_set_level(const bool level)
{
	PORTC_set_pin_level(0, level);
}

/**
 * \brief Toggle output level on PC0
 *
 * Toggle the pin level
 */
static inline void PC0_toggle_level()
{
	PORTC_toggle_pin_level(0);
}

/**
 * \brief Get level on PC0
 *
 * Reads the level on a pin
 */
static inline bool PC0_get_level()
{
	return PORTC_get_pin_level(0);
}

/**
 * \brief Set PC1 pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void PC1_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTC_set_pin_pull_mode(1, pull_mode);
}

/**
 * \brief Set PC1 data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void PC1_set_dir(const enum port_dir dir)
{
	PORTC_set_pin_dir(1, dir);
}

/**
 * \brief Set PC1 input/sense configuration
 *
 * Enable/disable PC1 digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Iterrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void PC1_set_isc(const PORT_ISC_t isc)
{
	PORTC_pin_set_isc(1, isc);
}

/**
 * \brief Set PC1 inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on PC1 is inverted
 *                     false = I/O on PC1 is not inverted
 */
static inline void PC1_set_inverted(const bool inverted)
{
	PORTC_pin_set_inverted(1, inverted);
}

/**
 * \brief Set PC1 level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void PC1_set_level(const bool level)
{
	PORTC_set_pin_level(1, level);
}

/**
 * \brief Toggle output level on PC1
 *
 * Toggle the pin level
 */
static inline void PC1_toggle_level()
{
	PORTC_toggle_pin_level(1);
}

/**
 * \brief Get level on PC1
 *
 * Reads the level on a pin
 */
static inline bool PC1_get_level()
{
	return PORTC_get_pin_level(1);
}

/**
 * \brief Set PF2 pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void PF2_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTF_set_pin_pull_mode(2, pull_mode);
}

/**
 * \brief Set PF2 data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void PF2_set_dir(const enum port_dir dir)
{
	PORTF_set_pin_dir(2, dir);
}

/**
 * \brief Set PF2 input/sense configuration
 *
 * Enable/disable PF2 digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Iterrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void PF2_set_isc(const PORT_ISC_t isc)
{
	PORTF_pin_set_isc(2, isc);
}

/**
 * \brief Set PF2 inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on PF2 is inverted
 *                     false = I/O on PF2 is not inverted
 */
static inline void PF2_set_inverted(const bool inverted)
{
	PORTF_pin_set_inverted(2, inverted);
}

/**
 * \brief Set PF2 level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void PF2_set_level(const bool level)
{
	PORTF_set_pin_level(2, level);
}

/**
 * \brief Toggle output level on PF2
 *
 * Toggle the pin level
 */
static inline void PF2_toggle_level()
{
	PORTF_toggle_pin_level(2);
}

/**
 * \brief Get level on PF2
 *
 * Reads the level on a pin
 */
static inline bool PF2_get_level()
{
	return PORTF_get_pin_level(2);
}

/**
 * \brief Set PF3 pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void PF3_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTF_set_pin_pull_mode(3, pull_mode);
}

/**
 * \brief Set PF3 data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void PF3_set_dir(const enum port_dir dir)
{
	PORTF_set_pin_dir(3, dir);
}

/**
 * \brief Set PF3 input/sense configuration
 *
 * Enable/disable PF3 digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Iterrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void PF3_set_isc(const PORT_ISC_t isc)
{
	PORTF_pin_set_isc(3, isc);
}

/**
 * \brief Set PF3 inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on PF3 is inverted
 *                     false = I/O on PF3 is not inverted
 */
static inline void PF3_set_inverted(const bool inverted)
{
	PORTF_pin_set_inverted(3, inverted);
}

/**
 * \brief Set PF3 level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void PF3_set_level(const bool level)
{
	PORTF_set_pin_level(3, level);
}

/**
 * \brief Toggle output level on PF3
 *
 * Toggle the pin level
 */
static inline void PF3_toggle_level()
{
	PORTF_toggle_pin_level(3);
}

/**
 * \brief Get level on PF3
 *
 * Reads the level on a pin
 */
static inline bool PF3_get_level()
{
	return PORTF_get_pin_level(3);
}

/**
 * \brief Set PF4 pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void PF4_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTF_set_pin_pull_mode(4, pull_mode);
}

/**
 * \brief Set PF4 data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void PF4_set_dir(const enum port_dir dir)
{
	PORTF_set_pin_dir(4, dir);
}

/**
 * \brief Set PF4 input/sense configuration
 *
 * Enable/disable PF4 digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Iterrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void PF4_set_isc(const PORT_ISC_t isc)
{
	PORTF_pin_set_isc(4, isc);
}

/**
 * \brief Set PF4 inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on PF4 is inverted
 *                     false = I/O on PF4 is not inverted
 */
static inline void PF4_set_inverted(const bool inverted)
{
	PORTF_pin_set_inverted(4, inverted);
}

/**
 * \brief Set PF4 level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void PF4_set_level(const bool level)
{
	PORTF_set_pin_level(4, level);
}

/**
 * \brief Toggle output level on PF4
 *
 * Toggle the pin level
 */
static inline void PF4_toggle_level()
{
	PORTF_toggle_pin_level(4);
}

/**
 * \brief Get level on PF4
 *
 * Reads the level on a pin
 */
static inline bool PF4_get_level()
{
	return PORTF_get_pin_level(4);
}

/**
 * \brief Set LIN_EN pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void LIN_EN_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTF_set_pin_pull_mode(5, pull_mode);
}

/**
 * \brief Set LIN_EN data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void LIN_EN_set_dir(const enum port_dir dir)
{
	PORTF_set_pin_dir(5, dir);
}

/**
 * \brief Set LIN_EN input/sense configuration
 *
 * Enable/disable LIN_EN digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Iterrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void LIN_EN_set_isc(const PORT_ISC_t isc)
{
	PORTF_pin_set_isc(5, isc);
}

/**
 * \brief Set LIN_EN inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on LIN_EN is inverted
 *                     false = I/O on LIN_EN is not inverted
 */
static inline void LIN_EN_set_inverted(const bool inverted)
{
	PORTF_pin_set_inverted(5, inverted);
}

/**
 * \brief Set LIN_EN level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void LIN_EN_set_level(const bool level)
{
	PORTF_set_pin_level(5, level);
}

/**
 * \brief Toggle output level on LIN_EN
 *
 * Toggle the pin level
 */
static inline void LIN_EN_toggle_level()
{
	PORTF_toggle_pin_level(5);
}

/**
 * \brief Get level on LIN_EN
 *
 * Reads the level on a pin
 */
static inline bool LIN_EN_get_level()
{
	return PORTF_get_pin_level(5);
}

#endif /* ATMEL_START_PINS_H_INCLUDED */
