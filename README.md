# STM32-AD9833-Generator
Library for the AD9833 microchip for STM32 microcontrollers using software or hardware (LL) SPI

## How to configure
You can use Software or Hardware SPI. Using Software or Hardware SPI only affects the AD9833_Init and AD9833_SendData functions. If you wish, you can change them to suit yourself.
The Software SPI has been tested at HCLK = 100 MHz and STMF411CEU6.
The Hardware SPI uses the LL library and is tested at 12.5 MBits/s with the following SPI settings in CubeMX:
 TransferDirection      FULL_DUPLEX
 Mode                   MASTER
 DataWidth              8BIT
 ClockPolarity          HIGH
 ClockPhase             1EDGE
 NSS                    NSS_SOFT
 BaudRate               BAUDRATEPRESCALER_DIV8
 BitOrder               MSB_FIRST

1. Using STM32CubeMX or manually, configure the pin connected to the FSYNC (CS) pin of the AD9833 chip as GPIO_Output. 
If you are using hardware SPI, configure it according to the parameters specified above. If you want to use software SPI, configure two additional pins as GPIO_Output.

2. In the AD9833.h file, correct the definition of the microcontroller port and pin number to which the FSYNC (CS) pin of the AD9833 chip is connected, to the one configured earlier.
```C
#define AD9833_CS_Port          GEN_SPI_CS_GPIO_Port    // Port of SPI CS pin
#define AD9833_CS_Pin           GEN_SPI_CS_Pin          // SPI CS pin
```

### If you are using Software SPI
3. Uncomment the line of code "#define AD9833_USE_SOFTWARE_SPI".
```C
#define AD9833_USE_SOFTWARE_SPI
```
4. Correct the definition of the microcontroller ports and pins to which the SDATA and SCLK pins of the AD9833 chip are connected.
```C
#define AD9833_SDATA_Port       GEN_SPI_MOSI_GPIO_Port  // Port of SPI SDATA pin
#define AD9833_SDATA_Pin        GEN_SPI_MOSI_Pin        // SPI SDATA pin
#define AD9833_SCLK_Port        GEN_SPI_SCK_GPIO_Port   // Port of SPI SCLK pin
#define AD9833_SCLK_Pin         GEN_SPI_SCK_Pin         // SPI SCLK pin
```
### If you are using Hardware SPI
3. If necessary, correct the definition of the microcontroller's SPI module to which the AD9833 is connected.
```C
#define AD9833_SPI_INSTANCE SPI1
```

## How to use
There some functions to change the behavior of AD9833 that you can use:

```C

```
