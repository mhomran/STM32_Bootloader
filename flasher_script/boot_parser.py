import argparse


BOOT_OPTIONS = (
  'Flash hex file into a specific image (0, 3) (default)',
  'Erase image (0, 3)',
  'Flash hex file into a specific sector (4, 11)',
  'Erase sector (4, 11)',
  'Reset controller'
  ) 
BOOT_OPTIONS_FLASH_IMAGE = 0
BOOT_OPTIONS_ERASE_IMAGE = 1
BOOT_OPTIONS_FLASH_SECTOR = 2
BOOT_OPTIONS_ERASE_SECTOR = 3
BOOT_OPTIONS_RESET = 4

MIN_SECTION = 4
MAX_SECTION = 11

MIN_IMAGE = 0
MAX_IMAGE = 3

def init_argparse() -> argparse.ArgumentParser:
    option_msg = ''

    for i, option in enumerate(BOOT_OPTIONS):
        option_msg += f"{i:2}. {option}\n"

    parser = argparse.ArgumentParser(
        usage="%(prog)s [options] port...",
        description="A bootloader for STM32F407VG board",
        formatter_class=argparse.RawTextHelpFormatter
    )
    parser.add_argument(
        "-v", "--version", action="version",
        version = f"{parser.prog} version 0.0.1"
    )

    parser.add_argument(
        "port", type=str,
        help="Enter the COM port e.g. COM1"
    )

    parser.add_argument("-a", "--action", 
    help=option_msg, type=int, choices=range(0, len(BOOT_OPTIONS)),
    default=BOOT_OPTIONS_FLASH_IMAGE
    )

    parser.add_argument(
        "-s", "--section", type=int, choices=range(MIN_SECTION, MAX_SECTION+1),
        help="specify the section to be erased/programmed")

    parser.add_argument(
        "-i", "--image", type=int, choices=range(MIN_IMAGE, MAX_IMAGE+1),
        help="specify the image to be erased/programmed (default:0)",
        default=MIN_IMAGE)

    parser.add_argument(
        "-f", "--file", type=str,
        help="specify the hex file path to be flashed")

    parser.add_argument(
        "-b", "--baud", type=int,
        help="specify the serial port baudrate (default 9600)",
        default=9600)

    return parser


