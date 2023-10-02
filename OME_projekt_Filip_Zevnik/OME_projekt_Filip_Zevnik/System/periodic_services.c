#include "stm32g4xx_ll_tim.h"
#include "periodic_services.h"
#include "kbd.h"

typedef struct
{
	TIM_TypeDef* TIM;
} periodic_services_handle_t;

static periodic_services_handle_t periodic_services;

void PSERV_init(void)
{
	periodic_services.TIM = TIM6;
	LL_TIM_EnableCounter(TIM6);
}

void PSERV_enable(void)
{
	LL_TIM_EnableUpdateEvent(periodic_services.TIM);
}

void PSERV_disable(void)
{
	LL_TIM_DisableUpdateEvent(periodic_services.TIM);
}

void PSERV_run_services_Callback(void)
{
	KBD_demo_toggle_LEDs_if_buttons_pressed();
}
