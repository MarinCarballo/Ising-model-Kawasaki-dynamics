void Energia(int* s, int n, int m, int a, int b, int c, int d){




}


void IntercambiarPosiciones(int* s, int r, int N, int n, int m, int a, int b, int c, int d){
    int aux;
    //ROTACION ARRIBA
    if (r==0){
        aux=*(s+n*N+m);
        *(s+n*N+m)=*(s+b*N+m);
        *(s+b*N+m)=aux;
    }
    //ROTACION ABAJO
    if (r==1){
        aux=*(s+n*N+m);
        *(s+n*N+m)=*(s+a*N+m);
        *(s+a*N+m)=aux;
    }
    //ROTACION DERECHA
    if (r==2){
        aux=*(s+n*N+m);
        *(s+n*N+m)=*(s+n*N+c);
        *(s+n*N+c)=aux;
    }
    //ROTACION IZQUIERDA
    if (r==3){
        aux=*(s+n*N+m);
        *(s+n*N+m)=*(s+n*N+d);
        *(s+n*N+d)=aux;
    }
}