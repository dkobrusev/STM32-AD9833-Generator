# STM32-AD9833-Generator
## Library for the AD9833 microchip for STM32 microcontrollers using software or hardware (LL) SPI

1. [How to configure](#how-to-configure)
2. [How to use](#how-to-use)
3. [Example](#example)

---

## How to configure
You can use Software or Hardware SPI. Using Software or Hardware SPI only affects the AD9833_Init and AD9833_SendData functions. If you wish, you can change them to suit yourself.

The Software SPI has been tested at HCLK = 100 MHz and STMF411CEU6.

The Hardware SPI uses the LL library and is tested at 12.5 MBits/s with the following SPI settings in CubeMX:

| Parameter | Value |
| :--- | :---: |
| TransferDirection | FULL_DUPLEX |
| DataWidth | 8BIT |
| ClockPolarity | HIGH |
| ClockPhase | 1EDGE |
| NSS | NSS_SOFT |
| BitOrder | MSB_FIRST |

1. Using STM32CubeMX or manually, configure the pin connected to the FSYNC (CS) pin of the AD9833 chip as GPIO_Output. 
If you are using hardware SPI, configure it according to the parameters specified above. If you want to use software SPI, configure two additional pins as GPIO_Output.

2. In the AD9833.c file, set the frequency value on the MCLK pin of the AD9833 chip.
```C
// Example for a frequency 8 MHz
const float AD9833_freq_ref = 8000000.0;
```
3. In the AD9833.h file, correct the definition of the microcontroller port and pin number to which the FSYNC (CS) pin of the AD9833 chip is connected, to the one configured earlier.
```C
// Example for CS in PB0 pin
#define AD9833_CS_Port          GPIOB       // Port of SPI CS pin
#define AD9833_CS_Pin           GPIO_PIN_0  // SPI CS pin
```

### If you are using Software SPI
4. Uncomment the line of code "#define AD9833_USE_SOFTWARE_SPI".
```C
#define AD9833_USE_SOFTWARE_SPI
```
5. Correct the definition of the microcontroller ports and pins to which the SDATA and SCLK pins of the AD9833 chip are connected.
```C
// Example for SDATA in PB1 pin and SCLK in PB2 pin
#define AD9833_SDATA_Port       GPIOB        // Port of SPI SDATA pin
#define AD9833_SDATA_Pin        GPIO_PIN_1   // SPI SDATA pin
#define AD9833_SCLK_Port        GPIOB        // Port of SPI SCLK pin
#define AD9833_SCLK_Pin         GPIO_PIN_2   // SPI SCLK pin
```
### If you are using Hardware SPI
4. If necessary, correct the definition of the microcontroller's SPI module to which the AD9833 is connected.
```C
#define AD9833_SPI_INSTANCE SPI1
```

---

## How to use
Below are the library functions that can be used to control the AD9833 chip.

```C
//void AD9833_SendData(uint16_t data); // Data transmission (uncomment it in AD9833.h if you want to use it in your program)  
void AD9833_Init(void); // AD9833 initialization
void AD9833_Reset_ON(void);     // Activation of the reset bit
void AD9833_Reset_OFF(void);    // Deactivation of the reset bit
void AD9833_SetFrequency_0(float frequency);    // Setting the frequency value in the FREQ0 register
void AD9833_SetFrequency_1(float frequency);    // Setting the frequency value in the FREQ1 register
void AD9833_SetPhase_0(float phase);    // Setting the phase value in the PHASE0 register
void AD9833_SetPhase_1(float phase);    // Setting the phase value in the PHASE1 register
void AD9833_SelectOutFrequencyRegister(AD9833_FreqReg f_reg);  // Selection of the active frequency register
void AD9833_SelectOutPhaseRegister(AD9833_PhaseReg p_reg);     // Selection of the active phase register
void AD9833_SetWaveform(AD9833_Waveform waveform);      // Waveform selection
void AD9833_SetConfigF0P0(float frequency, float phase, AD9833_Waveform waveform);      // Configuration and selection of the FREQ0 and PHASE0 registers, as well as the waveform 
void AD9833_SetConfigF1P1(float frequency, float phase, AD9833_Waveform waveform);      // Configuration and selection of the FREQ1 and PHASE1 registers, as well as the waveform
```
---

## Example
```C
#include "AD9833.h"

main()
{
    AD9833_Init(void);
    AD9833_SetConfigF0P0(10.0f, 0.0f, AD9833_WAVEFORM_SINE);
    float freq = 10.0f;
    
    while(1)
    {
        AD9833_SetFrequency_0(freq);
        freq *= 5;
        if (freq >= 8000000.0f) // 10 Hz to 8 MHz
            freq = 10.0f;
        HAL_Delay(200);
    }
}
```
