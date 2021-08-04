/*
 * HW.h
 *
 *  Created on: Oct 22, 2019
 *  Author: Asmaa Hashim
 */

#ifndef HW_H_
#define HW_H_


#include "data_types.h"

#define NULL (void*)0

#define DIR_OFFSET 1

#define DATA_OFFSET 2


  /***** this enum is used to define base address of  ports ******/


typedef enum {PORT_A=0x39 , PORT_B=0x36 , PORT_C=0x33  , PORT_D=0x30  }GPIO_port_type;





#endif /* HW_H_ */
