#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <cstring>
#define MAX_J 15
#define MAX_a -50
#define Max_b 50
double a=0,b=0;
int digpresicion=0;

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

void menupresentationdelosintegrantessuperpechudos(){
    printf("METODOS NUMERICOS II");
    printf("\nMuuuuuuuy buenos dias , tardes o nocheeees, sea bienvenido aaaaal mejoooor programa deeee:");
    printf("\n------------------------------------------\n");
    printf("\nINTEGRACION NUMERICA");
    printf("\n------------------------------------------\n");
    printf("\nINTEGRANTES:");
    printf("\nHERRERA HERNANDEZ DIEGO");
    printf("\nVELAZQUEZ MARTINEZ ANTONIO \n");
    printf("\nMuchas gracias por utilizar este programa usuario ^^");
};




int main(){
    int opc;
    double tolerancia, resultado;
    double (*funciones[])(double) = {funcion1, funcion2};
    menupresentationdelosintegrantessuperpechudos();

    printf("\nEste es un programa de integracion numerica en donde estan disponibles estas dos funciones, eliga una para proseguir\n");
    printf("\nFUNCIONES DISPONIBLES ");
    printf("\nEliga con precaucion la funcion a integrar ");

    printf("\n 1.- x^4*sqrt(3+2x^2)/3");
    printf("\n 2.- x^5/(5sqrt(x^2+4))\n");

    do {
        if (scanf("%d", &opc) != 1 || (opc != 1 && opc != 2)) {
            printf("Entrada inválida. Ingrese 1 o 2.\n");
            while (getchar() != '\n');
            opc = 0;
        }
    } while (opc != 1 && opc != 2);

    if (opc==1){
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


    return 0;
}
