#define LED_GREEN  12
#define LED_ORANGE 13
#define LED_RED    14
#define LED_BLUE   15
#define DELAY_COUNT_1MS 		 1250U
#define DELAY_COUNT_1S  		(1000U * DELAY_COUNT_1MS)

#include <inttypes.h>

void delay(uint32_t count);
void led_init_all(void);
void led_on(uint8_t led_no);
void led_off(uint8_t led_no);

uint8_t gLightFlag = 3;
uint8_t gLightFlag2 = 3;
uint8_t gLightFlag3 = 3;
uint8_t gLightFlag4 = 3;
uint8_t gLightFlag5 = 3;

int 
main () 
{
  led_init_all();
  while(1) {
    led_on(LED_GREEN);
    delay(DELAY_COUNT_1S);
    led_off(LED_GREEN);
    delay(DELAY_COUNT_1S);
  }

  return 0;
}

void delay(uint32_t count)
{
  for(uint32_t i = 0 ; i < count ; i++);
}

void led_init_all(void)
{

	uint32_t *pRccAhb1enr = (uint32_t*)0x40023830;
	uint32_t *pGpiodModeReg = (uint32_t*)0x40020C00;


	*pRccAhb1enr |= ( 1 << 3);
	//configure LED_GREEN
	*pGpiodModeReg |= ( 1 << (2 * LED_GREEN));
	*pGpiodModeReg |= ( 1 << (2 * LED_ORANGE));
	*pGpiodModeReg |= ( 1 << (2 * LED_RED));
	*pGpiodModeReg |= ( 1 << (2 * LED_BLUE));

  led_off(LED_GREEN);
  led_off(LED_ORANGE);
  led_off(LED_RED);
  led_off(LED_BLUE);
}

void led_on(uint8_t led_no)
{
  uint32_t *pGpiodDataReg = (uint32_t*)0x40020C14;
  *pGpiodDataReg |= ( 1 << led_no);
}

void led_off(uint8_t led_no)
{
  uint32_t *pGpiodDataReg = (uint32_t*)0x40020C14;
  *pGpiodDataReg &= ~( 1 << led_no);
}
