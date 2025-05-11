#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <cstring>

int n;

#define MAX_LINE_LENGTH 100


float** tabla_tridiagonal(float **diff){
    int rsize= (n-2);
	float **tridi = (float **) malloc(rsize * sizeof(float *));
    if (tridi == NULL) {
        printf("Error: No se pudo asignar memoria.\n");
        system("pause");
        exit(EXIT_FAILURE);
    }
    
    for(int i = 0; i < rsize; i++) {
        tridi[i] = (float *) malloc(rsize * sizeof(float));
        if (tridi[i] == NULL) {
            printf("Error: No se pudo asignar memoria para la fila %d.\n", i);
            system("pause");
            exit(EXIT_FAILURE);
        }
        for(int j = 0; j < rsize; j++) {
            tridi[i][j] = 0.0f;
        }
    }
	
	for(int i=0;i<rsize;i++){
		for(int j=0; j<rsize;j++){
			
			tridi[i][i]=2*(diff[i][0]+diff[i+1][0]);
			if(i<(n-3)){
				tridi[i][i+1]=diff[i+1][0];
				tridi[i+1][i]=diff[i+1][0];	
			}		
		}
	}

	return tridi;
}

void leertabla(float **matriz, int n) {
    printf("| Etapa de lectura del conjunto de puntos |");
    int i;
    for(i = 0; i < n; i++) {
        printf("\nIngresa el dato x%d de la tabla: ", i);
        if (scanf("%f", &matriz[i][0]) != 1) {
            printf("Entrada invalida. Por favor, ingrese un numero flotante.\n");
            while(getchar() != '\n');
            matriz[i][0] = 0.0;
        }
        printf("\nIngresa el dato f(x%d) de la tabla: ", i);
        do{
            if (scanf("%f", &matriz[i][1]) != 1) {
                printf("Entrada invalida. Por favor, ingrese un numero flotante.\n");
                while(getchar() != '\n');
                matriz[i][1] = 0.0;
            }
        }while(matriz[i][1]==0);

    }
}

float** init_tabla(){
    char opc,nombreArchivo[100];
    int error;

    printf("\nPara empezar es vital que me digas si quieres iniciar la tabla de datos por medio de un archivo\n si es asi teclea S o s: ");
    scanf("%c", &opc);
        if (opc=='s'||opc=='S') {
            fflush(stdin);
            printf("\nAdvertencia: El archivo debera tener unicamente elementos xi y yi separados por | y nada mas usuario, no puedes escribir nada mas en el archivo que dos numeros separados por | por cada linea\nTampoco pueden existir lineas o espacios sin nada");
            printf("\nAhora si ingresa el nombre del archivo (debe encontrarse en la misma direccion o debes poner la direccion completa del archivo usando backslash): ");

            fgets(nombreArchivo, sizeof(nombreArchivo), stdin);
            fflush(stdin);
            nombreArchivo[strcspn(nombreArchivo, "\n")] = '\0';


            FILE* archivo = fopen(nombreArchivo, "r");
            if (archivo == NULL) {
                printf("Error al abrir el archivo, ingresa el nombre o direccion de otro archivo que sea valido.\n");
                exit(0);
                return NULL;

            }
            char linea[MAX_LINE_LENGTH];
            n = 0;
            while (fgets(linea, sizeof(linea), archivo) != NULL) {
                (n)++;
            }
            rewind(archivo);

            //Matriz tabla de puntos
            float **tabla = (float **) malloc(n * sizeof(float *));
            if (tabla == NULL) {
                printf("Error: No se pudo asignar memoria.\n");
                system("pause");
                fclose(archivo);
                exit(EXIT_FAILURE);
            }

            for(int i = 0; i < n; i++) {
                tabla[i] = (float *) malloc(2 * sizeof(float));
                if (tabla[i] == NULL) {
                    printf("Error: No se pudo asignar memoria para la fila %d.\n", i);
                    system("pause");
                    fclose(archivo);
                    exit(EXIT_FAILURE);
                }
            }
            int i = 0;
            while (fgets(linea, sizeof(linea), archivo) != NULL && i < n) {
                linea[strcspn(linea, ",\n")] = '\0';

                char* token = strtok(linea, "|");
                if (token == NULL) continue;
                tabla[i][0] = atof(token);

                token = strtok(NULL, "|");
                if (token == NULL) continue;
                tabla[i][1] = atof(token);

                i++;
            }

            fclose(archivo);
            return tabla;
        }
    fflush(stdin);
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

    //Matriz tabla de puntos
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

    leertabla(tabla,n);
    return tabla;
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

void imprimir_tabla(float **matriz, int n) {
    printf("\nConjunto de puntos:\n");
    printf("|%-10s| %-10s|\n", "xi", "f(xi)");
    for (int i = 0; i < n; i++) {
        printf("|%-10.2f| %-10.2f|\n", matriz[i][0], matriz[i][1]);
    }

}
void imprimir_tabladiff(float **matriz, int n) {
    printf("\nTabla de diferencias divididas:\n");
    printf("|%-10s| %-10s|\n", "hi", "diff_i");
    for (int i = 0; i < n-1; i++) {
        printf("|%-10.2f| %-10.2f|\n", matriz[i][0], matriz[i][1]);
    }
}

void imprimir_matriztridi(float **matriz, int n) {
    int rsize= (n-2);
    printf("\nMatriz Tridiagonal para el sistema:\n");
    for (int i = 0; i < rsize; i++) {
    	for(int j=0;j<rsize;j++){
    	printf("|%-10.2f|", matriz[i][j]);	
    	}
    	printf("\n\n");
    }
}

void imprimir_matrizspline(float **matriz, int n) {
    printf("\nDatos Spline:\n");
    printf("|%-10s| %-10s|%-10s| %-10s|\n", "S_i", "a_i","b_i","c_i");
    for (int i = 0; i < n; i++) {
        for(int j=0;j<4;j++){
            printf("|%-10.2f|", matriz[i][j]);
        }
        printf("\n");
    }
}

float checkreppunto(float ***matriz,int *n) {
    printf("\n| Verificando que no haya un problema con las diferencias |\n");
    printf("\nSi dos puntos estan juntos y tienen el mismo valor en x_i el método tendra problemas....");
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
                    free(*matriz);
                    exit(EXIT_FAILURE);
                }
                *matriz = temp;
                (*n)--;
                i--;
                printf("\nAhora la tabla se ve de la siguiente forma y su tama�o ahora es: %d",*n);
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
	return 1;
}

float **spline(float **mat, float **tabla_diff,int n) {
    int rsize= (n-2);
    //Inicializacion del vector independiente
    float *vi = (float *) malloc(rsize * sizeof(float ));
    if (vi == NULL) {
        printf("Error: No se pudo asignar memoria.\n");
        system("pause");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rsize; ++i) {
        vi[i] = 6 * (tabla_diff[i + 1][1] - tabla_diff[i][1]);
    }

    //Inicializacion del vector X:
    float *x = (float *) malloc(rsize * sizeof(float ));
    if (x == NULL) {
        printf("Error: No se pudo asignar memoria.\n");
        system("pause");
        exit(EXIT_FAILURE);
    }

    //algoritmo de thomas
    for (int i = 1; i < rsize; i++) {
        float m = mat[i][i - 1] / mat[i - 1][i - 1];
        mat[i][i] -= m * mat[i - 1][i];
        vi[i] -= m * vi[i - 1];
    }
    // Paso 2: Sustitución hacia atrás
    x[rsize - 1] = vi[rsize - 1] / mat[rsize - 1][rsize - 1];
    for (int i = rsize - 2; i >= 0; i--) {
        x[i] = (vi[i] - mat[i][i + 1] * x[i + 1]) / mat[i][i];
    }

    //Inicializacion de tabla de Coeficientes Spline completo
    float **splineMat= (float **)malloc(n * sizeof(float *));
    if (splineMat== NULL) {
        printf("Error al asignar memoria para las filas.\n");
        exit(0);
    }
    for (int i = 0; i < n; i++) {
        splineMat[i] = (float *)malloc(4 * sizeof(float));
        if (splineMat[i] == NULL) {
            printf("Error al asignar memoria para la fila %d.\n", i);
            for (int k = 0; k < i; k++) free(splineMat[k]);
            free(splineMat);
            exit(0);
        }
        for (int j = 0; j < 4; j++) splineMat[i][j] = 0.0f;
    }
    //S en spline mat
    for (int i = 0; i < n ; i++) {
        if (i == 0) {
            splineMat[i][0] = 0.0f;  // S_0 = 0 (natural spline)
        } else if (i == n - 1) {
            splineMat[i][0] = 0.0f;  // S_{n-1} = 0 (natural spline)
        } else {
            splineMat[i][0] = x[i - 1];  // S_i = x[i-1]
        }
    }

    //Coeficientes a b y c
    for (int i = 0; i < n - 1; i++) {
        float h = tabla_diff[i][0];
        float S_i = (i == 0) ? 0.0f : x[i - 1];
        float S_ip1 = (i == n - 2) ? 0.0f : x[i];

        splineMat[i][1] = (S_ip1 - S_i) / (6.0f * h);                     // a_i
        splineMat[i][2] = (splineMat[i][0]==0) ? 0 : S_i / 2.0f ;                                     // b_i
        splineMat[i][3] = (tabla_diff[i][1] - (h * (2 * S_i + S_ip1)) / 6.0f); // c_i
        // d_i (f(x_i))
    }
    free(vi);
    free(x);
    return splineMat;

}


float** obtenciondiferencias(float **tabla){
	float **tabla_diferencias= (float **)malloc((n - 1) * sizeof(float *));
    if (tabla_diferencias== NULL) {
        printf("Error al asignar memoria para las filas.\n");
        exit(0);
    }
    for (int i = 0; i < n - 1; i++) {
        tabla_diferencias[i] = (float *)malloc(2 * sizeof(float));
        if (tabla_diferencias[i] == NULL) {
            printf("Error al asignar memoria para la fila %d.\n", i);
            exit(0);
        }
    }
	//diferencias hi
	for(int i=0; i<n-1; i++){
		tabla_diferencias[i][0]=tabla[i+1][0]-tabla[i][0];
		
	}
	
	//diferencias divididas
	for(int i=0; i<n-1; i++){
		tabla_diferencias[i][1]=((tabla[i+1][1]-tabla[i][1])/tabla_diferencias[i][0]);
		
	}
	
	return tabla_diferencias;
}


void imprimir_menuincreiblementeherculeo(){
    printf("METODOS NUMERICOS II");
    printf("\nINTEGRANTES:");
    printf("\nHERRERA HERNANDEZ DIEGO");
    printf("\nVELAZQUEZ MARTINEZ ANTONIO \n");
    printf("Usuario seas bienvenido al mejor programa de....(redoble de tambores)");
    printf("\n------------------------------------------\n");
    printf("APROXIMACION POLINOMIAL POR MEDIO DE SPLINE CUBICOS\n");
    printf("------------------------------------------\n");

}

void imprimir_polinomios(float **tabla,float **matSpline){
    printf("\n|Etapa de Impresion de Polinomios |");
    for(int i=0; i < n-1; ++i) {
        printf("\n Polinomio g%d(x): %-8f(x-%.2f)^3+%-8f(x-%.2f)^2+%-8f(x-%.2f)+%-8f    %.2f <x< %.2f",i, matSpline[i][1],tabla[i][0],matSpline[i][2],tabla[i][0],matSpline[i][3],tabla[i][0],tabla[i][1],tabla[i][0],tabla[i+1][0]);

    }
    printf("\n");
}

int main(){
	char opc,opc2;


    do{
        imprimir_menuincreiblementeherculeo();

        float **tabla= init_tabla();



        imprimir_tabla(tabla,n);
        printf("\nSon correctos los datos?, favor de checarlo de no ser asi..");
        printf("\nQuieres modificar algun punnto de la tabla? (Escribe S si quieres cambiar alguna fila, cualquier otra tecla si no quiere)\n");
        fflush(stdin);
        scanf("%c",&opc);
        if (opc=='S'||opc=='s') {
            mod_tabla(tabla,n);
            }
        imprimir_tabla(tabla,n);
        checkreppunto(&tabla,&n);
        system("pause");
        system("cls");


        printf("| Etapa de obtencion de datos para el spline cubico |");
        //tabla de diferencias, diferencias divididas
        float **tabla_diff=obtenciondiferencias(tabla);
        imprimir_tabladiff(tabla_diff,n);

        float **tridi=tabla_tridiagonal(tabla_diff);

        printf("\nTabla de tridiagonal para sistema de ecuaciones: ");
        imprimir_matriztridi(tridi,n);
        system("pause");

        float **matSpline=spline(tridi,tabla_diff,n);
        imprimir_matrizspline(matSpline,n);
        imprimir_polinomios(tabla,matSpline);

        system("pause");



        for (int i = 0; i < n-1; i++) free(tabla_diff[i]);
        free(tabla_diff);

        for (int i = 0; i < n-2; i++) free(tridi[i]);
        free(tridi);


        for (int i = 0; i < n; i++) free(tabla[i]);
        free(tabla);

        printf("\nQuieres volver a introducir otro conjunto de datos distinto? ");
        fflush(stdin);
        scanf("%c",&opc2);
    }while(opc2=='S'||opc=='s');
    printf("\n Perfeito usuario fue increible trabajar contigo muchas gracias por usar el programa mas pechudo de spline cubicooos! ^^");
    system("pause2.58|9.32\n"
           "4.13|11.43\n"
           "7.18|12.76\n"
           "9.91|12.91\n"
           "12.28|11.95\n"
           "14.36|10.63\n"
           "16.43|7.9\n"
           "21.03|8.46\n"
           "22.55|10.03");
	return 0;
}
