# Change Log

This document lays out what we've changed from the original Blitz3D.

## Runtime

### Graphics

- Apps are now DPI "aware." This means you will need to scale your windows & UIs based on user DPI. See [DPIScaleX()](https://github.com/kfprimm/blitz3d-ng/blob/master/_release/help/commands/2d_examples/DPIScaleX.bb)/[DPIScaleY()](https://github.com/kfprimm/blitz3d-ng/blob/master/_release/help/commands/2d_examples/DPIScaleX.bb).
- FreeImage has been upgraded to version 3.17. That means Blitz can load [a lot more file formats](http://freeimage.sourceforge.net/features.html).
- Added a check to filter out bad D3D7 graphics modes. Apparently anything over 2048x1536 won't work with 3D, despite what the driver says.

### Input

- DirectInput has been bumped to DirectInput8.
- Added CountJoys(),FindJoy(id$),JoyID$(),JoyName$(port).

### Networking

- DirectPlay related commands have been disabled.

### System

- Add ScreenWidth & ScreenHeight for getting the desktop width/height.
- CallDLL may not work on the 64-bit build.

## IDE

- It is now DPI "aware."
- `$BLITZPATH/tmp` is automatically created if it does not exist.
- Improved indentation handling.
- Added BB icon to executable & window.
- Fixed pasting junk data.
- Added 64-bit build.

## Debugger

- It is now DPI "aware" and scales based on user DPI setting.
- Added BB rocket icon to executable & window.

## Compiler

- Removed rocket icon from generated executables.
