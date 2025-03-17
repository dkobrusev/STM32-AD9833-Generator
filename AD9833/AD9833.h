/*************************************************************************************
 Title:         Analog Devices AD9833 DDS Wave Generator Library for STM32 Using Software or Hardware (LL) SPI
 Author:        dkobrusev
 Software:      IAR Embedded Workbench for ARM
 Hardware:      Any STM32 device

 //EU//
 You can use Software or Hardware SPI
 The Software SPI has been tested at HCLK = 100 MHz and STMF411CEU6
 The hardware SPI uses the LL library and is tested at 12.5 MBits/s with the following SPI settings:
 TransferDirection      FULL_DUPLEX
 Mode                   MASTER
 DataWidth              8BIT
 ClockPolarity          HIGH
 ClockPhase             1EDGE
 NSS                    NSS_SOFT
 BaudRate               BAUDRATEPRESCALER_DIV8
 BitOrder               MSB_FIRST

 To use the software SPI, uncomment the line "#define AD9833_USE_SOFTWARE_SPI" below.
 It affects the functions AD9833_SendData and AD9833_Init

 //RU//
 Вы можете использовать программный или аппаратный SPI
 Программный SPI был протестирован на частоте HCLK = 100 МГц
 Аппаратный SPI использует библиотеку LL и протестирован на скорости 12.5 MBits/s со следующими настройками SPI:
 TransferDirection      FULL_DUPLEX
 Mode                   MASTER
 DataWidth              8BIT
 ClockPolarity          HIGH
 ClockPhase             1EDGE
 NSS                    NSS_SOFT
 BaudRate               BAUDRATEPRESCALER_DIV8
 BitOrder               MSB_FIRST

 Чтобы использовать программный SPI раскомментируйте строчку "#define AD9833_USE_SOFTWARE_SPI" ниже.
 Она влияет на функции AD9833_SendData и AD9833_Init
*************************************************************************************/

/*************************************************************************************
                                     Registers
 DB15 и DB14 :
 00: Control Register
 01: Frequency Register 0
 10: Frequency Register 1
 11: Phase Register 0 или Phase Register 1 (в зависимости от DB13)
 DB13 :
 При DB15 = 1 и DB14 = 1:
 DB13 = 0: Phase Register 0
 DB13 = 1: Phase Register 1

*************************************************************************************/

#ifndef __AD9833_H
#define __AD9833_H
#include "main.h"

// ------------------------- Defines -------------------------

// CS Pin
#define AD9833_CS_Port          GEN_SPI_CS_GPIO_Port    // Port of SPI CS pin
#define AD9833_CS_Pin           GEN_SPI_CS_Pin          // SPI CS pin

// Hw or Sw SPI

//#define AD9833_USE_SOFTWARE_SPI

#ifdef AD9833_USE_SOFTWARE_SPI
// Software SPI
#define AD9833_SDATA_Port       GEN_SPI_MOSI_GPIO_Port  // Port of SPI SDATA pin
#define AD9833_SDATA_Pin        GEN_SPI_MOSI_Pin        // SPI SDATA pin
#define AD9833_SCLK_Port        GEN_SPI_SCK_GPIO_Port   // Port of SPI SCLK pin
#define AD9833_SCLK_Pin         GEN_SPI_SCK_Pin         // SPI SCLK pin

#define ASM_NOP()               asm("NOP")              // Assembly NOPE (Little Delay)
//
#else
// Hardware SPI
#define AD9833_SPI_INSTANCE SPI1
//
#endif

// Макрос для CS
#define AD9833_CS_SELECT()      HAL_GPIO_WritePin(GEN_SPI_CS_GPIO_Port, GEN_SPI_CS_Pin, GPIO_PIN_RESET)
#define AD9833_CS_UNSELECT()    HAL_GPIO_WritePin(GEN_SPI_CS_GPIO_Port, GEN_SPI_CS_Pin, GPIO_PIN_SET)

#define AD9833_CR_DEF_VALUE     0x2000 // FREQ0, PHASE0, SIN

// Register addresses
#define AD9833_CONTROL_REG      0x0000  // Control Register: DB15=0, DB14=0
#define AD9833_FREQ_REG_0       0x4000  // Frequency Register 0: DB15=0, DB14=1
#define AD9833_FREQ_REG_1       0x8000  // Frequency Register 1: DB15=1, DB14=0
#define AD9833_PHASE_REG_0      0xC000  // Phase Register 0: DB15=1, DB14=1, DB13=0
#define AD9833_PHASE_REG_1      0xE000  // Phase Register 1: DB15=1, DB14=1, DB13=1

// ------------------------- Enums -------------------------

// Output waveform
typedef enum
{
  AD9833_WAVEFORM_SINE = 0,             // Синусоидальная форма
  AD9833_WAVEFORM_TRIANGLE = 1,         // Треугольная форма
  AD9833_WAVEFORM_SQUARE = 2,           // Квадратная форма Vout = MSB of the DAC
  AD9833_WAVEFORM_SQUARE_HALF = 3       // Квадратная форма /2 Vout = MSB/2 of the DAC
} AD9833_Waveform;

// Freq Registers
typedef enum
{
  AD9833_FREQ0 = 0,
  AD9833_FREQ1 = 1
} AD9833_FreqReg;

// Phase Registers
typedef enum
{
  AD9833_PHASE0 = 0,
  AD9833_PHASE1 = 1
} AD9833_PhaseReg;

// ------------------------- Functions -------------------------

//void AD9833_SendData(uint16_t data);
void AD9833_Init(void); // Инициализация AD9833
void AD9833_Reset_ON(void);     // Активации бита сброса
void AD9833_Reset_OFF(void);    // Деактивации бита сброса
void AD9833_SetFrequency_0(float frequency);    // Установка значения частоты в FREQ0
void AD9833_SetFrequency_1(float frequency);    // Установка значения частоты в FREQ1
void AD9833_SetPhase_0(float phase);    // Установка значения фазы в PHASE0
void AD9833_SetPhase_1(float phase);    // Установка значения фазы в PHASE1
void AD9833_SelectOutFrequencyRegister(AD9833_FreqReg f_reg);  // Выбор рабочего регистра частоты
void AD9833_SelectOutPhaseRegister(AD9833_PhaseReg p_reg);      // Выбор рабочего регистра фазы
void AD9833_SetWaveform(AD9833_Waveform waveform);      // Выбор формы сигнала
void AD9833_SetConfigF0P0(float frequency, float phase, AD9833_Waveform waveform);      // Установка конфигурации для FREQ0 и PHASE0
void AD9833_SetConfigF1P1(float frequency, float phase, AD9833_Waveform waveform);      // Установка конфигурации для FREQ1 и PHASE1

#endif /* __AD9833_H */
