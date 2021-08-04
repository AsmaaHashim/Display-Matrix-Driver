/*
 * main.c
 *
 *  Created on: Oct 22, 2019
 *      Author: Asmaa Hashim
 */


#include "display_matrix.h"

#include <util/delay.h>


unint8 matrix ;

int main(void)
{

	display_matrix_inti(PORT_C , PORT_A , ROW_ANODE_COLUM_CATHODE , &matrix);


	while(1)
	{

		display_char(matrix , 'A' , 1);

		_delay_ms(1000);

		display_shape(matrix , LOVE);

		_delay_ms(1000);

}
}
