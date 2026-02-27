# Disclaimer

Voicemeeter is Audio Mixer Application endowed with Virtual Audio Device used as Virtual I/O to mix and manage any audio sources from or to any audio devices or applications

Official website: https://voicemeeter.com/

Voicemeeter is a donationware, all participations are welcome

This software also utilizes [Oklab](https://bottosson.github.io/posts/oklab/) as interpolation method for gradients

This software also utilizes [msdfgen](https://github.com/Chlumsky/msdfgen) to generate natively scalable textures

# Voicemeeter desk band

![Prototype](prototype.png)

## Motivation

More convenient simple sound management:

![System setup](system_setup.svg)

- Volume adjustment
- Active output switch (speakers/headphones/remote device/communication channel)
- Remote device support

Plenty of space always available on the taskbar

## Overview

### Tips

- Press Ctrl + C while desk band is focused to shut it down

### Limitations

- Bugs (by using this software you are doing so at your own risk)

#### Windows 11

- Dynamic scale adjustment is not supported, please shut down desk band before modifying display settings
- Technically it is impossible to track empty space left on the taskbar, to avoid overlapping please adjust your workflow/desk band layout

### Install

#### Windows 10

Following the [end of support](https://support.microsoft.com/en-us/windows/windows-10-support-has-ended-on-october-14-2025-2ca8b313-1946-43d3-b55c-2b95b107f281) Windows 10 compatibility is no longer maintained

#### Windows 11

Build solution/download release, run Voicemeeter.DeskBandit.exe

### Uninstall

#### Windows 11

Shut down desk band and remove binaries from your PC

### Features

- Scalable
- Interactive (left click/middle click/right click/wheel/left hold and move)
- Customizable

### Customization

By default UI is animated with target 100 FPS

In case if you experience any issues or seek performance boost/power safety you can disable animations via registry key:

```
HKEY_CURRENT_USER\SOFTWARE\VoicemeeterDeskBand
```

Add DWORD value Animated and set it to 0


Custom colors could be set via registry key:

```
HKEY_CURRENT_USER\SOFTWARE\VoicemeeterDeskBand\Theme
```

Value names could be taken from [Theme](src/Voicemeeter.UI.Controls.Cherry/include/Voicemeeter/UI/Graphics/Theme.hpp)

Color type is DWORD in hexadecimal format RRGGBBAA

For example, to change strip mute warning color to red use following value:

![ThemeWarning sample](theme_warning_sample.png)

Controls could be disabled via registry key:

```
HKEY_CURRENT_USER\SOFTWARE\VoicemeeterDeskBand\Mixer
```

Add DWORD value Vban and set it to 0 to disable VBAN button

Add DWORD value P, V, A1, A2, B1, B2 and set it to 0 to disable corresponding plug/knob control

#### Windows 11

Docking position could be changed via registry key:

```
HKEY_CURRENT_USER\SOFTWARE\VoicemeeterDeskBand
```

Add DWORD value Dock and set it either to 0 or 3:

Left = 0, Right = 3
