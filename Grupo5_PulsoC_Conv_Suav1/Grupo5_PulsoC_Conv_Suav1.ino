
// UNIVERSIDAD TÉCNICA DEL NORTE
// CHAMORRO ALFONSINA, HINOJOSA DAVID, YACELGA JORGE
 

#define tsig 90    // NUMERO DE MUESTRAS
#define tfir 21    // ORDEN DE FILTRO


// Muestras de las senales guardadas

extern double ADJ[tsig];                // VECTOR DE LAS MUESTRAS DE LA SENAL
extern double CFIR[tfir];               // VECTOR DE LOS COEFICIENTES DE FILTRO              

double out[tsig+tfir];                  // VECTOR OUT
double m_s[tsig];                       // VECTOR PARA EL FILTRO DE LA MEDIANA

void setup() {
Serial.begin(9600);                                                                             //HABILITA EL TERMINAL SERIAL
}

void loop() {
 conv((double *)&ADJ[0], (double *)&out[0], (double *)&CFIR[0], (int) tsig, (int) tfir);        //METODO CONVOLUCION
 med((double *)&ADJ[0], (double *)&m_s[0], (int) tsig);                                         //METODO MEDIANA
 graficar();                                                                                    //METODO PARA GRAFICAR
 SNR();                                                                                         //METODO PARA SNR
 delay(100);                                                                                    //DETENGO EL MICROCONTROLADOR
  
}

void conv(double x_n[],double y_n[], double h_n[], int x_tam, int h_tam){                       // CONVOLUCION
  
  int i,j;
  
  for(i=0;i<(x_tam+h_tam);i++){            // CICLO FOR PARA CONVOLUCION   
    y_n[i]=0;                                 
  }
  for(i=0;i<x_tam;i++){              
    for(j=0;j<h_tam;j++){                 
      y_n[i+j]+=x_n[i]*h_n[j];             // CALCULO CONVOLUCION
    }
  }
}





void med(double sig_in[], double m_s[], int sig_tam){                // SUAVIZADO PARA MEDIANA
  int i,j,k;
  float vec[5], sig_Temp;
  for(k=0;k<sig_tam;k++){                 // CICLO FOR PARA LA SENAL
    vec[0]=sig_in[k+0];                   // POSICION 1
    vec[1]=sig_in[k+1];                   // POSICION 2
    vec[2]=sig_in[k+2];                   // POSICION 3
    vec[3]=sig_in[k+3];                   // POSICION 4
    vec[4]=sig_in[k+4];                   // POSICION 5
    for(i=0;i<(4);i++){     
      for(j=i+1;j<5;j++){
        if(vec[j]<vec[i]){
          sig_Temp=vec[j];   
          vec[j]=vec[i];
          vec[i]=sig_Temp;  
        }
      }
    }
    m_s[k]=vec[1];
  }
}

 
 
void SNR(){                                                         // METODO SNR                              
  float sum=0;                                                      // VARIABLE PARA SUMA
  for(int i=0;i<tsig;i++){                                          // CILCO FOR PARA PULSOS
      sum += ADJ[i];                                                // SUMA TOTAL DE PULSOS
  }
  float prom=sum/tsig;                                              // PROMEDIO DE MUESTRAS
  float v1=map(prom,0,1023,0,5);
  Serial.println(String("Tension de señal: ")+String(v1));          // VOLTAJE DE LA SENAL

  float sum2=0;                                                     // VARIBALE PARA SUMA 2
  for(int i=0;i<tsig;i++){
    sum2 += m_s[i];
  }
  float prom2=sum2/tsig;                                               
  float v2=map(prom2,0,1023,0,5);
    
  Serial.println(String("Tension de señal filtrada: ")+String(v2));         // IMPRIMIR DE FORMA SERIAL

    double met_SNR=20*log(v2/v1);                                           // METRICA SNR
    Serial.println(String("SNR (dB): ")+String(met_SNR));                  // IMPRIMIR met_SNR
   //Serial.print("");
    
}

// Gráfica de la senal

void graficar(){                                                           // METODO GRAFICAR
  int i;
  for(i=0;i<tsig;i++){
    Serial.print(String("ECG orginal: ")+String(ADJ[i]));
    Serial.print(",");
    Serial.print(String("Filtrado con mediana: ")+String(m_s[i]));
    Serial.print(",");
    Serial.println(String("ECG suavizada: ")+String(out[i]));
    delay(5);
  }
}
