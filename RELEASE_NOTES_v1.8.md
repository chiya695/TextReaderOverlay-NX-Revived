# Release notes 鈥?v1.8

v1.8 is the first release intended as a clean daily-use build after the real-device debugging cycle.

## Highlights

- Text files display correctly with current libtesla root layout rules.
- UTF-8 Chinese/Japanese/Korean text renders through Switch shared fonts.
- Short files no longer show unrelated strings after EOF.
- Browser, Favorites, Help, bookmarks, scrolling, font-size adjustment, and horizontal panning remain available.
- The overlay remains under 1 MiB as a built OVL.

## Installation

Extract `TextReaderOverlay-NX-1.8.zip` to the SD-card root. If an older overlay is loaded, delete the old `switch/.overlays/TextReaderOverlay-NX.ovl`, copy v1.8, and reboot the Switch before testing.

## Known limitations

- Text files must use one of the documented encodings.
- Extremely long individual lines may require horizontal scrolling.
- The overlay is designed for `.txt` files and does not parse EPUB, HTML, Markdown, or rich-text formatting.
