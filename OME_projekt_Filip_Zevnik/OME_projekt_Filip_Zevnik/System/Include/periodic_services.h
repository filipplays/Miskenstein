#ifndef _PERIODIC_SERVICES_H
#define _PERIODIC_SERVICES_H

/*
 * __STATIC_INLINE void LL_TIM_EnableCounter (TIM_TypeDef * TIMx);
 * __STATIC_INLINE void LL_TIM_EnableUpdateEvent (TIM_TypeDef * TIMx);
 * __STATIC_INLINE void LL_TIM_DisableUpdateEvent (TIM_TypeDef * TIMx);
 * __STATIC_INLINE uint32_t LL_TIM_IsEnabledUpdateEvent (TIM_TypeDef * TIMx);
 * __STATIC_INLINE uint32_t LL_TIM_IsActiveUIFCPY (uint32_t Counter);
 *
 *
 */

void PSERV_run_services_Callback(void);
void PSERV_init(void);
void PSERV_enable(void);

#endif /* _PERIODIC_SERVICES_H */
