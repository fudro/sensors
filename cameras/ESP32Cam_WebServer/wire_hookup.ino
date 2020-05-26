/*
 * PROGRAMMING
 * Use USB to FTDI connector.
 * Make sure ESP32 pin GPIO0 is connected to GND.
 * Connect FTDI power (VCC) to 3.3v power pin on the ESP32.
 * Set FTDI jumper to 3.3v
 * 
 * WIRING CONNECTIONS
 * ESP32  |   FTDI
 * ---------------
 * U0T    |   RX  (OK to disconnect in Normal Operation Mode)
 * U0R    |   TX  (OK to disconnect in Normal Operation Mode)
 * IO0    |   Connect to GND (Programming Mode Only)
 * 3V3    |   VCC (Programming Mode Only)
 * 5V     |   VCC (Normal Operation Mode - if powering from USB. Make sure to set FTDI jumper to 5v.)
 * GND    |   GND
 * 
 * 
 * RUNNING
 * Connect FTDI power (VCC) to 5v power pin on the ESP32.
 * Set FTDI jumper to 5v
 * Remove the connection between GPIO0 and GND.
 * Connect USB to computer.
 * Open Serial Monitor.
 * Press RESET button on ESP32 Cam.
 * Launch the displayed IP address in a browser.
 */
