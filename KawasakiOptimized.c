//PROGRAMA DE KAWASAKI OFICIAL: Si se quieren mas de 120000 pasos MC, se usa el otro programa.
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define N 32// Numero de spins.
#define Tmax 1000 // Pasos Montecarlo
double Temp[Tmax];
double Temp2[Tmax];

void initialize_spins(int s[N][N]) {//Magnetización nula
    for(int i = 1; i < N-1; i++) {
        for(int j = 0; j < N; j++) {
            s[i][j] = (i % 2 == 0) ? 1 : -1; // Spins alternantes.
        }
    }
}

void random_magnetization(int s[N][N]){
     for(int i = 1; i < N-1; i++) {
        for(int j = 0; j < N; j++) {
            s[i][j]=rand()%2;
        if(s[i][j]==0){
            s[i][j]=-1;
            }
        }
    }
}

void fill_boundaries(int s[N][N]) {//Condiciones de contorno en y 
    for(int j = 0; j < N; j++) {
        s[0][j] = 1;
        s[N-1][j] = -1;
    }
}

void fixed_magnetization(int s[N][N]) {
    // Inicializar todos los espines en 1
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            s[i][j] = 1;
        }
    }
    
    // Numero de espines que queremos cambiar a -1
    int num_negatives = (N * N) / 3; // 
    
    int count = 0;
    for (int i = 0; i < N && count < num_negatives; i++) {
        for (int j = 0; j < N && count < num_negatives; j++) {
            if ((i + j) % 2 == 0) { // Patrón determinista
                s[i][j] = -1;
                count++;
            }
        }
    }
}

double calculate_energy(int s[N][N]) {
    double energy = 0.0;
    for(int n = 1; n < N-1; n++) {
        for(int m = 0; m < N; m++) {
            int a = (n + 1);
            int b = (n - 1);
            int c = (m + 1) % N;
            int d = (m - 1 + N) % N;
            energy += 0.5 * s[n][m] * (s[a][m] + s[b][m] + s[n][c] + s[n][d]);
        }
    }
    return energy;
}

void swap_spins(int s[N][N], int w, int z, int r) {
    int temp;
    switch(r) {
        case 0: temp = s[w][z]; s[w][z] = s[w-1][z]; s[w-1][z] = temp; break;
        case 1: temp = s[w][z]; s[w][z] = s[w+1][z]; s[w+1][z] = temp; break;
        case 2: temp = s[w][z]; s[w][z] = s[w][(z+1)%N]; s[w][(z+1)%N] = temp; break;
        case 3: temp = s[w][z]; s[w][z] = s[w][(z-1+N)%N]; s[w][(z-1+N)%N] = temp; break;
    }
}

int Checking(int s[N][N], int r, int w, int z){
    int a;
    a=0;
    if(r==0 && s[w][z]==s[w-1][z]){
        a=1;
    }
    if(r==1 && s[w][z]==s[w+1][z]){
        a=1;
    }
    if(r==2 && s[w][z]==s[w-1][(z+1)%N]){
        a=1;
    }
    if(r==3 && s[w][z]==s[w-1][(z-1+N)%N]){
        a=1;
    }
    return a;
}

int main () {                         
    clock_t begin = clock(); // Tiempo de compilacion
    srand(time(NULL)); // Semilla aleatoria

    FILE* fichero_out = fopen("Kawasaki.txt", "w");
    FILE* ficheroMag = fopen("Magnetizacion.txt", "w");
    FILE* ficheroDensidad = fopen("Densidad.txt", "w");
    FILE* ficheroDensidad2 = fopen("PromedioDensidad.txt", "w");
    FILE* ficheroEnergía = fopen("Energia.txt", "w");

    double T=1;
    double magSuperior[Tmax] = {0};
    double magInferior[Tmax] = {0};
    double densidadplus[N]= {0};
    double densidadminus[N]= {0};
    double sigma[N];
    double PromedioDensidadPlus, PromedioDensidadMinus;
    double a=100;//Parámetro de relajacion, cuanto quiero dejar al sistema evolucionar antes de medir
    int s[N][N];
    initialize_spins(s); //Para magnetización nula.
    //random_magnetization(s); //Para magnetizacion aleatoria
    //fixed_magnetization(s);
    fill_boundaries(s);

//for (double T = 1; T < 6; T=T+0.2) {//Esto se activa si se quiere calcular un observable en función de T
        initialize_spins(s); //Mag. Nula
        //random_magnetization(s); //Mag. Aleatoria
        //fixed_magnetization(s); //Mag. Fija
        fill_boundaries(s);
        double conta;
        conta=0;
        double PromedioEnergia;
        PromedioEnergia=0;
        for (int t = 0; t < Tmax; t++) {
            double sumSuperior = 0;
            double sumInferior = 0;

            #pragma omp parallel for//Hilos
            for (int k = 0; k < N * N; k++) {
                int w = rand() % (N-2) + 1; // Posición aleatoria, excluyendo primera y última fila.
                int z = rand() % N; 
                int r;
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

                int l=Checking(s, r, w, z);
                if(l==0){
                    double EC1 = calculate_energy(s);//Calculo energía sin cambiar
                    swap_spins(s, w, z, r);//Cambio spins
                    double EC2 = calculate_energy(s);//Calculo energia cambiada

                    double E = EC1 - EC2;
                    double p = (exp(-E/T) > 1) ? 1 : exp(-E/T);

                    if ((double)rand() / RAND_MAX > p) {
                        swap_spins(s, w, z, r); // Si el cambio NO es favorable, revierto el cambio.
                    }
                    PromedioEnergia-=calculate_energy(s);
                }
            }
            Temp2[t]=PromedioEnergia;
                for (int i = 0; i < N; i++) {
                    for (int j = 0; j < N-1; j++) {
                        fprintf(fichero_out, "%d, ", s[i][j]);
                    }
                    fprintf(fichero_out, "%d \n",  s[i][N-1]);
                }
                fprintf(fichero_out, "\n");
        if(t>a){//Dejo al sistema evolucionar 'a' pasos montecarlo para obtener resultados mas fiables.
            for (int i = 1; i < N-1; i++) {
                for (int j = 0; j < N; j++) {
                    if (i < N/2) {
                        sumSuperior += s[i][j];
                    } else {
                        sumInferior += s[i][j];
                    }
                }
            }

            magSuperior[t] = sumSuperior / (((N-1)/2) * N);
            magInferior[t] = sumInferior / (((N-1)/2) * N);
            }
        double contplus, contminus;
        PromedioDensidadPlus=0;
        PromedioDensidadMinus=0;
        contplus=0;
        contminus=0;
    for(int i=0;i<N;i++){
            contplus=0;
            contminus=0;
            for(int j=0;j<N;j++){
                if(s[i][j]==1){
                    contplus+=1;
                }
                else{
                    contminus+=1;
                }
            }//DENSIDAD POR COLUMNA:
            densidadplus[i]+=contplus/N;
            densidadminus[i]=contminus/N;
        }
        //     for(int j=0;j<N;j++){//Promedio de columnas
        //         PromedioDensidadPlus+=densidadplus[j]/N;
        //         PromedioDensidadMinus+=densidadminus[j]/N;
        // }
        conta=conta+densidadplus[0];//Voy sumando el valor de la densidad de una columna en un contador.
        Temp[t]=densidadplus[0];//Para el error de la densidad
        }//FinMontecarlo

        double MediaSuperior = 0;
        double MediaInferior = 0;
        double varianzaSup = 0;
        double varianzaInf = 0;
        double varianzaDensidad;
        for (int t = a; t < Tmax; t++) {//Media magnetizacion
            MediaSuperior += magSuperior[t] /(Tmax-a);
            MediaInferior += magInferior[t] /(Tmax-a);
        }

        for (int t = a; t < Tmax; t++) {//Varianza magnetizacion
            varianzaSup += pow(magSuperior[t] - MediaSuperior, 2) / (Tmax-a);
            varianzaInf += pow(magInferior[t] - MediaInferior, 2) / (Tmax-a);
        }

        double errorSup = sqrt(varianzaSup) / sqrt(Tmax-a);
        double errorInf = sqrt(varianzaInf) / sqrt(Tmax-a);
    
        conta=conta/Tmax;//Densidad promediada
        double VarianzaDensidad;
        VarianzaDensidad=0;
        for (int t=0; t<Tmax; t++){//Varianza densidad
            VarianzaDensidad+=pow(Temp[t] - conta, 2) / (Tmax);
        }
        double errorDensidad = sqrt(VarianzaDensidad) / sqrt(Tmax);

        //Energia Promedio Por Partícula 
        PromedioEnergia=PromedioEnergia/Tmax;//Promedio entre todos los pasos MonteCarlo
        PromedioEnergia=PromedioEnergia/(2*N);//Promedio por partícula

        double VarianzaEnergia;
        VarianzaEnergia=0;
        for (int t=0; t<Tmax; t++){//Varianza Energía
            VarianzaEnergia+=pow(Temp2[t]/(Tmax*2*N) - PromedioEnergia, 2) / (Tmax);
        }
        double ErrorEnergia = sqrt(VarianzaEnergia) / sqrt(Tmax);
        for(int i=0;i<N; i++){
            //sigma[i]=densidadplus[i]/Tmax;
            //fprintf(ficheroDensidad, "%lf, %lf, %d \n", sigma[i], 0.003, i);//Densidad en funcion de y
        }
        fprintf(ficheroMag, "%lf, %lf, %lf, %lf, %lf\n", MediaSuperior, 3*errorSup, MediaInferior, 3*errorInf, T);
        fprintf(ficheroDensidad, "%lf, %lf, %lf, %lf, %lf \n", conta, 3*errorDensidad, 1-conta, 3*errorDensidad, T); //DensidadPlus, DensidadMinus, Si se activa este desactivar la otra densidad!!!!!
        fprintf(ficheroDensidad2, "%lf, %lf, %lf \n",1 - PromedioDensidadMinus, PromedioDensidadMinus, T);
        fprintf(ficheroEnergía, "%lf, %lf, %lf \n", PromedioEnergia, 3*ErrorEnergia, T);
        printf("Temperatura terminada");
    //}//Fin temperatura Activar este corchete si se quieren calcular observables

    fclose(fichero_out);
    fclose(ficheroMag);
    fclose(ficheroDensidad);
    fclose(ficheroEnergía);

    clock_t end = clock(); // Tiempo que ha tardado en ejecutarse
    double tiempo = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Tiempo de compilacion = %lf\n", tiempo);

    return 0;
}