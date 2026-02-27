#include <stdio.h>
#include <math.h>

#define MAX 10
#define PI 3.1416

struct pizza {
    float rp;  //radio de la pizza
    int cantp; //cantidad de pizzas
    int cT; //cantidad de toppings
    int tT[6]; //tipo de toppings
    float pp; //precio de la pizza
};

int main() {
    struct pizza orden[MAX];
    int i,cantp,opciont;
    float total = 0;

    printf("¿Cuantas pizzas desea ordenar? ");
    scanf("%d", &cantp);
    if(cantp > MAX) {
    printf("Solo se permiten %d pizzas. Se ajusta automaticamente.\n", MAX);
    cantp = MAX;
    }
    for(int i = 0; i < cantp; i++) {
        printf("\n--- Pizza %d ---\n", i+1);
        
        printf("--Tamaño de la pizza--\n ");
        printf("Seleccione el tamaño de la pizza: \n");
        printf("1) Tamaño chico (radio de 15 cm)\n");
        printf("2) Tamaño mediano (radio de 20 cm)\n");
        printf("3) Tamaño grande (radio de 25 cm)\n");
        scanf("%d", &opciont);
        switch(opciont) {
        case 1:
        orden[i].rp = 15;
        break;
        case 2:
        orden[i].rp = 20;
        break;
        case 3:
        orden[i].rp = 25;
        break;
        default:
        printf("Opcion invalida. Se asigna tamaño chico.\n");
        orden[i].rp = 15;
        }
        printf("--Cantidad de toppings-- ");
        printf("\n¿Cuantos toppings desea ordenar? (Maximo 6):\n");
        scanf("%d", &orden[i].cT);
        if(orden[i].cT < 0 || orden[i].cT > 6) {
            printf("Cantidad invalida. Se asignan 0 toppings.\n");
            orden[i].cT = 0;
        }
        for(int t = 0; t < orden[i].cT; t++) {
        printf("\nSeleccione el topping %d:\n", t+1);   
        printf("1) Pepperoni\n");
        printf("2) Jamón\n");
        printf("3) Queso extra\n");
        printf("4) Salami\n");
        printf("5) Aceitunas\n");
        printf("6) Champiñones\n");
        scanf("%d", &orden[i].tT[t]);
        if(orden[i].tT[t] < 1 || orden[i].tT[t] > 6) {
                printf("Opcion invalida. Se asigna Pepperoni.\n");
                orden[i].tT[t] = 1;
            }
        }
        float area = PI * pow(orden[i].rp, 2);
        orden[i].pp = (area * 0.05) + (orden[i].cT * 10); //cada cm cuadrado cuesta $0.05, y cada topping $10
    }
    for(int i = 0; i < cantp; i++) {
        for(int j = i+1; j < cantp; j++) {
            if(orden[i].rp == orden[j].rp && orden[j].pp > 0 && orden[i].pp > 0) {
                orden[j].pp = 0; // la segunda es gratis
                break;
            }
        }
    }
    printf("\n========= TICKET =========\n");
    for(int i = 0; i < cantp; i++) {
        printf("Pizza %d:\n", i+1);
        printf(" Radio: %.2f\n", orden[i].rp);
        printf(" Toppings: %d\n", orden[i].cT);
        printf(" Precio: $%.2f\n\n", orden[i].pp);
        total += orden[i].pp; //el mas igual es una forma abreviada de escribir total mas orden
    }

    printf("TOTAL A PAGAR: $%.2f\n", total);
    return 0;
}
