// UNIVERSIDAD TÉCNICA DEL NORTE
// CHAMORRO ALFONSINA, HINOJOSA DAVID, YACELGA JORGE
// SISTEMAS EMBEBIDOS
// PROYECTO 1 BIMESTRE

#define tsig 90                                                 // TAMAÑO DE LA EMTRADA

extern double fsensor[tsig];                                // LLAMA A LA SEÑAL
int senal;                                                         // VARIABLE DONDE SE GUARDA EL FILTRO
double promSenal;                                                    // VARIABLE PARA EL PROMEDIO DE LA SEÑAL
double promFiltro;                                                   // GUARDA PROMEDIO SEÑAL FILTADA
double out[tsig];                                            // vec DE SALIDA
double vec[5];                                                  // ALMACENAR DATOS DE FILTRADO
void medFiltro (double *sen, double *outSen, int senTam, int filtro);
void graficSen(void);                                            // GRAFICACION DE LA SEÑAL



void setup() {
  Serial.begin(9600);                                             // INICIO DE LA TERMINAL SERIAL
}

void loop() {
  filtroGauss ((double *)&fsensor[0], (double *)&out[0], (int) tsig,5);     // METODO GAUS
  graficSen();                                                                       // GRAFICACION DE LA SEÑAL
  SNR();                                                                               // METODO SNR
  delay(150);                                                                          // PAUSA DE VISUALIZACION
}

//Filtro Gaussiano

void filtroGauss (double *sig, double *out, int sigTam, int filtrado){//metodo gausssiano
int i,j,k;//variables
  for(k=0;k<sigTam;k++){                       // CICLO METODO GAUSS
    vec[0]=sig[k+0];                      // REMPLAZA POSICION DEL vec 0
    vec[1]=sig[k+1];                      // REMPLAZA POSICION DEL vec 1
    vec[2]=sig[k+2];                      // REMPLAZA POSICION DEL vec 2
    vec[3]=sig[k+3];                      // REMPLAZA POSICION DEL vec 3
    vec[4]=sig[k+4];                      // REMPLAZA POSICION DEL vec 4
    i=0;
    while(i<=3){
      i++;
      j=i+1;
      while(j<=4){
        j++;
        if(vec[j]<vec[i]){                    // SI LA POSICION ES MENOR A LA DEL vec
          senal=vec[j];                          // REMPLAZA LA POSICION DEL vec EN EL vec DE LA SEÑAL
            vec[j]=vec[i];                    // COMPARACION DE LOS vecES
            vec[i]=senal;                        // REMPLAZA EL VALOR DE LA SEÑAL EN EL vec POSICION
        }
      }
    }
    out[k]=vec[1];                           // RESULTADO
  }
  }
  
void graficSen(void){                            // GRAFICACION DE LA SEÑAL
  int i; 
  for(i=0;i<tsig;i++){                          // CICLO DE RECORRIDO DE LA SEÑAL
      Serial.print(fsensor[i]);                // POSICION DE LOS DATOS
      Serial.print(",");
      Serial.print((out[i]));                   // SALIDA DE LA SEÑAL - GAUSS
      Serial.println(",");
    delay(10);                                     // TEMPORIZADOR
  }
}

//Métrica SNR

void SNR(){ 
  int i;
  for(i=0;i<90;i++){                                 // RECORRE SEGUN EL N DE DATOS DE LA SEÑAL
    promSenal=promSenal+fsensor[i];                  // GUARDA VALORES PROM EN CADA PUNTO
    }
   double promSenal1 = promSenal/90;                     // DIVISION PARA EL NUMERO DE LA SEÑAL
   for(i=0;i<90;i++){                                // RECORRIDO DE LOS DATOS DE LA SEÑAL FILTRADA
    promFiltro=promFiltro+out[i];
    }
   double promFiltro2 = promFiltro/90;                   // DIVISION PARA EL NUMERO DE DATOS DE LA SEÑAL FILTRADA
   double vout = (promSenal1*5)/1023;                  // CAMBIO DE DATOS DE HASTA 1023 EN LA SEÑAL FILTRADA
   double vfilt = (promFiltro2*5)/1023;                // CAMBIO DE DATOS DE HASTA 1023 EN LA SEÑAL FILTRADA PARA EL VOLTAJE FILTRADO
   double snr = 20*log(vfilt/vout);                  // CALCULA LA METRICA
   
  Serial.println("");
  Serial.println(String("promedio señal ingresada: ")+String(promSenal1));            // IMPRIME EL VALOR PROMEDIO
  Serial.println(String("promedio señal filtrada: ")+String(promFiltro2));            // IMPRIME EL VALOR PROMEDIO
  Serial.println(String("Voltaje de salida señal ingresada: ")+String(vout));       // IMPRIME EL VALOR PROMEDIO DEL VOLTAJE DE LA SEÑAL INGRESADA 
  Serial.println(String("Voltaje de salida señal filtrada: ")+String(vfilt));       // IMPRIME EL VALOR PROMEDIO DEL VOLTAJE DE LA SEÑAL FILTRADA
  Serial.println(String("SNR: ")+String(snr)+String(" decibeles"));                 // IMPRIME EL VALOR DE LA METRICA SNR
  }
