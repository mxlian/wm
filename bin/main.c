// **************************************************************************
//
// Copyright 2007 Maximiliano Padulo
//
// **************************************************************************
// This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **************************************************************************

// 26/09/2008 - a la mierda el RTOS. El PIC tiene una pila muy pequeña y es
// imposible implementarlo de esta forma.

// DISCLAIMER: El código está sujeto a las limitaciones del compilador CCS,
// y probablemente a mi inexperiencia y desconocimiento del área.  Se debe
// tener en cuenta q programar en un PIC no es lo mismo que programar en una PC.
// Muchas veces conviene hacer sacrificios que uno normalmente no haría en un
// programa de PC, con tal de obtener mayor provecho del escaso hardware
// disponible. Por eso vas a ver muchos casos de programación sucia en el
// código, sin las cuales es imposible poner todo el programa en el
// microcontrolado.


#include <16F877A.h>

#fuses HS,WDT,NOPROTECT,NOLVP
#use delay(clock=4000000)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)  // Jumpers: 8 to 11, 7 to 12
//#use RTOS(timer=0, minor_cycle=10ms)
//#include <delay.c>

#include <WM_defs.h>

#include <tones.c> // WM_defs.h ya define TONE_PIN, as� que se debe comentar la linea q lo redefine en tones.c
#include <lcd.c>
#include <stdlib.h>
#include <string.h>

#include <progs.h>
// --------------------------- GLOBAL VARIABLES ---------------------------
int punteroFunciones=EEPROM_PROGRAMA_INICIO;
int bcp=0; // Byte que indica el tipo de tarea a ejecutarse (ver CODIGO DE TAREAS)
long param1;  // Es long porque la funcion espera inicial puede tener un valor de hasta 1440.
int param2,param3,param4; // Parametros para las funciones. Las funciones fueron creadas
									// para q los parametros se les pasasen de esta forma porq 
									// se usaba RTOS. Pero las deje, porq ahorran espacio en la pila
									// y ademas no me parecio q quedase TAAAN sucio el codigo usando 
									// esto (bah queda sucio si, pero no justifica todo el cambio).
char str1[TAMANO_STR1]; 
int str1Pos=0;
// char str2[25];
// ---------------------------- Lista de Tareas ---------------------------
#include <Tasks.h>
#include <menu.h>




void main(void) {	
	// Inicializacion variables
	str1Pos=0;
	lcd_init(); // Inicializamos el display LCD
	// Apagamos todos los relays de potencia...
	output_low(RLY_EV);
	output_low(RLY_AC);
	output_low(RLY_BA);
	output_low(RLY_TR);
	// Inicializamos Watch Dog Timer
	setup_wdt(WDT_2304MS);
	// Inicializar ADC
	setup_adc_ports( AN0 );
	setup_adc(ADC_CLOCK_INTERNAL );
	set_adc_channel( 0 );
	
    // En este paso se chequea la EEPROM para ver si el lavado fue interrumpido
    // o no. Si fue interrumpido (corte de luz por ejemplo) se debe continuar
    // desde el ultimo paso incompleto

	switch (read_EEPROM (EEPROM_LAVADO_INTERRUMPIDO)){
		case LAVADO_NO_EN_CURSO: lcd_gotoxy(1,1);
			printf(lcd_putc, "  Iniciando el");
			lcd_gotoxy(1,2);
			printf(lcd_putc, "     sistema");
			generate_tone(800, 500);
			delay_ms (500);	
			menu();
			reset_cpu();
			break;
		case LAVADO_EN_CURSO: lcd_gotoxy(1,1);
			lcd_gotoxy(1,1);
			printf(lcd_putc, "  Iniciando el");
			lcd_gotoxy(1,2);
			printf(lcd_putc, "    programa");

			generate_tone(1000, 100);
			printf(lcd_putc, ".");
			delay_ms (500);

			generate_tone(1000, 100);
			printf(lcd_putc, ".");
			delay_ms (500);
			
			restart_wdt();

			generate_tone(1000, 100);			
			printf(lcd_putc, ".");
			delay_ms (500);

			Dispatcher(); // Activar dispatcher. Activando el modo interectivo de las tareas		
			break;
		default:	// En el caso q tenga un valor distinto (la primera vez q se ejecuta en el microcontrolador, o por algun error) lo restauramos al valor por defecto.
			lcd_gotoxy(1,1);
			printf(lcd_putc, "ERR EE_LAV_INT");
			lcd_gotoxy(1,2);
			printf(lcd_putc, "Reiniciando...");
			write_EEPROM (EEPROM_LAVADO_INTERRUMPIDO,LAVADO_NO_EN_CURSO);
			generate_tone(800, 2000);
			reset_cpu();
		break;
	};

	lcd_gotoxy(1,1);
	printf(lcd_putc, " Fin del lavado ");
	str1Pos=0;  // Tocamos la variable del display para q empieze donde queremos
	strcpy(str1," Por favor, proceda a colgar la ropa. ");
	while (true) {
		generate_tone(800, 500);
		display (0);
		delay_ms (500);	
		display (0);
		delay_ms (500);	
		display (0);
		delay_ms (500);			
	}	
}

