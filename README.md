### Unicode ("Unique code point")

Unicode provides a unique number for every character, 
no matter what the platform, no matter what the program, no matter what the language.

- Each of these unique numbers is called a code point, 
and is typically represented using the “U+” prefix, 
followed by the unique number written in hexadecimal form.
- Currently, the Unicode standard defines more than 1,114,000 code points.
- for example, 学 (U+5B66), C (U+0043)

### Code point to actual bits: UTF-8, UTF-16 Encodings

A code point is an abstract concept.
How are these Unicode code points represented concretely using computer bits?

Answer is concept of Unicode encoding.

- Unicode encoding is a particular, well-defined way of representing Unicode code point values in bits. 
- In this example, we discuss only UTF-8, UTF-16.
- Both of which are variable-length encodings capable of encoding all possible Unicode “characters” or, better, code points.
- conversions between these two encodings are lossless.

code unit: minimal size of bit representing a single character.
- UTF-8, as its name suggests, uses 8-bit code units.
- UTF-16, uses 16-bits code units.

### The UTF-8 Format

couple of design goals.

- backward-­compatible with ASCII.
- endian-neutral because of just a sequence of 8-bit code units.
- for example,

```
C (U+0043) -> 0x43 (exactly same as the ASCII code)
学 (U+5B66) -> 0xE5 0xAD 0xA6 (three byte sequence)
☃ (U+2603) -> 0xE2 0x98 0x83 (three byte sequence)
🍺 (U+1F37A) -> 0xF0 0x9F 0x8D 0xBA (located outside the BMP)
```

- In UTF-8, the high-order bits in binary are important
- The leading high-order bits of the first byte tell how many bytes were used to encode the value
- In 1-byte encodings which range from 0 to 127, the high-order bit will always be zero
- All single byte encodings have the following form:

```
+---+---+---+---+---+---+---+---+  
| 0 |   |   |   |   |   |   |   |  
+---+---+---+---+---+---+---+---+  
```

7 bits are available to code the number.

### 2-Byte Encodings

In 2-byte encodings, the first three high-order bits of the first byte are set to "110" and then
first two high-order bits of the second byte are set to "10"

```
+---+---+---+---+---+---+---+---+  
| 1 | 1 | 0 |   |   |   |   |   |  
+---+---+---+---+---+---+---+---+  
+---+---+---+---+---+---+---+---+  
| 1 | 0 |   |   |   |   |   |   |  
+---+---+---+---+---+---+---+---+  
```

This gives 11 bits (5 + 6) to encode the actual character.

### 3-Byte Encodings

In 3-byte encodings, the first four high-order bits fo the first byte are set to "1110" and then
the first two high-order bits of the remaining bytes are set to "10".

```
+---+---+---+---+---+---+---+---+  
| 1 | 1 | 1 | 0 |   |   |   |   |  
+---+---+---+---+---+---+---+---+  
+---+---+---+---+---+---+---+---+  
| 1 | 0 |   |   |   |   |   |   |  
+---+---+---+---+---+---+---+---+  
+---+---+---+---+---+---+---+---+  
| 1 | 0 |   |   |   |   |   |   |  
+---+---+---+---+---+---+---+---+  
```

This gives 16 bits (4 + 6 + 6) to encode the actual character.

### 4-Bytes Encodings

In 4-byte encodings, the first five high-order bits of the first byte are set to "11110" and then
the first two high-order bits of the remaining bytes are set to "10".

```
+---+---+---+---+---+---+---+---+
| 1 | 1 | 1 | 1 | 0 |   |   |   |
+---+---+---+---+---+---+---+---+
+---+---+---+---+---+---+---+---+
| 1 | 0 |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+
+---+---+---+---+---+---+---+---+
| 1 | 0 |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+
+---+---+---+---+---+---+---+---+
| 1 | 0 |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+
```

This gives 21 bits (3 + 6 + 6 + 6) to encode the actual character.

### UTF-16 Format

- UTF-16 is basically the de facto standard encoding used by many systems like
Windows, Java, Qt, Unreal engine, ICU etc..
- UTF-16 uses 16-bit code units
- Unicode code points are encoded in UTF-16 using just one or two 16-bit code units.
- there are both big-endian UTF-16 and little-endian UTF-16
- for example,

```
C (U+0043) -> 0x0043  (compatible with ASCII code)
学 (U+5B66) -> 0x5B66 (direct correspondence between their code point)
☃ (U+2603) -> 0x2603 (same as codepoint, 16-bit word)
🍺 (U+1F37A) -> 0xD83C 0xDF7A (located outside the BMP, two 16-bit code units)
```

### Endianess

In UTF-16 encodings, endianess is matter.  
there are two kind of encodings: UTF-16BE, UTF-16LE  

- Bits representation  

```
|15            8|7             0|  
+---+---+---+---+---+---+---+---+  
| y | y | y | y | x | x | x | x |  
+---+---+---+---+---+---+---+---+  
```

- UTF-16BE  

```
   first byte       second byte  
|7             0|7             0|  
+---+---+---+---+---+---+---+---+  
| y | y | y | y | x | x | x | x |  
+---+---+---+---+---+---+---+---+  
```

- UTF-16LE  

```
   first byte       second byte  
|7             0|7             0|  
+---+---+---+---+---+---+---+---+  
| x | x | x | x | y | y | y | y |  
+---+---+---+---+---+---+---+---+  
```

### Surrogate

outside of BMP area codepoint will convert to UTF-16 surrogate pair.

```
|31                           24|
+---+---+---+---+---+---+---+---+  
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |  
+---+---+---+---+---+---+---+---+  
|23                           16|
+---+---+---+---+---+---+---+---+  
| 0 | 0 | 0 | z | z | z | z | z |  
+---+---+---+---+---+---+---+---+  
|15                            8|
+---+---+---+---+---+---+---+---+  
| x | x | x | x | x | x | y | y |  
+---+---+---+---+---+---+---+---+
|7                             0|  
+---+---+---+---+---+---+---+---+  
| y | y | y | y | y | y | y | y |  
+---+---+---+---+---+---+---+---+  
```

- High surrogate: U+D800 ~ U+DBFF  

```
+---+---+---+---+---+---+---+---+  
| 1 | 1 | 0 | 1 | 1 | 0 | Z | Z |  
+---+---+---+---+---+---+---+---+  
+---+---+---+---+---+---+---+---+  
| Z | Z | x | x | x | x | x | x |  
+---+---+---+---+---+---+---+---+  
```
- Low surrogate: U+DC00 ~ U+DFFF  

```
+---+---+---+---+---+---+---+---+  
| 1 | 1 | 0 | 1 | 1 | 1 | y | y |  
+---+---+---+---+---+---+---+---+  
+---+---+---+---+---+---+---+---+  
| y | y | y | y | y | y | y | y |  
+---+---+---+---+---+---+---+---+  
```

- UTF-16 surrogate pair (yyyy = zzzzz - 1)

10 free bits for each surrogate = 10 + 10 = 20 bits  

### Unicode Planes

Unicode defines a concept of plane as a continuous group of 65,536 (216) code points.

- Unicode is divided into a total of 17 code areas, each with 65,536 characters (16 bits), currently only about 10 percent of these are used.

#### Basic Multilingual Plane (BMP)

- The first plane is identified as plane 0 or Basic Multilingual Plane (BMP).
- codepoint range from U+0000 to U+FFFF.
- all these BMP characters are represented in UTF-16 using a single 16-bit code unit.

#### Supplementary Multilingual Plane (SMP)

- The second plane, the additional multi-lingual area.
- Supplementary characters are located in planes other than the BMP.
- codepoint range from U+10000 to U+1FFFF.
- encoded in UTF-16 using two 16-bit code units, also known as surrogate pairs.

#### Supplementary Ideographic Plane (SIP)

- The third plane, exclusively Japanese, Chinese and Korean characters.
- codepoint range from U+20000 to U+2FFFF.

#### Plane 3 to 13

- Not occupied
- codepoint range from U+30000 to U+DFFFF

#### Supplementary Special-purpose Plane (SSP)

- the additional area for specific uses.
- codepoint range from U+E0000 to U+EFFFF

#### Private Use Area

- the last two area. reserved for private use and can be used individually.
- codepoint range from U+F0000 to U+10FFFF
- they can not be used uniformly.
