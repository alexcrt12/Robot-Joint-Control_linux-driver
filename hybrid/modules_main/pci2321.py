from enum import Enum, auto
from dataclasses import dataclass
from io_backend import IoBackend


class Port(Enum):
    P1A = auto()
    P1B = auto()
    P1C = auto()
    P2A = auto()
    P2B = auto()
    P2C = auto()


class Dir(Enum):
    Input = auto()
    Output = auto()


@dataclass
class PortInfo:
    data_off: int
    dir_off: int
    byte_index: int
    dir_bit: int


class Pci2321:
    def __init__(self, bdf: str = "0000:05:01.0"):
        self._io = IoBackend(bdf)

    @staticmethod
    def _port_mapping(port: Port) -> PortInfo:
        mapping = {
            Port.P1A: PortInfo(0x00, 0x04, 0, 0),
            Port.P1B: PortInfo(0x00, 0x04, 1, 1),
            Port.P1C: PortInfo(0x00, 0x04, 2, 2),
            Port.P2A: PortInfo(0x10, 0x14, 0, 0),
            Port.P2B: PortInfo(0x10, 0x14, 1, 1),
            Port.P2C: PortInfo(0x10, 0x14, 2, 2),
        }
        # Fallback implicitly to avoid errors, matching C++ logic
        return mapping.get(port, PortInfo(0x00, 0x00, 0, 0))

    def configure_port(self, port: Port, direction: Dir) -> None:
        info = self._port_mapping(port)

        reg = self._io.read_reg(info.dir_off)
        if direction == Dir.Output:
            reg |= (1 << info.dir_bit)
        else:
            # & 0xFFFFFFFF restricts the Python integer to 32 unsigned bits
            reg &= ~(1 << info.dir_bit) & 0xFFFFFFFF

        self._io.write_reg(info.dir_off, reg)

    def write_channel(self, port: Port, bit: int, on: bool) -> None:
        info = self._port_mapping(port)
        bitpos = info.byte_index * 8 + bit  # Position in the 32-bit register

        reg = self._io.read_reg(info.data_off)
        if on:
            reg |= (1 << bitpos)
        else:
            reg &= ~(1 << bitpos) & 0xFFFFFFFF

        self._io.write_reg(info.data_off, reg)

    def write_port(self, port: Port, value: int) -> None:
        info = self._port_mapping(port)
        shift = info.byte_index * 8

        reg = self._io.read_reg(info.data_off)

        # MODIFY
        # Clear the 8 bits at the target position
        reg &= ~(0xFF << shift) & 0xFFFFFFFF
        # Set the new value (masked to 8 bits to match uint8_t cast)
        reg |= (value & 0xFF) << shift

        self._io.write_reg(info.data_off, reg)

    def read_port(self, port: Port) -> int:
        info = self._port_mapping(port)
        shift = info.byte_index * 8

        # READ
        reg = self._io.read_reg(info.data_off)

        # EXTRACT
        value = (reg >> shift) & 0xFF
        return value