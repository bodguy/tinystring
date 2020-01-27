#ifndef UTF_TYPES_H
#define UTF_TYPES_H

#include <cstdlib>
#include <cassert>

#if defined(_MSC_VER)
#define FORCE_INLINE	__forceinline
#else	// defined(_MSC_VER)
#define	FORCE_INLINE inline __attribute__((always_inline))
#endif

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

typedef char i8;
typedef short i16;
typedef int i32;
typedef long i64;

typedef size_t usize;
typedef int isize;
typedef float f32;
typedef double f64;

constexpr u16 SUR_LEAD_START = 0xD800;
constexpr u16 SUR_LEAD_END = 0xDBFF;
constexpr u16 SUR_TRAIL_START = 0xDC00;
constexpr u16 SUR_TRAIL_END = 0xDFFF;
constexpr u32 SUR_OFFSET = 0x10000 - (SUR_LEAD_START << 10) - SUR_TRAIL_START;

static const u8 g_trailingBytesForUTF8[256] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
};

static const u32 g_offsetsFromUTF8[6] = {0x00000000UL, 0x00003080UL, 0x000E2080UL, 0x03C82080UL, 0xFA082080UL, 0x82082080UL};

FORCE_INLINE bool is_legal_utf8_internal(const i8* str, u8 a) {
  switch ((u8)*str) {
    case 0xE0:
      if (a < 0xA0) {
        return false;
      }
      break;
    case 0xED:
      if (a < 0x9F) {
        return false;
      }
      break;
    case 0xF0:
      if (a < 0x90) {
        return false;
      }
      break;
    case 0xF4:
      if (a < 0x8F) {
        return false;
      }
      break;
    default:
      if (a < 0x80) {
        return false;
      }
  }

  return true;
}

FORCE_INLINE bool is_legal_utf8(const i8* str, usize len) {
  u8 a;
  const i8* src_ptr = str + len;

  switch (len) {
    default:
      return false;
    case 4:
      a = *--src_ptr;
      if (a < 0x80 || a > 0xBF) {
        return false;
      }
    case 3:
      a = *--src_ptr;
      if (a < 0x80 || a > 0xBF) {
        return false;
      }
    case 2:
      a = *--src_ptr;
      if (a > 0xBF || !is_legal_utf8_internal(str, a)) {
        return false;
      }
    case 1:
      if ((u8)*str >= 0x80 && (u8)*str < 0xC2) {
        return false;
      }
  }

  return (u8)*str <= 0xF4;
}

template<typename T>
FORCE_INLINE usize length(const T* str) {
  assert(str != NULL);
  const T* tmp = str;

  while (*tmp != '\0') {
    ++tmp;
  }

  return tmp - str;
}

FORCE_INLINE u32 next_char_utf8(const i8*& str) {
  assert(str != NULL);

  const i8* src = str;
  u16 extra_bytes = g_trailingBytesForUTF8[(u8)*src];

  for (u16 i = 1; i <= extra_bytes; ++i) {
    if (*(src + i) == '\0') {
      return 0;
    }
  }

  if (!is_legal_utf8(src, extra_bytes + 1)) {
    return 0;
  }

  u32 ch = 0;
  switch (extra_bytes) {
    case 5:
      ch += (u8)*src++;
      ch <<= 6;
    case 4:
      ch += (u8)*src++;
      ch <<= 6;
    case 3:
      ch += (u8)*src++;
      ch <<= 6;
    case 2:
      ch += (u8)*src++;
      ch <<= 6;
    case 1:
      ch += (u8)*src++;
      ch <<= 6;
    case 0:
      ch += (u8)*src++;
  }

  ch -= g_offsetsFromUTF8[extra_bytes];

  if (ch != '\0') {
    str = src;
  }

  return ch;
}

template <typename T1, typename T2>
FORCE_INLINE i32 compare(const T1* str1, const T2* str2, usize len1 = -1, usize len2 = -1) {
  assert(str1 != NULL);
  assert(str2 != NULL);

  return 0;
}

// utf8 -> utf16
//u16* from_utf8(const u8* v, usize len) {
//  std::vector<u16> result;
//  usize i = 0;
//
//  while (i < len) {
//    u16 bytes;
//    usize code_unit;
//    u8 ch = v[i++];
//
//    if (ch <= 0x7F) {
//      bytes = ch;
//      code_unit = 0;
//    } else if (ch <= 0xBF) {
//      throw std::logic_error("not a UTF-8 string");
//    } else if (ch <= 0xDF) {
//      bytes = ch & 0x1F;
//      code_unit = 1;
//    } else if (ch <= 0xEF) {
//      bytes = ch & 0x0F;
//      code_unit = 2;
//    } else if (ch <= 0xF7) {
//      bytes = ch & 0x07;
//      code_unit = 3;
//    } else {
//      throw std::logic_error("not a UTF-8 string");
//    }
//
//    for (size_t j = 0; j < code_unit; ++j) {
//      if (i == len)
//        throw std::logic_error("not a UTF-8 string");
//      u8 ch = v[i++];
//      if (ch < 0x80 || ch > 0xBF)
//        throw std::logic_error("not a UTF-8 string");
//      bytes <<= 6;
//      bytes += ch & 0x3F;
//    }
//
//    if (bytes >= 0xD800 && bytes <= 0xDFFF)
//      throw std::logic_error("not a UTF-8 string");
//    if (bytes > 0x10FFFF)
//      throw std::logic_error("not a UTF-8 string");
//    result.push_back(bytes);
//  }
//
//  u16* output = (u16*)malloc(sizeof(u16) * result.size());
//  for (u16 bytes : result) {
//    if (bytes <= 0xFFFF) {
//      utf16 += bytes;
//    } else {
//      // surrogate pair
//      bytes -= 0x10000;
//      utf16 += ((bytes >> 10) + 0xD800);
//      utf16 += ((bytes & 0x3FF) + 0xDC00);
//    }
//  }
//
//  return utf16;
//}

#endif //UTF_TYPES_H
