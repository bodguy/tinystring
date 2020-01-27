#ifndef UTF_UTF_UTILS_H
#define UTF_UTF_UTILS_H

#include "types.h"

inline bool is_valid_codepoint(u32 cp) {
  // Excludes the surrogate pair code points ([0xD800, 0xDFFF]) and
  // codepoints larger than 0x10FFFF (the highest codepoint allowed).
  return cp < 0xD800u || (cp >= 0xE000u && cp <= 0x10FFFFu);
}

inline bool is_valid_char(u32 cp) {
  // Excludes non-characters (U+FDD0..U+FDEF, and all codepoints ending in 0xFFFE or 0xFFFF)
  // from the set of valid code points.
  return cp < 0xD800u ||
    (cp >= 0xE000u && cp < 0xFDD0u) ||
    (cp > 0xFDEFu && cp <= 0x10FFFFu &&
    (cp & 0xFFFEu) != 0xFFFEu);
}

#endif //UTF_UTF_UTILS_H
