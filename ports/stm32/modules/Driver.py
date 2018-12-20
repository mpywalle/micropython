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
