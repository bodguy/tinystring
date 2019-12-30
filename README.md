### The UTF-8 Format

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



