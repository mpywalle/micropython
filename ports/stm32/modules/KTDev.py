from pyb import I2C

class i2cDev():

    def __init__(self, id):
        self.i2c_slave_id = id
        self.i2c_bus_id = 2
        self.i2c_baudrate = 100000
        self.mem_write = 0
        self.mem_read = 0
        self.write = 0
        self.read = 0
        self.i2c_bus = self._i2c_bus_init()

    def _i2c_bus_init(self):
        i2c = I2C(self.i2c_bus_id, I2C.MASTER, baudrate=self.i2c_baudrate)
        return i2c

    def i2cDev_ready(self):
        device_list = self.i2c_bus.scan()
        return (self.i2c_slave_id in device_list) and self.i2c_bus.is_ready(self.i2c_slave_id)

    def i2cDev_send_data(self, data):
        self.i2c_bus.send(data, self.i2c_slave_id)
        self.write = self.write + 1
        return None

    def i2cDev_recv_data(self, length):
        data = self.i2c_bus.recv(length, self.i2c_slave_id)
        self.read = self.read + 1
        return data

    def i2cDev_mem_write(self, start_addr, data):
        self.i2c_bus.mem_write(data, self.i2c_slave_id, start_addr)
        self.mem_write = self.mem_write + 1
        return None

    def i2cDev_mem_read(self, start_addr, length):
        data = self.i2c_bus.mem_read(length, self.i2c_slave_id, start_addr)
        self.mem_read = self.mem_read + 1
        return data

    def get_devID(self):
        deviceID = self.i2cDev_mem_read(0x00, 4)
        return deviceID.strip()
