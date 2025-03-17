# STM32-AD9833-Generator
Library for the AD9833 microchip for STM32 microcontrollers using software or hardware (LL) SPI

## How to use
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

1. 
2. Determine the port and pin of the microcontroller to which the CS pin of the AD9833 chip is connected
```C
#define AD9833_CS_Port          GEN_SPI_CS_GPIO_Port    // Port of SPI CS pin
#define AD9833_CS_Pin           GEN_SPI_CS_Pin          // SPI CS pin
```

### Software SPI
Uncomment the line "#define AD9833_USE_SOFTWARE_SPI" like this:
```C
#define AD9833_USE_SOFTWARE_SPI
```
Define the port and pin of the microcontroller to which the SDATA and SCLK pins of the AD9833 chip are connected
```C
#define AD9833_SDATA_Port       GEN_SPI_MOSI_GPIO_Port  // Port of SPI SDATA pin
#define AD9833_SDATA_Pin        GEN_SPI_MOSI_Pin        // SPI SDATA pin
#define AD9833_SCLK_Port        GEN_SPI_SCK_GPIO_Port   // Port of SPI SCLK pin
#define AD9833_SCLK_Pin         GEN_SPI_SCK_Pin         // SPI SCLK pin
```
### Hardware SPI
Define the SPI module of the microcontroller to which the AD9833 chip is connected
```C
#define AD9833_SPI_INSTANCE SPI1
```

There some functions to change the behavior of AD9833 that you can use:

```C
void AD9833_Init(uint16_t Wave,float FRQ,float Phase);   // Initializing AD9833
void AD9833_SetWave(uint16_t Wave);                      // Sets Output Wave Type
void AD9833_SetWaveData(float Frequency,float Phase);    // Sets Wave Frequency & Phase
```
