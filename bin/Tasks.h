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

// ---------------------------------------- PROTOTIPOS --------------------------------------
void Carga ();
void Descarga ();
void Lavado();
void Centrifugado ();
void Espera ();
void Dispatcher ();

void posicionarServo(long posServo);
void Error_EEPROM();
void error_carga();
void LimpiarEEPROM();
void Cancelar();
void Display(short cancelarEnabled);
void rtos_yield();
// ---------------------------------------- CODIGO --------------------------------------

void posicionarServo(long posServo){
	long delayInicial=300;

	output_low(SV_CTRL);
	output_high(SV_CTRL);
	delay_us(delayInicial);
	delay_us(posServo*8);
	output_low(SV_CTRL);
	delay_ms(10);
}

void Error_EEPROM(){
	lcd_gotoxy(1,1);
	printf(lcd_putc," Error memoria! ");
	str1Pos=0;  // Tocamos la variable del display para q empieze donde queremos
	strcpy(str1,".  Se encontro un sector no valido en la EEPROM");

	while (input(BUT_IZQ)==1 && input(BUT_DER)==1 && input(BUT_MID)==1){
		generate_tone(800, 250);
		display(0);
	};

	LimpiarEEPROM();
	
	reset_cpu();
}

void error_carga(){
	output_low(RLY_EV); // Se cierra la Electro Válvula

	lcd_gotoxy(1,1);
	printf(lcd_putc,"Error carga H2O!");

	str1Pos=1;  // Tocamos la variable del display para q empieze donde queremos
	strcpy(str1,".  Si la canilla esta abierta, revisar presometro");

	while (input(BUT_IZQ)==1 && input(BUT_DER)==1 && input(BUT_MID)==1){
		generate_tone(800, 250);
		display(0);
	};

	LimpiarEEPROM();
	
	Descarga ();
	reset_cpu();
	
}

void LimpiarEEPROM(){
	write_EEPROM (EEPROM_LAVADO_INTERRUMPIDO,LAVADO_NO_EN_CURSO);
	while (punteroFunciones<0xFF){ // Borramos toda la parte de programa de la eeprom.
		write_EEPROM (punteroFunciones,0x00);
		punteroFunciones++;
	};
}

void Cancelar(){
	int i=0;
	lcd_gotoxy(1,1);
	printf(lcd_putc,"Cancelar Lavado?");
	lcd_gotoxy(1,2);
	printf(lcd_putc,"<--NO      SI-->");
	
	while (i<240 && input(BUT_IZQ)==1 && input(BUT_DER)==1) { // Si no se apreta nada luego de 12 segundos sale...
		restart_wdt();
		i++;
		delay_ms(50);
	}
	if( input(BUT_IZQ)==1 && input(BUT_DER)==0){ // Solo entra si se apreta un solo boton, el derecho.
		lcd_gotoxy(1,2);
		printf(lcd_putc,"  Cancelando... ");
		generate_tone(800, 50);
		LimpiarEEPROM();
		delay_ms(500);
		reset_cpu();
	}
	else {

//  // Usado para desactivar el display movil del LCD
//	lcd_gotoxy(1,1);
//	printf(lcd_putc,"Realizando el   ");
//	lcd_gotoxy(1,2);
//	printf(lcd_putc,"lavado. Espere. "); 
//  // Comentar lo de abajo

		lcd_gotoxy(1,1);
		printf(lcd_putc,"WM v5.2 | T=   C"); 
		lcd_gotoxy(15,1);
		lcd_putc(0xDF); 
	};
}	

void Display(short cancelarEnabled){
	int i;
	int tamanoStr;
	
	restart_wdt(); // Este es el lugar principal donde se resetea el WDT
	
	if (cancelarEnabled==1 && input(BUT_MID)==0) Cancelar();  // Nos fijamos si se quiere cancelar el programa...

//  // Usado para desactivar el display movil del LCD
//  // Comentar lo de abajo
	lcd_gotoxy(1,2);
	tamanoStr=strlen(str1);
	if (str1Pos >=tamanoStr-1) str1Pos=0;
	for (i=0;i<LARGO_LINEA_LCD;i++){
		if ((str1Pos+i)<(tamanoStr))
			lcd_putc(str1[str1Pos+i]);
		else	
			lcd_putc(str1[str1Pos+i-tamanoStr]);
	};	
	str1Pos++;
}
	
void rtos_yield(){ // Para reemplazar al yield  del rtos
	long value;


	// debe durar aprox 1 seg	
		delay_ms(327);
	display(1);
		delay_ms(327);
	display(1);
		delay_ms(327);
	display(1);
		

	value = read_adc();

//  // Usado para desactivar el display movil del LCD
//  // Comentar lo de abajo
	lcd_gotoxy(13,1);
	printf(lcd_putc, "%02Lu", (value/2));
}	
	
void Dispatcher (){
	int i;

//  // Usado para desactivar el display movil del LCD
//	lcd_gotoxy(1,1);
//	printf(lcd_putc,"Realizando el   ");
//	lcd_gotoxy(1,2);
//	printf(lcd_putc,"lavado. Espere. "); 
//  // Comentar lo de abajo	

	lcd_gotoxy(1,1);
	printf(lcd_putc,"WM v5.2 | T=   C"); 
	lcd_gotoxy(15,1);
	lcd_putc(0xDF); 

	bcp=read_eeprom (punteroFunciones);

	while (bcp==0){ // Leo desde la eeprom hasta encontrar el lugar en donde quedo el programa
		punteroFunciones++;
		bcp=read_eeprom (punteroFunciones);
	}

	while (bcp!=EEPROM_CHAR_FIN_PROGRAMA) { // Leo el programa desde la eeprom hasta que se llegue al cararcter de fin de programa
		switch (bcp){
			case 0x01: param1=read_eeprom (punteroFunciones+1); // Carga de AGUA
						Carga();
						break;

			case 0x02:  param1=read_eeprom (punteroFunciones+1); // Lavado
						param2=read_eeprom (punteroFunciones+2);
						param3=read_eeprom (punteroFunciones+3);
						param4=read_eeprom (punteroFunciones+4);
						Lavado();
						break;

			case 0x03:  Descarga(); // Desagote
						break;

			case 0x04:  param1=read_eeprom (punteroFunciones+1); // Centrifugado
						Centrifugado();
						break;

			case 0x05:  param1=read_eeprom (punteroFunciones+1); // Rutina de Espera
						Espera();
						break;

			default:  	Error_EEPROM(); // RUTINA DE ERROR
						break;
		}
	
		// Borrar las funciones ya listas...
		i=2;
		if (bcp==0x02) i=i+3; // El lavado ocupa 5 espacios por eso le sumo 3
		if (bcp==0x03) i=i-1; // La descarga ocupa 1 espacio por eso resto 1
		
		
		punteroFunciones+=i; // el punteroFunciones se pone en la proxima funcion a cargar
		
		while (i>0) {
			write_eeprom (punteroFunciones-i,0x00); // Se borran los bytes anteriores
			i--;
		}

		bcp=read_eeprom (punteroFunciones); // Cargo la proxima funcion
	}

	write_EEPROM (EEPROM_LAVADO_INTERRUMPIDO,LAVADO_NO_EN_CURSO);
}

void Carga (){
	long i;
	int gaveta;
	int posGaveta;
	str1Pos=0;  // Tocamos la variable del display para q empieze donde queremos
	strcpy(str1," [Cargando] No te olvides de abrir el grifo! ");
	gaveta=param1;
	if (gaveta>4) Error_EEPROM(); // Rutina de error
	posGaveta=read_eeprom(EEPROM_GAVETAS_INICIO+gaveta);
	for (i=0;i<50;i++) posicionarServo(posGaveta); // Se posiciona el servo en la gaveta correspondiente
	output_high(RLY_EV); // Apertura de la Electro Válvula de entrada de agua

	i=0;
	while (input(PRSMTR1)==1){
		if (i>(TIEMPO_MAX_CARGA*60)) error_carga();
		rtos_yield();
		i++;
	}
	output_low(RLY_EV); // Se cierra la Electro Válvula
	// Calcular estadisticas de carga
	
}

void Lavado(){  // Debe declarase como una tarea con un intervalo de 1seg
	int minutos;
	char minutosStr[4];
	int seg_cycle_on;
	int seg_cycle_off;
	int temp; 
	signed int contadorTemperatura=0;
	int segundos=0;
	int i;
	short int rotationalFlag=0;
	minutos=param1;
	seg_cycle_on=param2;
	seg_cycle_off=param3;
	temp=param4;
	str1Pos=29;  // Tocamos la variable del display para q empieze donde queremos
	
	output_high (RLY_ML);	// Indicamos q se trabaja con el Motor de Lavado
	while (minutos > 0) {
		itoa(minutos,10,minutosStr);
		strcpy(str1," min para esta fase.  [Lavando]  Faltan ");
		strcat(str1,minutosStr); 
		
		if (read_adc() > (temp + GRADOS_MARGEN_TEMPERATURA)) {
			if (contadorTemperatura >=3)
				output_low (RLY_TR);
			else
				contadorTemperatura++;
		}
		else if (read_adc() < (temp - GRADOS_MARGEN_TEMPERATURA)) {
			if (contadorTemperatura <=-3)
				output_high (RLY_TR);
			else
				contadorTemperatura--;
		}
		else {
			contadorTemperatura=0;
		};
		
		if (rotationalFlag){ // Hago una mascara solo para evaluar el ultimo bit
			output_toggle (RLY_SM); //	output_bit(RLY_SM, !input(RLY_SM));
			delay_ms (DELAY_AC_SWITCHING);
			output_high (RLY_AC);//output_high (RLY_AC);
			for (i=0;i<seg_cycle_on;++i) rtos_yield(); // Me voy a dormir tantas veces como segundos sean especificados
			segundos+=seg_cycle_on; // Sumo la cantidad de ciclos q estuvo durmiendo
		}
		else {
		output_low (RLY_AC);
			for (i=0;i<seg_cycle_off;++i) rtos_yield(); // Me voy a dormir tantas veces como segundos sean especificados
			segundos+=seg_cycle_off; // Sumo la cantidad de ciclos q estuvo durmiendo
		}
										// ATENCION: FALTA CONTEMPLAR SI NO TIENE SUFICIENTE AGUA! NO DEBE LAVAR SIN AGUA.
		rotationalFlag=!rotationalFlag;
		if (segundos>60) {
			segundos-=60;
			minutos--;
		}
	}
	output_low (RLY_AC); // Se deja apagado el motor antes de salir
	output_low (RLY_SM);
	output_low (RLY_ML);
	output_low (RLY_TR);
}	


void Descarga (){
	int segundosDescargando=0;
	char segundosStr[4];

	str1Pos=0;  // Tocamos la variable del display para q empieze donde queremos
	strcpy(str1," [Desagotando] Esperando estado del presometro. ");

	output_high(RLY_BA);
	while (input(PRSMTR1)==0){  // Espero hasta que el presometro se desactive y a partir de ahi empiezo a contar.
	rtos_yield();               // De esta forma se tiene una medida mas exacta para la descarga.
	};

	str1Pos=6;  // Tocamos la variable del display para q empieze donde queremos
	while (segundosDescargando<TIEMPO_DESCARGA) {
		itoa((TIEMPO_DESCARGA-segundosDescargando),10,segundosStr);
		strcpy(str1," segs.  [Descargando]  Faltan ");
		strcat(str1,segundosStr); 
		rtos_yield();
		segundosDescargando++;
	}	
	output_low(RLY_BA);
}	

void Centrifugado (){
	char segundosStr[4];
	int tiempoCentrifugado;
	tiempoCentrifugado=param1;

	output_low(RLY_ML); // Activamos el motor de CENTRIFUGADO
	delay_ms (DELAY_AC_SWITCHING); 
	output_high(RLY_AC); 
	output_high(RLY_BA);  // Tambien tiene q estar la bomba de agua para sacar el agua del centrifugado
	
	str1Pos=6;  // Tocamos la variable del display para q empieze donde queremos
	while (tiempoCentrifugado>0){
		itoa(tiempoCentrifugado,10,segundosStr);
		strcpy(str1," segs.  [Secando]  Faltan ");
		strcat(str1,segundosStr); 
		rtos_yield();

		tiempoCentrifugado--;
	}
	output_low(RLY_AC); // Apagamos el motor
	output_low(RLY_BA);
}

void Espera (){
	long minutosEspera;
	int segundos=0;
	char minutosStr[5];
	minutosEspera=param1; 
	str1Pos=24;  // Tocamos la variable del display para q empieze donde queremos
	while (minutosEspera>0){
		itoa(minutosEspera,10,minutosStr);
		strcpy(str1," minutos para continuar.  [Esperando]  Faltan ");
		strcat(str1,minutosStr); 
		rtos_yield();

		segundos++;
		if (segundos>=60){
			segundos=0;
			minutosEspera--;
		}
	}
}
