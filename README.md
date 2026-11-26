| Supported Targets | ESP32-S3 |
| ----------------- | -------- |

# FTX-1 Network Companion

*Built for hams by a lazy ham*

**English** | [日本語](README.ja.md)

![FTX-1 Network Companion browser UI: S-meters, VFO display, scope and FT8 decoder (callsigns in the screenshot are dummies)](docs/screenshot.png)

Propagation never sits still, and sometimes you just can't stop wondering what the bands are doing right now. Is 20 m open? Is that station still coming through?
You'd like to know, but getting up and walking to the shack, let alone settling in front of the radio, feels like too much effort.

This is built for exactly those moments. The rig stays where it is. From anywhere in the house, a PC or Mac with a browser is all you need to change frequency and mode, listen to the receive audio, read the S-meter, and even watch FT8 decodes scroll by.
Do it from the sofa, or next to your after-dinner coffee. For the gloriously lazy ham, it is the right tool.

One thing is deliberate: the network cannot make the radio transmit (see "Restrictions that keep the radio from transmitting"). This is a tool for listening.

## Overview

Firmware for the ESP32-S3, a small Wi-Fi-capable microcontroller board. You connect it to the Yaesu FTX-1 over USB, and it relays the rig's receive audio and status over Wi-Fi to the PC / Mac at hand.

- **Audio**: the ESP32-S3 takes the receive audio from the FTX-1's USB audio and streams it to your browser over Wi-Fi. The FTX-1 appears to a computer like a "USB microphone", using the standard USB audio class (UAC 1.0).
- **Control and status**: through the FTX-1's USB serial port, the browser changes frequency and mode and reads the S-meter and other values. This way of controlling and monitoring a radio from a computer is called **CAT** (Computer Aided Transceiver).
- **FT8**: the received audio is decoded inside the browser.

All the PC / Mac needs is a browser on the same LAN. No dedicated software is required.

```
FTX-1
 ├─ USB Audio (UAC 1.0) ──────────> ESP32-S3 ──Wi-Fi / HTTPS──> browser / ffplay / VLC
 └─ USB Serial (CP210x / CAT) ────>            ├─ Audio streaming (PCM / WAV)
                                               ├─ Real-time FT8 decoder in the browser
                                               ├─ CAT control (frequency, mode, squelch, ...)
                                               └─ OTA firmware update
```

> "OTA" in the diagram stands for Over-The-Air: updating the firmware over the network, without a cable (see "OTA update").

## Hardware requirements

| Item | Requirement |
| --- | --- |
| Board | ESP32-S3 (tested with the **ESP32-S3-DevKitC-1**; see "Recommended board" below) |
| Flash | **4 MB or more** (two OTA slots use 3.4 MB; the default configuration assumes **16 MB**) |
| PSRAM | Not required |
| Network | A LAN with Wi-Fi (2.4 GHz) |

> If your module has a flash size other than 16 MB, see [Changing the flash size](#changing-the-flash-size) after setting up the development environment.

### Recommended board

This project targets the **ESP32-S3-DevKitC-1**, a development board from Espressif, the maker of the ESP32. It is an entry-level, inexpensive board and is easy to get from online shops such as Amazon. You can start using it with just a USB cable to your PC.

The author uses a compatible board from Freenove (see "How to connect it" for details). Compatible boards with the same layout (two USB ports, a USB-to-UART bridge, an ESP32-S3-WROOM-1 module) should work the same way.

A bare board can short against metal or wiring around the radio, or collect dust. Putting it in a case is recommended if needed. When choosing one, keep the following in mind.

- Both USB ports must stay usable (see "How to connect it" below for what each one is for).
- You must be able to reach the BOOT / RST buttons on the board.
- On many boards the Wi-Fi antenna is on the board itself, so a resin case is the safer choice over a metal one.

### How to connect it

The ESP32-S3-DevKitC-1 has two USB ports. Use them as follows.

| Port | Connect to | When |
| --- | --- | --- |
| **USB-OTG** | **FTX-1** (audio and CAT) | In normal use |
| **USB-UART** | **PC** | Only for the initial setup and flashing |

The label for each port is printed on the board (the silkscreen), next to the port. On the Freenove board the two ports are labeled, and other boards are likely to be labeled in the same way. Check the label before you connect.

- The port that goes to the FTX-1 is USB-OTG. In normal use you do not need to connect a PC to the USB-UART port.
- Connect the USB-UART port to a PC only to use the serial menu (Wi-Fi settings and so on) and to flash the firmware (see "First-time setup and flashing").
- In normal use, the USB-OTG port goes to the FTX-1, so connect a power source to the **USB-UART port**. It does not need to be anything special: an ordinary USB charger or a USB battery pack is enough (the power consumption is small).
- **Use USB cables that carry data.** A charge-only cable does not work for either port.
- **The connector shapes depend on the board.** The official ESP32-S3-DevKitC-1 has two **Micro-USB** ports (see Espressif's [user guide](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-devkitc-1/user_guide_v1.1.html)), while the FTX-1 has a **USB Type-C** port. Choose the cable or adapter to match the connector on your board and on the FTX-1.
- **The author's setup (one example):** a compatible board from Freenove ([FNK0099](https://store.freenove.com/products/fnk0099), with two **USB Type-C** ports) connected to the FTX-1 with a good-quality data cable that has **USB Type-C on both ends** (not a charge-only cable). Because both sides are Type-C, one such cable is enough.
- The ESP32-S3 acts as the USB host (the parent side), and the FTX-1 is the device it reads from.

> **Do not expose it to the Internet.** The firmware takes a reasonable level of security into account, but it is not designed to have a port open to the Internet (for example by port forwarding on your router). Use it only on a trusted LAN at home. See "Security notes" for details.

## How to approach this project

### Who this is for

People who can shrug and say, "Well, that's about what you'd expect." It is not for anyone who needs polish or thorough support.
If it works, lucky you. If it doesn't, have a look inside and fix it. If you can take it in that spirit, we will get along fine.

### Forks are very welcome

It only contains the features I wanted in order to be lazier. Whatever is missing, please feel free to fork it and build more features for the lazy ham.
I would love to see more improvements out there that make life easier.
For technical evaluations and future improvement ideas (such as on-device FT8 decoding directly on the ESP32-S3), see [`docs/future_ideas.md`](docs/future_ideas.md).

### About issues

Issue reports are welcome. Handling them is **best effort**, though: replies, fixes and new features are not promised.
Please be patient, or, even better, fork it and share what you fixed.

## Disclaimer

This project implements only the features that I, a lazy ham, found necessary to make my own life easier. It is not a general-purpose product and does not try to cover every use case.

- The software is provided "AS IS", without warranty of any kind. There is no guarantee that it works on any particular radio, environment or purpose, and no promise of maintenance, bug fixes or support.
- The author accepts no liability whatsoever for any damage arising from the use of, or the inability to use, this software, including damage to radios or peripheral equipment, loss of data, network or security problems, and any other direct or indirect damage.
- Use it entirely at your own risk. Flashing the firmware, OTA updates and connecting equipment are done on your own judgement and responsibility.
- Complying with the laws and regulations that apply to operating a radio station and to receiving radio signals is your own responsibility.
- This is a personal project. It is not affiliated with, or endorsed by, Yaesu Musen Co., Ltd. or any other product or company mentioned here.

## License

The copyright of this project belongs to the author. The code is released under the [PolyForm Noncommercial License 1.0.0](LICENSE) (the source files carry `SPDX-License-Identifier: PolyForm-Noncommercial-1.0.0`).

- **Allowed**: you may use, copy, modify and redistribute it freely for any noncommercial purpose, such as personal use, hobby projects and amateur radio.
- **Not allowed**: commercial use. Please contact the author if you would like to use it commercially.
- **Please keep the origin**: when you redistribute it or publish a modified version, keep the `LICENSE` file (with its `Required Notice:` line) and say where it came from.

Because commercial use is excluded, this license is not an "open source" license in the sense of the Open Source Initiative's definition. It is "source-available".

Part of this project follows the structure of the example code of Espressif's `usb_host_uac` component (Apache License 2.0, the USB Host task in [`main/usb_audio.c`](main/usb_audio.c)), and that part remains under the Apache License 2.0. Details are in [`NOTICE`](NOTICE).
Third-party components fetched at build time (the ESP-IDF and the components under `managed_components`) keep their own licenses. For the projects the FT8 decoder refers to, see "About the FT8 decoder".

---

## Development environment setup

### Option A: Dev Container (recommended)

A **Dev Container** puts the whole development environment in a Docker container.
ESP-IDF has many installation steps and is awkward to manage in Python, so a Dev Container
sets up "ESP-IDF v6.1 + openssl + the Espressif extension" automatically and gives you the same
build environment on macOS, Linux, and Windows.

**Requirements**:
- [Docker Desktop](https://www.docker.com/products/docker-desktop/)
- [VS Code](https://code.visualstudio.com/) (extension: [Dev Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers))

```bash
# 1. Get the repository
git clone <URL of this repository>

# 2. Open the folder in VS Code and choose "Reopen in Container"
#    The first run downloads the Docker image (espressif/idf:v6.1), which takes a few minutes
#    ESP-IDF v6.1, openssl, and the Espressif extension are set up automatically
```

Flashing over USB serial needs the `--privileged` flag (already set in `devcontainer.json`).

> **Note**: With Docker Desktop on macOS / Windows, the USB serial port may not be visible from inside the container. In that case, only build inside the container and flash from the PC side (outside the container). The `build/` folder created by the build is also visible from the PC side as is. For the flashing steps, see "Flashing from outside the container" in "First-time setup and flashing".

### Option B: Local (macOS / Linux)

**Requirements**:

| Tool | Version | Where to get it |
| --- | --- | --- |
| ESP-IDF | **v6.1** | [Installation guide](https://docs.espressif.com/projects/esp-idf/en/v6.1/esp32s3/get-started/index.html) |
| openssl | any (the system one is fine) | macOS: `brew install openssl` / Ubuntu: `apt install openssl` |
| Python | 3.10 or later (ESP-IDF checks this at startup and stops if it is too old) | macOS: `brew install python` / Ubuntu: `apt install python3 python3-venv` / [python.org](https://www.python.org/downloads/) |
| cmake | 3.22 or later (bundled with ESP-IDF) | Same as above |

ESP-IDF components (`espressif/usb_host_uac`, `espressif/usb_host_cp210x_vcp`, `espressif/mdns`, and others) are
downloaded from the [ESP Component Registry](https://components.espressif.com) during `idf.py build`.

```bash
# 1. Install ESP-IDF v6.1 (first time only)
mkdir -p ~/esp && cd ~/esp
git clone -b v6.1 --recursive https://github.com/espressif/esp-idf.git
cd esp-idf && ./install.sh esp32s3

# 2. Get the repository
git clone <URL of this repository>
cd ftx1_companion

# 3. Activate the ESP-IDF environment
source ~/esp/esp-idf/export.sh        # bash / zsh

# 4. Build (the first build downloads the dependent components)
idf.py set-target esp32s3
idf.py build

# 5. Flash the ESP32-S3 (PORT is /dev/cu.usbserial-* or similar on macOS)
idf.py -p PORT flash monitor
```

> **Note**: Running `idf.py set-target` or `idf.py fullclean` regenerates the TLS certificate (`build/certs/`).
> Accept the certificate in the browser again afterwards.

## Changing the flash size

The default is 16 MB (`CONFIG_ESPTOOLPY_FLASHSIZE_16MB=y` in [`sdkconfig.defaults`](sdkconfig.defaults)).
If your module is different, change the following.

1. In [`sdkconfig.defaults`](sdkconfig.defaults), change `CONFIG_ESPTOOLPY_FLASHSIZE_16MB=y` to match your module
   (for example `CONFIG_ESPTOOLPY_FLASHSIZE_8MB=y` for 8 MB).

2. Rebuild:

   **Option A (Dev Container)**: run in the VS Code terminal (inside the container)
   ```bash
   idf.py fullclean && idf.py build
   ```

   **Option B (local)**: run in a terminal with the ESP-IDF environment activated
   ```bash
   source ~/esp/esp-idf/export.sh
   idf.py fullclean && idf.py build
   ```

> The partition layout (ota_0 / ota_1, 1700 KB each) does not need changing for 4 MB or more.

## First-time setup and flashing

### 1. Build and flash

Credentials (Wi-Fi password and OTA password) do not have to be built in. You can build and flash as is.

Connect **only the USB-UART port** of the ESP32-S3 to the PC, then run the following. Replace `PORT` with the name of the ESP32-S3's serial port as seen from the PC.

```bash
idf.py build
idf.py -p PORT flash monitor
```

| OS | Example `PORT` | How to find it |
| --- | --- | --- |
| macOS | `/dev/cu.usbserial-0001` | `ls /dev/cu.usb*` |
| Linux | `/dev/ttyUSB0` | `ls /dev/ttyUSB*` |
| Windows | `COM3` | "Ports (COM & LPT)" in Device Manager |

- Unplug and replug the cable: the name that disappears and reappears is the ESP32-S3's port.
- If no port appears, you may need the driver for the board's USB-UART chip (CP210x).
- To leave `idf.py monitor`, press `Ctrl` + `]`.

#### Flashing from outside the container

If the USB serial port is not visible from inside the Dev Container, build inside the container and flash only from the PC side.

```bash
# Inside the container (build, and create the file used for flashing)
idf.py build
idf.py merge-bin     # creates build/merged-binary.bin

# On the PC side (outside the container). Install esptool once, then flash
python3 -m pip install esptool
python3 -m esptool --chip esp32s3 -p PORT write-flash 0x0 build/merged-binary.bin
```

After flashing, open the ESP32-S3's port with any serial monitor you like, such as Tera Term (115200 bps).

### 2. Interactive setup over the USB UART serial console (stored in NVS)

Settings such as the Wi-Fi network and passwords are stored in the ESP32-S3's **NVS** (Non-Volatile Storage: an area inside the microcontroller that keeps its contents when the power is off).
After flashing, or at boot, connect a PC to the USB-UART port and open a serial monitor for the ESP32-S3 (`idf.py monitor`, Tera Term, and so on).

- **First boot (Wi-Fi not configured)**: the configuration menu opens automatically.
- **Already configured**: a 3-second countdown is shown at boot. Press `c`, `m`, or `Enter` to enter the configuration menu. You may also press the BOOT button, but **only after the board has started**. If you hold it down during a reset or power-up, the board starts in flashing mode instead of running the firmware, and the menu does not appear.

```text
**************************************************
*       FTX-1 Companion Configuration Menu       *
**************************************************

==================================================
           CURRENT RUNTIME CONFIGURATION
==================================================
 1) Wi-Fi SSID       : (not set)
 2) Wi-Fi Password   : (empty / disabled)
 3) OTA User ID      : ota
 4) OTA Password     : (empty / disabled)
 9) Web User ID      : admin
 0) Web Password     : (empty / disabled)
 h) Host name        : ftx-1c.local
==================================================
Options:
  1: Set Wi-Fi SSID
  2: Set Wi-Fi Password
  3: Set OTA User ID
  4: Set OTA Password
  9: Set Web User ID
  0: Set Web Password
  h: Set Host name (mDNS / DHCP)
  5: Show passwords in plain text
  6: Save to NVS and Restart device
  7: Exit and continue boot without saving
  8: Erase NVS configuration (Reset to defaults)
--------------------------------------------------
Enter selection (0-9, h):
```

Set at least these three.

1. Enter `1`, then the Wi-Fi SSID.
2. Enter `2`, then the Wi-Fi password (shown masked).
3. Enter `0`, then the **web password** (at least 12 characters; this is the password you use to log in from the browser).

Then set the following as needed.

- `9`: the web user ID (default `admin`)
- `3` / `4`: the OTA update user ID and password (the password must be at least 12 characters)
- `h`: the host name (the default is `ftx-1c`; see "Host name (mDNS)" below)

Finally, enter `6` to run "Save to NVS and Restart device". **Nothing takes effect until you save with `6`.**

> **The web password is required.** Without it, the browser shows `Access is disabled (no password configured)` (HTTP 403) and nothing opens. It never opens without authentication.
>
> The SSID and passwords accept only half-width alphanumerics and symbols (ASCII). Non-ASCII characters, such as Japanese, cannot be entered.

> **Benefit when publishing on Git**: all passwords are stored in the microcontroller's NVS (non-volatile memory). No private settings remain in the Git repository or in build artifacts, so you can publish the source or share binaries safely.

### 3. Check it in the browser

1. After the restart the device connects to Wi-Fi, and the serial monitor log shows `Got IP: x.x.x.x` and `Open https://ftx-1c.local/`.
2. Turn on the FTX-1 and connect the ESP32-S3's **USB-OTG port** to a USB port on the FTX-1 (see "How to connect it").
3. In a browser on a PC / Mac on the same LAN, open `https://ftx-1c.local/` (or `https://<the IP shown in the log>/`). Use `https://`, not `http://`.
4. Because the certificate is self-signed, **a warning appears**. This is your own device on your own LAN, so check the details and continue (accept) (see "Self-signed certificate").
5. You are asked for a user ID and password. The ID is `admin` (or the ID you set with `9`), and the password is the web password you set with `0`.
6. Press **POWER / PLAY** to start playing the received audio. The button changes to `PWR OFF / STOP`; press the same button again to stop.

If it does not work, see "Troubleshooting".

## Host name (mDNS)

While no host name is stored in NVS (first boot, or after erasing the configuration with `8`), the host name is `ftx-1c`.
It is reachable at `https://ftx-1c.local/` through **mDNS** (Multicast DNS: a way to reach a device on the same LAN as "name.local" without a DNS server; using [`espressif/mdns`](https://components.espressif.com/components/espressif/mdns)).
The same name is also sent as the DHCP host name (the name shown in your router's client list).

### Changing the host name (serial menu)

1. Open the menu in the serial monitor (automatic when Wi-Fi is not configured; otherwise press `c` / `m` / `Enter` within 3 seconds after boot).
2. Enter `h`, then the host name **without** `.local`. Press Enter alone to keep the current value.
3. Enter `6` (Save to NVS and Restart device). **The change takes effect only after saving and restarting.** Leaving with `7` discards it.

```text
Enter selection (0-9, h): h
Enter Host name without ".local" [Current: ftx-1c] (a-z, 0-9, '-'; DEFAULT to reset; Enter to keep): radio-shack
-> Host name set to: radio-shack.local
```

- Allowed: lowercase `a-z`, `0-9` and `-`; no `-` at the start or end; up to 63 characters. Names with uppercase letters, `_`, spaces or `.` are rejected ("Rejected" is printed) and the old value is kept.
- Enter `DEFAULT` to go back to `ftx-1c` (option `8`, "Erase NVS configuration", also resets it together with the other settings).
- If the stored value is invalid for any reason, it is replaced by `ftx-1c` at boot.
- The URL is also printed in the boot log as `Open https://<host name>.local/`.
- Update bookmarks and commands (`ffplay`, `curl`, and so on) to the new name. The old name no longer works.

### Notes

- **Certificate name**: the certificate SAN (`ftx-1c` / `ftx-1c.local`) is fixed at build time. With a different host name the browser shows a name-mismatch warning in addition to the self-signed one. You can still proceed after checking it; to avoid the warning, see "Using a certificate for another host name" under "Self-signed certificate".
- **Duplicate names**: if another device with the same name is on the LAN, mDNS may automatically rename this one, for example `ftx-1c-2.local`. When you use several units, give each a different host name.
- **Browser state**: a new host name is a different site for the browser, so you have to log in (Basic authentication) and accept the certificate again.
- macOS, iOS, Windows 10 and later, and recent Linux (avahi) resolve `.local` out of the box.
- Where it does not resolve, use the IP address shown in the log.

## Usage

### Web UI (Browser Console)

Opening `https://ftx-1c.local/` in your browser provides a full-featured transceiver console:

- **Dual VFO Displays & Controls**:
  - **MAIN (VFO-A)**: Highlighted in sky blue. Mapped to the L-channel audio (left ear).
  - **SUB (VFO-B)**: Highlighted in mint green. Mapped to the R-channel audio (right ear).
  - **Band & Mode Switching**: Instant single-click selection across 160m–70cm bands and standard amateur modes (LSB, USB, CW, AM, FM, DATA-L, DATA-U, DATA-F, RTTY, PSK, C4FM).
  - **Tuning Dial & Step Controls**: Smooth frequency adjustment via mouse drag, wheel scrolling, and step buttons (◀ / ▶).
  - **VFO Swap / Copy**: `A⇄B` (swap), `A→B`, and `B→A` helper buttons.
- **Spectrum Scope & Waterfall**:
  - Real-time audio FFT visualization. Automatically selects the optimal SPAN per mode (CW: 1 kHz, AM/FM: 6 kHz, SSB/DATA: 3.5 kHz) with manual override (1k / 2.5k / 3.5k / 6k).
- **Pseudo Squelch**:
  - Variance-based detection for AM and level-based detection for FM/SSB, with independent L/R channel gating.
- **FT8 Real-Time Decoder & Live World Map**:
  - In-browser decoding via Web Worker on every 15-second slot.
  - Clicking `DECODER ON` opens a separate live world map window (`/map`), plotting decoded grid locators in real time.
- **Audio Recording (REC)**:
  - `REC MAIN` / `REC SUB` buttons save recent received audio directly as WAV files in the browser.
- **Power & System Controls**:
  - **POWER / PLAY**: Start/stop the HTTPS audio stream.
  - **REBOOT**: Remotely reboot the ESP32-S3 microcontroller cleanly.
  - **RIG OFF**: Turn off the FTX-1 transceiver hardware over CAT (with confirmation prompt).

### Endpoints

All of them require Basic authentication (see "Security notes" below).

| Path | Description |
| --- | --- |
| `/` | Browser player |
| `/status` | Stream state and diagnostics (JSON): firmware version (`version`), build time (`build`), audio format, uptime, heap (`heap_free` / `heap_min` / `heap_block`; these include PSRAM if it is enabled), internal-RAM-only heap (`int_free` / `int_min` / `int_block`), Wi-Fi `rssi`, and stream send statistics (`stream.slow_sends` / `stream.max_send_ms`, and others) |
| `POST /cat` | Controls the radio (frequency, mode, volume, and so on; JSON, `X-Requested-With: ftx1` required) |
| `POST /reboot` | Reboot the ESP32-S3 (JSON, `X-Requested-With: ftx1` required) |
| `/stream.pcm` | Raw PCM (little endian). The format is reported in the `X-Sample-Rate` / `X-Channels` / `X-Bytes-Per-Sample` headers |
| `/stream.wav` | With a WAV header (indefinite length). For external players |
| `POST /ota` | Firmware update (Basic authentication; see "OTA update" below) |

Example with an external player (48 kHz / 16 bit / mono):

```
ffplay -nodisp -tls_verify 0 https://ftx-1c.local/stream.wav
```

## Pseudo squelch (in the browser)

The USB audio of the Yaesu FTX-1 may ignore the radio's own squelch (noise comes out even with no signal).
So the player page ([`main/index.html`](main/index.html)) has a pseudo squelch that judges the audio and mutes it.
**It affects only playback in this page; `/stream.pcm` and `/stream.wav` are unchanged.**

### Detection modes

| Mode | Opens when | Use |
| --- | --- | --- |
| Variation (default) | The level swing (max − min) over the last 300 ms exceeds the "variation threshold" (default 8 dB) | AM |
| Level | The level exceeds the "threshold" (default -34 dBFS). "Invert" reverses it | FM / SSB |

- With no signal, AM noise settles at about -27 dBFS because of AGC, and the level can be about the same while a signal is received.
  The level cannot tell them apart, so AM is judged by the swing.
  In measurements, 99% of the 300 ms swings with no signal were 6.4 dB or less (max 7.2 dB), and 10 to 40 dB while receiving.
- After opening it holds for 0.4 s so word endings are not cut. Hysteresis is 3 dB (level mode) and the fade is 10 ms.
- Settings (on/off, mode, thresholds, which audio to judge) are stored in the browser's localStorage.

### L and R are judged separately

The FTX-1 USB audio is 48 kHz stereo: L is MAIN (VFO-A) and R is SUB (VFO-B). This does not change with the frequency band.
By default ("judge L and R separately") each channel has its own gate, so when only one side receives, the other stays closed.
Below the meter, the level, swing, and open/closed state of each channel are shown. If it misbehaves, adjust the thresholds using those numbers.

- Raise the variation threshold: when it opens by mistake with no signal (about 10 to 12 dB)
- Lower the variation threshold: when it does not open while receiving (about 6 dB)
- Choosing something like "judge on R, open and close both" makes one gate open and close both channels together.

### Limitations

- A continuous voice that stays at almost the same level for 300 ms or more can cause it to close (eased by the 0.4 s hold).
- It does not judge by the content of the audio, so noise with large swings (such as lightning) opens it.
- Reading the radio's BUSY or S-meter over CAT would be more accurate, but it needs USB serial (CDC) host support and is not implemented.

## About the FT8 decoder

The real-time FT8 decoder implemented in the browser ([`main/index.html`](main/index.html)) is based on the specifications and implementations of the following open-source projects.

| Project | What was referred to |
| --- | --- |
| **[ft8_lib](https://github.com/kgoba/ft8_lib)** by Karlis Goba (YL3JG) — MIT License | LDPC(174, 91) parity-check matrix, CRC-14 polynomial, 8-FSK Gray mapping, message unpacking specification |
| **[WSJT-X](https://wsjt.sourceforge.io/)** by Joe Taylor (K1JT) et al. — GPLv3 | The FT8 protocol specification in general (Costas array, 77-bit message format, LDPC code parameters) |

It is a full-stack FT8 decoder written only in JavaScript for the browser, using the projects above as reference.
To the best of the author's knowledge the code of those projects was not ported or copied directly,
but the protocol constants (parity-check matrix, Gray code table, CRC polynomial, Costas sequence, and so on)
are the specification values published by WSJT-X / ft8_lib, used as they are.

This project would not exist without the contributions of those who came before. We are deeply grateful to all the developers.

### Decoding performance settings

- Up to the top 200 sync candidates are tried, over an audio range of about 125 to 3000 Hz (assuming a crowded 40 m slot).
- Candidates whose Costas match ratio is below 3.5 are not decoded (to suppress false detections).
- Decoding runs once per slot in a Web Worker (a separate thread). A crowded slot takes 1.5 to 1.8 seconds on a Mac, but audio playback and the UI do not stop. The Worker is created from a Blob URL, so the CSP allows `worker-src blob:`.
- The second pass (subtracting decoded signals and searching again) is not implemented. For that reason it cannot pick up weak stations hidden behind strong ones as well as WSJT-X or ft8_lib.

### Time synchronization (NTP) and latency considerations

Due to Wi-Fi transmission and browser audio buffering, an unavoidable transmission latency (typically 100 to 300 ms) exists between when the transceiver receives audio and when it arrives at the browser. The system is designed to account for this:

- **Absorption by DT (Time Offset) search**: The FT8 protocol is inherently designed to tolerate propagation delays and clock skews between stations. The decoder searches for timing offsets (DT) during sync detection (Costas array search). The network latency is naturally absorbed as a positive shift in DT (typically +0.1s to +0.4s) and does not affect decoding success.
- **Adjusted decode trigger timing (14.0s)**: In a standard 15-second FT8 slot, transmission takes place between 0.5s and 13.14s (for DT=0). To accommodate transmission delay and buffering of late-arriving symbols, the decoder snapshots 15.0 seconds of audio and triggers the Web Worker at **14.0s** into the cycle rather than the 15.0s slot boundary. This ensures trailing symbols are not cut off.
- **Waveform continuity**: Network jitter in packet arrival does not alter the sample rate timing; audio is placed into the continuous ring buffer at fixed sample rates (48 kHz / 6.4 kHz), preserving the exact relative phase and timing across symbols.

### FT8 world map

![FT8 world map window: stations plotted by grid locator, with the list of decoded stations below (callsigns in the screenshot are dummies)](docs/ft8_map.png)

Pressing `DECODER ON` opens a world map in a separate window (`/map`) and plots each station at the grid locator (4 characters) contained in the decoded message.

- Call signs are not always shown on the map. Click a marker to see it together with the grid, SNR, distance, and bearing. You can also select a station by clicking a row in the list, and a station that is off screen is moved into view. When several stations overlap at the same grid, each click cycles through them in turn. Click an empty spot or press Esc to deselect.
- The map can be zoomed and dragged (up to ×40). Zoom with the mouse wheel, a double click, or `+` / `−` in the header, and return to the whole view with `FIT`. When zoomed in, grid squares (such as `PM95`) and their names appear.
- Enter your own grid (4 or 6 characters) in `My grid` in the header and press `SET` to enable the distance and bearing display and the great-circle lines. The value is stored in the browser's `localStorage`.
- Station colors are by band (160 m to 6 m), not by MAIN / SUB. The legend lists only the bands currently shown. QSO lines are amber and your own station is red. The list has a BAND column.
- Only messages that contain a grid (CQ and first calls) are plotted. Report exchanges, `RR73`, `73`, and hashed call signs are not.
- Old spots fade and disappear after 15 minutes. Closing and reopening the map window restores the spots so far.
- The coastline is the [Natural Earth](https://www.naturalearthdata.com/) (public domain) 50m land data, thinned at 0.02 degrees and embedded as vectors in [`main/map.html`](main/map.html) (about 220 KB). The outline stays smooth when zoomed in, and it works without an Internet connection.
- If the pop-up is blocked, allow pop-ups for this site from the blocked indicator in the address bar.

### Future improvement ideas

For technical evaluations, memory estimations on on-device FT8 decoding on the ESP32-S3, two-pass decoding, and other ideas, see [`docs/future_ideas.md`](docs/future_ideas.md).

## OTA update

You can rewrite the firmware over the network.

```bash
source ~/esp/esp-idf/export.sh   # not needed inside the Dev Container
idf.py build
read -rs PW   # type the password (it is not kept in the command history)
curl -k -u "ota:$PW" -H "X-Requested-With: ftx1" -X POST --data-binary @build/ftx1_companion.bin https://ftx-1c.local/ota
unset PW
```

The `X-Requested-With: ftx1` header is required as CSRF protection (without it the request gets 403).

- Set the password with `4` in the serial menu (stored in NVS, at least 12 characters). Reading it with `read -rs` as above keeps it out of the command history. Keep `ota:` plus the password to 95 characters or fewer.
- Authentication is HTTP Basic and is sent over HTTPS only. The comparison is constant-time, and a failure is delayed by 1 second.
- The received image is validated by `esp_ota_end()` (magic, chip, checksum, SHA-256). A broken image is rejected with 400 and does not affect the running version.
- There are two 1700K OTA slots (`ota_0` / `ota_1`, no factory app). The update is written to the other slot, then the device switches to it and restarts.
- Rollback is enabled. A new version is marked "confirmed" once Wi-Fi is connected and the web server has started.
  A version that keeps failing to boot is restarted without being confirmed, and the bootloader goes back to the previous version.
- A second `POST /ota` during an update returns 409.
- **After changing the partition layout (for example moving from a single-app layout), the first update must be done over serial with `idf.py flash`.**
- Verified: authentication failure (401), broken image (400), a normal update (`ota_0` ↔ `ota_1`), and the state after restart.
  Not verified: actual rollback behavior, and updating while USB audio is being streamed.

## Self-signed certificate

The self-signed certificate for HTTPS (EC P-256, valid for 2 years, SAN: `ftx-1c` / `ftx-1c.local`) is
generated automatically with `openssl` into `build/certs/` at build time and embedded in the firmware.

- The private key and the certificate are in `build/certs/`, which is in `.gitignore`, so they are not committed.
- Running `idf.py fullclean` or `idf.py set-target` regenerates the certificate, and you must accept the browser warning again.
- As long as you rebuild in the same build directory, the same certificate is used.
- TLS 1.2 / 1.3 as provided by mbedTLS is negotiated.
- Because it is self-signed, browsers show a warning. Trust it only when you are connecting to your own device on your own LAN.

### Using a certificate for another host name

Changing the host name in the serial menu does not change the certificate SAN. To avoid the name-mismatch warning, regenerate the certificate too:

1. In [`main/CMakeLists.txt`](main/CMakeLists.txt), change `CN=` in `-subj` and `-addext "subjectAltName=..."` to the new host name (for example `DNS:radio-shack.local,DNS:radio-shack`).
2. Delete `build/certs`, rebuild, and flash the firmware.
3. Set the same host name with `h` in the serial menu, then save and restart.

## Security notes

### Authentication

All endpoints (`/`, `/map`, `/status`, `/cat`, `POST /reboot`, `/stream.*`, `POST /ota`) are protected by HTTP Basic authentication. Use them over HTTPS only.

- Set the web ID and password with `9` (Web User ID, default `admin`) and `0` (Web Password) in the serial menu. They are stored in NVS.
- Passwords must be at least 12 characters. IDs cannot contain `:` or spaces.
- If no web password is set, the OTA ID / password is used instead. If neither is set, access is refused with 403 (it never opens without authentication).
- Five failures from the same client IP lock it out for 60 seconds and return 429.
- Requests that change state (`POST /cat`, `POST /reboot`, `POST /ota`) require the `X-Requested-With: ftx1` header and a same-origin `Origin` / `Sec-Fetch-Site` as CSRF protection.
- Responses carry `nosniff`, `X-Frame-Options: DENY`, `Referrer-Policy: no-referrer`, and a CSP.

### Restrictions that keep the radio from transmitting

So that the radio cannot be put into transmit over the network, the CAT commands the ESP32 can send to the radio are limited ([`main/cat.c`](main/cat.c)).

- The browser does not build CAT commands. `POST /cat` accepts only the JSON keys `target` / `freq` / `mode` / `vol` / `sq` / `notch` / `notch_hz` / `power` / `action`.
  Unknown keys, duplicate keys, and `cmd` (a raw command) are rejected with 400. `action` is only one of `swap` / `copy_main_to_sub` / `copy_sub_to_main`.
- Commands sent from the ESP32 to the radio use an allow list (`ID` `SV` `AB` `BA` `AI` `RI` `RM` `PS` `FA` `FB` `MD` `AG` `SQ` `BP` `EX`).
  For `EX` only the write that sets the DATA-USB `USB OUT LEVEL` to 100 (`EX010411`) is allowed.
  Commands that lead to transmission, such as `TX` `MX` `KY` `PB` `VX` and `PC`, have no call path in the code and are also blocked by the allow list.
  As a result, this firmware has no operation that puts the radio into transmit.
- Frequency settings are range-checked. Only values from 30 kHz to 470 MHz (30000 to 470000000 Hz, the range the FTX-1 accepts) are passed to the radio; anything outside it is rejected.
- As a second safeguard, consider also setting `RPTT SELECT` / `TX INHIBIT` on the radio itself.
- Transmission that does not go through the ESP32 (microphone PTT, front-panel operation) cannot be prevented.

### Input validation and build settings

- The request body length, the JSON, and the USB audio format (channels, sample size, sample rate) are validated, and values outside the limits are rejected.
- FT8 decode results and similar text are HTML-escaped before display (XSS protection).
- Stack protection (`CONFIG_COMPILER_STACK_CHECK_MODE_STRONG`) and heap poisoning (`CONFIG_HEAP_POISONING_LIGHT`) are enabled.

### Remaining risks

- Secure Boot, flash encryption, and NVS encryption are not supported. Someone who gets the device physically may be able to read the passwords in NVS.
- The HTTPS private key is embedded in the firmware. Because the certificate is self-signed, the warning must be accepted the first time.
- Consider replacing it with a certificate from a trusted CA before real use.
- **Do not expose it to the Internet.** Do not make this device reachable from outside, for example by port forwarding on your router. The firmware takes a reasonable level of security into account, but it is not designed on the assumption that it faces the Internet. Use it only on a trusted LAN.
- If you want to use it away from home, do not open a port; use a way into your LAN that is safe, such as a VPN.

## Limitations

- Only one client can receive the stream at a time.
- Only one audio input device is used (a second and later ones are ignored).
- Only UAC 1.0 devices are supported.
- Browser playback has a delay of a few hundred milliseconds.
- Smartphone browsers are not supported. The UI is designed for the browser of a PC / Mac.

## Troubleshooting

### Flashing and the serial menu

- The serial port is not found, or `Failed to connect` is shown: check that the cable is not charge-only, and that the PC is connected to the **USB-UART port** (not the USB-OTG one).
- `Permission denied` on Linux: on Ubuntu and similar, run `sudo usermod -aG dialout $USER` and log in again (the group name differs between distributions).
- The log stops at `waiting for download`: the board entered flashing mode because **BOOT was held down** during a reset or power-up. Release BOOT and press the RST (reset) button, and it starts normally.
- The boot log stops at `Wi-Fi connection failed`: the board could not join Wi-Fi within 30 seconds. Press the RST button to restart, press `Enter` during the 3-second countdown to open the menu, and fix the SSID (`1`) and password (`2`). It cannot join a 5 GHz-only SSID.

### Browser

- `Access is disabled (no password configured)` (HTTP 403) is shown: the web password is not set. Set it with `0` in the serial menu.
- You cannot log in (401), or `Too many failed attempts` (429) is shown: check the ID and password. After 5 failures in a row from the same device (IP address), requests are refused for 60 seconds.

### Audio and connection

- `No USB audio device streaming` (HTTP 503): no USB audio input device is connected or recognized. Check `usb_audio` in the serial log.
- Audio drops out: open `/status` and check the following.
  - `heap_min` has fallen to a few KB: internal RAM is short. The setting that allocates TLS buffers only when needed (`CONFIG_MBEDTLS_DYNAMIC_BUFFER`) is enabled by default.
  - `stream.max_send_ms` is above about 330 ms (the 64 KB ring buffer), or `rssi` is worse than -75 dBm: check the Wi-Fi signal.
  - Running `audioDiag` in the browser console shows the receive wait (`maxReadMs`) and the browser processing time (`maxProcMs`).
- `ftx-1c.local` does not open: check that it is on the same LAN / subnet, and try opening the IP address from the log directly.
- The browser refuses the connection: check that you opened `https://`, not `http://`.
- The device is not recognized: it may not be UAC 1.0, or a hub may not supply enough power.
- The CAT connection starts about 3 seconds after the FTX-1's USB device appears. This wait is intentional: nothing is sent to the radio while it is still starting up. Then the firmware asks `ID;` once at each CAT-1 RATE, in this order: the rate that worked last time, 115200, 38400 (the radio's factory setting), 19200, 9600, 4800. You do not have to set the rate in this firmware. If no rate gets an answer (`ID` + 4 digits), it tries again every 5 seconds. Check the `cat` lines in the serial log: they show the rate found, every command sent at start-up (`TX`), the radio's answers (`RX`), and any `?;` (the radio's error answer).
- The scope (waterfall) does not match the frequency shown for VFO A / VFO B: this rarely happens. Please be kind to the rig and press the **A→B (A=B)** button to set the frequencies again.

## Source layout

| File | Role |
| --- | --- |
| [`main/main.c`](main/main.c) | Boot sequence (NVS → config check / serial menu → USB audio → Wi-Fi → web server → mDNS) |
| [`main/app_config.c`](main/app_config.c) | Wi-Fi / OTA / web credentials kept in NVS (non-volatile storage) |
| [`main/console_menu.c`](main/console_menu.c) | Interactive configuration menu on the USB UART serial console (SSID, passwords, OTA settings) |
| [`main/usb_audio.c`](main/usb_audio.c) | USB Host and [`espressif/usb_host_uac`](https://components.espressif.com/components/espressif/usb_host_uac): detects the input device, starts and reads the stream |
| [`main/wifi_sta.c`](main/wifi_sta.c) | Wi-Fi station connection (DHCP host name `ftx-1c` by default) |
| [`main/web_server.c`](main/web_server.c) | HTTPS server (player page, status, PCM / WAV streams). The server and the stream task run on core 1, away from Wi-Fi and USB on core 0 |
| [`main/ota.c`](main/ota.c) | OTA update (`POST /ota`, Basic authentication, rollback) |
| [`main/cat.c`](main/cat.c) | CAT control (over the CP210x USB serial port: reads and sets BUSY / S-meter / frequency / mode / volume / squelch) |
| [`main/index.html`](main/index.html) | Browser player (Web Audio API) with a pseudo squelch |
| [`main/map.html`](main/map.html) | FT8 world map window (`/map`). The coastline data is embedded, so it works offline |

### Using it with another radio (a hint)

This project is written for the FTX-1 only and has been tested only on it. It is not meant to grow into a multi-radio library. If you want to try another radio in your own fork, start from the comment at the top of [`main/cat.c`](main/cat.c): it lists every CAT command the firmware sends, with its parameters and the answers it reads, so you can compare it with that radio's CAT manual. The same comment names the radio-specific places (the allow-list, the frequency range, the mode table, the answer parsers, the `EX` menu item, the MAIN / SUB assumption and how the CAT port is found). Whatever you change, re-check that no command that could make the radio transmit can pass the allow-list.

## Glossary

Short explanations of the technical terms used in this document.

### USB and the board

- **USB-OTG port**: the ESP32-S3 port that goes to the FTX-1. The ESP32-S3 acts as the USB host (the parent side).
- **USB-UART port**: the port for a PC (flashing, the serial menu) and for power.
- **UAC (USB Audio Class)**: the standard USB audio specification. The FTX-1 appears to a computer as a "USB microphone" in this way.
- **CDC**: a standard USB class for serial communication.
- **CP210x**: a USB-to-serial chip from Silicon Labs. The FTX-1's CAT port uses it. Some boards also use it for their USB-UART port, and a PC may then need its driver.
- **CAT (Computer Aided Transceiver)**: the way to control a radio from a computer. This project uses it to read and set the frequency, mode and so on.

### Network

- **mDNS**: lets you reach the device by a name such as `ftx-1c.local` instead of an IP address.
- **DHCP**: the router's function that hands out IP addresses automatically.
- **LAN**: the network inside your home.
- **TLS / HTTPS**: encrypts the communication between the browser and the device.
- **SAN (Subject Alternative Name)**: the list of host names written in a certificate. If the name you open is not in it, the browser shows a warning.

### Security

- **CSRF**: an attack that makes your browser send a request you did not intend, from another web page. The `X-Requested-With` header blocks it.
- **XSS**: an attack that makes a web page run injected script. Displayed text is escaped to prevent it.
- **CSP (Content Security Policy)**: a rule the page gives the browser about what it may load and run.
- **Basic authentication**: a simple login method using an ID and a password. It is used together with HTTPS.

### Firmware and development

- **NVS (Non-Volatile Storage)**: an area in the microcontroller that keeps its contents when the power is off. Wi-Fi settings and passwords are stored here.
- **OTA (Over-The-Air)**: updating the firmware over the network, without a cable.
- **ESP-IDF**: the official development environment (SDK) for the ESP32.
- **Dev Container**: a development environment packaged in a Docker container.
- **FT8**: a digital mode for weak-signal contacts. The browser decodes it from the receive audio.
