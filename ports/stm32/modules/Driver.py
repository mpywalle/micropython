from KTDev import i2cDev

class DMotor(i2cDev):

    def __init__(self):
        super(DMotor, self).__init__(16)
        if super(DMotor, self).i2cDev_ready():
            self.enable = True

    def get_status(self):
        data = super(DMotor, self).i2cDev_mem_read(0x01, 4)
        status = {'lmotor_speed': data[0], 'lmotor_rotation': data[2], 'rmotor_speed': data[1], 'rmotor_rotation': data[3]}
        return status

    def set_speed(self, lmotor_speed, lmotor_rotation, rmotor_speed, rmotor_rotation):
        if lmotor_speed not in range(0, 100):
            return -1
        if rmotor_speed not in range(0, 100):
            return -1 
        if lmotor_rotation not in [0, 1, 2]:
            return -1
        if rmotor_rotation not in [0, 1, 2]:
            return -1

        speed = bytes([lmotor_speed, rmotor_speed, lmotor_rotation, rmotor_rotation])
        super(DMotor, self).i2cDev_mem_write(0x01, speed)

        return 0

class LED(i2cDev):

    def __init__(self):
        super(LED, self).__init__(20)
        if super(LED, self).i2cDev_ready():
            self.enable = True

    def get_status(self):
        data = super(LED, self).i2cDev_mem_read(0x02, 4)
        status = {'red': data[0], 'green': data[1], 'blue': data[2]}
        return status

    def set_color(self, red, green, blue):
        if (red not in range(0, 256)) or (green not in range(0, 256)) or (blue not in range(0, 256)):
            return -1

        color = bytes([red, green, blue, 0x00])
        super(LED, self).i2cDev_mem_write(0x02, color)

        return 0
