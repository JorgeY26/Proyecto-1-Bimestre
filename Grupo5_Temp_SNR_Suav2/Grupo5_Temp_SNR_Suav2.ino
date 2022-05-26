// UNIVERSIDAD TÉCNICA DEL NORTE 
// CHAMORRO ALFONSINA, HINOJOSA DAVID, YACELGA JORGE
// SISTEMAS EMBEBIDOS
// PROYECTO 1 BIMESTRE

#include "signal3.h"                                 // IMPORTAMOS EL ARCHIVO DE LAS MUESTRAS

#define tamVent 17                             // DEFINIMOS EL prom
int centrarVen = tamVent / 2;             // FILTRO prom
int tamSen = sizeof(senECG) / sizeof(int);
int senFiltrada[300];                           // GUARDA LOS DATOS DE LAS MUESTRAS
int vDer = 0; 
int vIzq = 0; 


void setup() {
  
  Serial.begin(9600);
  Serial.println(tamSen);
  
  for (int i = 0; i < 300; i++) {senFiltrada[i] = 0;}                          // ENCIERRE DEL vec DE SALIDA
  
  for (int i = centrarVen; i < tamSen - centrarVen; i++) {           // RECORRIDO DEL vec 
    
    Serial.print((senECG[i]));                                                 // GRAFICA LA SEÑAL ORIGINAL
    
    vDer = i - centrarVen;                                          // POSICION INICIAL VENTANA
    vIzq = i + centrarVen + 1;                                    // POSICION FINAL VENTANA
    Serial.print(",");
    
    int value = calc_mean(vDer, vIzq);                        // CALCULO prom
    senFiltrada[i] = value;                                                    // ALMACENAMIENTO DEL VALOR EN EL vec DE SALIDA
    Serial.println(value);
  };

  float mean_input = calc_mean_snr(senECG, 300);                                 // prom DE LA SEÑAL ORIGINAL
  float deviation_input = calc_stdrDeviation(senECG, 300);                       // DESVIACIÓN ESTANDAR 
  Serial.println("SNR Input Signal: " + String(mean_input/deviation_input)); 


  float mean_output = calc_mean_snr(senFiltrada, 300);                           // SNR DE LA S. FILTRADA
  float deviation_output = calc_stdrDeviation(senFiltrada, 300);                 // DESVIACION ESTANDAR
  Serial.println("SNR Output Signal: " + String(mean_output/deviation_output));     // GRAFICACION DEL RESULTADO
}

void loop() {
}
int calc_mean(int valor_inicial, int valor_final) {                                 // CALCULO DEL prom
  int sum = 0;
  for ( int i = valor_inicial; i <= valor_final; i++) {                             // VALOR INCIAL AL FINAL
    sum += senECG[i];                                                           // GUARDA LOS VALORES DE LA SEÑAL EN TODAS LAS POSICIONES
  }
  return sum / tamVent;                                                        // RETORNA sum Y TAMAÑO VENTANA
}

void printvec (int vec[], int n) {
  for (int i = 0; i < n; i++) {
    Serial.println(vec[i]);
  }
}


float calc_mean_snr(int vec[], int n) {                                          // prom DE LA SEÑAL
  int sum = 0;
  for (int i = 0; i < n; i++) {                                                     // sum DE LA SEÑAL
    sum += vec[i];
  }
  return sum / n;                                                                  // DIVISION PARA LA LONGITUD DEL vec
}


float calc_stdrDeviation(int vec[], int n) {                                    // DESVIACION ESTANDAR
  float prom = calc_mean_snr(vec, n);                                       // LLAMA AL prom
  float sumtoria = 0;                                                             // VARIABLE sumTORIA
  for (int i = 0; i < n; i++) {                                                    // vec MENOS prom AL CUADRADO
    sumtoria += pow(vec[i] - prom, 2);
  }
  return sqrt(sumtoria / (n - 1));                                                // RAIZ 2 PARA DESVIACION ESTANDAR
}
