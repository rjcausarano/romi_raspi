#RPi Pinouts

#I2C Pins 
#GPIO2 -> SDA
#GPIO3 -> SCL

from smbus2 import SMBus

# Open i2c bus 1 and read one byte from address 80, offset 0
bus = SMBus(1)
#b = bus.read_byte_data(1, 0)
#print(b)
#bus.close()

# Write a byte to address 4, offset 5
address = 4
write_offset = 1
read_offset = 3

data = 0

bus.write_byte_data(address, write_offset, data)
# print(bus.read_byte_data(address, read_offset))
#bus.close()

