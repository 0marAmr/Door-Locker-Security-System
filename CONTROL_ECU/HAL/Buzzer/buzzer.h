 /******************************************************************************
 *
 * [FILE NAME]:     buzzer.h
 *
 * [AUTHOR]:        Omar Amr
 *
 * [DATE]:          02-11-2022
 *
 * [Description]:   Header file for the buzzer driver
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "../../Utils/std_types.h"

#define BUZZER_PORT_ID		PORTD_ID
#define BUZZER_PIN_ID		PIN2_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void BUZZER_init(void);
void BUZZER_(void);
void BUZZER_stop(void);


#endif /* BUZZER_H_ */
