//ANTIGUO PROGRAMA DE KAWASAKI, IGNORAR YA QUE NO ESTÁ OPTIMIZADO.

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "FuncionesKawasaki.h"
#include <omp.h>

int main (){                         
clock_t begin= clock();//Tiempo de compilacion
srand(time(NULL));//Semilla aleatoria
int N,n,m,i,j, k, Tmax, t, a, b, c, d, w, z;//Variables de iteracion
Tmax=10000;//Pasos Montecarlo
double media, varianza;
FILE* fichero_out;
FILE* ficheroMag;
FILE* ficheroDensidad;
fichero_out=fopen("Kawasaki.txt", "w");
ficheroMag=fopen("Magnetizacion.txt","w");
ficheroDensidad=fopen("Densidad.txt","w");
double sumSuperior, sumInferior;
double magSuperior[Tmax], magInferior[Tmax];
for(i=0;i<Tmax;i++){
    magInferior[i]=0;
    magSuperior[i]=0;
}
N=32;//Numero de spins.
int s[N][N], r;
double EC1, EC2, E, T,p;
E=0;
T=1;//Temperatura
//Lleno la matriz de 1 y -1 aleatoreamente. Menos la primera y la última fila
// for(i=1;i<N-1;i++){
//     for(j=0;j<N;j++){
//         s[i][j]=rand()%2;
//     if(s[i][j]==0){
//         s[i][j]=-1;
//         }
//     }
// }

//Empiezo a hacer cosas
int* sp=s[0];
for (T=1;T<2;T++){
    // Inicializo la matriz para tener una magnetización nula.
    for(i=0; i<N; i++){
        for(j=0; j<N; j++){
        // Asigno spin +1 a filas pares y spin -1 a filas impares
        // Habrá el mismo número de up que down.
            if(i % 2 == 0){
                s[i][j] = 1; // Up spin
            } else {
                s[i][j] = -1; // Down spin
            }
        }   
    }
    double conta;
    conta=0;
for(j=0;j<N;j++){//Lleno la última fila de -1 y la primera de 1.
        s[0][j]=1;
        s[N-1][j]=-1;
}
for(int k=0;k<100;k++){
      for(i=0;i<N;i++){
            for(j=0;j<N-1;j++){
                fprintf(fichero_out, "%d, ", s[i][j]);
            }
            fprintf(fichero_out, "%d", s[i][N-1]);
            fprintf(fichero_out, "\n");
        }
    fprintf(fichero_out, "\n");
}
for(t=0;t<Tmax;t++){
    for(i=0;i<Tmax;i++){
        magInferior[i]=0;
        magSuperior[i]=0;
    }
    sumSuperior = 0;
    sumInferior = 0;
    for(k=0;k<N*N;k++){
        EC1=0;
        EC2=0;
        //Posición w, z aleatoria de la malla.
        w=rand()%(N-2)+1;//Entre N-2 para que no acceda a la última ni penúltima fila.
        z=rand()%(N);
        //Rotación aleatoria.
        if(w==1){
            r=rand()%3+1;
        }
        else if(w==N-2){
            r=rand()%3;
                if(r==1){
                    r=3;
                }
        }
        else{
        r=rand()%4;
        }
            for(n=1;n<N-1;n++){//Calculo la energía de toda la malla (menos las fijas, fila 1 y N-1)
                for(m=0;m<N;m++){
                //Condiciones de contorno
                    a = (n + 1);
                    b = (n - 1);
                    c = (m + 1) % N;
                    d = (m - 1 + N) % N;

                    EC1+=0.5*s[n][m]*(s[a][m]+s[b][m]+s[n][c]+s[n][d]);

                }
            }
            a = (w + 1);
            b = (w - 1);
            c = (z + 1) % N;
            d = (z - 1 + N) % N;
        IntercambiarPosiciones(sp,r,N,w,z,a,b,c,d);//Intercambio posiciones con un spin vecino (elegido por r aleatorio)

            for(n=1;n<N-1;n++){//Vuelvo a calcular las energias para mi nueva malla intercambiada
                for(m=0;m<N;m++){
                //Condiciones de contorno
                    a = (n + 1);
                    b = (n - 1);
                    c = (m + 1) % N;
                    d = (m - 1 + N) % N;

                    EC2+=0.5*s[n][m]*(s[a][m]+s[b][m]+s[n][c]+s[n][d]);
        
                }
            }

        E=EC1-EC2;//Calculo la diferencia entre estas dos mallas
        //Busco minimo
        if(exp(-E/T)>1){
            p=1;
        }
        else 
            p=exp(-E/T);
        
        //Genero numero aleatorio
        double x;
        x=rand();
        x/=RAND_MAX;
        //Si mi cambio NO es favorable, vuelvo a dejar las cosas como estaban, en caso contrario,
        //el cambio estaba bien hecho.
        if(p<x){
            a = (w + 1);
            b = (w - 1);
            c = (z + 1) % N;
            d = (z - 1 + N) % N;
            IntercambiarPosiciones(sp, r, N, w, z, a, b, c, d);
        }
    }
    // if(t%100==0){
    //   for(i=0;i<N;i++){
    //         for(j=0;j<N-1;j++){
    //             fprintf(fichero_out, "%d, ", s[i][j]);
    //         }
    //         fprintf(fichero_out, "%d", s[i][N-1]);
    //         fprintf(fichero_out, "\n");
    //     }
    // fprintf(fichero_out, "\n");
    // }

    //MAGNETIZACIÓN 
        for(i=1; i<N-1; i++){
            for(j=0; j<N; j++){
                if(i < N/2){
                    sumSuperior += s[i][j];
                } else {
                    sumInferior += s[i][j];
                }
            }
        }
        magSuperior[t]=sumSuperior/(((N-1)/2)*N);
        magInferior[t]=sumInferior/(((N-1)/2)*N);
//DENSIDAD PROMEDIO EN Y
double contplus, contminus,PromedioDensidadPlus, PromedioDensidadMinus;
double densidadplus[N], densidadminus[N];
PromedioDensidadPlus=0;
PromedioDensidadMinus=0;
contplus=0;
contminus=0;
    for(j=0;j<N;j++){
            contplus=0;
            contminus=0;
            for(i=0;i<N;i++){
                if(s[i][j]==1){
                    contplus+=1;
                }
                else{
                    contminus+=1;
                }
            }//DENSIDAD POR COLUMNA:
            densidadplus[j]=contplus/N;
            densidadminus[j]=contminus/N;
        }
    // for(j=0;j<N;j++){//Promedio de columnas
    //     PromedioDensidadPlus+=densidadplus[j]/N;
    //     PromedioDensidadMinus+=densidadminus[j]/N;
    // }
    conta=conta+densidadplus[0];
// printf("Densidad Plus=%lf, %lf \n", PromedioDensidadPlus, T);
// printf("Densidad Minus=%lf, %lf \n", PromedioDensidadMinus, T);    
}//fin montecarlo
double MediaSuperior, MediaInferior, varianzaSup, varianzaInf;
MediaSuperior=0;
MediaInferior=0;
varianzaSup=0;
varianzaInf=0;
for(t=0; t<Tmax; t++){//MEDIA
    MediaSuperior+=magSuperior[t]/Tmax;
    MediaInferior+=magInferior[t]/Tmax;
}
for(t=0; t<Tmax; t++){//VARIANZA
    varianzaSup+=pow((magSuperior[t]-MediaSuperior),2)/Tmax;
    varianzaInf+=pow((magInferior[t]-MediaInferior),2)/Tmax;
}
double errorSup, errorInf;
errorSup=sqrt(varianzaSup)/sqrt(Tmax);
errorInf=sqrt(varianzaInf)/sqrt(Tmax);
conta=conta/Tmax;
fprintf(ficheroMag, "%lf, %lf, %lf, %lf, %lf \n", MediaSuperior, errorSup,  MediaInferior, errorInf, T);
fprintf(ficheroDensidad, "%lf, %lf, %lf \n", conta, 1-conta, T);
}
fclose(fichero_out);
fclose(ficheroMag);
fclose(ficheroDensidad);
clock_t end=clock();//Tiempo que ha tardado en ejecutarse
double tiempo= (double) (end-begin)/ CLOCKS_PER_SEC;
    printf("Tiempo de compilacion=%lf \n", tiempo);
return 0;
}