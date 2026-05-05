/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <Common/Types.h>

namespace Platform {

enum class Key : u8 {
    BackSpace = 0x08,
    Tab,
    Enter = 0x0D,

    Shift = 0x10,
    Control,
    Alt,

    CapsLock = 0x14,
    Escape = 0x1B,
    Space = 0x20,

    PageUp,
    PageDown,
    End,
    Home,
    LeftArrow,
    UpArrow,
    RightArrow,
    DownArrow,

    PrntScr = 0x2C,
    Insert,
    Delete,

    Num0 = 0x30,
    Num1,
    Num2,
    Num3,
    Num4,
    Num5,
    Num6,
    Num7,
    Num8,
    Num9,

    A = 0x41,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,

    NumPad0 = 0x60,
    NumPad1,
    NumPad2,
    NumPad3,
    NumPad4,
    NumPad5,
    NumPad6,
    NumPad7,
    NumPad8,
    NumPad9,

    F1 = 0x70,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,

    NumLock = 0x90,
    ScrollLock,

    Total
};

enum class MouseButton : u8 {
    Left,
    Right,
    Middle,
    Total
};

}
