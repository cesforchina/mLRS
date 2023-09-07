//*******************************************************
// Copyright (c) MLRS project
// GPL3
// https://www.gnu.org/licenses/gpl-3.0.de.html
// OlliW @ www.olliw.eu
//*******************************************************
// hal
//*******************************************************

//-------------------------------------------------------
// TX DIY "easy-to-solder" E77 E22 dual, STM32WLE5CC
//-------------------------------------------------------

//#define DEVICE_HAS_DIVERSITY
#define DEVICE_HAS_JRPIN5
//#define DEVICE_HAS_IN
#define DEVICE_HAS_SERIAL_OR_COM // serial or COM is selected by pressing BUTTON during power on
#define DEVICE_HAS_DEBUG_SWUART
#define DEVICE_HAS_I2C_DISPLAY


//-- Timers, Timing, EEPROM, and such stuff

#define DELAY_USE_DWT

#define SYSTICK_TIMESTEP          1000
#define SYSTICK_DELAY_MS(x)       (uint16_t)(((uint32_t)(x)*(uint32_t)1000)/SYSTICK_TIMESTEP)

#define EE_START_PAGE             120 // 256 kB flash, 2 kB page

#define MICROS_TIMx               TIM16

#define CLOCK_TIMx                TIM2
#define CLOCK_IRQn                TIM2_IRQn
#define CLOCK_IRQHandler          TIM2_IRQHandler
//#define CLOCK_IRQ_PRIORITY        10


//-- UARTS
// UARTB = serial port
// UARTC = COM (CLI)
// UARTD = serial2 BT/ESP port
// UART  = JR bay pin5
// UARTE = in port, SBus or whatever
// UARTF = --
// SWUART= debug port

//#define UARTB_USE_UART2 // serial // PA2,PA3
//#define UARTB_BAUD                TX_SERIAL_BAUDRATE
//#define UARTB_USE_TX
//#define UARTB_TXBUFSIZE           TX_SERIAL_TXBUFSIZE
//#define UARTB_USE_TX_ISR
//#define UARTB_USE_RX
//#define UARTB_RXBUFSIZE           TX_SERIAL_RXBUFSIZE

#define UARTB_USE_UART1_REMAPPED // com USB/CLI // PB6,PB7
#define UARTB_BAUD                TX_SERIAL_BAUDRATE
#define UARTB_USE_TX
#define UARTB_TXBUFSIZE           TX_COM_TXBUFSIZE
#define UARTB_USE_TX_ISR
#define UARTB_USE_RX
#define UARTB_RXBUFSIZE           TX_SERIAL_RXBUFSIZE

//#define UARTE_USE_UART2 // in pin // PA3
//#define UARTE_BAUD                 100000 // SBus normal baud rate, is being set later anyhow
////#define UARTE_USE_TX
////#define UARTE_TXBUFSIZE            256
////#define UARTE_USE_TX_ISR
//#define UARTE_USE_RX
//#define UARTE_RXBUFSIZE            512

#define UART_USE_UART2 // JR pin5, MBridge // PA2,PA3
#define UART_BAUD                 400000
#define UART_USE_TX
#define UART_TXBUFSIZE            512
#define UART_USE_TX_ISR
#define UART_USE_RX
#define UART_RXBUFSIZE            512

//#define JRPIN5_RX_TX_INVERT_INTERNAL
//#define JRPIN5_FULL_INTERNAL

#define SWUART_USE_TIM17 // debug
#define SWUART_TX_IO              IO_PA8
#define SWUART_BAUD               115200
#define SWUART_USE_TX
#define SWUART_TXBUFSIZE          512
//#define SWUART_TIM_IRQ_PRIORITY   11


//-- SX12xx & SPI

#define SPI_USE_SUBGHZSPI

#define SX_BUSY                   0 // busy is provided by subghz, we need to define a dummy to fool sx126x_driver lib

#define SX_RX_EN                  IO_PA7
#define SX_TX_EN                  IO_PA6

#define SX_DIO_EXTI_IRQn              SUBGHZ_Radio_IRQn
#define SX_DIO_EXTI_IRQHandler        SUBGHZ_Radio_IRQHandler
//#define SX_DIO_EXTI_IRQ_PRIORITY    11

#define SX_USE_CRYSTALOSCILLATOR

void sx_init_gpio(void)
{
    gpio_init(SX_TX_EN, IO_MODE_OUTPUT_PP_LOW, IO_SPEED_VERYFAST);
    gpio_init(SX_RX_EN, IO_MODE_OUTPUT_PP_LOW, IO_SPEED_VERYFAST);
}

bool sx_busy_read(void)
{
    return subghz_is_busy();
}

// we need to provide it as we don't have SX_RESET defined, but is empty since reset is done by spi_init()
void sx_reset(void)
{
}

void sx_amp_transmit(void)
{
    gpio_low(SX_RX_EN);
    gpio_high(SX_TX_EN);
}

void sx_amp_receive(void)
{
    gpio_low(SX_TX_EN);
    gpio_high(SX_RX_EN);
}

void sx_dio_init_exti_isroff(void)
{
    // there is no EXTI_LINE_44 interrupt flag
    //LL_EXTI_DisableEvent_32_63(SX_DIO_EXTI_LINE_x);
    //LL_EXTI_DisableIT_32_63(SX_DIO_EXTI_LINE_x);

    NVIC_SetPriority(SX_DIO_EXTI_IRQn, SX_DIO_EXTI_IRQ_PRIORITY);
    //NVIC_EnableIRQ(SX_DIO_EXTI_IRQn);
}

void sx_dio_enable_exti_isr(void)
{
    // there is no EXTI_LINE_44 interrupt flag
    //LL_EXTI_ClearFlag_32_63(SX_DIO_EXTI_LINE_x);
    //LL_EXTI_EnableIT_32_63(SX_DIO_EXTI_LINE_x);

    NVIC_EnableIRQ(SX_DIO_EXTI_IRQn);
}

void sx_dio_exti_isr_clearflag(void)
{
    // there is no EXTI_LINE_44 interrupt flag
}

/*

//-- SX12xx II & SPIB

#define SPIB_USE_SPI1             // PA5, PA11, PA12
#define SPIB_USE_SCK_IO           IO_PA5
#define SPIB_USE_MISO_IO          IO_PA11
#define SPIB_USE_MOSI_IO          IO_PA12
#define SPIB_CS_IO                IO_PB12
#define SPIB_USE_CLK_LOW_1EDGE    // datasheet says CPHA = 0  CPOL = 0
#define SPIB_USE_CLOCKSPEED_9MHZ

#define SX2_RESET                 IO_PB2
#define SX2_DIO1                  IO_PC13
#define SX2_BUSY                  IO_PA15
#define SX2_RX_EN                 IO_PA0
#define SX2_TX_EN                 IO_PB8

#define SX2_DIO1_SYSCFG_EXTI_PORTx    LL_SYSCFG_EXTI_PORTC
#define SX2_DIO1_SYSCFG_EXTI_LINEx    LL_SYSCFG_EXTI_LINE13
#define SX2_DIO_EXTI_LINE_x           LL_EXTI_LINE_13
#define SX2_DIO_EXTI_IRQn             EXTI15_10_IRQn
#define SX2_DIO_EXTI_IRQHandler       EXTI15_10_IRQHandler
//#define SX2_DIO_EXTI_IRQ_PRIORITY   11

void sx2_init_gpio(void)
{
    gpio_init(SX2_RESET, IO_MODE_OUTPUT_PP_HIGH, IO_SPEED_VERYFAST);
    gpio_init(SX2_DIO1, IO_MODE_INPUT_PD, IO_SPEED_VERYFAST);
    gpio_init(SX2_BUSY, IO_MODE_INPUT_PU, IO_SPEED_VERYFAST);
    gpio_init(SX2_TX_EN, IO_MODE_OUTPUT_PP_LOW, IO_SPEED_VERYFAST);
    gpio_init(SX2_RX_EN, IO_MODE_OUTPUT_PP_LOW, IO_SPEED_VERYFAST);
}

bool sx2_busy_read(void)
{
    return (gpio_read_activehigh(SX2_BUSY)) ? true : false;
}

void sx2_amp_transmit(void)
{
    gpio_low(SX2_RX_EN);
    gpio_high(SX2_TX_EN);
}

void sx2_amp_receive(void)
{
    gpio_low(SX2_TX_EN);
    gpio_high(SX2_RX_EN);
}

void sx2_dio_init_exti_isroff(void)
{
    LL_SYSCFG_SetEXTISource(SX2_DIO1_SYSCFG_EXTI_PORTx, SX2_DIO1_SYSCFG_EXTI_LINEx);

    // let's not use LL_EXTI_Init(), but let's do it by hand, is easier to allow enabling isr later
    LL_EXTI_DisableEvent_0_31(SX2_DIO_EXTI_LINE_x);
    LL_EXTI_DisableIT_0_31(SX2_DIO_EXTI_LINE_x);
    LL_EXTI_DisableFallingTrig_0_31(SX2_DIO_EXTI_LINE_x);
    LL_EXTI_EnableRisingTrig_0_31(SX2_DIO_EXTI_LINE_x);

    NVIC_SetPriority(SX2_DIO_EXTI_IRQn, SX2_DIO_EXTI_IRQ_PRIORITY);
    NVIC_EnableIRQ(SX2_DIO_EXTI_IRQn);
}

void sx2_dio_enable_exti_isr(void)
{
    LL_EXTI_ClearFlag_0_31(SX2_DIO_EXTI_LINE_x);
    LL_EXTI_EnableIT_0_31(SX2_DIO_EXTI_LINE_x);
}

void sx2_dio_exti_isr_clearflag(void)
{
    LL_EXTI_ClearFlag_0_31(SX2_DIO_EXTI_LINE_x);
}

*/

//-- In port
// UART_UARTx = USART2

void in_init_gpio(void)
{
}

void in_set_normal(void)
{
    LL_USART_Disable(USART2);
    LL_USART_SetRXPinLevel(USART2, LL_USART_RXPIN_LEVEL_STANDARD);
    LL_USART_Enable(USART2);
}

void in_set_inverted(void)
{
    LL_USART_Disable(USART2);
    LL_USART_SetRXPinLevel(USART2, LL_USART_RXPIN_LEVEL_INVERTED);
    LL_USART_Enable(USART2);
}


//-- Button

#define BUTTON                    IO_PB3

void button_init(void)
{
    gpio_init(BUTTON, IO_MODE_INPUT_PU, IO_SPEED_DEFAULT);
}

bool button_pressed(void)
{
    return gpio_read_activelow(BUTTON);
}


//-- LEDs

#define LED_GREEN                 IO_PB4
#define LED_RED                   IO_PB5

void leds_init(void)
{
    gpio_init(LED_GREEN, IO_MODE_OUTPUT_PP_LOW, IO_SPEED_DEFAULT);
    gpio_init(LED_RED, IO_MODE_OUTPUT_PP_LOW, IO_SPEED_DEFAULT);
}

void led_green_off(void) { gpio_low(LED_GREEN); }
void led_green_on(void) { gpio_high(LED_GREEN); }
void led_green_toggle(void) { gpio_toggle(LED_GREEN); }

void led_red_off(void) { gpio_low(LED_RED); }
void led_red_on(void) { gpio_high(LED_RED); }
void led_red_toggle(void) { gpio_toggle(LED_RED); }



//-- Position Switch

void pos_switch_init(void)
{
}

uint8_t pos_switch_read(void)
{
    return 0;
}



//-- 5 Way Switch

#define FIVEWAY_SWITCH_CENTER     IO_PC13 // POS_3
#define FIVEWAY_SWITCH_UP         IO_PA15 // A = POS_2
#define FIVEWAY_SWITCH_DOWN       IO_PB0 // D = POS_5
#define FIVEWAY_SWITCH_LEFT       IO_PB2 // C = POS_4
#define FIVEWAY_SWITCH_RIGHT      IO_PB12 // B = POS_1

void fiveway_init(void)
{
    gpio_init(FIVEWAY_SWITCH_CENTER, IO_MODE_INPUT_PU, IO_SPEED_DEFAULT);
    gpio_init(FIVEWAY_SWITCH_UP, IO_MODE_INPUT_PU, IO_SPEED_DEFAULT);
    gpio_init(FIVEWAY_SWITCH_DOWN, IO_MODE_INPUT_PU, IO_SPEED_DEFAULT);
    gpio_init(FIVEWAY_SWITCH_LEFT, IO_MODE_INPUT_PU, IO_SPEED_DEFAULT);
    gpio_init(FIVEWAY_SWITCH_RIGHT, IO_MODE_INPUT_PU, IO_SPEED_DEFAULT);
}

uint8_t fiveway_read(void)
{
    return ((uint8_t)gpio_read_activelow(FIVEWAY_SWITCH_UP) << KEY_UP) +
           ((uint8_t)gpio_read_activelow(FIVEWAY_SWITCH_DOWN) << KEY_DOWN) +
           ((uint8_t)gpio_read_activelow(FIVEWAY_SWITCH_LEFT) << KEY_LEFT) +
           ((uint8_t)gpio_read_activelow(FIVEWAY_SWITCH_RIGHT) << KEY_RIGHT) +
           ((uint8_t)gpio_read_activelow(FIVEWAY_SWITCH_CENTER) << KEY_CENTER);
}

/*

//-- 5 Way Switch
// PC2: resistor chain Vcc - 4.7k - down - 1k - left - 2.2k - right - 4.7k - up
// PC13: center

#define FIVEWAY_SWITCH_CENTER     IO_PC13
#define FIVEWAY_ADCx              ADC2 // could also be ADC1
#define FIVEWAY_ADC_IO            IO_PC2 // ADC12_IN8
#define FIVEWAY_ADC_CHANNELx      LL_ADC_CHANNEL_8

extern "C" { void delay_us(uint32_t us); }

void fiveway_init(void)
{
    gpio_init(FIVEWAY_SWITCH_CENTER, IO_MODE_INPUT_PU, IO_SPEED_DEFAULT);
    LL_RCC_SetADCClockSource(LL_RCC_ADC12_CLKSOURCE_SYSCLK);
    rcc_init_afio();
    rcc_init_adc(FIVEWAY_ADCx);
    adc_init_one_channel(FIVEWAY_ADCx);
    adc_config_channel(FIVEWAY_ADCx, LL_ADC_REG_RANK_1, FIVEWAY_ADC_CHANNELx, FIVEWAY_ADC_IO);
    adc_enable(FIVEWAY_ADCx);
    delay_us(100);
    adc_start_conversion(FIVEWAY_ADCx);
}

uint16_t fiveway_adc_read(void)
{
    return LL_ADC_REG_ReadConversionData12(FIVEWAY_ADCx);
}

uint8_t fiveway_read(void)
{
    uint8_t center_pressed = gpio_read_activelow(FIVEWAY_SWITCH_CENTER);
    uint16_t adc = LL_ADC_REG_ReadConversionData12(FIVEWAY_ADCx);
    if (adc < (0+200)) return (1 << KEY_DOWN); // 0
    if (adc > (655-200) && adc < (655+200)) return (1 << KEY_LEFT); // 655
    if (adc > (1595-200) && adc < (1595+200)) return (1 << KEY_RIGHT); // 1595
    if (adc > (2505-200) && adc < (2505+200)) return (1 << KEY_UP); // 2505
    return (center_pressed << KEY_CENTER);
}

*/

//-- Serial or Com Switch
// use COM if BUTTON is pressed during power up
// BUTTON becomes bind button later on

bool E77_ser_or_com_serial = true; // we use serial as default

void ser_or_com_init(void)
{
  gpio_init(BUTTON, IO_MODE_INPUT_PU, IO_SPEED_DEFAULT);
  uint8_t cnt = 0;
  for (uint8_t i = 0; i < 16; i++) {
    if (gpio_read_activelow(BUTTON)) cnt++;
  }
  E77_ser_or_com_serial = !(cnt > 8);
}

bool ser_or_com_serial(void)
{
  return E77_ser_or_com_serial;
}


//-- Display I2C

//#define I2C_USE_I2C2              // PA11, PA12
//#define I2C_CLOCKSPEED_400KHZ     // not all displays seem to work well with I2C_CLOCKSPEED_1000KHZ
//#define I2C_USE_DMAMODE

#define I2C_USE_I2C1              // PA9, PA10
#define I2C_CLOCKSPEED_400KHZ     // not all displays seem to work well with I2C_CLOCKSPEED_1000KHZ
#define I2C_USE_DMAMODE

/*

//-- Buzzer
// Buzzer is active high // TODO: needs pin and AF check! do not use

#define BUZZER                    IO_PB9XXX
#define BUZZER_IO_AF              IO_AF_12
#define BUZZER_TIMx               TIM1
#define BUZZER_IRQn               TIM1_UP_IRQn
#define BUZZER_IRQHandler         TIM1_UP_IRQHandler
#define BUZZER_TIM_CHANNEL        LL_TIM_CHANNEL_CH3N
//#define BUZZER_TIM_IRQ_PRIORITY   14

*/

//-- POWER

#define POWER_GAIN_DBM            0 // gain of a PA stage if present
#define POWER_SX126X_MAX_DBM      SX126X_POWER_MAX // maximum allowed sx power
#define POWER_USE_DEFAULT_RFPOWER_CALC

#define RFPOWER_DEFAULT           2 // index into rfpower_list array

const rfpower_t rfpower_list[] = {
    { .dbm = POWER_MIN, .mW = INT8_MIN },
    { .dbm = POWER_0_DBM, .mW = 1 },
    { .dbm = POWER_10_DBM, .mW = 10 },
    { .dbm = POWER_20_DBM, .mW = 100 },
    { .dbm = POWER_22_DBM, .mW = 158 },
};


//-- TEST

uint32_t porta[] = {
    LL_GPIO_PIN_0, LL_GPIO_PIN_1, LL_GPIO_PIN_2, LL_GPIO_PIN_3, LL_GPIO_PIN_5,
    LL_GPIO_PIN_9, LL_GPIO_PIN_10, LL_GPIO_PIN_11, LL_GPIO_PIN_12,
    LL_GPIO_PIN_15,
};

uint32_t portb[] = {
    LL_GPIO_PIN_2, LL_GPIO_PIN_3, LL_GPIO_PIN_4, LL_GPIO_PIN_6, LL_GPIO_PIN_7,
    LL_GPIO_PIN_8,
    LL_GPIO_PIN_12,
};

uint32_t portc[] = {
    //LL_GPIO_PIN_13,
};

