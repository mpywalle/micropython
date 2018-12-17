from KTDev import i2cDev

class TP(i2cDev):

    def __init__(self):
        super(TP, self).__init__(80)
        if super(TP, self).i2cDev_ready():
            self.enable = True
            self._TemperatureSensor_enable()

    def _TemperatureSensor_enable(self):
        super(TP, self).i2cDev_mem_write(0x01, 1)
    
    def get_temp(self):
        degree = super(TP, self).i2cDev_mem_read(0x20, 4)
        return degree.strip()

    def get_devID(self):
        deviceID = super(TP, self).i2cDev_mem_read(0x00, 4)
        return deviceID.strip()

