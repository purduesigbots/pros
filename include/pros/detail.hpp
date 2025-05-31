/**
 * \file pros/detail.hpp
 *
 * Contains PROS internal functions used in port literal functions.
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * Copyright (c) 2017-2024 Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License v. 2.0. If a copy of the MPL was not distributed with this
 * file You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */
#ifndef _PROS_DETAIL_HPP_
#define _PROS_DETAIL_HPP_

#include <array>
#include <charconv>
#include <cstdint>

namespace pros::detail {
template <char ...Chars>
consteval uint8_t parse_port() {
    const std::array chars{Chars...};
    const char* str = chars.begin();
    int base = 10;
    uint8_t port_num{0};
    if (chars.size() >= 2 && chars[0] == '0') {
        str += 2;
        // from_chars doesn't seem to handle hex/binary/octal prefixes,
        // so we handle them ourselves
        switch (chars[1]) {
            case 'X': case 'x':
                // hex literal
                base = 16;
                break;
            case 'B': case 'b':
                // binary literal
                base = 2;
                break;
            default:
                // octal literal
                str -= 1;
                base = 8;
        }
    }
    auto result = std::from_chars(str, chars.end(), port_num, base);
    if (result.ptr != chars.end() || result.ec != std::errc{}) {
        return 0;
    }
    return port_num;
}

template <char ...Chars>
consteval bool is_valid_port() {
    auto port_num = parse_port<Chars...>();
    return port_num >= 1 && port_num <= 21;
}

// Tests:
static_assert(is_valid_port<'1'>() == true, "Kernel test failed");
static_assert(is_valid_port<'0'>() == false, "Kernel test failed");
// 0x16 = 22
static_assert(is_valid_port<'0','x','1','6'>() == false, "Kernel test failed");
// 0x15 = 21
static_assert(is_valid_port<'0','x','1','5'>() == true, "Kernel test failed");
// 025 = 21
static_assert(is_valid_port<'0','2','5'>() == true, "Kernel test failed");
// 0b1 = 1
static_assert(is_valid_port<'0','b','1'>() == true, "Kernel test failed");
// 0x1 = 1
static_assert(is_valid_port<'0','x','1'>() == true, "Kernel test failed");
static_assert(is_valid_port<'2','2'>() == false, "Kernel test failed");

}

#endif // _PROS_DETAIL_HPP_
