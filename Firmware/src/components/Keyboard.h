#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <cstdint>

#include "config/user_config.h"

namespace Keyboard{
extern bool capsLockState;
extern bool numLockState;
extern bool scrollLockState;

#ifdef ENABLE_DEBUG_FUNCTIONS
    /**
     * @brief Check if any keyboard key is pressed
     * @note Debug function
     */
    bool IsKeyboardKeyPressed();
#endif
    /**
     * @brief Get indexes of pressed keys
     * @return Pointer to array of key indexes, or nullptr if no keys are pressed
     */
    uint8_t* GetKeyPressIndexes();
}

#endif