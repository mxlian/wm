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

#SEPARATE  
void configMenu(){
int posServo=0,i; 

	lcd_gotoxy(1,1);
	printf(lcd_putc, "+Config Gavetas+");

	output_high(RLY_EV);

	for (i=0;i<=3;++i){	
		posServo=read_EEPROM (EEPROM_GAVETAS_INICIO+i);
		lcd_gotoxy(1,2);
		printf(lcd_putc, "Gav:%u Pos:%03u    ",i+1,posServo);
		while (input(BUT_MID)==0) restart_wdt(); // Para esperar a q suelte el boton OK
		while (input(BUT_MID)==1){
			if( input(BUT_IZQ)==0 && input(BUT_DER)==1){ // Solo entra si se apreta un solo boton
				if (posServo>0) {
					--posServo;
					lcd_gotoxy(1,2);
					printf(lcd_putc, "Gav:%u Pos:%03u    ",i+1,posServo);
					generate_tone(800, 20);
					delay_ms(30);
				}
				else {
					generate_tone(800, 600);//PITIDO ERROR
				};
		
			};
			
			if( input(BUT_IZQ)==1 && input(BUT_DER)==0){ // Solo entra si se apreta un solo boton
				if (posServo<255) {
					++posServo;
					lcd_gotoxy(1,2);
					printf(lcd_putc, "Gav:%u Pos:%03u    ",i+1,posServo);
					generate_tone(800, 20);
					delay_ms(30);
				}
				else {
					generate_tone(800, 600);//PITIDO ERROR
				};
				
			};
			restart_wdt();
			posicionarServo(posServo);
		};
		write_eeprom (EEPROM_GAVETAS_INICIO+i,posServo);
	};
	output_low(RLY_EV);
	
	lcd_gotoxy(1,1);
	printf(lcd_putc,"Configuracion OK");

	while (input(BUT_MID)==0); // Para esperar a q suelte el boton OK

	delay_ms(1000); 

	lcd_gotoxy(1,1);
	printf(lcd_putc,"[OK]   TEMP=  C ");

	Descarga();

}

void MostrarPrograma(int programa){
	lcd_gotoxy(1,2);
	generate_tone(800, 100);
	if (programa==1)
			printf(lcd_putc, "%u-Ropa algodon  ",programa); // El string debe ser tan largo como el mas largo de todos.
	if (programa==2)
			printf(lcd_putc, "%u-Ropa delicada ",programa);
	if (programa==3)
			printf(lcd_putc, "%u-Ropa muy sucia",programa);
	if (programa==4)
			printf(lcd_putc, "%u-Modo agresivo ",programa);
	if (programa==5)
			printf(lcd_putc, "%u-Desagote      ",programa);
	if (programa==6)
			printf(lcd_putc, "%u-Centrifugado  ",programa);
	if (programa==7)
			printf(lcd_putc, "%u-[CONFIGURAR]  ",programa);
	while (input(BUT_IZQ)==0 || input(BUT_DER)==0) restart_wdt();
}

 //#org 0x800  // Se coloca en el segundo segmento porq no entra en el primer segmento
 			// y ademas usando la directiva #separate hace q cuando se llame a la directiva rtos_terminate()
 			// el programa salta a cualquier posicion de memoria q no tiene nada q ver... No se porq es.
#SEPARATE
void menu(){
	int programa=1; 
	int i;
	ldiv_t time; // Para cargar la funcion de espera
	short prelavado=0;
	int temperatura=0;

	lcd_gotoxy(1,1);
	printf(lcd_putc, "+Menu Programas+");
	MostrarPrograma(programa);
	while (input(BUT_MID)==1){

			if( input(BUT_IZQ)==0 && input(BUT_DER)==1){ // Solo entra si se apreta un solo boton
				if (programa>1) 
					MostrarPrograma(--programa);
				else {
					generate_tone(800, 600);//PITIDO ERROR
				};

			};
			
			if( input(BUT_IZQ)==1 && input(BUT_DER)==0){ // Solo entra si se apreta un solo boton
				if (programa<CANT_MAX_PROGRAMAS) 
					MostrarPrograma(++programa);
				else {
					generate_tone(800, 600);//PITIDO ERROR
				};
			};
			restart_wdt();
	};

	while (input(BUT_MID)==0) { restart_wdt(); delay_ms(100); };  // Espero q libere el boton
	 
// ------------------------------------------------------------------------ Eleccion del prelavado
	if (programa<5 && programa != 2){
	
			
			lcd_gotoxy(1,1);
			printf(lcd_putc, "Desea prelavado?");
			lcd_gotoxy(1,2);
			printf(lcd_putc, "<<-SI      NO->>");
			
			i=1; // Se usa i como delay 
			while (i<201 && input(BUT_IZQ)==1 && input(BUT_DER)==1 && input(BUT_MID)==1){		
				i++;
				restart_wdt();
				delay_ms(50);
			};
			if( input(BUT_IZQ)==0) {
				prelavado=1;
				i=1;
			} // Solo activa el prelavado si se presiona el boton izquierdo
			if (i!=201) generate_tone(800, 100);//PITIDO ERROR
	}

	while (input(BUT_IZQ)==0 || input(BUT_DER)==0 || input(BUT_MID)==0) { restart_wdt(); delay_ms(100); };  // Espero q libere el boton
	
// ------------------------------------------------------------------------ Eleccion de la temperatura	
	if (programa<5){

			lcd_gotoxy(1,1);
			printf(lcd_putc, "  Temperatura:  ");
			lcd_gotoxy(1,2);
			printf(lcd_putc, " >> APAGADO <<  ");
			
			while (i<201 && input(BUT_MID)==1){

				if (input(BUT_IZQ)==0 && input(BUT_DER)==1){ // Solo entra si se apreta un solo boton
					if (temperatura>0) {
						temperatura=temperatura-GRADOS_INCREMENTOS_TEMPERATURA_MENU;
						lcd_gotoxy(1,2);
						if (temperatura==0) {
							printf(lcd_putc, " >> APAGADO <<  ");
						}
						else {
							printf(lcd_putc, " >>>  %02u C  <<< ",temperatura);
							lcd_gotoxy(9,2);
							lcd_putc(0xDF);
						};
						generate_tone(800, 50);
					}
					else {
						generate_tone(800, 600);//PITIDO ERROR
					};
				}
				else if (input(BUT_IZQ)==1 && input(BUT_DER)==0){ // Solo entra si se apreta un solo boton
					i=0; // Con esto indico q el wdt se debe resetear siempre, parando la temporizacion.
					if (temperatura<90) {
						temperatura=temperatura+GRADOS_INCREMENTOS_TEMPERATURA_MENU;
						lcd_gotoxy(1,2);
						printf(lcd_putc, " >>>  %02u C  <<< ",temperatura);
						lcd_gotoxy(9,2);
						lcd_putc(0xDF);
						generate_tone(800, 50);
					}
					else {
						generate_tone(800, 600);//PITIDO ERROR
					};
				};
				while (input(BUT_IZQ)==0 || input(BUT_DER)==0) restart_wdt();  // Espero q libere el boton				
				if (i>0) i++;
				
				restart_wdt();
				delay_ms(50);
			};
	if (i!=201) generate_tone(800, 100);//PITIDO ERROR	
	}
	
	while (input(BUT_MID)==0) { i=1; restart_wdt(); delay_ms(100); };
	
// ------------------------------------------------------------------------ Eleccion de tiempo espera
	if (programa<5){
	// Aqui harcodeamos la funcion espera. Tocando directamente param1 & bcp.
		param1=0;
		bcp=0x05;
			lcd_gotoxy(1,1);
			printf(lcd_putc, "Retraso inicial:");
			lcd_gotoxy(1,2);
			printf(lcd_putc, " >>  %02u:%02uh  << ",0,0);
			
			
			while (i<201 && input(BUT_MID)==1){

				if( input(BUT_IZQ)==0 && input(BUT_DER)==1){ // Solo entra si se apreta un solo boton
					if (param1>0) {
						param1=param1 - 10;
						lcd_gotoxy(1,2);
						time=ldiv(param1,60);
						printf(lcd_putc, " >>  %02u:%02uh  << ",time.quot,time.rem);
						generate_tone(800, 50);
					}
					else {
						generate_tone(800, 600);//PITIDO ERROR
					};
			
				}
			
				else if( input(BUT_IZQ)==1 && input(BUT_DER)==0){ // Solo entra si se apreta un solo boton
					i=0; // Con esto indico q el wdt se debe resetear siempre, parando la temporizacion.
					if (param1<1440){ // Un dia tiene 1440 minutos.
							param1= param1 + 10;
							lcd_gotoxy(1,2);
							time=ldiv(param1,60);
							printf(lcd_putc, " >>  %02u:%02uh  << ",time.quot,time.rem);
							generate_tone(800, 50);
					}
					else {
						generate_tone(800, 600);//PITIDO ERROR
					};
				};
				
				while (input(BUT_IZQ)==0 || input(BUT_DER)==0) restart_wdt();
				
				restart_wdt();
				if (i>0) i++;
				delay_ms(50);
			};
	if (i!=201) generate_tone(800, 100);//PITIDO ERROR				
	};		
	while (input(BUT_MID)==0) restart_wdt();
			
// ------------------------------------------------------------------------ Grabacion del programa
	if (programa <= 4) { 
		int limiteInferior;
		limiteInferior = TAMANO_BYTES_PROGRAMA * (programa -1);
		if (prelavado==0 && programa != 2) limiteInferior=limiteInferior+8; // Para saltearse el prelavado. Programa 2 no lleva prelavado.
		for (i=0;(limiteInferior + i)<(TAMANO_BYTES_PROGRAMA * programa);i++){ 
	    	if (programBinary[limiteInferior + i] == 0xFF){
	    		write_eeprom (EEPROM_PROGRAMA_INICIO+i,temperatura);
	    	}
	    	else {
	    		write_eeprom (EEPROM_PROGRAMA_INICIO+i,programBinary[limiteInferior + i]);
			}
//			lcd_gotoxy(1,1);
//			printf(lcd_putc, "Escribo:%02u-0x%02X",i,programBinary [programa] [i]);
//			delay_ms(1000);
	    }
	    write_eeprom (EEPROM_LAVADO_INTERRUMPIDO,LAVADO_EN_CURSO);
	    Espera ();
	}
	else if (programa == 5){
    		write_eeprom (EEPROM_PROGRAMA_INICIO+0,0x03); // Descarga	
	    	write_eeprom (EEPROM_PROGRAMA_INICIO+1,EEPROM_CHAR_FIN_PROGRAMA);
	    	write_eeprom (EEPROM_LAVADO_INTERRUMPIDO,LAVADO_EN_CURSO);
	}
	else if (programa == 6){
		write_eeprom (EEPROM_PROGRAMA_INICIO+0,0x03); // Descarga
	    	write_eeprom (EEPROM_PROGRAMA_INICIO+1,0x04); // Centrifuga
	    	write_eeprom (EEPROM_PROGRAMA_INICIO+2,0xB4); // Centrifuga 3 minutos
	    	write_eeprom (EEPROM_PROGRAMA_INICIO+3,EEPROM_CHAR_FIN_PROGRAMA);
	    	write_eeprom (EEPROM_LAVADO_INTERRUMPIDO,LAVADO_EN_CURSO);
	}
	else if (programa == 7){
		configMenu();
    	write_eeprom (EEPROM_LAVADO_INTERRUMPIDO,LAVADO_NO_EN_CURSO);  // Disable la posible ejecucion de otro programa previo
	}
	else if (programa == 8){
	}

}

