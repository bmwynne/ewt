Encrypt We Trust
================

*Build Process: 
1. Download our EWT repo...
2. cd EWT
3. mkdir build && cd build
4. cmake .. -DCMAKE_TOOLCHAIN_FILE=../arm.cmake -DBUILD_TYPE=firmware
5. make
6. See "Download Firmware to Board"

*Made Changes to file structure?
1. cd build
2. make clean
3. make rebuild_cache
4. make
5. See "Download Firmware to Board"

*Download Firmware to Board:
1. Git Clone [edbg](https://github.com/ataradov/edbg.git) at desired location.
   - a. This is edbg, see commands from ataradov's README

2. Within 'build' folder...
   - a. [location to edbg]/./edbg/./edbg -bpv -t atmel_cm4 -f bin/firmware.bin

3. Success
4. Failure?
   - a. Retry...

*Useful Documentation/Links:
   - [Atmel Studio Framework Xplained Pro Docs](http://asf.atmel.com/docs/latest/common.components.wifi.winc1500.ap_scan_example.sam4s_xplained_pro/html/group__sam4s__xplained__pro__features__group.html)

   - [Atmel Studio Framework Suite](http://www.microchip.com/avr-support/advanced-software-framework-(asf))

