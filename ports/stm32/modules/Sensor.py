from KTDev import i2cDev

class TP(i2cDev):

    def __init__(self):
        super(TP, self).__init__(80)
        if super(TP, self).i2cDev_ready():
            self.enable = True
            self._TemperatureSensor_enable()

    def _TemperatureSensor_enable(self):
        try:
            super(TP, self).i2cDev_mem_write(0x01, 1)
            return 0
        except Exception as e:
            print("Exception: %s" %e)
            return -1
    
    def get_temp(self):
        try:
            degree = super(TP, self).i2cDev_mem_read(0x20, 4)
            return degree.strip()
        except Exception as e:
            print("Exception: %s" %e)
            return -1

#    def get_devID(self):
#        deviceID = super(TP, self).i2cDev_mem_read(0x00, 4)
#        return deviceID.strip()

class Button(i2cDev):

    def __init__(self):
        super(Button, self).__init__(100)
        if super(Button, self).i2cDev_ready():
            self.enable = True

    def get_status(self):
        try:
            data = super(Button, self).i2cDev_mem_read(0x02, 2)
            status = {'status': data[0], 'count': data[1]}
            return status
        except Exception as e:
            print("Exception: %s" %e)
            return -1
