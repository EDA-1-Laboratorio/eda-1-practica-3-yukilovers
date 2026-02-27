#include <stdio.h>
#include <math.h>

#define PI 3.14159

//Círculo con: Radio positivo, Par de números reales
//que indicarán la posición del centro del círculo
struct circulo {
    float RADIO;
    float k; //Desplazamiento verticual
    float h; //Desplazamiento horizontal
};

float calcular_distancia(float centroA_h, float centroA_k, float centroB_h, float centroB_k){
    float distancia;
    distancia = sqrt(pow((centroB_h - centroA_h), 2) + pow((centroB_k - centroA_k), 2) ); // Raiz[(x2-x1)^2 + (y2-y1)^2]

    return distancia;
}

int main(){
    float radio_ingresado = 1;
    float posicion_h_ingresada = 0;
    float posicion_k_ingresada = 0;

    struct circulo circ1;
    struct circulo circ2;

    float distancia;
    float suma_radios;
    float diferencia_radios;
    float area_interseccion;
    

    /*
    Primero se le solicita al usuario que ingrese los datos para dos círculos
    */

    printf("Por favor ingresa los datos para dos circulos! \n\n");

    for(int i = 1; i <= 2; i++){
        printf("===== C I R C U L O - %d ===== \n", i);
        printf("Radio positivo: ");

        while( (scanf("%f", &radio_ingresado) != 1) || (radio_ingresado <= 0) ){
            printf("ERROR! Por favor ingresa un valor unicamente positivo. ");     //El while es para asegurarse de que sea un número (Y no letras)
            printf("Radio positivo: ");                                             //Y mayor a 0

            while(getchar() != '\n'); //Limpiar buffer
        }

        printf("Posicion horizontal (h): ");

        while(scanf("%f", &posicion_h_ingresada) != 1){
            printf("ERROR! Ingresa solo un valor numerico. ");
            printf("Posicion horizontal (h): ");

            while(getchar() != '\n');
        }

        printf("Posicion vertical (k): ");

        while(scanf("%f", &posicion_k_ingresada) != 1){
            printf("ERROR! Ingresa solo un valor numerico. ");
            printf("Posicion vertical (k): ");

            while(getchar() != '\n');
        }

        printf("\n");

        if (i == 1){
            circ1.RADIO = radio_ingresado;
            circ1.h = posicion_h_ingresada;
            circ1.k = posicion_k_ingresada;
        } else if (i == 2){
            circ2.RADIO = radio_ingresado;
            circ2.h = posicion_h_ingresada;
            circ2.k = posicion_k_ingresada;
        }
    }

    // Para que no haya intersección, la distancia entre los dos centros tiene que ser mayor a la suma de los radios
    // Si la distancia es menor que la resta de los radios, un círculo está adentro de otro
    // Si no se cumplen ninguna de estas dos condiciones, hay una intersección

    distancia = calcular_distancia(circ1.h, circ1.k, circ2.h, circ2.k);
    suma_radios = circ1.RADIO + circ2.RADIO;
    diferencia_radios = fmax(circ1.RADIO, circ2.RADIO) - fmin(circ1.RADIO, circ2.RADIO);
    
    if (distancia == 0 && circ1.RADIO == circ2.RADIO){
        printf("Los circulos son identicos.\n");
        area_interseccion = PI * circ1.RADIO * circ1.RADIO;
        printf("El área de intersección es de: %f", area_interseccion);
    } else if (distancia >= (suma_radios)){
        printf("No existe interseccion entre los dos circulos!\n");
        printf("Al no haber interseccion, el áaea de interseccion es = 0");
    } else if (distancia <= (diferencia_radios)){
        printf("Un circulo se encuentra adentro del otro, por lo que se intersectan completamente.\n");
        area_interseccion = PI * pow(fmin(circ1.RADIO, circ2.RADIO), 2);
        printf("El area de interseccion es de: %f", area_interseccion);
    } else {
        printf("Los dos círculos se intersectan. \n");
        float parte1 = (circ1.RADIO * circ1.RADIO) * acos((distancia * distancia + circ1.RADIO * circ1.RADIO - circ2.RADIO * circ2.RADIO) / (2 * distancia * circ1.RADIO));
        float parte2 = (circ2.RADIO * circ2.RADIO) * acos((distancia * distancia + circ2.RADIO * circ2.RADIO - circ1.RADIO * circ1.RADIO) / (2 * distancia * circ2.RADIO));
        float parte3 = 0.5 * sqrt((circ1.RADIO + circ2.RADIO - distancia) * (circ2.RADIO + distancia - circ1.RADIO) * (circ1.RADIO + distancia - circ2.RADIO) * (circ1.RADIO + circ2.RADIO + distancia));

        area_interseccion = parte1 + parte2 - parte3;
        printf("El area de interseccion es de: %f", area_interseccion);
    }

    return 0;
}
