import os


class IoBackend:
    def __init__(self, bdf: str):
        """
        Initializes the IoBackend, enables the PCI device, reads BAR1,
        and prepares the port interface for x86 I/O.
        """
        self.bdf = bdf

        # 1. Enable the device
        self._enable_device(bdf)

        # 2. Read the IO base address from BAR1
        self.io_base = self._read_bar1_base(bdf)

        # 3. Open /dev/port for hardware I/O (Equivalent to iopl setup)
        try:
            self._port_fd = os.open('/dev/port', os.O_RDWR)
        except PermissionError:
            raise PermissionError("Accessing /dev/port requires root privileges. Run with sudo.")
        except FileNotFoundError:
            raise RuntimeError("/dev/port not found. Ensure you are on a Linux system.")

    def __del__(self):
        # Cleanup file descriptor on destruction.
        if hasattr(self, '_port_fd'):
            try:
                os.close(self._port_fd)
            except OSError:
                pass

    def write_reg(self, offset: int, value: int) -> None:

        # Writes a 32-bit value to the register. (Equivalent to outl)

        port_address = self.io_base + offset
        os.lseek(self._port_fd, port_address, os.SEEK_SET)

        # Convert the 32-bit integer to 4 bytes in little-endian format
        os.write(self._port_fd, value.to_bytes(4, byteorder='little'))

    def read_reg(self, offset: int) -> int:
        """
        Reads a 32-bit value from the register. (Equivalent to inl)
        """
        port_address = self.io_base + offset
        os.lseek(self._port_fd, port_address, os.SEEK_SET)

        # Read 4 bytes and convert back to an integer
        data = os.read(self._port_fd, 4)
        return int.from_bytes(data, byteorder='little')

    @staticmethod
    def _enable_device(bdf: str) -> None:
        # Writes '1' to the sysfs enable file if not already enabled.
        enable_path = f"/sys/bus/pci/devices/{bdf}/enable"
        try:
            with open(enable_path, 'r+') as f:
                content = f.read().strip()
                if content != '1':
                    f.seek(0)
                    f.write('1')
        except FileNotFoundError:
            raise FileNotFoundError(f"PCI device '{bdf}' not found at {enable_path}")
        except PermissionError:
            raise PermissionError("Enabling a PCI device requires root privileges. Run with sudo.")

    @staticmethod
    def _read_bar1_base(bdf: str) -> int:
        # Parses the sysfs resource file to extract the BAR1 base address.
        resource_path = f"/sys/bus/pci/devices/{bdf}/resource"
        try:
            with open(resource_path, 'r') as f:
                lines = f.readlines()
                if len(lines) < 2:
                    raise ValueError(f"Device {bdf} does not appear to have a BAR1.")

                # The resource file contains space-separated values per BAR.
                # Line 0 is BAR0, Line 1 is BAR1. First hex value is the base address.
                bar1_line = lines[1]
                base_str = bar1_line.split()[0]
                return int(base_str, 16)

        except FileNotFoundError:
            raise FileNotFoundError(f"Resource file for {bdf} not found at {resource_path}")