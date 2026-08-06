# Changelog

## 1.9 - 2026-08-06

- Added a Settings menu with a persistent `Automatic Line Wrapping` toggle.
- Wrapped long logical lines into measured visual rows without loading the whole file into memory.
- Kept horizontal panning available when wrapping is disabled.
- Made font-size changes invalidate the wrapping cache so row boundaries stay correct.


## 1.8 - 2026-08-05

- Fixed rendering beyond the detected end of a file.
- Ignored empty lines before passing strings to the libtesla renderer, avoiding unrelated heap data appearing after short files.
- Kept the root-level CustomDrawer boundary fix introduced during the real-device debugging cycle.
- Kept streamed decoding for UTF-8, UTF-16/32, GB18030/GBK, and Windows-1252.
- Kept Switch shared-font fallback for multilingual text.
- Moved diagnostics behind the `-` key so normal reading stays uncluttered.

## 1.7 - 2026-08-05

- Fixed the zero-sized root reader viewport under current libtesla.
- Added temporary on-screen rendering and decoding diagnostics.
- Added a multi-encoding test fixture set.

## 1.6 and earlier

See the commit history for the staged modernization work: current libnx/libtesla APIs, SD mounting, Browser fixes, memory reduction, font fallback, and encoding support.
