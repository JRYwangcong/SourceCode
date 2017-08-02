#ifndef     ISR_H
#define     ISR_H

#ifdef _ISR_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

#define ISR_TIMER0_INTERVAL     1 // unit: 1ms
INTERFACE   void ms1MsHandler(void);
INTERFACE   void ISR_R2IRQ_PM_IRQ(void); // R2 IRQ #46, source is PM_IRQ
INTERFACE   void ISR_R2IRQ_PM_FIQ(void); // R2 FIQ #2, source is PM_FIQ
INTERFACE   void ISR_R2IRQ_COMB_IRQ(void);
INTERFACE   void ISR_R2IRQ_SC_L0(void); // R2 IRQ #56 source is Scaler L0
INTERFACE   void ISR_R2IRQ_SC_L1(void); // R2 IRQ #55 source is Scaler L1
INTERFACE   void ISR_R2IRQ_SC_R0(void); // R2 IRQ #54 source is Scaler R0
INTERFACE   void ISR_R2IRQ_SC_R1(void); // R2 IRQ #53 source is Scaler R1
INTERFACE   void IsrModeChangeCombo(BYTE eIPMux);
#endif //#ifndef     ISR_H

