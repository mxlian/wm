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

//-------------------------PUERTOS----------------------------
//-------------------- PORT A
#define TEMP_IN PIN_A0
#define BUT_IZQ PIN_A1
#define BUT_DER PIN_A2
#define BUT_MID PIN_A3


//-------------------- PORT B
#define RLY_EV PIN_B1	//Activa la electrovalvula.
#define RLY_ML PIN_B2	//ML=1 Motor lavado, ML=0 motor centrifugado
#define RLY_SM PIN_B3	//Determina el sentido de marcha. SM=1 Gira izquierda
#define RLY_AC PIN_B4	//Relay para energizar el motor. Activarlo ultimo de todo en el 
						//movimiento del motor. De esta forma se concentra todo el desgaste
						//de conmutacion en el. Incorporar un snubber para reducirlo.
#define RLY_BA PIN_B5	//Bomba Agua para desagote
#define RLY_TR PIN_B6 	//Termo Resistencia
#define LNK_LED PIN_B7 	//Indica el link entre el pic y la pc

//-------------------- PORT C
#define SV_CTRL PIN_C0	// Linea de control del servo
#define BUZZER PIN_C1	//
#define TONE_PIN PIN_C1 // tones.c requiere declarar tone_pin. 
                        // OJO que tones.c lo redeclara por PIN_B0 y puede pisar esta declaración.

//-------------------- PORT E
#define PRSMTR1 PIN_E1	//Presometro 1 y 2 en caso de tener 2 niveles. Sino solo se usa el 1.
#define PRSMTR2 PIN_E2	//Ambos estan conectados a pullups, por lo q son de logica negativa.


//---------------------- CONSTANTES --------------------------
#define CANT_MAX_PROGRAMAS 7 
#define TAMANO_BYTES_PROGRAMA 43
#define DELAY_AC_SWITCHING 100 // En ms. Indica cuanto tiempo espera antes de activar RLY_AC. Para q evitar desgaste de los otros contactos. 
#define TIEMPO_DESCARGA 50  // Cantidad de segundos q se activa la bomba de desagote
#define LAVADO_EN_CURSO 0xFE // Indica el valor q se escribe en EEPROM_LAVADO_INTERRUMPIDO cuando se comienza la ejecucion de un programa
#define LAVADO_NO_EN_CURSO 0xF0 // Cuando no hay lavado (antonimo de LAVADO_EN_CURSO). Debe entrar al menu para elegir programa
#define LARGO_LINEA_LCD 0x10
#define TAMANO_STR1 0x32 // 50 caracteres
#define TIEMPO_MAX_CARGA 11 // Cantidad de minutos de carga, despues de los cuales se considera q hubo un error con el presometro.
#define GRADOS_INCREMENTOS_TEMPERATURA_MENU 15 // Cuantos grados incrementa la temperatura el menu al tocar las teclas
#define GRADOS_MARGEN_TEMPERATURA 3 // +- Cantidad de Grados Centigrados que sirven para controlar la histeresis del sistema. 

//------------------------ EEPROM ----------------------------
#define EEPROM_GAVETAS_INICIO 0x00 //Necesita 4 bytes consecutivos
#define EEPROM_LAVADO_INTERRUMPIDO 0x04 //Necesita 1 byte
#define EEPROM_PROGRAMA_INICIO 0x05 //Necesita tantos bytes como dure el programa
#define EEPROM_CHAR_FIN_PROGRAMA 0xF7  // Caracter q indica fin de programa
