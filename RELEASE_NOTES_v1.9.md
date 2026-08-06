# Release notes — v1.9

This release adds an optional automatic line-wrapping mode for long text lines. It is aimed at readers who use the overlay for books, scripts, logs, and other files where a single logical line can extend far beyond the screen.

## What changed

- Added `Settings...` to the main menu.
- Added `Automatic Line Wrapping`, enabled by default for new and existing users.
- Measures rendered text against the available screen width and breaks long lines into visual rows.
- Keeps bookmarks and resume positions associated with the original file line.
- Keeps the original horizontal-panning behavior when wrapping is turned off.
- Bounds the wrapping cache and clears it when the font size or available width changes.

## Installation

Extract `TextReaderOverlay-NX-1.9.zip` to the SD-card root. Remove any older `sd:/switch/.overlays/TextReaderOverlay-NX.ovl` before copying if the old overlay is still present, then reboot the Switch before testing.

Open Tesla Menu, select `Text Reader`, and use `Settings...` to change the wrapping preference. The setting is stored in `sd:/switch/.overlays/TextReaderOverlay/config.json`.

## Attribution

This is a maintained compatibility fork of [diwo/TextReaderOverlay-NX](https://github.com/diwo/TextReaderOverlay-NX). The original project, concept, and GPL licensing are retained and credited in the repository documentation.
