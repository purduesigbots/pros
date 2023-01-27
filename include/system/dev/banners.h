/**
 * \file system/dev/banners.h
 *
 * PROS banners printed over serial line
 *
 * This file should only be included ONCE (in system/dev/serial_daemon.c)
 *
 * See system/dev/serial_daemon.c for discussion
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once
// The banners look extra messed up due to the ANSI escape sequences and needing
// to escape backslashes and quotes
static char const* const short_banner =
    "\n\n\n"
    " [1;33m _____  _____   ____   _____ [0m\n"
    " [1;33m|  __ \\|  __ \\ / __ \\ / ____|[0m          [1;33mPowered by "
    "PROS for VEX V5[0m\n"
    " [1;33m| |__) | |__) | |  | | (___  [0m    Version:  [1m%29s[0m\n"
    " [1;33m|  ___/|  _  /| |  | |\\___ \\ [0m    Uptime:   [1m%23lu.%03lu "
    "s[0m\n"
    " [1;33m| |    | | \\ \\| |__| |____) |[0m    Compiled: [1m%29s[0m\n"
    " [1;33m|_|    |_|  \\_\\\\____/|_____/ [0m    Directory:[1m%29s[0m\n"
    "\n\n";

static char const* const large_banner =
    "\n\n\n"
    "         [1;33m_+=+_[0m\n"
    "     [1;33m .-`  .  `-. [0m         8888888b.  8888888b.   .d88888b.   "
    ".d8888b.\n"
    "  [1;33m _+`     \"     `+_ [0m      888   Y88b 888   Y88b d88P\" "
    "\"Y88b d88P  Y88b\n"
    "  [1;33m\\\\\\sssssssssssss///[0m      888    888 888    888 888     "
    "888 Y88b.\n"
    "     [1;33m.ss\\  [37m*[33m  /ss.[0m         888   d88P 888   d88P "
    "888     888  \"Y888b.\n"
    " [1;33m.+bm[0m  [1;33m.s[37m  *  [33ms.[0m  [1;33mmd+.[0m     "
    "8888888P\"  8888888P\"  888     888     "
    "\"Y88b.\n"
    "[1;33m.hMMMMs[0m [1;33m.[37m  *  [33m.[0m [1;33msMMMMh.[0m    888 "
    "       888 T88b   888     888       "
    "\"888\n"
    " [1;33m`\\hMMMb[0m [1;33m\\[37m | [33m/[0m [1;33mdMMMh:[0m      "
    "888        888  T88b  Y88b. .d88P Y88b  "
    "d88P\n"
    "   [1;33m-SNMNo[0m  [1;33m-[0m  [1;33moNMNs-[0m       888        "
    "888   T88b  \"Y88888P\"   \"Y8888P\"\n"
    "     [1;33m`+dMh\\./dMd/[0m\n"
    "        [1;33m`:yNy:`[0m                      [1;33mPowered by PROS "
    "for VEX V5[0m\n"
    "           [1;33m\"[0m                Copyright (c) Purdue University "
    "ACM SIGBots\n"
    "Version:[1m%13s[0m     Platform:  [1mV%d.%d.%d (b%d)[0m     "
    "Uptime:[1m%5lu.%03lu s[0m\n"
    "Compiled:   [1m%20s[0m     Directory:  [1m%22s[0m\n"
    "\n\n";
