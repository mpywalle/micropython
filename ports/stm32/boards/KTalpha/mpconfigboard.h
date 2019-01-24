#define MICROPY_HW_BOARD_NAME       "KTalphav0.2"
#define MICROPY_HW_MCU_NAME         "STM32F405RG"

#define MICROPY_PY_THREAD           (1)
#define MICROPY_PY_THREAD_GIL       (1)

#define MICROPY_HW_ENABLE_INTERNAL_FLASH_STORAGE (0)
#define MICROPY_HW_HAS_SWITCH       (0)
#define MICROPY_HW_HAS_FLASH        (1)
#define MICROPY_HW_HAS_SDCARD       (0)
#define MICROPY_HW_HAS_MMA7660      (1)
#define MICROPY_HW_HAS_LCD          (0)
#define MICROPY_HW_ENABLE_RNG       (1)
#define MICROPY_HW_ENABLE_RTC       (1)
#define MICROPY_HW_ENABLE_SERVO     (0)
#define MICROPY_HW_ENABLE_DAC       (0)
#define MICROPY_HW_ENABLE_USB       (1)

// enable trace log
#define MICROPY_PY_DEBUG			(0)

//for POWER MANAGE CHIP (jlrjlr_5305)	
#define MICROPY_POWER_CHIP_ACT	    (1)

// use external SPI flash for storage
#define MICROPY_HW_SPIFLASH_SIZE_BITS (8 * 1024 * 1024)        // 8M bits flash for KT ver alpha
#define MICROPY_HW_SPIFLASH_CS      (pin_B12)
#define MICROPY_HW_SPIFLASH_SCK     (pin_B13)
#define MICROPY_HW_SPIFLASH_MOSI    (pin_B15)
#define MICROPY_HW_SPIFLASH_MISO    (pin_B14)

// block device config for SPI flash
extern const struct _mp_spiflash_config_t spiflash_config;
extern struct _spi_bdev_t spi_bdev;
#define MICROPY_HW_BDEV_IOCTL(op, arg) ( \
    (op) == BDEV_IOCTL_NUM_BLOCKS ? (MICROPY_HW_SPIFLASH_SIZE_BITS / 8 / FLASH_BLOCK_SIZE) : \
    (op) == BDEV_IOCTL_INIT ? spi_bdev_ioctl(&spi_bdev, (op), (uint32_t)&spiflash_config) : \
    spi_bdev_ioctl(&spi_bdev, (op), (arg)) \
)
#define MICROPY_HW_BDEV_READBLOCKS(dest, bl, n) spi_bdev_readblocks(&spi_bdev, (dest), (bl), (n))
#define MICROPY_HW_BDEV_WRITEBLOCKS(src, bl, n) spi_bdev_writeblocks(&spi_bdev, (src), (bl), (n))

// HSE is 12MHz
#define MICROPY_HW_CLK_PLLM (12)
#define MICROPY_HW_CLK_PLLN (336)
#define MICROPY_HW_CLK_PLLP (RCC_PLLP_DIV2)
#define MICROPY_HW_CLK_PLLQ (7)
#define MICROPY_HW_CLK_LAST_FREQ (1)

// The pyboard has a 32kHz crystal for the RTC
#define MICROPY_HW_RTC_USE_LSE      (0)
#define MICROPY_HW_RTC_USE_US       (0)
#define MICROPY_HW_RTC_USE_CALOUT   (0)

// UART config
#define MICROPY_HW_UART1_NAME   "XB"
#define MICROPY_HW_UART2_TX     (pin_A2)
#define MICROPY_HW_UART2_RX     (pin_A3)
#define MICROPY_HW_UART3_NAME   "YB"
#define MICROPY_HW_UART3_TX     (pin_C10)
#define MICROPY_HW_UART3_RX     (pin_C11)

// I2C busses
#define MICROPY_HW_I2C1_NAME "X"
#define MICROPY_HW_I2C1_SCL (pin_B8)
#define MICROPY_HW_I2C1_SDA (pin_B9)
#define MICROPY_HW_I2C2_NAME "Y"
#define MICROPY_HW_I2C2_SCL (pin_B10)
#define MICROPY_HW_I2C2_SDA (pin_B11)

// SPI busses
#define MICROPY_HW_SPI1_NAME "X"
#define MICROPY_HW_SPI1_NSS  (pin_B12)
#define MICROPY_HW_SPI1_SCK  (pin_B13)
#define MICROPY_HW_SPI1_MISO (pin_B14)
#define MICROPY_HW_SPI1_MOSI (pin_B15)

// USRSW has no pullup or pulldown, and pressing the switch makes the input go low
#define MICROPY_HW_USRSW_PIN        (pin_B3)
#define MICROPY_HW_USRSW_PULL       (GPIO_PULLUP)
#define MICROPY_HW_USRSW_EXTI_MODE  (GPIO_MODE_IT_FALLING)
#define MICROPY_HW_USRSW_PRESSED    (0)


// The pyboard has 4 LEDs
#define MICROPY_HW_LED1             (pin_C0) // green
#define MICROPY_HW_LED2             (pin_C1) // blue
#define MICROPY_HW_LED3             (pin_C2) // red
//#define MICROPY_HW_LED3_PWM         { TIM2, 2, TIM_CHANNEL_1, GPIO_AF1_TIM2 }
#define MICROPY_HW_LED_ON(pin)      (mp_hal_pin_low(pin))
#define MICROPY_HW_LED_OFF(pin)     (mp_hal_pin_high(pin))

// SD card detect switch
#define MICROPY_HW_SDCARD_DETECT_PIN        (pin_A8)
#define MICROPY_HW_SDCARD_DETECT_PULL       (GPIO_PULLUP)
#define MICROPY_HW_SDCARD_DETECT_PRESENT    (GPIO_PIN_RESET)

// USB config
#define MICROPY_HW_USB_FS              (1)
#define MICROPY_HW_USB_VBUS_DETECT_PIN (pin_A9)
#define MICROPY_HW_USB_OTG_ID_PIN      (pin_A10)

// MMA accelerometer config
#define MICROPY_HW_MMA_AVDD_PIN     (pin_B5)

// Bootloader configuration (only needed if Mboot is used)
#define USE_MBOOT 1

// #define MBOOT_I2C_PERIPH_ID 1
// #define MBOOT_I2C_SCL (pin_B8)
// #define MBOOT_I2C_SDA (pin_B9)
// #define MBOOT_I2C_ALTFUNC (4)

#define MBOOT_BOOTPIN_PIN (pin_A0)
#define MBOOT_BOOTPIN_PULL (MP_HAL_PIN_PULL_UP)
#define MBOOT_BOOTPIN_ACTIVE (0)

#define MBOOT_SPIFLASH_ADDR (0x80000000)
#define MBOOT_SPIFLASH_BYTE_SIZE (1 * 1024 * 1024)
#define MBOOT_SPIFLASH_LAYOUT "/0x80000000/64*16Kg"
#define MBOOT_SPIFLASH_ERASE_BLOCKS_PER_PAGE (16/4)
#define MBOOT_SPIFLASH_SPIFLASH (&spi_bdev.spiflash)
#define MBOOT_SPIFLASH_CONFIG (&spiflash_config)

// Bootloader LED GPIO port/pin
#define BOOTLDR_LED1_PIN     (GPIO_PIN_0)
#define BOOTLDR_LED1_PORT    (GPIOC)
#define BOOTLDR_LED2_PIN     (GPIO_PIN_1)
#define BOOTLDR_LED2_PORT    (GPIOC)
#define BOOTLDR_LED3_PIN     (GPIO_PIN_2)
#define BOOTLDR_LED3_PORT    (GPIOC)

// Flash sectors for the bootloader.
// Flash FS sector, main FW sector, max sector. 
// TODO, need to redefine
#define BT_INT_FLASH_LAYOUT        {1, 4, 11}
