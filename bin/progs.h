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

// El valor 0xFF es usado como valor null. Durante la carga del programa en la EEPROM cambia su valor
const int programBinary [172] = {// 42 * 4
	// ------------------------------ PROG 1 NORMAL
	0x01, 0x00, // Carga de agua Gaveta 1 (0x00) de prelavado
	 0x02,0x0C,0x07,0x05,0xFF, // Lavar por 12 minutos con rotacion 10s y 5s descanso
	 0x03, // Descarga
	
	 0x01, 0x01, // Carga de agua Gaveta 2 (0x01) de lavado
	 0x02,0x18, 0x05,0x05,0xFF, // Lavar por 25 (0x18) minutos con rotacion 5s y 5s descanso
	 0x03, // Descarga
	
	 0x01,0x01,  // Carga de agua Gaveta 2 (0x01) de lavado
	 0x02,0x07,0x05,0x05,0x00, // Lavar por 7 minutos con rotacion 5s y 5s descanso
	 0x03, // Descarga
	
	 0x01,0x01, // Carga de agua Gaveta 2 (0x01) de lavado
	 0x02,0x05,0x04,0x03,0x00, // Lavar por 5 minutos con rotacion 4s y 3s descanso
	 0x03, // Descarga
	
	 0x01,0x03, // Carga de agua Gaveta 4 (0x03) de suavizante
	 0x02,0x05,0x05,0x08,0x00, // Lavar por 5 minutos con rotacion 5s y 8s descanso
	 0x03, // Descarga
	
	 0x04,0x78, // Centrifuga por 2 min = 120 seg
	
	 EEPROM_CHAR_FIN_PROGRAMA,
 

// ------------------------------ PROG 2 DELICADO    NO USA PRELAVADO
	
    0x01, 0x01, // Carga de agua Gaveta 2 (0x01) de lavado
	0x02, 0x0A, 0x05,0x0F,0xFF, // Lavar por 10 minutos con rotacion 5s y 15s descanso
	
	0x05,0x05,  // Espera 5 minutos. Tipo modo float, la ropa queda reposando..
	0x02, 0x05, 0x05,0x08,0xFF, // Lavar por 5 minutos con rotacion 5s y 8s descanso
	0x03, // Descarga
	
	0x01,0x01, // Carga de agua Gaveta 2 (0x01) de lavado
	0x02,0x03,0x04,0x05,0x00, // Lavar por 3 minutos con rotacion 4s y 5s descanso
	0x03, // Descarga
	
	0x01,0x01, // Carga de agua Gaveta 2 (0x01) de lavado
	0x02,0x04,0x04,0x05,0x00, // Lavar por 4 minutos con rotacion 4s y 5s descanso
	0x03, // Descarga
	
	0x01,0x03, // // Carga de agua Gaveta 4 (0x03) de suavizante
	0x02,0x05,0x03,0x07,0x00, // Lavar por 05 minutos con rotacion 3s y 7s descanso
	0x03, // Descarga
	
	0x04,0x78, // Centrifuga por 2 min = 120 seg
	
	EEPROM_CHAR_FIN_PROGRAMA,
	0x00, // Caracter de relleno para igualar cantidad bytes
	
// ------------------------------ PROG 3 SUCIO
   0x01, 0x00, // Carga de agua Gaveta 1 (0x00) de prelavado
	0x02,0x14,0x0A,0x05,0xFF, // Lavar por 20 minutos con rotacion 10s y 5s descanso
	0x03, // Descarga
	
	0x01, 0x01, // Carga de agua Gaveta 2 (0x01) de lavado
	0x02, 0x23, 0x08,0x05,0xFF, // Lavar por 35 minutos con rotacion 8s y 5s descanso
	0x03, // Descarga
	
	0x01,0x01, // Carga de agua Gaveta 2 (0x01) de lavado
	0x02,0x0A,0x08,0x05,0x00, // Lavar por 10 minutos con rotacion 8s y 5s descanso
	0x03, // Descarga
	
	0x01,0x01, // Carga de agua Gaveta 2 (0x01) de lavado
	0x02,0x05,0x07,0x03,0x00, // Lavar por 5 minutos con rotacion 7s y 3s descanso
	0x03, // Descarga
	
	0x01,0x03, // Carga de agua Gaveta 4 (0x03) de suavizante
	0x02,0x05,0x05,0x08,0x00, // Lavar por 5 minutos con rotacion 5s y 8s descanso
	0x03, // Descarga
	
	0x04,0xB4, // Centrifuga por 3 min = 180 seg
	
	EEPROM_CHAR_FIN_PROGRAMA,

// ------------------------------ PROG 4 AGRESIVO
   0x01, 0x00, // Carga de agua Gaveta 1 (0x00) de prelavado
	0x02,0x14,0x0A,0x05,0xFF, // Lavar por 20 minutos con rotacion 10s y 5s descanso
	0x03, // Descarga
	
	0x01,0x01,// Carga de agua Gaveta 2 (0x01) de lavado
	0x02, 0x3C, 0x07,0x0A,0xFF, // Lavar por 60 minutos con rotacion 7s y 10s descanso
	0x03, // Descarga
	
	0x01,0x01, // Carga de agua Gaveta 2 (0x01) de lavado
	0x02,0x05,0x07,0x03,0x00, // Lavar por 5 minutos con rotacion 7s y 3s descanso
	0x03, // Descarga
	
	0x01,0x02, // Carga de agua Gaveta 3 (0x02) de aclarado
	0x02,0x08,0x07,0x03,0x00, // Lavar por 8 minutos con rotacion 7s y 3s descanso
	0x03, // Descarga
	
	0x01,0x03, // Carga de agua Gaveta 4 (0x03) de suavizante
	0x02,0x05,0x03,0x07,0x00, // Lavar por 5 minutos con rotacion 3s y 7s descanso
	0x03, // Descarga
	
	0x04,0x78, // Centrifuga por 2 min = 120 seg
	
	EEPROM_CHAR_FIN_PROGRAMA};
