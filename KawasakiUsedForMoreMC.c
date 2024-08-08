//ESTE PROGRAMA DE KAWASAKI SOLO SE HA USADO PARA OBTENER DATOS DE MAYORES PASOS MONTECARLO.

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define N 16// Numero de spins.
#define Tmax 1000000// Pasos Montecarlo

int h=100000;//h es cada cuantos paso montecarlo mido
double x; 
double a=0;//Parámetro de rejalacion, cuanto quiero que mi sistema espere hasta calcular.

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

void fill_boundaries(int s[N][N]) {//Condiciones de contorno en y 
    for(int j = 0; j < N; j++) {
        s[0][j] = 1;
        s[N-1][j] = -1;
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
    FILE* ficheroEnergía = fopen("Energia.txt", "w");
    FILE* ficheroCalor = fopen("Calor.txt", "w");

    int DimensionVector=Tmax/h;
    double magSuperior[DimensionVector];
    double magInferior[DimensionVector];
    double densidadplus[N]= {0};
    double densidadminus[N]= {0};
    double sigma[N];
    double Temp[DimensionVector];
    double Temp2[DimensionVector];
    double calorespecifico;
    int s[N][N];
    initialize_spins(s); //Para magnetización nula.
    //random_magnetization(s); //Para magnetizacion aleatoria
    //fixed_magnetization(s);
    fill_boundaries(s);

for (double T = 1; T < 5; T=T+0.2) {//Esto se activa si se quiere calcular un observable en función de T
        initialize_spins(s); //Mag. Nula
        //random_magnetization(s); //Mag. Aleatoria
        //fixed_magnetization(s); //Magnetización fija
        fill_boundaries(s);
        double conta;
        conta=0;
        double PromedioEnergia;
        double PromedioEnergiaCuadrado;
        PromedioEnergia=0;
        PromedioEnergiaCuadrado=0;
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
                    PromedioEnergiaCuadrado+=calculate_energy(s)*calculate_energy(s);
                }
            }
            //Temp2[t]=PromedioEnergia;
            if(t==499999){
                for (int i = 0; i < N; i++) {
                    for (int j = 0; j < N-1; j++) {
                        fprintf(fichero_out, "%d, ", s[i][j]);
                    }
                    fprintf(fichero_out, "%d \n",  s[i][N-1]);
                }
                fprintf(fichero_out, "\n");
            }
        //MAGNETIZACION
        if(t>a){//Dejo al sistema evolucionar a pasos montecarlo para obtener resultados mas fiables.
            for (int i = 1; i < N-1; i++) {
                for (int j = 0; j < N; j++) {
                    if (i < N/2) {
                        sumSuperior += s[i][j];
                    } else {
                        sumInferior += s[i][j];
                    }
                }
            }
            if((t%h)==0){
            magSuperior[t/h] = sumSuperior / (((N-1)/2) * N);
            magInferior[t/h] = sumInferior / (((N-1)/2) * N);
            }
            }
        double contplus, contminus,PromedioDensidadPlus, PromedioDensidadMinus;
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
            // for(j=0;j<N;j++){//Promedio de columnas
            //     PromedioDensidadPlus+=densidadplus[j]/N;
            //     PromedioDensidadMinus+=densidadminus[j]/N;
        // }
        conta=conta+densidadplus[0];//Voy sumando el valor de la densidad de una columna en un contador.
        //Temp[t]=densidadplus[0];
        }//FinMontecarlo

        double MediaSuperior = 0;
        double MediaInferior = 0;
        double varianzaSup = 0;
        double varianzaInf = 0;
        double varianzaDensidad;
        for (int t = 0; t < DimensionVector; t++) {//Media magnetizacion
            MediaSuperior += magSuperior[t] /(DimensionVector);
            MediaInferior += magInferior[t] /(DimensionVector);
        }

        for (int t = a; t < DimensionVector; t++) {//Varianza magnetizacion
            if(t%h==0){
            varianzaSup += pow(magSuperior[t] - MediaSuperior, 2) / (DimensionVector);
            varianzaInf += pow(magInferior[t] - MediaInferior, 2) / (DimensionVector);
            }
        }

        double errorSup = sqrt(varianzaSup) / sqrt(DimensionVector);
        double errorInf = sqrt(varianzaInf) / sqrt(DimensionVector);
    
        conta=conta/Tmax;//Densidad promediada
        double VarianzaDensidad;
        VarianzaDensidad=0;
        for (int t=0; t<Tmax; t++){//Varianza densidad
            //VarianzaDensidad+=pow(Temp[t] - conta, 2) / (Tmax);
        }
        double errorDensidad = sqrt(VarianzaDensidad) / sqrt(Tmax);

        //Energia Promedio Por Partícula 
        PromedioEnergia=PromedioEnergia/Tmax;//Promedio entre todos los pasos MonteCarlo
        PromedioEnergia=PromedioEnergia/(N*N);//Promedio por partícula
        //Energia Promedio Cuadrado Por Partícula 
        PromedioEnergiaCuadrado=PromedioEnergiaCuadrado/Tmax;//Promedio entre todos los pasos MonteCarlo
        PromedioEnergiaCuadrado=PromedioEnergiaCuadrado/(N*N);//Promedio por partícula

        calorespecifico=(1/(N*N*T))*(PromedioEnergiaCuadrado-PromedioEnergia*PromedioEnergia);

        double VarianzaEnergia;
        VarianzaEnergia=0;
        //for (int t=0; t<Tmax; t++){//Varianza Energía
        //    VarianzaEnergia+=pow(Temp2[t]/(Tmax*2*N) - PromedioEnergia, 2) / (Tmax);
        //}
        //double ErrorEnergia = sqrt(VarianzaEnergia) / sqrt(Tmax);
        for(int i=0;i<N; i++){
            sigma[i]=densidadplus[i]/Tmax;
            fprintf(ficheroDensidad, "%lf, %lf, %d \n", sigma[i], 0.003, i);
        }
        fprintf(ficheroMag, "%lf, %lf, %lf, %lf, %lf\n",MediaSuperior, 3*errorSup, MediaInferior, 3*errorInf, T);
        fprintf(ficheroCalor, "%lf, %lf \n", calorespecifico, T);
        //fprintf(ficheroDensidad, "%lf, %lf, %lf, %lf, %lf \n", conta, 3*errorDensidad, 1-conta, 3*errorDensidad, T); //DensidadPlus, DensidadMinus, 
        //fprintf(ficheroEnergía, "%lf, %lf, %lf \n", PromedioEnergia, 3*ErrorEnergia, T);
        printf("Temperatura terminada");
    }//Fin temperatura

    fclose(fichero_out);
    fclose(ficheroMag);
    fclose(ficheroCalor);
    fclose(ficheroDensidad);
    fclose(ficheroEnergía);

    clock_t end = clock(); // Tiempo que ha tardado en ejecutarse
    double tiempo = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Tiempo de compilacion = %lf\n", tiempo);

    return 0;
}
