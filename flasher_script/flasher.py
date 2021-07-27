import serial, boot_parser
from intelhex import IntelHex

DEBUG = True

class Flasher:
    def __init__(self, serial_port, baudrate):
        try:
            self.serial = serial.Serial(serial_port, baudrate=baudrate, timeout=30)
        except :
            print(f"Couldn't open serial port: {serial_port}")
            if not DEBUG:
                exit()

    def _crc_stm32(self, data):
        CRC_POLYNOMIAL = 0x04C11DB7
        #Computes CRC checksum using CRC-32 polynomial 
        crc = 0xFFFFFFFF

        for d in data:
            crc ^= d
            for i in range(32):
                if crc & 0x80000000:
                    crc = (crc << 1) ^ CRC_POLYNOMIAL 
                else:
                    crc = (crc << 1)

        return (crc & 0xFFFFFFFF)
    
    def flash_image(self, file_name, image_no):
        ih = IntelHex();
        try:
            ih.loadhex(file_name)
        except:
            print("Couldn't open the file")
            if not DEBUG:
                exit()
                
        saddr = ih.minaddr()
        eaddr = ih.maxaddr()
        content = ih.todict()


if __name__ == "__main__":

    parser_obj = boot_parser.init_argparse()
    args = parser_obj.parse_args()

    port = args.port
    baudrate = args.baud
    flasher = Flasher(port, baudrate)


    action = args.action;

    if boot_parser.BOOT_OPTIONS_FLASH_IMAGE == action:
        flasher.flash_image(args.file, args.image)

