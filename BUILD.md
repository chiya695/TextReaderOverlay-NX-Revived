# Building

This fork uses the current upstream `libtesla` headers and the modern libnx HID callback types.

## Requirements

Install the latest devkitPro Switch development group, then open the devkitPro MSYS2 shell and set:

```sh
export DEVKITPRO=/opt/devkitpro
export DEVKITA64=$DEVKITPRO/devkitA64
export PORTLIBS=$DEVKITPRO/portlibs
export PATH=$DEVKITA64/bin:$PORTLIBS/switch/bin:$DEVKITPRO/tools/bin:$PATH
export PKG_CONFIG_PATH=$PORTLIBS/switch/lib/pkgconfig
make release
```

The generated `TextReaderOverlay-NX-1.9.1.zip` contains the SD card layout. Keep `Atmosphere`, `nx-ovlloader`, and Tesla Menu updated together with the overlay.
