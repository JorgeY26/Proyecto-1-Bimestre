// -----------------------UNIVERSIDAD TÉCNICA DEL NORTE-------------------------------
// INTEGRANTES: CHAMORRO ALFONSINA, HINOJOSA DAVID, YACELGA JORGE
// ---------------------------SISTEMAS EMBEBIDOS-------------------------------------- 
// ---------------------------PROYECTO 1 BIMESTRE------------------------------------


#define tam_muest 90                                // Se define el tamaño del vector 
#define tam_fil 21                                // Se define el tamaño de las filas

 extern double ECG_AmpADJ[tam_muest];               // Definir el vector de muestras a utilizar
 extern double Filtro_fir[tam_fil];               // Definir el filtro a usar
 double output[tam_muest+tam_fil];                  // sum de vector y filas para la señal suavizada
 double output_sig[tam_muest];

void setup() {
  Serial.begin(9600);                             //Habilitar el puerto serial
}

void loop() {
 conv((double *)&ECG_AmpADJ, (double *)&output[0], (double *)&Filtro_fir[0], (int) tam_muest, (int) tam_fil);         // Realizar el metodo de convolucion
 avg_filter((double *)&ECG_AmpADJ, (double *)&output_sig[0], (int) tam_muest,5);                                      // Encontramos la media del suavizado
 graficar();  
 SNRmetrics();   
 delay(100);
}

//convolucion 

void conv (double x_n[],double y_n[], double h_n[], int x_tam, int h_tam){  
  int i,j;                                    // Creacion de las variables para ciclo for
  for(i=0; i<(x_tam+h_tam); i++){
    y_n[i]=0;                                
    }
  for(i=0; i<x_tam; i++){
    for(j=0; j<h_tam; j++){
      y_n[i+j] += x_n[i]*h_n[j];              // Calculo de la convolucion
    }
  }
}

// ALGORITMO DE SUAVIZADO MEDIA

void avg_filter(double sig_in[], double output_sig[], int sig_tam, int filter){
  int i,j; 
  i=floor(filter/2);  
  while(i<sig_tam-floor(filter/2)-1){ 
    i++;
    output_sig[i]=0;
    j=-floor(filter/2);
    while(j<floor(filter/2)){
      j++;
      output_sig[i]=output_sig[i]+sig_in[i+j];
      }
      output_sig[i]=output_sig[i]/((filter*2)+1);
    }
  }

// CALCULO SNR

void SNRmetrics(){
  int i;
  float sum=0;
  for(i=0;i<tam_muest;i++){                                                       // Ciclo for de las muestras
    sum += ECG_AmpADJ[i];                                                         // Incremento del vector de muestras a usar
    }
  float prom=sum/tam_muest;
  float v1=map(prom,0,1023,0,5);
  Serial.println(String("Tension de señal: ")+String(v1));                        // Imprimir la tension de la señal
  
  float sum2=0;
  for(i=0;i<tam_muest;i++){                                                       // Inicializacion ciclo FOR mostrando el recorrido de la señal filtrada
    sum2 += output[i];                                                            // Sumar las muestras de la señal Filtrada
    }
  float prom2=sum2/tam_muest;                          
  float v2=map(prom2,0,1023,0,5);
  Serial.println(String("Voltaje de Señal Filtrada: ")+String(v2));               // Impresion tension de la Señal Filtrada

  //metrics SNR en tensiones
  
  double metricsSNR = 20*log(v2/v1);                                              // Encuentro metricas segun sus voltajes
  Serial.println(String("SNR (dB): ")+String(metricsSNR));
  }

// Metodo para graficar la señal 

void graficar(){   
  int i;
  for(i=0; i<tam_muest; i++){
    Serial.print(ECG_AmpADJ[i]);
    Serial.print(",");
    Serial.print(output_sig[i]);
    Serial.print(",");
    Serial.println(output[i]);
    delay(5);
    }
  }  
