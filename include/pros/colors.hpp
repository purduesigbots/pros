/**
 * \file pros/colors.hpp
 *
 * Contains enum class definitions of colors
 *
 * This file should not be modified by users, since it gets replaced whenever
 * a kernel upgrade occurs.
 *
 * \copyright (c) 2017-2024, Purdue University ACM SIGBots.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License v. 2.0. If a copy of the MPL was not distributed with this
 * file You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \defgroup cpp-colors C++ Color API
 */
#ifndef _PROS_COLORS_HPP_
#define _PROS_COLORS_HPP_


namespace pros{
/**
 * \ingroup cpp-colors
 */

/**
 * \addtogroup cpp-colors
 *  @{
 */

/**
 * \enum Color
 * @brief
 * Enum class of possible colors
 *
 * Contains common colors, all members are self descriptive. 
 */
enum class Color {
  alice_blue = 0x00F0F8FF,
  antique_white = 0x00FAEBD7,
  aqua = 0x0000FFFF,
  aquamarine = 0x007FFFD4,
  azure = 0x00F0FFFF,
  beige = 0x00F5F5DC,
  bisque = 0x00FFE4C4,
  black = 0x00000000,
  blanched_almond = 0x00FFEBCD,
  blue = 0x000000FF,
  blue_violet = 0x008A2BE2,
  brown = 0x00A52A2A,
  burly_wood = 0x00DEB887,
  cadet_blue = 0x005F9EA0,
  chartreuse = 0x007FFF00,
  chocolate = 0x00D2691E,
  coral = 0x00FF7F50,
  cornflower_blue = 0x006495ED,
  cornsilk = 0x00FFF8DC,
  crimson = 0x00DC143C,
  cyan = 0x0000FFFF,
  dark_blue = 0x0000008B,
  dark_cyan = 0x00008B8B,
  dark_goldenrod = 0x00B8860B,
  dark_gray = 0x00A9A9A9,
  dark_grey = dark_gray,
  dark_green = 0x00006400,
  dark_khaki = 0x00BDB76B,
  dark_magenta = 0x008B008B,
  dark_olive_green = 0x00556B2F,
  dark_orange = 0x00FF8C00,
  dark_orchid = 0x009932CC,
  dark_red = 0x008B0000,
  dark_salmon = 0x00E9967A,
  dark_sea_green = 0x008FBC8F,
  dark_slate_gray = 0x002F4F4F,
  dark_slate_grey = dark_slate_gray,
  dark_turquoise = 0x0000CED1,
  dark_violet = 0x009400D3,
  deep_pink = 0x00FF1493,
  deep_sky_blue = 0x0000BFFF,
  dim_gray = 0x00696969,
  dim_grey = dim_gray,
  dodger_blue = 0x001E90FF,
  fire_brick = 0x00B22222,
  floral_white = 0x00FFFAF0,
  forest_green = 0x00228B22,
  fuchsia = 0x00FF00FF,
  gainsboro = 0x00DCDCDC,
  ghost_white = 0x00F8F8FF,
  gold = 0x00FFD700,
  goldenrod = 0x00DAA520,
  gray = 0x00808080,
  grey = gray,
  green = 0x00008000,
  green_yellow = 0x00ADFF2F,
  honeydew = 0x00F0FFF0,
  hot_pink = 0x00FF69B4,
  indian_red = 0x00CD5C5C,
  indigo = 0x004B0082,
  ivory = 0x00FFFFF0,
  khaki = 0x00F0E68C,
  lavender = 0x00E6E6FA,
  lavender_blush = 0x00FFF0F5,
  lawn_green = 0x007CFC00,
  lemon_chiffon = 0x00FFFACD,
  light_blue = 0x00ADD8E6,
  light_coral = 0x00F08080,
  light_cyan = 0x00E0FFFF,
  light_goldenrod_yellow = 0x00FAFAD2,
  light_green = 0x0090EE90,
  light_gray = 0x00D3D3D3,
  light_grey = light_gray,
  light_pink = 0x00FFB6C1,
  light_salmon = 0x00FFA07A,
  light_sea_green = 0x0020B2AA,
  light_sky_blue = 0x0087CEFA,
  light_slate_gray = 0x00778899,
  light_slate_grey = light_slate_gray,
  light_steel_blue = 0x00B0C4DE,
  light_yellow = 0x00FFFFE0,
  lime = 0x0000FF00,
  lime_green = 0x0032CD32,
  linen = 0x00FAF0E6,
  magenta = 0x00FF00FF,
  maroon = 0x00800000,
  medium_aquamarine = 0x0066CDAA,
  medium_blue = 0x000000CD,
  medium_orchid = 0x00BA55D3,
  medium_purple = 0x009370DB,
  medium_sea_green = 0x003CB371,
  medium_slate_blue = 0x007B68EE,
  medium_spring_green = 0x0000FA9A,
  medium_turquoise = 0x0048D1CC,
  medium_violet_red = 0x00C71585,
  midnight_blue = 0x00191970,
  mint_cream = 0x00F5FFFA,
  misty_rose = 0x00FFE4E1,
  moccasin = 0x00FFE4B5,
  navajo_white = 0x00FFDEAD,
  navy = 0x00000080,
  old_lace = 0x00FDF5E6,
  olive = 0x00808000,
  olive_drab = 0x006B8E23,
  orange = 0x00FFA500,
  orange_red = 0x00FF4500,
  orchid = 0x00DA70D6,
  pale_goldenrod = 0x00EEE8AA,
  pale_green = 0x0098FB98,
  pale_turquoise = 0x00AFEEEE,
  pale_violet_red = 0x00DB7093,
  papay_whip = 0x00FFEFD5,
  peach_puff = 0x00FFDAB9,
  peru = 0x00CD853F,
  pink = 0x00FFC0CB,
  plum = 0x00DDA0DD,
  powder_blue = 0x00B0E0E6,
  purple = 0x00800080,
  red = 0x00FF0000,
  rosy_brown = 0x00BC8F8F,
  royal_blue = 0x004169E1,
  saddle_brown = 0x008B4513,
  salmon = 0x00FA8072,
  sandy_brown = 0x00F4A460,
  sea_green = 0x002E8B57,
  seashell = 0x00FFF5EE,
  sienna = 0x00A0522D,
  silver = 0x00C0C0C0,
  sky_blue = 0x0087CEEB,
  slate_blue = 0x006A5ACD,
  slate_gray = 0x00708090,
  slate_grey = slate_gray,
  snow = 0x00FFFAFA,
  spring_green = 0x0000FF7F,
  steel_blue = 0x004682B4,
  tan = 0x00D2B48C,
  teal = 0x00008080,
  thistle = 0x00D8BFD8,
  tomato = 0x00FF6347,
  turquoise = 0x0040E0D0,
  violet = 0x00EE82EE,
  wheat = 0x00F5DEB3,
  white = 0x00FFFFFF,
  white_smoke = 0x00F5F5F5,
  yellow = 0x00FFFF00,
  yellow_green = 0x009ACD32,
};
} // namespace pros

  ///@}

#endif //_PROS_COLORS_HPP_
