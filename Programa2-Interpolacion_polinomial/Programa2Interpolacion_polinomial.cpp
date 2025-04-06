#include <stdio.h>
#include <stdlib.h>
#include <cmath>
int n;
float** init_tabla(){
    printf("Bien usuario para empezar, ingresa el numero de filas que tendra la tabla que vas a ingresar: ");
    do{
        if (scanf("%d", &n)!=1) {
            printf("Entrada invalida. Por favor, ingrese un numero entero.\n");
            while(getchar() != '\n');
            if(n<0){
                printf("\n Camara usuario no pueden existir tablas con numero de datos negativos, ponte pilas");
            }
            n=0;
        }
    }while(n<= 0);
    fflush(stdin);
    float **tabla = (float **) malloc(n * sizeof(float *));
    if (tabla == NULL) {
        printf("Error: No se pudo asignar memoria.\n");
        system("pause");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < n; i++) {
        tabla[i] = (float *) malloc(2 * sizeof(float));
        if (tabla[i] == NULL) {
            printf("Error: No se pudo asignar memoria para la fila %d.\n", i);
            system("pause");
            exit(EXIT_FAILURE);
        }
    }

    return tabla;
}

void leertabla(float **matriz, int n) {
    printf("| Etapa de lectura de datos |");
    int i;
    for(i = 0; i < n; i++) {
        printf("\nIngresa el dato x%d de la tabla: ", i);
       	if (scanf("%f", &matriz[i][0]) != 1) {
            printf("Entrada invalida. Por favor, ingrese un numero flotante.\n");
            while(getchar() != '\n');
            matriz[i][0] = 0.0;
        }
        printf("\nIngresa el dato f(x%d) de la tabla: ", i);
        if (scanf("%f", &matriz[i][1]) != 1) {
            printf("Entrada invalida. Por favor, ingrese un numero flotante.\n");
            while(getchar() != '\n');
            matriz[i][1] = 0.0;
        }
    }
}

void imprimir_tabla(float **matriz, int n) {
    printf("\nTabla de datos:\n");
    printf("|%-10s| %-10s|\n", "xi", "f(xi)");
    for (int i = 0; i < n; i++) {
        printf("|%-10.2f| %-10.2f|\n", matriz[i][0], matriz[i][1]);
    }
}
void imprimir_tabla_dff(float **matriz, float **difN, int n) {
    printf("\n\nTabla de Diferencias Finitas:\n");
    printf("| %-8s | %-8s ", "x", "f(x)");
    for(int j = 1; j < n; j++) {
        printf("| df^%df(x) ", j);
    }
    printf("|\n");

    for(int i = 0; i < n; i++) {
        printf("| %-8.2f | %-8.2f ", matriz[i][0], difN[i][0]);

        for(int j = 1; j < n-i; j++) {
            printf("| %-8.2f ", difN[i][j]);
        }

        for(int j = n-i; j < n; j++) {
            printf("| %-8s ", "-");
        }
        printf("|\n");
    }
}

void ordenar_insertion(float **matriz, int n) {
    for (int i = 1; i < n; i++) {
        float k_x = matriz[i][0];
        float k_fx = matriz[i][1];
        int j = i - 1;
        while (j >= 0 && matriz[j][0] > k_x) {
            matriz[j+1][0] = matriz[j][0];
            matriz[j+1][1] = matriz[j][1];
            j--;
        }
        matriz[j+1][0] = k_x;
        matriz[j+1][1] = k_fx;
    }
}

void check_ord(float **matriz, int n){
    printf("\nRoboMetoBot esta verificando si la tabla esta ordenada y si no esta ordenada la ordenara");
    for(int i=0; i<n-1; i++ ){ /*ordenalatabla*/
        if(matriz[i][0]>matriz[i+1][0]) {

            ordenar_insertion(matriz,n);
        }
    }
}

float checkequidis(float ***matriz,int *n) {
    printf("\n| Verificando Equidistancia |\n");
    for (int i = 0; i < *n - 1; i++) {
        if ((*matriz)[i][0] == (*matriz)[i + 1][0]) {
            printf("\nSe encontro un punto repetidooo");
            if ((*matriz)[i][1] == (*matriz)[i + 1][1]) {
                printf("\nTienen el mismo valor para f(xi), asi que solo eliminare el punto repetido");
                free((*matriz)[i + 1]);

                for (int j = i + 1; j < *n - 1; j++) {
                    (*matriz)[j] = (*matriz)[j + 1];
                }
                float **temp = static_cast<float **>(realloc(*matriz, (*n - 1) * sizeof(float *)));
                if (temp == NULL) {
                    printf("Error: No se pudo ajustar la memoria\n");
                    exit(EXIT_FAILURE);
                }
                *matriz = temp;
                (*n)--;
                i--;
                printf("\nAhora la tabla se ve de la siguiente forma y su tamaño ahora es: %d",*n);
                imprimir_tabla(*matriz,*n);
            } else {
                printf("\nEl punto repetido, tiene distintos f(xi)");
                printf("\nQuieres conservar el valor de f(x%d): %.4f o el valor de f(x%d): %.4f?", i, (*matriz)[i][1],
                       i, (*matriz)[i + 1][1]);
                printf("\nElige la opcion 1 o la opcion 2:\n");
                int opc;
                do {
                    if (scanf("%d", &opc) != 1 || (opc != 1 && opc != 2)) {
                        printf("Entrada invalida. Por favor, ingrese un numero entero 1 o 2.\n");
                        while (getchar() != '\n');
                        opc = 0;
                    }
                } while (opc < 0);

                if (opc == 2) {
                    (*matriz)[i][1] = (*matriz)[i + 1][1];
                }
                free((*matriz)[i + 1]);

                for (int j = i + 1; j < *n - 1; j++) {
                    (*matriz)[j] = (*matriz)[j + 1];
                }
                float **temp = static_cast<float **>(realloc(*matriz, (*n - 1) * sizeof(float *)));
                if (temp == NULL) {
                    printf("Error: No se pudo ajustar la memoria\n");
                    exit(EXIT_FAILURE);
                }
                *matriz = temp;
                (*n)--;
                i--;
            }
        }
    }
    if (*n < 3) {
        printf("\nSe necesitan al menos 3 puntos para poder calcular una equidistancia");
        return -1;
    }
    float intervaloref = (*matriz)[1][0] - (*matriz)[0][0];
    bool equidis=true;

    for (int i = 1; i < *n - 1; i++) {
        float intervaloa = (*matriz)[i + 1][0] - (*matriz)[i][0];
        if (fabs(intervaloa - intervaloref) > 0.0001f) {
            equidis = false;
            break;

        }
    }
    if (equidis) {
        printf("\nOjooo, la tabla si es equidistante");
        return fabs(intervaloref);
    } else {
        printf("\nNel, la tabla no es equidistante");
        return -1;
    }
}

float interpolacion_dff_newton(float **matriz, int n,int grado, float interpol_pto, float h){
    float **difN = (float **) malloc(n * sizeof(float *));
    for(int i = 0; i < n; i++) {
        difN[i] = (float *) malloc(n * sizeof(float));
    }
    for(int i=0; i<n;i++){
        difN[i][0]=matriz[i][1];
    }
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < n-i; ++j) {
            difN[j][i]=difN[j+1][i-1]-difN[j][i-1];
        }
    }
    imprimir_tabla_dff(matriz, difN, n);
    float s=(interpol_pto-matriz[0][0])/h;
    /*Calculo del punto apartir del polinomio*/
    float pnx=difN[0][0];
    float s2=1.0;
    float diffx_2;
    float factorial=1;
    for (int i = 1; i < grado+1; ++i) {
        s2*=(s-(i-1));

        factorial*=i;
        diffx_2=difN[0][i]/factorial;
        pnx +=(s2*diffx_2);
    }
    for(int i = 0; i < n; i++) {
        free(difN[i]);
    }
    free(difN);
    return pnx;
}

void mod_tabla(float **tabla,int n){
    int f;
    bool input_valid;
    printf("Ingrese la fila a modificar empezando desde 0: ");
    do{
        if (scanf("%d", &f)!=1) {
            printf("Entrada invalida. Por favor, ingrese un numero entero dentro del rango de 0 a n.\n");
            while (getchar() != '\n');
        }
        if (f < 0 || f >= n) {
            printf("Error: La fila debe estar entre 0 y %d.\n", n-1);
            continue;
        }
        input_valid = 1;
    }while(input_valid!=1);
    printf("\nIngresa el valor de la tabla en [%d][0]: ",f);
    scanf("%f",&tabla[f][0]);
    printf("\nIngresa el valor de la tabla en [%d][0]: ",f);
    scanf("%f",&tabla[f][1]);
}

void imprimir_menusuperpoderoso(){
    printf("METODOS NUMERICOS II");
    printf("\nINTEGRANTES:");
    printf("\nHERRERA HERNANDEZ DIEGO");
    printf("\nVELAZQUEZ MARTINEZ ANTONIO \n");
    printf("Usuario seas bienvenido al mejor programa de....");
    printf("\n------------------------------------------\n");
    printf("METODO DE DIFERENCIAS DIVIDIDAS DE NEWTON\n");
    printf("------------------------------------------\n");

}


int main() {
    int grado,again;
    char opc,opc2,opc_reintentar,backtothemenu,backtothelecture;
    bool cancontinue=false;
    float interpol_pto;

    do{
        float **tabla= init_tabla();
        imprimir_menusuperpoderoso();
        do{
            leertabla(tabla, n);
            imprimir_tabla(tabla,n);
            printf("\nSon correctos los datos?, favor de checarlo no ser asi..");
            printf("\nQuieres modificar alguna fila de la tabla? (Escribe S si quieres cambiar alguna fila, cualquier otra tecla si no quiere)\n");
            fflush(stdin);
            scanf("%c",&opc);
            if (opc=='S'||opc=='s') {
                mod_tabla(tabla,n);
            }

            system("cls");

            printf("\n| Etapa de verificacion de la tabla |\n");
            check_ord(tabla,n);
            imprimir_tabla(tabla,n);
            float h=checkequidis(&tabla,&n);
            do{
                if(h>0){
                    cancontinue=true;
                    printf("\nBien su tabla ya tiene todos los requisitos, o deberia contar con todos los requisitos para poder someterse al metodo de diferencias de newton ^^");
                }
                else{

                    printf("\n");
                    printf("\n  SUSPENSO DRAMATICO!                  ");
                    printf("\n  No pudimos salvar la tabla, ya que no es equidistante :c...");
                    printf("\n  Pero solo usted puede salvarla!        ");
                    printf("\n");
                    printf("\n\nDesea reingresar otra tabla ? (S/Cualquier otra tecla= N)");
                    fflush(stdin);
                    scanf("%c",&opc_reintentar);
                    if (opc_reintentar == 'S'||opc_reintentar == 's') {
                        tabla= init_tabla();
                        leertabla(tabla,n);
                        imprimir_tabla(tabla,n);
                        printf("\nSon correctos los datos?, favor de checarlo no ser asi..");
                        printf("\nQuieres modificar alguna fila de la tabla? (Escribe S si quieres cambiar alguna fila, cualquier otra tecla si no quiere)\n");
                        fflush(stdin);
                        scanf("%c",&opc);
                        if (opc=='S'||opc=='s') {
                            mod_tabla(tabla,n);
                        }
                        check_ord(tabla,n);
                        imprimir_tabla(tabla,n);
                        h=checkequidis(&tabla,&n);
                        imprimir_tabla(tabla,n);
                    }else{
                        printf("\nFinalizando programa...");
                        system("pause");
                        printf("\nLo lamento pero no es posible utilizar newton,asi que adios usuario!");
                        exit(printf("\nFIN"));

                    }
                }
            }while(!cancontinue);
            fflush(stdin);
            printf("\n| Etapa de interpolacion del punto |\n");
            printf("\nIngresa el grado de polinomio: \n" );
            do {
                printf("\nIngrese el grado del polinomio (1 a %d): ", n-1);

                if (scanf("%d", &grado) != 1) {  // Si la entrada no es un número
                    printf("Error: Debe ser un número entero.\n");
                    while(getchar() != '\n');  // Limpiar el búfer
                    grado = -1;
                }
                else if (grado <= 0 || grado > n-1) {  // Si el número está fuera de rango
                    printf("Error: El grado debe estar entre 1 y %d.\n", n-1);
                    grado = -1;
                }
            } while (grado < 0);
            fflush(stdin);
            do{
                printf("\nBien usuario ahora ingrese el punto a interpolar dentro de la tabla: ");
                do{
                    if (scanf("%f", &interpol_pto)!=1) {
                        printf("Entrada invalida. Por favor, ingrese un numero entero.\n");
                        while(getchar() != '\n');
                        if(interpol_pto<0){
                            printf("\n Camara usuario no pueden existir tablas con numero de datos negativos, ponte pilas");
                        }

                        interpol_pto=NULL;
                    }
                    for(int i=0;i<n;i++){
                        if (interpol_pto==tabla[i][0]){
                            printf("\nEl punto a interpolar ya se encuentra en la tabla, por favor ingrese un punto valido usuario..\n");
                            interpol_pto=NULL;
                            break;
                        }
                    }
                }while(interpol_pto==NULL);
                float pnpto=interpolacion_dff_newton(tabla,n,grado,interpol_pto,h);
                printf("\nLa aproximacion del polinomio es: %.4f ", pnpto );

                printf("\nQuiere interpolar otro punto a partir del polinomio de Newton de grado %d? ",grado);
                printf("\nIngrese S o s si quiere interpolar otro punto: ");
                fflush(stdin);
                scanf("%c",&opc2);
                if(opc2=='s'||opc2=='S'){
                    again=1;
                }else{
                    again=0;
                }
            }while(again==1);

            system("cls");

            printf("\nDesea ingresar otra tabla de tamano %d?: ",n);
            printf("\nIngrese S o s: ");
            fflush(stdin);
            scanf("%c",&backtothelecture);
        }while(backtothelecture=='S'||backtothelecture=='s');
        for(int i = 0; i < n; i++) {
            free(tabla[i]);
        }
        free(tabla);

        system("cls");

        printf("\nSi quiere volver al menu principal ingrese S o s unicamente...\n");
        printf("Al elegir esta opcion usted ingresara otra tabla completamente nueva\n");
        fflush(stdin);
        scanf("%c",&backtothemenu);

    }while(backtothemenu=='S'||backtothemenu=='s');

    printf("\nFinalizando programa...");
    printf("\nMuy bien usuario, espero haber sido de ayuda gracias por usar este programa ^^\n");
    system("pause");
    return 0;
}

