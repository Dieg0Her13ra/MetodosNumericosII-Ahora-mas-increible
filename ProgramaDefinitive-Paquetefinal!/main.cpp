#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <math.h>
#include <string>
#include <cstring>

int n;
#define N2 3 //para sistemas de 3x3
#define N 2 //para sistemas de 2x2 por eso es N2
#define MAX_ITER 100
#define MAX_LINE_LENGTH 100
#define MAX_J 15
#define MAX_a (-50)
#define Max_b 50

double a=0,b=0;
int digpresicion=0;
//Programa 1

void F1(double x[N], double Fx[N]) {
    Fx[0] = (x[0]*x[0]) + (x[0] * x[1]) + (2*x[1]*x[1]) - 5; // x^2 + xy + 2y^2 -5 = 0
    Fx[1] = (5*x[1]) - (2*x[0]*x[1]*x[1]) + 3; // 5y - 2xy^2 + 3 = 0
}

void F2(double x[N], double Fx[N]) {
    Fx[0] = (x[0]*x[0]) - (3*x[1]*x[1]) - 10; // x^2 - 3y^2 - 10 = 0
    Fx[1] = (2*x[1]*x[1]) - (3*x[0]*x[1]) + 1; // 2y^2 - 3xy + 1 = 0
}

void F3(double x[N2], double Fx[N2]) {
    Fx[0] = (3*x[0]*x[0]) + (x[1] * x[1]) - 8*x[1] + (2*x[2]*x[2]) - 5; // 3x^2 + y^2 - 8y + 2z^2 - 5 = 0
    Fx[1] = (-2*x[0]*x[0]) - 12*x[0] + (x[1]*x[1]) - (3*x[2]*x[2]) + 10; // -2x^2 - 12x + y^2 - 3z^2 + 10 = 0
    Fx[2] = -x[0] + 2*x[1] + 5*x[2]; // -x + 2y + 5z = 0
}

void F4(double x[N2], double Fx[N2]) {
    Fx[0] = (x[0]*x[0]) + (x[1] * x[1]) - (2*x[2]) + 3; // x^2 + y^2 - 2z + 3 = 0
    Fx[1] = x[0] + x[1] + x[2] - 5; // x + y + z - 5 = 0
    Fx[2] = (x[0]*x[0]) - (x[1]*x[1]) + (x[2]*x[2]) - 9; // x^2 - y^2 + z^2 -9 = 0
}

double norm(double v[], int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += v[i] * v[i];
    }
    return sqrt(sum);
}

void matvec2(double A[N][N], double v[N], double result[N]);
void matvec3(double A[N2][N2], double v[N2], double result[N2]);

// Función para multiplicar matriz por vector (2x2)
void matvec2(double A[N][N], double v[N], double result[N]) {
    for (int i = 0; i < N; i++) {
        result[i] = 0.0;
        for (int j = 0; j < N; j++) {
            result[i] += A[i][j] * v[j];
        }
    }
}

// Función para multiplicar matriz por vector (3x3)
void matvec3(double A[N2][N2], double v[N2], double result[N2]) {
    for (int i = 0; i < N2; i++) {
        result[i] = 0.0;
        for (int j = 0; j < N2; j++) {
            result[i] += A[i][j] * v[j];
        }
    }
}

void print_system(int system_num) {
    printf("\nSistema de ecuaciones %d:\n", system_num);
    switch(system_num) {
        case 1:
            printf("\nSistema 1:");
            printf("\nf1(x,y): x^2 + xy + 2y^2 -5 = 0");
            printf("\nf2(x,y): 5y - 2xy^2 + 3 = 0");
            break;
        case 2:
            printf("\nSistema 2:");
            printf("\nf1(x,y): x^2 - 3y^2 - 10 = 0");
            printf("\nf2(x,y): 2y^2 - 3xy + 1 = 0");
            break;
        case 3:
            printf("\nSistema 3:");
            printf("\nf1(x,y,z): 3x^2 + y^2 - 8y + 2z^2 - 5 = 0");
            printf("\nf2(x,y,z): -2x^2 - 12x + y^2 - 3z^2 + 10 = 0");
            printf("\nf3(x,y,z): -x + 2y + 5z = 0");
            break;
        case 4:
            printf("\nSistema 4:");
            printf("\nf1(x,y,z): x^2 + y^2 - 2z + 3 = 0");
            printf("\nf2(x,y,z): x + y + z - 5 = 0");
            printf("\nf3(x,y,z): x^2 - y^2 + z^2 -9 = 0");

            break;
    }
    printf("\n");
}

// Metodo broyden2x2
void broyden2(void (*F)(double[N], double[N]), double x0[N], double x[N],double tol, int max_iter, int system_num) {
    double Fx[N], Fx0[N], s[N], y[N], B[N][N], Bs[N];
    double normFx;
    int iter = 0;


    print_system(system_num);
    printf("Iteración  x           y           Error\n");
    printf("-----------------------------------------\n");

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            B[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }

    F(x0, Fx0);
    normFx = sqrt(Fx0[0]*Fx0[0] + Fx0[1]*Fx0[1]);

    for (int i = 0; i < N; i++) {
        x[i] = x0[i];
    }

    // Mostrar aproximación inicial
    printf("%-9d %-10.6f %-10.6f %-10.6f\n", iter, x[0], x[1], normFx);

    while (normFx > tol && iter < max_iter) {

        double det = B[0][0]*B[1][1] - B[0][1]*B[1][0];
        if (fabs(det) < 1e-10) {
            printf("Matriz singular.\n");
            return;
        }

        s[0] = (B[1][1]*(-Fx0[0]) - B[0][1]*(-Fx0[1]))/det;
        s[1] = (-B[1][0]*(-Fx0[0]) + B[0][0]*(-Fx0[1]))/det;


        x[0] += s[0];
        x[1] += s[1];

        F(x, Fx);
        normFx = sqrt(Fx[0]*Fx[0] + Fx[1]*Fx[1]);


        y[0] = Fx[0] - Fx0[0];
        y[1] = Fx[1] - Fx0[1];


        Fx0[0] = Fx[0];
        Fx0[1] = Fx[1];


        matvec2(B, s, Bs);

        double sTs = s[0]*s[0] + s[1]*s[1];
        if (sTs < 1e-10) break;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                B[i][j] += (y[i] - Bs[i]) * s[j] / sTs;
            }
        }

        iter++;
        printf("%-9d %-10.6f %-10.6f %-10.6f\n", iter, x[0], x[1], normFx);
    }
    printf("\n");
    if (normFx <= tol) {
        printf("Solución encontrada:\n");
        printf("x = %.8f, y = %.8f\n", x[0], x[1]);
        printf("Error final: %.8f\n", normFx);
    } else {
        printf("No converge después de %d iteraciones.\n", max_iter);
    }
}

// Metodo broyden3x3
void broyden3(void (*F)(double[N2], double[N2]), double x0[N2], double x[N2],double tol, int max_iter, int system_num) {
    double Fx[N2], Fx0[N2], s[N2], y[N2], B[N2][N2], Bs[N2];
    double normFx;
    int iter = 0;

    print_system(system_num);
    printf("Iteración  x           y           z           Error\n");
    printf("-----------------------------------------\n");

    // Inicialización de B (matriz identidad)
    for (int i = 0; i < N2; i++) {
        for (int j = 0; j < N2; j++) {
            B[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }

    // Evaluación inicial
    F(x0, Fx0);
    normFx = sqrt(Fx0[0]*Fx0[0] + Fx0[1]*Fx0[1] + Fx0[2]*Fx0[2]);

    // Copiar x0 a x
    for (int i = 0; i < N2; i++) {
        x[i] = x0[i];
    }

    while (normFx > tol && iter < max_iter) {
        // Resolver B*s = -F(x) (usando eliminación gaussiana simple)

        // Crear matriz aumentada
        double Aug[N2][N2+1];
        for (int i = 0; i < N2; i++) {
            for (int j = 0; j < N2; j++) {
                Aug[i][j] = B[i][j];
            }
            Aug[i][N2] = -Fx0[i];
        }

        // Eliminación gaussiana
        for (int k = 0; k < N2; k++) {
            // Pivoteo parcial
            int max_row = k;
            for (int i = k+1; i < N2; i++) {
                if (fabs(Aug[i][k]) > fabs(Aug[max_row][k])) {
                    max_row = i;
                }
            }

            // Intercambiar filas
            if (max_row != k) {
                for (int j = 0; j <= N2; j++) {
                    double temp = Aug[k][j];
                    Aug[k][j] = Aug[max_row][j];
                    Aug[max_row][j] = temp;
                }
            }

            // Eliminación
            for (int i = k+1; i < N2; i++) {
                double factor = Aug[i][k] / Aug[k][k];
                for (int j = k; j <= N2; j++) {
                    Aug[i][j] -= factor * Aug[k][j];
                }
            }
        }

        // Sustitución hacia atrás
        for (int i = N2-1; i >= 0; i--) {
            s[i] = Aug[i][N2];
            for (int j = i+1; j < N2; j++) {
                s[i] -= Aug[i][j] * s[j];
            }
            s[i] /= Aug[i][i];
        }

        // Actualizar x
        for (int i = 0; i < N2; i++) {
            x[i] += s[i];
        }

        // Evaluar F en el nuevo punto
        F(x, Fx);
        normFx = sqrt(Fx[0]*Fx[0] + Fx[1]*Fx[1] + Fx[2]*Fx[2]);

        // Calcular y = F(x_new) - F(x_old)
        for (int i = 0; i < N2; i++) {
            y[i] = Fx[i] - Fx0[i];
        }

        // Actualizar Fx0
        for (int i = 0; i < N2; i++) {
            Fx0[i] = Fx[i];
        }

        // Actualizar B usando la fórmula de Broyden
        matvec3(B, s, Bs);

        double sTs = s[0]*s[0] + s[1]*s[1] + s[2]*s[2];
        if (sTs < 1e-10) {
            printf("Paso demasiado pequeño.\n");
            break;
        }

        for (int i = 0; i < N2; i++) {
            for (int j = 0; j < N2; j++) {
                B[i][j] += (y[i] - Bs[i]) * s[j] / sTs;
            }
        }

        iter++;
        printf("%-9d %-10.6f %-10.6f %-10.6f %-10.6f\n",iter, x[0], x[1], x[2], normFx);

    }
    printf("\n");
    if (normFx <= tol) {
        printf("Solución encontrada:\n");
        printf("x = %.8f\n", x[0]);
        printf("y = %.8f\n", x[1]);
        printf("z = %.8f\n", x[2]);
        printf("Error final: %.8e\n", normFx);
    } else {
        printf("No converge después de %d iteraciones.\n", max_iter);
        printf("Última aproximación:\n");
        printf("x = %.8f\n", x[0]);
        printf("y = %.8f\n", x[1]);
        printf("z = %.8f\n", x[2]);
        printf("Error final: %.8e\n", normFx);
    }

}


//Programa 2
double** tabla_tridiagonal(double **diff){
    int rsize= (n-2);
    double **tridi = (double **) malloc(rsize * sizeof(double *));
    if (tridi == NULL) {
        printf("Error: No se pudo asignar memoria.\n");
        system("pause");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < rsize; i++) {
        tridi[i] = (double *) malloc(rsize * sizeof(double));
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

void leertabla(double **matriz, int n) {
    printf("| Etapa de lectura del conjunto de puntos |");
    int i;
    for(i = 0; i < n; i++) {
        printf("\nIngresa el dato x%d de la tabla: ", i);
        if (scanf("%lf", &matriz[i][0]) != 1) {
            printf("Entrada invalida. Por favor, ingrese un numero flotante.\n");
            while(getchar() != '\n');
            matriz[i][0] = 0.0;
        }
        printf("\nIngresa el dato f(x%d) de la tabla: ", i);
        do{
            if (scanf("%lf", &matriz[i][1]) != 1) {
                printf("Entrada invalida. Por favor, ingrese un numero flotante.\n");
                while(getchar() != '\n');
                matriz[i][1] = 0.0;
            }
        }while(matriz[i][1]==0);

    }
}

double** init_table2(){
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
        double **tabla = (double **) malloc(n * sizeof(double *));
        if (tabla == NULL) {
            printf("Error: No se pudo asignar memoria.\n");
            system("pause");
            fclose(archivo);
            exit(EXIT_FAILURE);
        }

        for(int i = 0; i < n; i++) {
            tabla[i] = (double *) malloc(2 * sizeof(double ));
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
    double **tabla = (double **) malloc(n * sizeof(double *));
    if (tabla == NULL) {
        printf("Error: No se pudo asignar memoria.\n");
        system("pause");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < n; i++) {
        tabla[i] = (double *) malloc(2 * sizeof(double ));
        if (tabla[i] == NULL) {
            printf("Error: No se pudo asignar memoria para la fila %d.\n", i);
            system("pause");
            exit(EXIT_FAILURE);
        }
    }

    leertabla(tabla,n);
    return tabla;
}

void mod_tabla(double **tabla,int n){
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
    scanf("%lf",&tabla[f][0]);
    printf("\nIngresa el valor de la tabla en [%d][0]: ",f);
    scanf("%lf",&tabla[f][1]);
}

void imprimir_tabla(double **matriz, int n) {
    printf("\nConjunto de puntos:\n");
    printf("|%-10s| %-10s|\n", "xi", "f(xi)");
    for (int i = 0; i < n; i++) {
        printf("|%-10.2f| %-10.2f|\n", matriz[i][0], matriz[i][1]);
    }

}
void imprimir_tabladiff(double **matriz, int n) {
    printf("\nTabla de diferencias divididas:\n");
    printf("|%-10s| %-10s|\n", "hi", "diff_i");
    for (int i = 0; i < n-1; i++) {
        printf("|%-10.2f| %-10.2f|\n", matriz[i][0], matriz[i][1]);
    }
}

void imprimir_matriztridi(double **matriz, int n) {
    int rsize= (n-2);
    printf("\nMatriz Tridiagonal para el sistema:\n");
    for (int i = 0; i < rsize; i++) {
        for(int j=0;j<rsize;j++){
            printf("|%-10.2f|", matriz[i][j]);
        }
        printf("\n\n");
    }
}

void imprimir_matrizspline(double **matriz, int n) {
    printf("\nDatos Spline:\n");
    printf("|%-10s| %-10s|%-10s| %-10s|\n", "S_i", "a_i","b_i","c_i");
    for (int i = 0; i < n; i++) {
        for(int j=0;j<4;j++){
            printf("|%-10.2f|", matriz[i][j]);
        }
        printf("\n");
    }
}

double checkreppunto(double ***matriz,int *n) {
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
                double **temp = static_cast<double **>(realloc(*matriz, (*n - 1) * sizeof(double *)));
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
                double **temp = static_cast<double **>(realloc(*matriz, (*n - 1) * sizeof(double *)));
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

double **spline(double **mat, double **tabla_diff,int n) {
    int rsize= (n-2);
    //Inicializacion del vector independiente
    double *vi = (double *) malloc(rsize * sizeof(double ));
    if (vi == NULL) {
        printf("Error: No se pudo asignar memoria.\n");
        system("pause");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rsize; ++i) {
        vi[i] = 6 * (tabla_diff[i + 1][1] - tabla_diff[i][1]);
    }

    //Inicializacion del vector X:
    double *x = (double *) malloc(rsize * sizeof(double ));
    if (x == NULL) {
        printf("Error: No se pudo asignar memoria.\n");
        system("pause");
        exit(EXIT_FAILURE);
    }

    //algoritmo de thomas
    for (int i = 1; i < rsize; i++) {
        double m = mat[i][i - 1] / mat[i - 1][i - 1];
        mat[i][i] -= m * mat[i - 1][i];
        vi[i] -= m * vi[i - 1];
    }
    // Paso 2: Sustitución hacia atrás
    x[rsize - 1] = vi[rsize - 1] / mat[rsize - 1][rsize - 1];
    for (int i = rsize - 2; i >= 0; i--) {
        x[i] = (vi[i] - mat[i][i + 1] * x[i + 1]) / mat[i][i];
    }

    //Inicializacion de tabla de Coeficientes Spline completo
    double **splineMat= (double **)malloc(n * sizeof(double *));
    if (splineMat== NULL) {
        printf("Error al asignar memoria para las filas.\n");
        exit(0);
    }
    for (int i = 0; i < n; i++) {
        splineMat[i] = (double *)malloc(4 * sizeof(double));
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
        double h = tabla_diff[i][0];
        double S_i = (i == 0) ? 0.0f : x[i - 1];
        double S_ip1 = (i == n - 2) ? 0.0f : x[i];

        splineMat[i][1] = (S_ip1 - S_i) / (6.0f * h);                     // a_i
        splineMat[i][2] = (splineMat[i][0]==0) ? 0 : S_i / 2.0f ;                                     // b_i
        splineMat[i][3] = (tabla_diff[i][1] - (h * (2 * S_i + S_ip1)) / 6.0f); // c_i
        // d_i (f(x_i))
    }
    free(vi);
    free(x);
    return splineMat;

}


double** obtenciondiferencias(double **tabla){
    double **tabla_diferencias= (double **)malloc((n - 1) * sizeof(double *));
    if (tabla_diferencias== NULL) {
        printf("Error al asignar memoria para las filas.\n");
        exit(0);
    }
    for (int i = 0; i < n - 1; i++) {
        tabla_diferencias[i] = (double *)malloc(2 * sizeof(double ));
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

void imprimir_polinomios(double **tabla,double **matSpline){
    printf("\n|Etapa de Impresion de Polinomios |");
    for(int i=0; i < n-1; ++i) {
        printf("\n Polinomio g%d(x): %-8f(x-%.2f)^3+%-8f(x-%.2f)^2+%-8f(x-%.2f)+%-8f    %.2f <x< %.2f",i, matSpline[i][1],tabla[i][0],matSpline[i][2],tabla[i][0],matSpline[i][3],tabla[i][0],tabla[i][1],tabla[i][0],tabla[i+1][0]);

    }
    printf("\n");
}

//Programa 3
double funcion1(double x){
    return (pow(x,4)* sqrt(3+2*pow(x,2)))/3;
}

double funcion2(double x){
    return pow(x,5)/ pow((pow(x,2)+4),1.0/5.0);
}

void imprimir_tabla_romberg(double matriz[MAX_J][MAX_J], int niveles, double a, double b) {
    printf("\nTabla de Romberg:\n");
    printf("| %-2s | %-6s | %-16s | %-10s | %-10s | %-10s | %-10s |\n",
           "k", "h", "I(h) R. trapecio", "  O(h^4)  ", "  O(h^6)  ", "  O(h^8)  ", "  O(h^10)  ");
    printf("|----|--------|-------------------|-----------|-----------|-----------|-----------|\n");

    for(int k = 1; k <= niveles; k++) {
        printf("| %-2d | %-6.4f | %-17.8f ", k, (b-a)/pow(2, k-1), matriz[k-1][0]);

        // Imprimir las columnas de extrapolación
        for(int col = 1; col < niveles && col <= 5; col++) {
            if(k > col) {
                printf("| %-9.8f ", matriz[k-1][col]);
            } else {
                printf("");
            }
        }
        printf("|\n");
    }
}


void leerintervaloypresicion(){
    printf("\nIngrese el intervalo [a,b]: ");
    printf("\n tenga en cuenta que los valores maximos para a es -50 y para b es 50");
    printf("\n Ingrese a: ");
    do {
        if (scanf("%lf", &a) != 1|| a<MAX_a||a>Max_b-1) {
            printf("Entrada invalida. Por favor, ingrese un numero que se ajuste a los limites.\n");
            while (getchar() != '\n');
            a = NULL;
        }
    } while (a==NULL);
    printf("\n Ingrese b: ");
    do {
        if (scanf("%lf", &b) != 1 || b>Max_b||b<MAX_a+1|| b<a) {
            printf("Entrada invalida. Por favor, ingrese un numero se ajuste a los limites.\n");
            while (getchar() != '\n');
            b = NULL;
        }
    } while (b==NULL);
    printf("\nIngrese el numero de digitos de presicion: ");
    do {
        if (scanf("%d", &digpresicion) != 1||digpresicion<0) {
            printf("Entrada invalida. Por favor, ingrese un numero.\n");
            while (getchar() != '\n');
            digpresicion = 0;
        }
    } while (digpresicion<0);
}

double trapecio(double a, double b, int J,double (*f)(double)){
    if(J==0){
        return (b-a) * (f(a) + f(b) )/2.0;
    }
    int n = 1 << (J-1);
    double h = (b-a) / (1 << J);
    double sum=0;
    for (int l = 1; l <= n; l++ ){
        double x = a + (2*l - 1 )* h;
        sum += f(x);
    }

    return 0.5 * trapecio(a,b,J-1,f)+h*sum;
}

double romberg(double (*f)(double), double a, double b, double tolerancia){

    double tabla[MAX_J][MAX_J];
    int J = 0;
    tabla[0][0] = trapecio( a, b, 0, f);

    do {
        J++;
        tabla[J][0] = trapecio(a, b, J, f);

        for (int k = 1; k <= J; k++) {
            double factor = pow(4, k);
            tabla[J][k] = (factor * tabla[J][k - 1] - tabla[J - 1][k - 1]) / (factor - 1);
        }

        if (J >= 1 && fabs(tabla[J][J] - tabla[J - 1][J - 1]) < tolerancia) {
            break;
        }
    } while (J < MAX_J - 1);

    imprimir_tabla_romberg(tabla,J+1,a,b);

    return tabla[J][J];

}

int main(){
    int opc0,opc01;
    char opc,opc2, opc3, opc4, opcreturn;
    double tol;
    int max_iter;
    //programaIntegracion
    int opc1;
    double tolerancia, resultado;
    double (*funciones[])(double) = {funcion1, funcion2};


    do {
        system("cls");
        printf("METODOS NUMERICOS II");
        printf("\nINTEGRANTES:");
        printf("\nHERRERA HERNANDEZ DIEGO");
        printf("\nVELAZQUEZ MARTINEZ ANTONIO \n");
        printf("Usuario seas bienvenido al mejor programa de....(redoble de tambores, explosiones de cohetes, musica a todo volumeeen)");
        printf("\n------------------------------------------\n");
        printf("PAQUETE DE PROGRAMAS BIEN METODICOS BIEN NUMERICOS 2!!!!\n");
        printf("------------------------------------------\n");
        printf("\nLes venimos ofreciendo estos programas: ");
        printf("\nSOLUCION DE ECUACIONES NO LINEALES POR METODO DE BROYDEN");
        printf("\nDIFERENCIAS DIVIDIDAS, SPLINE CUBICO IMPLEMENTADO");
        printf("\nINTEGRACION NUMERICA");

        printf("\nEliga con una opcion 1-3 para que empiece el caos, para que empiece la destruccioon (bueno en este caso los metodos numericos verda?):\n");
        do {
            if (scanf("%d", &opc0) != 1 || (opc0 < 0 || opc0 > 3)) {
                printf("Entrada inválida. Ingrese 1 o 2.\n");
                while (getchar() != '\n');
                opc0 = 0;
            }
        } while (opc0 == 0);
        switch(opc0) {
            case 1:
                double x0[N], x[N];
                double x0_3[N2], x_3[N2];
                do{
                    system("cls");
                    printf("\n------------------------------------------\n");
                    printf("SOLUCION DE ECUACIONES NO LINEALES POR METODO DE BROYDEN\n");
                    printf("------------------------------------------\n");

                    printf("\nElige uno de los 4 sistemas de ecuaciones no lineales para poder solucionarlo por el metodo de broyden usuario!");
                    printf("\nSistema 1:");
                    printf("\nf1(x,y): x^2 + xy + 2y^2 -5 = 0");
                    printf("\nf2(x,y): 5y - 2xy^2 + 3 = 0");
                    printf("\n");
                    printf("\nSistema 2:");
                    printf("\nf1(x,y): x^2 - 3y^2 - 10 = 0");
                    printf("\nf2(x,y): 2y^2 - 3xy + 1 = 0");
                    printf("\n");
                    printf("\nSistema 3:");
                    printf("\nf1(x,y,z): 3x^2 + y^2 - 8y + 2z^2 - 5 = 0");
                    printf("\nf2(x,y,z): -2x^2 - 12x + y^2 - 3z^2 + 10 = 0");
                    printf("\nf3(x,y,z): -x + 2y + 5z = 0");
                    printf("\n");
                    printf("\nSistema 4:");
                    printf("\nf1(x,y,z): x^2 + y^2 - 2z + 3 = 0");
                    printf("\nf2(x,y,z): x + y + z - 5 = 0");
                    printf("\nf3(x,y,z): x^2 - y^2 + z^2 -9 = 0");

                    printf("\nFavor de elegir una opcion de las 4 disponibles para que el programa pueda empezar a hacer su magia increible: ");
                    do {
                        if (scanf("%d", &opc01) != 1 || (opc01 < 0 || opc01 > 4)) {
                            printf("Entrada inválida. Ingrese un sistema de ecuaciones existente.\n");
                            while (getchar() != '\n');
                            opc01 = 0;
                        }
                    } while (opc01 == 0);

                    printf("\nAhora ingrese la tolerancia que quisiese manejar para el metodo, es importante que recuerde que la tolerancia debe ser menor a 1 : ");
                    printf("\nUn ejemplo de formato valido podria ser 0.00000x: ");
                    do {
                        if (scanf("%lf", &tol) != 1 || (tol > 1) ) {
                            printf("Entrada inválida.\n");
                            while (getchar() != '\n');
                            tol = 1;
                        }
                    } while (tol == 1);

                    printf("\nAhora ingrese el numero maximo de iteraciones, recuerda no poner ninguna barrabazada usuario sea prudente : ");
                    do {
                        if (scanf("%d", &max_iter) != 1 || (max_iter < 1)) {
                            printf("Entrada inválida.\n");
                            while (getchar() != '\n');
                            max_iter = -1;
                        }
                    } while (max_iter == -1);

                    switch (opc01) {
                        case 1:
                            do{
                                printf("Ingrese la aproximación inicial (x0 y0): ");
                                do {
                                    if (scanf("%lf", &x0[0]) != 1 ) {
                                        printf("Entrada inválida.\n");
                                        while (getchar() != '\n');
                                        x0[0] = NULL;
                                    }
                                    if (scanf("%lf", &x0[1]) != 1 ) {
                                        printf("Entrada inválida.\n");
                                        while (getchar() != '\n');
                                        x0[1] = NULL;
                                    }
                                } while (x0[0] == NULL||x0[1]== NULL);
                                broyden2(F1, x0, x, tol, max_iter,opc01);
                                printf("Solución aproximada encontrada usuario!!: x = %lf, y = %lf\n", x[0], x[1]);
                                printf("\nQuieres intentar con otro vector inicial? ");
                                printf("\nIngresa S o s si es que si: ");
                                fflush(stdin);
                                scanf("%c",&opc4);
                            }while(opc4=='s'||opc4=='S');

                            break;
                        case 2:
                            do {
                                printf("Ingrese la aproximación inicial (x0 y0): ");
                                do {
                                    if (scanf("%lf", &x0[0]) != 1 ) {
                                        printf("Entrada inválida.\n");
                                        while (getchar() != '\n');
                                        x0[0] = NULL;
                                    }
                                    if (scanf("%lf", &x0[1]) != 1 ) {
                                        printf("Entrada inválida.\n");
                                        while (getchar() != '\n');
                                        x0[1] = NULL;
                                    }
                                } while (x0[0] == NULL||x0[1]== NULL);
                                broyden2(F2, x0, x, tol, max_iter,opc01);
                                printf("Solución aproximada encontrada usuario!!: x = %lf, y = %lf\n", x[0], x[1]);
                                printf("\nIngresa S o s si es que si: ");
                                fflush(stdin);
                                scanf("%c",&opc4);
                            }while(opc4=='s'||opc4=='S');
                            break;
                        case 3:
                            do {
                                printf("Ingrese la aproximación inicial (x0 y0 z0): ");
                                do {
                                    if (scanf("%lf", &x0_3[0]) != 1 ) {
                                        printf("Entrada inválida.\n");
                                        while (getchar() != '\n');
                                        x0_3[0] = NULL;
                                    }
                                    if (scanf("%lf", &x0_3[1]) != 1 ) {
                                        printf("Entrada inválida.\n");
                                        while (getchar() != '\n');
                                        x0_3[1] = NULL;
                                    }
                                    if (scanf("%lf", &x0_3[2]) != 1 ) {
                                        printf("Entrada inválida.\n");
                                        while (getchar() != '\n');
                                        x0_3[2] = NULL;
                                    }
                                } while (x0_3[0] == NULL||x0_3[1]== NULL||x0_3[2] == NULL);
                                broyden3(F3, x0_3, x_3, tol, max_iter,opc01);
                                printf("Solución aproximada encontrada usuario!!: x = %lf, y = %lf, z = %lf\n", x_3[0], x_3[1], x_3[2]);
                                printf("\nIngresa S o s si es que si: ");
                                fflush(stdin);
                                scanf("%c",&opc4);
                            }while(opc4=='s'||opc4=='S');

                            break;
                        case 4:
                            do {
                                printf("Ingrese la aproximación inicial (x0 y0 z0): ");
                                do {
                                    if (scanf("%lf", &x0_3[0]) != 1 ) {
                                        printf("Entrada inválida.\n");
                                        while (getchar() != '\n');
                                        x0_3[0] = NULL;
                                    }
                                    if (scanf("%lf", &x0_3[1]) != 1 ) {
                                        printf("Entrada inválida.\n");
                                        while (getchar() != '\n');
                                        x0_3[1] = NULL;
                                    }
                                    if (scanf("%lf", &x0_3[2]) != 1 ) {
                                        printf("Entrada inválida.\n");
                                        while (getchar() != '\n');
                                        x0_3[2] = NULL;
                                    }
                                } while (x0_3[0] == NULL||x0_3[1]== NULL||x0_3[2] == NULL);
                                broyden3(F4, x0_3, x_3, tol, max_iter,opc01);
                                printf("Solución aproximada encontrada usuario!! : x = %lf, y = %lf, z = %lf\n", x_3[0], x_3[1], x_3[2]);
                                printf("\nIngresa S o s si es que si: ");
                                fflush(stdin);
                                scanf("%c",&opc4);
                            }while(opc4=='s'||opc4=='S');

                            break;
                        default:
                            printf("\nnambre carnal como llegaste aqui, por las limitaciones del codigo ni se podria pero igual lo hiciste");
                            printf("\nSupongo que felicidades?, alch no tengo nada que darte jaja pero ps exito");
                            break;
                    }
                    printf("\nQuieres probar suerte con alguna otra funcion?");
                    printf("\nIngresa S o s si es que si: ");
                    fflush(stdin);
                    scanf("%c", &opc3);
                }while(opc3=='S'||opc3=='s');
                break;
            case 2:
                do{
                    printf("\n------------------------------------------\n");
                    printf("APROXIMACION POLINOMIAL POR MEDIO DE SPLINE CUBICOS\n");
                    printf("------------------------------------------\n");

                    double **tabla= init_table2();

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
                    double **tabla_diff=obtenciondiferencias(tabla);
                    imprimir_tabladiff(tabla_diff,n);

                    double **tridi=tabla_tridiagonal(tabla_diff);

                    printf("\nTabla de tridiagonal para sistema de ecuaciones: ");
                    imprimir_matriztridi(tridi,n);
                    system("pause");

                    double **matSpline=spline(tridi,tabla_diff,n);
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
                system("pause");
                break;
            case 3:
                do {
                    printf("\n------------------------------------------\n");
                    printf("\nINTEGRACION NUMERICA");
                    printf("\n------------------------------------------\n");
                    printf("\nEste es un programa de integracion numerica en donde estan disponibles estas dos funciones, eliga una para proseguir\n");
                    printf("\nFUNCIONES DISPONIBLES ");
                    printf("\nEliga con precaucion la funcion a integrar ");

                    printf("\n 1.- x^4*sqrt(3+2x^2)/3");
                    printf("\n 2.- x^5/(5sqrt(x^2+4))\n");

                    do {
                        if (scanf("%d", &opc1) != 1 || (opc1 != 1 && opc1 != 2)) {
                            printf("Entrada inválida. Ingrese 1 o 2.\n");
                            while (getchar() != '\n');
                            opc1 = 0;
                        }
                    } while (opc1 != 1 && opc1 != 2);

                    if (opc1==1){
                        printf("\nUsted a elegido la opcion 1 ");
                        printf("\nx^4*sqrt(3+2x^2)/3");
                    }else{
                        printf("\nUsted a elegido la opcion 2 ");
                        printf("\nx^5/(5sqrt(x^2+4))");
                    }

                    leerintervaloypresicion();
                    printf("\n Bien usuario, el intervalo que usted ingreso es [%.2lf, %.2lf]",a,b);
                    tolerancia = pow(10, -digpresicion);
                    resultado = romberg(funciones[opc-1],a,b, tolerancia);
                    printf("\nQuieres probar suerte con alguna otra funcion?");
                    printf("\nIngresa S o s si es que si: ");
                    fflush(stdin);
                    scanf("%c", &opc3);
                }while(opc3=='S'||opc3=='s');
                break;
        }
        fflush(stdin);
        printf("\nQuieres volver al incio usuario?, o ya hasta aqui lo dejamos?");
        printf("\nSolo ingresa s o S si quieres volver: ");
        scanf("%c", &opcreturn);
    }while(opcreturn=='s'||opcreturn=='S');
    return 0;
}
