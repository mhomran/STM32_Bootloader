from os import times
import serial, boot_parser, struct
from intelhex import IntelHex

CRC_POLYNOMIAL = 0x04C11DB7

PDU_TYPES = (
  "BOOT_IF_TYPE_DATA_RECORD",
  "BOOT_IF_TYPE_VERIFY_IMAGE",
  "BOOT_IF_TYPE_EXTENDED_LINEAR_ADDR_RECORD",
  "BOOT_IF_TYPE_LOCK_FLASH",
  "BOOT_IF_TYPE_UNLOCK_FLASH",
  "BOOT_IF_TYPE_ERASE_SECTOR",
  "BOOT_IF_TYPE_ERASE_IMAGE",
  "BOOT_IF_TYPE_RESET_AND_ENTER_APP",
  "BOOT_IF_SET_ACTIVE_IMAGE",
  "BOOT_IF_TYPE_ACK",
  "BOOT_IF_TYPE_ERR"
  )   

IMAGE_ADDR_RANGE = (
    (0x08010000, 0x0803FFFF), 
    (0x08040000, 0x0807FFFF),
    (0x08080000, 0x080BFFFF),
    (0x080C0000, 0x080FFFFF)
)

DUMMY_ADDR = 0
CRC_SIZE = 4
ADDR_SIZE = 2
TYPE_SIZE = 1
ACK_FRAME_SIZE = ADDR_SIZE + TYPE_SIZE + CRC_SIZE  

ADDR_OFFSET = 0
TYPE_OFFSET = 2
DATA_OFFSET = 3

SERIAL_TIME_OUT = 5

class Flasher:
    def __init__(self, serial_port, baudrate):
        try:
            self.serial = serial.Serial(serial_port, baudrate=baudrate)
            # timeout=SERIAL_TIME_OUT)
        except :
            raise Exception(f"Couldn't open serial port: {serial_port}")

    def _calculate_crc(self, data):
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
    
    def _send_pdu(self, data):
        success = False
        trials = 2
        trial = 0
        while trial != trials:
            trial = trial + 1

            self.serial.flushInput()
            self.serial.write(data)

            rx_data_bytes = self.serial.read(ACK_FRAME_SIZE)
            if len(rx_data_bytes) != ACK_FRAME_SIZE:
                print("Timeout")
                continue

            pdu_type = struct.unpack_from("B", rx_data_bytes, offset=TYPE_OFFSET)[0]
            crc_bytes = rx_data_bytes[-CRC_SIZE:]
            crc = struct.unpack("I", crc_bytes)[0]

            if self._calculate_crc(rx_data_bytes[0:-CRC_SIZE]) != crc:
                print("CRC didn't match")
                continue

            if pdu_type != PDU_TYPES.index('BOOT_IF_TYPE_ACK'):
                print("No ACK")
                continue
            
            success = True  
            break 
        if success:
            return
        else:
            raise Exception("Couldn't send the PDU") 

    def _unlock_flash(self):
        pdu = []
        pdu.append(DUMMY_ADDR)
        pdu.append(PDU_TYPES.index('BOOT_IF_TYPE_UNLOCK_FLASH'))

        pdu_bytes = struct.pack("HB", *pdu)
        crc = self._calculate_crc(pdu_bytes)
        crc_bytes = struct.pack("I", crc)
        pdu_bytes = pdu_bytes + crc_bytes

        self._send_pdu(pdu_bytes)

    def _lock_flash(self):
        pdu = []
        pdu.append(DUMMY_ADDR)
        pdu.append(PDU_TYPES.index('BOOT_IF_TYPE_LOCK_FLASH'))

        pdu_bytes = struct.pack("HB", *pdu)
        crc = self._calculate_crc(pdu_bytes)
        crc_bytes = struct.pack("I", crc)
        pdu_bytes = pdu_bytes + crc_bytes

        self._send_pdu(pdu_bytes)

    def _verify_image(self, image_no):
        pdu = []
        pdu.append(DUMMY_ADDR)
        pdu.append(PDU_TYPES.index('BOOT_IF_TYPE_VERIFY_IMAGE'))
        pdu.append(image_no)

        pdu_bytes = struct.pack("HBB", *pdu)
        crc = self._calculate_crc(pdu_bytes)
        crc_bytes = struct.pack("I", crc)
        pdu_bytes = pdu_bytes + crc_bytes

        self._send_pdu(pdu_bytes)        

    def flash_image(self, file_name, image_no):
        
        ih = IntelHex();
        
        if file_name == None:
            raise Exception("Specify the file with -f")

        ih.loadhex(file_name)
       
        start_addr = ih.minaddr()
        if start_addr == None:
            raise Exception("Nothing to flash")
        end_addr = ih.maxaddr()
        content = ih.todict()
        
        if IMAGE_ADDR_RANGE[image_no][0] > start_addr or end_addr > IMAGE_ADDR_RANGE[image_no][1]:
            raise Exception("addresses out of range of the image")

        self._unlock_flash()

        pdu_addr = start_addr
        pdu = []
        pdu_bytes = []
        data = []
        crc = 0
        crc_bytes = 0
        extended_addr = None
        while pdu_addr <= end_addr:
            pdu = []
            pdu_bytes = []
            data = []

            if extended_addr != ((pdu_addr & 0xFFFF0000) >> 16):
                extended_addr = ((pdu_addr & 0xFFFF0000) >> 16)

                pdu.append(extended_addr)
                pdu.append(PDU_TYPES.index('BOOT_IF_TYPE_EXTENDED_LINEAR_ADDR_RECORD'))

                pdu_bytes = struct.pack("HB", *pdu)
                crc = self._calculate_crc(pdu_bytes)
                crc_bytes = struct.pack("I", crc)
                pdu_bytes = pdu_bytes + crc_bytes

                self._send_pdu(pdu_bytes)
            else:
                if pdu_addr & 0b11 != 0: # not word aligned
                    pdu_addr = pdu_addr &  ~(0b11) 
                
                flash = False
                for data_index in range(16):
                    data_addr = pdu_addr + data_index
                    if data_addr in content:
                        flash = True
                        data.append(content[data_addr])
                    else:
                        data.append(0xFF)
                if flash:
                    pdu.append(pdu_addr & 0xFFFF)
                    pdu.append(PDU_TYPES.index('BOOT_IF_TYPE_DATA_RECORD'))
                    pdu = pdu + data
                    pdu_bytes = struct.pack("HB" + "B"*16, *pdu)

                    crc = self._calculate_crc(pdu_bytes)
                    crc_bytes = struct.pack("I", crc)
                    pdu_bytes = pdu_bytes + crc_bytes

                    self._send_pdu(pdu_bytes)
                    
                pdu_addr = pdu_addr + 16
        
        self._lock_flash()
            

        self._verify_image(image_no)

    def erase_image(self, image_no):
        self._unlock_flash()

        pdu = []
        pdu.append(DUMMY_ADDR)
        pdu.append(PDU_TYPES.index('BOOT_IF_TYPE_ERASE_IMAGE'))
        pdu.append(image_no)

        pdu_bytes = struct.pack("HBB", *pdu)
        crc = self._calculate_crc(pdu_bytes)
        crc_bytes = struct.pack("I", crc)
        pdu_bytes = pdu_bytes + crc_bytes

        self._send_pdu(pdu_bytes)

        self._lock_flash()

    def reset(self):
        pdu = []
        pdu.append(DUMMY_ADDR)
        pdu.append(PDU_TYPES.index('BOOT_IF_TYPE_RESET_AND_ENTER_APP'))

        pdu_bytes = struct.pack("HB", *pdu)
        crc = self._calculate_crc(pdu_bytes)
        crc_bytes = struct.pack("I", crc)
        pdu_bytes = pdu_bytes + crc_bytes

        self._send_pdu(pdu_bytes)

    def set_active_image(self, image_no):
        pdu = []
        pdu.append(DUMMY_ADDR)
        pdu.append(PDU_TYPES.index('BOOT_IF_SET_ACTIVE_IMAGE'))
        pdu.append(image_no)

        pdu_bytes = struct.pack("HBB", *pdu)
        crc = self._calculate_crc(pdu_bytes)
        crc_bytes = struct.pack("I", crc)
        pdu_bytes = pdu_bytes + crc_bytes

        self._send_pdu(pdu_bytes)

if __name__ == "__main__":

    parser_obj = boot_parser.init_argparse()
    args = parser_obj.parse_args()

    port = args.port
    baudrate = args.baud

    try:
        flasher = Flasher(port, baudrate)
        action = args.action;
        if boot_parser.BOOT_OPTIONS_FLASH_IMAGE == action:
            flasher.flash_image(args.file, args.image)
        elif boot_parser.BOOT_OPTIONS_ERASE_IMAGE == action:
            flasher.erase_image(args.image)
        elif boot_parser.BOOT_OPTIONS_RESET == action:
            flasher.reset()
        elif boot_parser.BOOT_OPTIONS_SET_ACTIVE_IMAGE == action:
            flasher.set_active_image(args.image)
    except Exception as e:
        print(e)


