/*
 * display_matrix.c
 *
 *  Created on: Oct 22, 2019
 *  Author: Asmaa Hashim
 */


#include "display_matrix.h"

#include <avr/io.h>

#include "util/delay.h"

/******************************************************************************************
 * this macro is used to increment the counter if the counter is incremented 4 times this means
 * that the timer count 1s
**********************************************************************************************/

#define COUNT_TIME(REG , BIT)\
	if(REG & 1<< BIT)\
	{\
		counter++;\
	REG |=(1 << BIT);\
    }
	
	
static  display_matrix_data buffer[MAX_NUM_OF_MATRIX];

/*this array is used to store the  binary code of all characters(a : z) */

static const unint8 char_code[26][8]={ {0b11111111 ,0b11100111 , 0b11011011 , 0b10111101,
		                                0b10000001 , 0b10111101 , 0b10111101 , 0b10111101},

		                               {0b10000111 , 0b10111011,0b10111011 ,0b10000111 ,
				                        0b10111011, 0b10111011 ,0b10000111 , 0b11111111  },

									   {0b11000001 , 0b10000001,0b00011111 , 0b00111111,
									    0b00111111 ,0b000111111 , 0b10000001 , 0b11000001},

									   {0b00000011 , 0b00000001,0b00111001 , 0b00111001,
										0b00111001 , 0b00111001 ,0b00000001 ,0b00000011},

									   {0b00000000 , 0b00000000 , 0b00111111 ,0b00000000 ,
										0b00000000, 0b00111111 , 0b00000000 ,0b00000000 },

									   {0b00000000 , 0b00000000 , 0b00111111, 0b00000000 ,
									    0b00000000, 0b00111111 , 0b00111111 , 0b00111111 },

									   {0b00000000 , 0b00000000 , 0b00111111 ,0b00100000 ,
										0b00100000, 0b00111100 , 0b00000000 ,0b00000000 } 
									};


/*this array is used to store the  binary code of shapes*/

static const unint8 shape_code[3][8]={
										/*this is the binary code to display th word love on matrix*/
	                                   {0b11111111, 0b10011001 , 0b00000000 ,0b00000000 ,  
		                                0b10000001 ,0b11000011 , 0b11100111,0b11111111},
										
									  /*this is the binary code to display th word simple on matrix*/
		                              {0b11000011, 0b10111101 , 0b01011010 ,0b01111110 ,  
		                               0b01011010 ,0b01100110 , 0b10111101,0b11000011},

                                     };


/* this array will b eused to set one row */

static const unint8 row_port[]={1,2,4,8,16,32,64,128}; // this array is  used to set one row

static unint8 handler=0;

/* this counter is used in ISR function to control the time*/

static unint8  counter=0 ;


typedef struct{
	GPIO_port_type row_port ;

	GPIO_port_type colum_port;

	display_matrix_type type;


}display_matrix_data;


  void timer_init(void)
  {

  	TCNT0 =0;  // data register

  	OCR0 =250;  // OUTPUT COMPAR REGISTER

  	/*
  	 * FOC0 --->> non PWM mode
  	 * WGM01 --->> CTC mode
  	 * CS00 & CS02 --->> for prescaler 1024
  	 */

  	TCCR0 =((unint8)1 << FOC0) | ((unint8)1 << WGM01) | ((unint8)1 <<CS00) | ((unint8)1 << CS02);



  }

display_matrix_error_type display_matrix_inti(GPIO_port_type row_port , GPIO_port_type colum_port,display_matrix_type type , unint8* object)
{

	timer_init();

	if((row_port == PORT_A || row_port== PORT_B || row_port == PORT_C || row_port == PORT_D) && (colum_port == PORT_A || colum_port== PORT_B || colum_port == PORT_C || colum_port == PORT_D))
	{
		if(type == ROW_ANODE_COLUM_CATHODE || type == ROW_CATHODE_COLUM_ANODE)
		{
			if(object != NULL)
			{
				buffer[handler].colum_port=colum_port;
				buffer[handler].row_port=row_port;
				buffer[handler].type=type;
				*object=handler;
				handler++;
				*((volatile unint8*)row_port + DIR_OFFSET) = 0XFF;
				*((volatile unint8*)colum_port + DIR_OFFSET) = 0XFF;


			}
			else
			{

				return invalid_object;
			}



		}
		else
		{

			return invalid_type;
		}



	}
	else
	{

		return invalid_port;
	}

	return good;




}

display_matrix_error_type  display_char( unint8 object , unint8 ch , unint8 t)
{

	if(object < handler && object >=0)
	{
		if((ch >='A' && ch <='Z') || (ch >='a' && ch <='z'))
		{

			if(ch >='a' && ch <='z')
			{
				ch -=(unint8)32;
			}

			ch -=(unint8)65;   // so i can get the code of char from array
			                   // example : A ->>>char_code[0]

			unint8  j ;  // counters


			if(buffer[object].type == ROW_ANODE_COLUM_CATHODE)
			{

				*((volatile unint8*)buffer[object].colum_port + DATA_OFFSET ) = 0xff;
				*((volatile unint8*)buffer[object].row_port + DATA_OFFSET)=0x00;

				t*=4;


			while( counter < t )
			{

				for(j=0 ; j<8 ; j++)
				{
					*((volatile unint8*)buffer[object].row_port + DATA_OFFSET) = row_port[j];
					*((volatile unint8*)buffer[object].colum_port + DATA_OFFSET ) = char_code[ch][j];

					_delay_us(200);


				}


				*((volatile unint8*)buffer[object].row_port + DATA_OFFSET)=0x00;

				*((volatile unint8*)buffer[object].colum_port + DATA_OFFSET ) = 0xff;


				COUNT_TIME(TIFR , OCF0); // macro




			}

			}
			else
			{
				*((volatile unint8*)buffer[object].row_port + DATA_OFFSET) = 0xff;

				t*=4;

           while(counter<t)
				{

					for(j=0 ; j<8 ; j++)
					{
						*((volatile unint8*)buffer[object].row_port + DATA_OFFSET) = ~(row_port[j]);

						*((volatile unint8*)buffer[object].colum_port +DATA_OFFSET ) = ~(char_code[ch][j]);

						_delay_us(500);



					}


					*((volatile unint8*)buffer[object].row_port + DATA_OFFSET) = 0xff;


					*((volatile unint8*)buffer[object].colum_port +DATA_OFFSET ) = 0x00;

					COUNT_TIME(TIFR , OCF0);




				}
			}

			counter=0;



		}
		else
		{
			return invalid_draw;
		}


	}
	else
	{

		return invalid_object;
	}
	return good;



}

display_matrix_error_type display_shape( unint8 object , shape_name shape)
{

	if(object < handler && object >=0)
	{
		if((shape==LOVE ) || (shape == SMILE) || (shape==FLOWER))
		{



			unint8  j , i;  // counters


			if(buffer[object].type == ROW_ANODE_COLUM_CATHODE)
			{

				*((volatile unint8*)buffer[object].colum_port + DATA_OFFSET ) = 0xff;
				*((volatile unint8*)buffer[object].row_port + DATA_OFFSET)=0x00;



			for(i=0 ; i<100 ; i++) //execute 100 times yet the eye can catch
			{

				for(j=0 ; j<8 ; j++)
				{
					*((volatile unint8*)buffer[object].row_port + DATA_OFFSET) = row_port[j];
					*((volatile unint8*)buffer[object].colum_port + DATA_OFFSET ) = shape_code[shape][j];

					_delay_us(500);


				}


				*((volatile unint8*)buffer[object].row_port + DATA_OFFSET)=0x00;

				*((volatile unint8*)buffer[object].colum_port + DATA_OFFSET ) = 0xff;




			}

			}
			else
			{
				*((volatile unint8*)buffer[object].row_port + DATA_OFFSET) = 0xff;



				for(i=0 ;i<100  ; i++)
				{

					for(j=0 ; j<8 ; j++)
					{
						*((volatile unint8*)buffer[object].row_port + DATA_OFFSET) = ~(row_port[j]);

						*((volatile unint8*)buffer[object].colum_port +DATA_OFFSET ) = ~(shape_code[shape][j]);

						_delay_us(500);



					}


					*((volatile unint8*)buffer[object].row_port + DATA_OFFSET) = 0xff;


					*((volatile unint8*)buffer[object].colum_port +DATA_OFFSET ) = 0x00;



				}
			}



		}
		else
		{
			return invalid_draw;
		}


	}
	else
	{

		return invalid_object;
	}
	return good;


}




