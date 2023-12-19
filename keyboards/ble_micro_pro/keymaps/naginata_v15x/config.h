/* Copyright 2019 sekigon-gonnoc
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

// place overrides here


// BMP用薙刀式
#define NG_BMP

// #define NG_USE_KAWASEMI // Mac汎用 → かわせみ専用


#ifdef NG_USE_KAWASEMI
// Keycodes used for starting Unicode input on different platforms
#   define UNICODE_KEY_MAC KC_NO   // かわせみ3用
#endif
