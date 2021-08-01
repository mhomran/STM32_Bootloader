import sys
from intelhex import IntelHex

CRC_POLYNOMIAL = 0x04C11DB7
WORD_SIZE = 4
IMG_HEADER_WORDS = 7
IMG_HEADER_SIZE = WORD_SIZE*IMG_HEADER_WORDS

def calculate_crc(data):
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
    
    
def calculate_image_crc(file_name):
    ih = IntelHex();

    if file_name == None:
        raise Exception("Error: Specify the file name")

    ih.loadhex(file_name)

    start_addr = ih.minaddr()
    if start_addr == None:
        raise Exception("Error: Nothing to flash")
    end_addr = ih.maxaddr()
    content = ih.todict()

    addr = start_addr + IMG_HEADER_SIZE
    data = []
    while addr <= end_addr:
        if addr in content:
            data.append(content[addr])
        else:
            raise Exception("Error: Non contagious memory region!")

        addr = addr + 1
    
    if len(data) == 0:
        raise Exception("Error: Image without content")

    return calculate_crc(data)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("please enter the name of the hex file")
        exit()

    try:
        print("0X%08X" % calculate_image_crc(sys.argv[1]))
    except Exception as e:
        print(e)


