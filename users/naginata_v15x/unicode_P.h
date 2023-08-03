/* Copyright 2022 QMK / Satoru NAKAYA <@tor-nky>
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

#if defined(__AVR__) || defined(__DOXYGEN__)
/**
 * \brief Type out a PROGMEM string of Unicode characters, with a delay between each character.
 *
 * On ARM devices, this function is simply an alias for send_unicode_string_P(string).
 *
 * \param string The string to type out.
 */
const char *decode_utf8_P(const char *str, int32_t *code_point);
void send_unicode_string_P(const char *string);
#else
#    define send_unicode_string_P(string) send_unicode_string(string)
#endif

/**
 * \brief Shortcut macro for send_unicode_string_P(PSTR(string)).
 *
 * On ARM devices, this define evaluates to send_unicode_string(string).
 */
#define SEND_UNICODE_STRING(string) send_unicode_string_P(PSTR(string))
