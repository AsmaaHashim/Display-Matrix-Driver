/*
 * display_matrix.h
 *
 *  Created on: Oct 22, 2019
 *  Author: Asmaa Hashim
 */

#ifndef DISPLAY_MATRIX_H_
#define DISPLAY_MATRIX_H_

#include "HW.h"

/*This constant defines the maximum number of sevSegment user can use*/

#define MAX_NUM_OF_MATRIX 2


/* this enum show type of led matrix */

typedef enum {
	           ROW_ANODE_COLUM_CATHODE , 
	           ROW_CATHODE_COLUM_ANODE
	
	         }display_matrix_type;

/* this enum show error type */

typedef enum {
				invalid_port,
				invalid_type,
				invalid_object,
				invalid_draw,
				good

             }display_matrix_error_type;


/* this enum define the shapes that willbe displayed on led matrix */

typedef enum
            {
				LOVE ,
				SMILE,
				FLOWER
			
			}shape_name;


/*this function is used to intialize the display matrix */

display_matrix_error_type display_matrix_inti
(GPIO_port_type row_port , GPIO_port_type colum_port ,display_matrix_type type , unint8* object);

/* this function is used to display char*/

display_matrix_error_type  display_char( unint8 object , unint8 ch , unint8 t);

/*this function is used to display  shapes*/

display_matrix_error_type display_shape( unint8 object , shape_name shape);






















#endif /* DISPLAY_MATRIX_H_ */
