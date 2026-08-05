TextReaderOverlay-NX encoding test fixtures

Recommended order on Switch:
1. 00_ascii_utf8.txt - proves basic layout/rendering.
2. 01_utf8_no_bom.txt - UTF-8 multilingual text.
3. Test the remaining encodings.

Press Minus in v1.8 to show the detected encoding and line count.

Expected detections:
00/01: UTF-8
02: UTF-8 BOM
03: UTF-16 LE
04: UTF-16 BE
05: UTF-32 LE
06: UTF-32 BE
07: UTF-16 LE (detected)
08: UTF-16 BE (detected)
09: GB18030/GBK
10: Windows-1252
