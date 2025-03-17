/*************************************************************************************
 Title:         Analog Devices AD9833 DDS Wave Generator Library for STM32 Using Software or Hardware (LL) SPI
 Author:        dkobrusev
 Software:      IAR Embedded Workbench for ARM
 Hardware:      Any STM32 device
*************************************************************************************/
#include "AD9833.h"

// ------------------- Variables ----------------

// Частота опорного генератора
const float AD9833_freq_ref = 8000000.0f; // Частота опорного генератора в Гц

// Union значения Control Register и структуры с его битами
union
{
  uint16_t value;
  struct
  {
    uint8_t Rsrv0 : 1;          // D0
    uint8_t Mode : 1;           // D1
    uint8_t Rsrv1 : 1;          // D2
    uint8_t DIV2 : 1;           // D3
    uint8_t Rsrv2 : 1;          // D4
    uint8_t OPBITEN : 1;        // D5
    uint8_t SLEEP12 : 1;        // D6
    uint8_t SLEEP1 : 1;         // D7
    uint8_t Reset : 1;          // D8
    uint8_t Rsrv3 : 1;          // D9
    uint8_t PSELECT : 1;        // D10
    uint8_t FSELECT : 1;        // D11
    uint8_t HLB : 1;            // D12
    uint8_t B28 : 1;            // D13
  } bits;
} AD9833_CR = {.value = AD9833_CR_DEF_VALUE};

// -------------------------------- Functions --------------------------------

/* Функция отправки данных */
void AD9833_SendData(uint16_t data)
{ 
  // Активируем устройство
  AD9833_CS_SELECT();
  
#ifdef AD9833_USE_SOFTWARE_SPI
  // Software SPI
  for (uint8_t i = 0; i < 16 ; i++) {  
    
    if(data & 0x8000) // Установка первого бита (MSB)
      HAL_GPIO_WritePin(AD9833_SDATA_Port, AD9833_SDATA_Pin, GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(AD9833_SDATA_Port, AD9833_SDATA_Pin, GPIO_PIN_RESET);
    
    ASM_NOP(); // Задержка
    HAL_GPIO_WritePin(AD9833_SCLK_Port, AD9833_SCLK_Pin, GPIO_PIN_RESET); // Считывание данных по срезу CLK
    ASM_NOP(); // Задержка
    HAL_GPIO_WritePin(AD9833_SCLK_Port,AD9833_SCLK_Pin, GPIO_PIN_SET); // Фронт CLK
    data <<= 1; // Сдвиг данных на 1 бит (следующий бит)
  }
  
  ASM_NOP(); // Задержка
#else
  // Hardware SPI
  // Отправляем 1-е 8-битное слово
  while (!LL_SPI_IsActiveFlag_TXE(AD9833_SPI_INSTANCE));
  LL_SPI_TransmitData8(AD9833_SPI_INSTANCE, data >> 8);
  
  // Отправляем 2-е 8-битное слово
  while (!LL_SPI_IsActiveFlag_TXE(AD9833_SPI_INSTANCE));
  LL_SPI_TransmitData8(AD9833_SPI_INSTANCE, data & 0xFF);
  
  // Ждем завершения передачи
  while (LL_SPI_IsActiveFlag_BSY(AD9833_SPI_INSTANCE));  
#endif
  
  // Деактивируем устройство
  AD9833_CS_UNSELECT();
}

/* Функция инициализации */
void AD9833_Init(void)
{
  AD9833_CS_UNSELECT();
  
#ifdef AD9833_USE_SOFTWARE_SPI
  // Software SPI
  // Set All SPI pings to High
  HAL_GPIO_WritePin(AD9833_SDATA_Port, AD9833_SDATA_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(AD9833_SCLK_Port, AD9833_SCLK_Pin, GPIO_PIN_SET);
#else
  // Hardware SPI
  LL_SPI_Enable(AD9833_SPI_INSTANCE);
  while ((AD9833_SPI_INSTANCE->SR & SPI_SR_BSY)!= 0);
#endif
  
  AD9833_SetConfigF0P0(137.0f, 0.0f, AD9833_WAVEFORM_SINE); // FREQ0, PHASE0, 137 Hz, 0 rad, Sin
  AD9833_SetFrequency_1(4000.0f); // FREQ1, 4000 Hz
  AD9833_SetPhase_1(0.0f); // PHASE1, 0 rad
}

/* Функция активации бита сброса */
void AD9833_Reset_ON(void)
{
  AD9833_CR.bits.Reset = 1;
  AD9833_SendData(AD9833_CONTROL_REG | AD9833_CR.value);
}

/* Функция деактивации бита сброса */
void AD9833_Reset_OFF(void)
{
  AD9833_CR.bits.Reset = 0;
  AD9833_SendData(AD9833_CONTROL_REG | AD9833_CR.value);
}

/* Функция установки частоты в регистр FREQ0 */
void AD9833_SetFrequency_0(float frequency)
{
  // Расчет 28-битного слова частоты
  uint32_t freq_word = (uint32_t)((frequency * (1 << 28)) / AD9833_freq_ref);
  uint16_t freq_msb = (freq_word >> 14) & 0x3FFF;
  uint16_t freq_lsb = freq_word & 0x3FFF;
  
  // Установка Control Register для загрузки 28-битного значения частоты
  AD9833_CR.bits.B28 = 1;
  AD9833_SendData(AD9833_CONTROL_REG | AD9833_CR.value);
  
  // Установка младших 14 бит частоты
  AD9833_SendData(AD9833_FREQ_REG_0 | freq_lsb);
  
  // Установка старших 14 бит частоты
  AD9833_SendData(AD9833_FREQ_REG_0 | freq_msb);
}

/* Функция установки частоты в регистр FREQ1 */
void AD9833_SetFrequency_1(float frequency)
{
  // Расчет 28-битного слова частоты
  uint32_t freq_word = (uint32_t)((frequency * (1 << 28)) / AD9833_freq_ref);
  uint16_t freq_msb = (freq_word >> 14) & 0x3FFF;
  uint16_t freq_lsb = freq_word & 0x3FFF;
  
  // Установка Control Register для загрузки 28-битного значения частоты
  AD9833_CR.bits.B28 = 1;
  AD9833_SendData(AD9833_CONTROL_REG | AD9833_CR.value);
  
  // Установка младших 14 бит частоты
  AD9833_SendData(AD9833_FREQ_REG_1 | freq_lsb);
  
  // Установка старших 14 бит частоты
  AD9833_SendData(AD9833_FREQ_REG_1 | freq_msb);
}

/* Функция установки фазы в регистр PHASE0 */
void AD9833_SetPhase_0(float phase)
{
  uint16_t phase_data = ((uint16_t)(phase * 651.9f) & 0xFFF); // 4096/(2*3.14159) = 651.89919, 0xFFF - 12 bit
  
  // Установка Phase Register 0
  AD9833_SendData(AD9833_PHASE_REG_0 | phase_data);
}

/* Функция установки фазы в регистр PHASE1 */
void AD9833_SetPhase_1(float phase)
{
  uint16_t phase_data = ((uint16_t)(phase * 651.9f) & 0xFFF); // 4096/(2*3.14159) = 651.89919, 0xFFF - 12 bit
  
  // Установка Phase Register 1
  AD9833_SendData(AD9833_PHASE_REG_1 | phase_data);
}

/* Функция выбора рабочего регистра частоты */
void AD9833_SelectOutFrequencyRegister(AD9833_FreqReg f_reg)
{
  // Установка бита FSELECT в Control Register
  if (f_reg == AD9833_FREQ0)
  {
    // FREQ0
    AD9833_CR.bits.FSELECT = 0;
  }
  else
  {
    // FREQ1
    AD9833_CR.bits.FSELECT = 1;
  }
  AD9833_SendData(AD9833_CONTROL_REG | AD9833_CR.value);
}

/* Функция выбора рабочего регистра фазы */
void AD9833_SelectOutPhaseRegister(AD9833_PhaseReg p_reg)
{
  // Установка бита FSELECT в Control Register
  if (p_reg == AD9833_PHASE0)
  {
    // PHASE0
    AD9833_CR.bits.PSELECT= 0;
  }
  else
  {
    // PHASE1
    AD9833_CR.bits.PSELECT = 1;
  }
  AD9833_SendData(AD9833_CONTROL_REG | AD9833_CR.value);
}

/* Установка формы сигнала */
void AD9833_SetWaveform(AD9833_Waveform waveform)
{
  switch (waveform)
  {
  case AD9833_WAVEFORM_SINE:
    AD9833_CR.bits.OPBITEN = 0;
    AD9833_CR.bits.Mode = 0;
    break;
    
  case AD9833_WAVEFORM_TRIANGLE:
    AD9833_CR.bits.OPBITEN = 0;
    AD9833_CR.bits.Mode = 1;
    break;
    
  case AD9833_WAVEFORM_SQUARE:
    AD9833_CR.bits.OPBITEN = 1;
    AD9833_CR.bits.Mode = 0;
    AD9833_CR.bits.DIV2 = 1;
    break;
    
  case AD9833_WAVEFORM_SQUARE_HALF:
    AD9833_CR.bits.OPBITEN = 1;
    AD9833_CR.bits.Mode = 0;
    AD9833_CR.bits.DIV2 = 0;
    break;
  }
  AD9833_SendData(AD9833_CONTROL_REG | AD9833_CR.value);
}

/**
  * @brief  Set the output configuration of the AD9833 chip for FREQ0 and PHASE0.
  *
  * @note   This function uses the functions described above to configure and use the FREQ0 and PHASE0 registers of the AD9833 chip..
  *
  * @param  frequency - Frequency in Hz (0..AD9833_freq_ref).
  *
  * @param  phase - Phase in radians (0..2pi).
  *
  * @param  waveform - output signal shape.
  *          This parameter can be one of the AD9833_Waveform enum values:
  *            @arg AD9833_WAVEFORM_SINE: sinusoidal shape
  *            @arg AD9833_WAVEFORM_TRIANGLE: triangular shape
  *            @arg AD9833_WAVEFORM_SQUARE: square shape (DAC data MSB)
  *            @arg AD9833_WAVEFORM_SQUARE_HALF: square shape (DAC data MSB/2)
  */

void AD9833_SetConfigF0P0(float frequency, float phase, AD9833_Waveform waveform)
{
  // Reset AD9833
  AD9833_Reset_ON();
  // Select FREQ0 and PHASE0 in CR
  AD9833_CR.bits.FSELECT = 0;
  AD9833_CR.bits.PSELECT = 0;
  // Set Freq
  AD9833_SetFrequency_0(frequency);
  // Set Phase
  AD9833_SetPhase_0(phase);
  // Set Waweform
  AD9833_SetWaveform(waveform);
  // Disable Reset AD9833
  AD9833_Reset_OFF();
}

/**
  * @brief  Set the output configuration of the AD9833 chip for FREQ1 and PHASE1.
  *
  * @note   This function uses the functions described above to configure and use the FREQ1 and PHASE1 registers of the AD9833 chip..
  *
  * @param  frequency - Frequency in Hz (0..AD9833_freq_ref).
  *
  * @param  phase - Phase in radians (0..2pi).
  *
  * @param  waveform - output signal shape.
  *          This parameter can be one of the AD9833_Waveform enum values:
  *            @arg AD9833_WAVEFORM_SINE: sinusoidal shape
  *            @arg AD9833_WAVEFORM_TRIANGLE: triangular shape
  *            @arg AD9833_WAVEFORM_SQUARE: square shape (DAC data MSB)
  *            @arg AD9833_WAVEFORM_SQUARE_HALF: square shape (DAC data MSB/2)
  */

void AD9833_SetConfigF1P1(float frequency, float phase, AD9833_Waveform waveform)
{
  // Reset AD9833
  AD9833_Reset_ON();
  // Select FREQ1 and PHASE1 in CR
  AD9833_CR.bits.FSELECT = 1;
  AD9833_CR.bits.PSELECT = 1;
  // Set Freq
  AD9833_SetFrequency_1(frequency);
  // Set Phase
  AD9833_SetPhase_1(phase);
  // Set Waweform
  AD9833_SetWaveform(waveform);
  // Disable Reset AD9833
  AD9833_Reset_OFF();
}