#!/usr/bin/env python3
import sys
import time
import struct
try:
    import serial
except ImportError:
    print("Error: 'pyserial' library not found. Please install it using 'pip install pyserial'")
    sys.exit(1)

# Telemetry Struct format (12 bytes total)
# <  : little-endian
# H  : uint16_t (header, 2 bytes)
# I  : uint32_t (packet_id, 4 bytes)
# h  : int16_t (temp_raw, 2 bytes)
# H  : uint16_t (status_flags, 2 bytes)
# H  : uint16_t (crc16, 2 bytes)
PACKET_FORMAT = "<H I h H H"
PACKET_SIZE = struct.calcsize(PACKET_FORMAT)
SYNC_WORD = 0xA55A

def calculate_crc16_ccitt(data: bytes) -> int:
    """
    Computes CRC-16 CCITT-FALSE (Polynomial: 0x1021, Init: 0xFFFF)
    matching the STM32 USART driver.
    """
    crc = 0xFFFF
    for byte in data:
        crc ^= (byte << 8)
        for _ in range(8):
            if crc & 0x8000:
                crc = ((crc << 1) ^ 0x1021) & 0xFFFF
            else:
                crc = (crc << 1) & 0xFFFF
    return crc

def parse_telemetry(port_name, baudrate=115200):
    print(f"[*] Opening serial port {port_name} at {baudrate} baud...")
    try:
        ser = serial.Serial(port_name, baudrate, timeout=1.0)
    except Exception as e:
        print(f"[-] Error opening serial port: {e}")
        return

    print("[*] Listening for telemetry packets (Sync: 0xA55A)... Press Ctrl+C to stop.")
    
    # State-based byte sync parser
    buffer = bytearray()
    
    try:
        while True:
            # Read whatever bytes are available
            data = ser.read(1)
            if not data:
                continue
            
            buffer.extend(data)
            
            # We need at least PACKET_SIZE bytes to form a packet
            while len(buffer) >= PACKET_SIZE:
                # Look for the sync header (0xA55A in little-endian is [0x5A, 0xA5])
                if buffer[0] == 0x5A and buffer[1] == 0xA5:
                    packet_bytes = buffer[:PACKET_SIZE]
                    
                    # Unpack the 12-byte telemetry packet
                    header, packet_id, temp_raw, status_flags, rcv_crc = struct.unpack(PACKET_FORMAT, packet_bytes)
                    
                    # Compute CRC on the first 10 bytes of the packet
                    calc_crc = calculate_crc16_ccitt(packet_bytes[:-2])
                    
                    # Verify CRC
                    crc_ok = (calc_crc == rcv_crc)
                    status_str = "OK" if crc_ok else "ERROR (CRC MISMATCH)"
                    
                    # Print parsed information
                    print(f"[{time.strftime('%H:%M:%S')}] Packet #{packet_id} | "
                          f"Temp: {temp_raw / 10.0:.1f}°C ({temp_raw} raw) | "
                          f"Status: 0x{status_flags:04X} | "
                          f"CRC: 0x{rcv_crc:04X} (Calculated: 0x{calc_crc:04X}) | "
                          f"Status: {status_str}")
                    
                    # Consume the processed packet
                    buffer = buffer[PACKET_SIZE:]
                else:
                    # Sync word not found at start, discard the first byte and search again
                    buffer.pop(0)
                    
    except KeyboardInterrupt:
        print("\n[*] Exiting telemetry viewer.")
    finally:
        ser.close()

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python telemetry_viewer.py <SERIAL_PORT> [BAUDRATE]")
        print("Example: python telemetry_viewer.py COM3")
        print("         python telemetry_viewer.py /dev/ttyUSB0 115200")
        
        # Try to list available ports
        try:
            import serial.tools.list_ports
            ports = list(serial.tools.list_ports.comports())
            if ports:
                print("\nAvailable ports:")
                for p in ports:
                    print(f" - {p.device}: {p.description}")
            else:
                print("\nNo serial ports detected.")
        except Exception:
            pass
        sys.exit(1)
        
    port = sys.argv[1]
    baud = int(sys.argv[2]) if len(sys.argv) > 2 else 115200
    parse_telemetry(port, baud)
