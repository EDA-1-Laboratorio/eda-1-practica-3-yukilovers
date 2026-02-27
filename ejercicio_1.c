#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<stdbool.h>

struct naipes {
    int puntos;
    char palo;
};

struct jugador {
    struct naipes mano[2];
    int score_jerarquia; // 1 (mejor) a 10 (peor)
    int carta_max;
};

// --- FUNCIÓN PARA EVALUAR LA MANO ---
int evaluarMano(struct naipes mano_j[], struct naipes mesa[]) {
    struct naipes total[7];
    int i, j;
    
    for(i = 0; i < 2; i++) total[i] = mano_j[i];
    for(i = 0; i < 5; i++) total[i+2] = mesa[i];

    // 1. Ordenar descendente
    for(i = 0; i < 6; i++) {
        for(j = 0; j < 6 - i; j++) {
            if(total[j].puntos < total[j+1].puntos) {
                struct naipes temp = total[j];
                total[j] = total[j+1];
                total[j+1] = temp;
            }
        }
    }

    // 2. Contar frecuencias y palos
    int frec_puntos[14] = {0};
    int frec_palos[256] = {0};
    for(i = 0; i < 7; i++) {
        frec_puntos[total[i].puntos]++;
        frec_palos[(int)total[i].palo]++;
    }

    // 3. Identificar Color y Corridas
    bool tiene_color = false;
    char palo_color = ' ';
    for(i = 0; i < 256; i++) {
        if(frec_palos[i] >= 5) {
            tiene_color = true;
            palo_color = (char)i;
        }
    }

    bool tiene_corrida = false;
    int consecutivos = 1;
    for(i = 0; i < 6; i++) {
        if(total[i].puntos == total[i+1].puntos + 1) {
            consecutivos++;
            if(consecutivos >= 5) tiene_corrida = true;
        } else if (total[i].puntos != total[i+1].puntos) {
            consecutivos = 1; 
        }
    }

    // Validación especial para Corrida Real (As, K, Q, J, 10)
    bool tiene_as = (frec_puntos[1] > 0);
    bool corrida_real = (tiene_as && frec_puntos[13]>0 && frec_puntos[12]>0 && frec_puntos[11]>0 && frec_puntos[10]>0);

    int trios = 0, pares = 0, poker = 0;
    for(i = 1; i <= 13; i++) {
        if(frec_puntos[i] == 4) poker++;
        if(frec_puntos[i] == 3) trios++;
        if(frec_puntos[i] == 2) pares++;
    }

    // 4. JERARQUÍA SEGÚN TUS REGLAS
    if (corrida_real && tiene_color) return 1;    // 1. Corrida real
    if (tiene_color && tiene_corrida) return 2;   // 2. Corrida de color
    if (poker > 0) return 3;                      // 3. Póker
    if (trios > 0 && pares > 0) return 4;         // 4. Casa llena
    if (tiene_color) return 5;                    // 5. Color
    if (tiene_corrida) return 6;                  // 6. Corrida
    if (trios > 0) return 7;                      // 7. Tercia
    if (pares >= 2) return 8;                     // 8. Dos pares
    if (pares == 1) return 9;                     // 9. Un par
    
    return 10; // 10. Carta más alta
}

int main() {
    srand(time(NULL));
    struct naipes cartas[52];
    struct naipes mesa[5]; // Este arreglo debe llenarse en las rondas
    char posibles_palos[] = {'C', 'T', 'D', 'E'};
    struct jugador jugadores[2];
    int elecciones[2], i, j;

    // Generar mazo
    int carta_idx = 0;
    for(i = 0; i < 4; i++) {
        for(int p = 1; p <= 13; p++) {
            cartas[carta_idx].puntos = p;
            cartas[carta_idx].palo = posibles_palos[i];
            carta_idx++;
        }
    }
    // Mezclar
    for(i = 0; i < 52; i++) {
        int r = rand() % 52;
        struct naipes temp = cartas[i];
        cartas[i] = cartas[r];
        cartas[r] = temp;
    }

    int sig_carta = 0;
    for(i = 0; i < 2; i++) {
        for(j = 0; j < 2; j++) jugadores[i].mano[j] = cartas[sig_carta++];
    }

    // --- RONDA 1 ---
    for(i = 0; i < 2; i++) {
        printf("\n=== JUGADOR %i ===\nCarta 1: %d-%c | Carta 2: %d-%c\n", i+1, jugadores[i].mano[0].puntos, jugadores[i].mano[0].palo, jugadores[i].mano[1].puntos, jugadores[i].mano[1].palo);
        printf("1) Me quedo\n2) Retirarse\nSeleccion: ");
        scanf("%i", &elecciones[i]);
        if(elecciones[i] == 2) { 
            printf("\nJugador %i se retira. Gana Jugador %i!\n", i + 1, (i == 0) ? 2 : 1);
            return 0; 
        }
    }

    // --- RONDA 2 (FLOP) ---
    printf("\n--- MESA (FLOP) ---\n");
    for(i = 0; i < 3; i++) {
        mesa[i] = cartas[sig_carta++];
        printf("Mesa Carta %i: %i de %c\n", i+1, mesa[i].puntos, mesa[i].palo);
    }

    for(i = 0; i < 2; i++) {
        printf("\n=== JUGADOR %i ===\nCarta 1: %d-%c | Carta 2: %d-%c\n", i+1, jugadores[i].mano[0].puntos, jugadores[i].mano[0].palo, jugadores[i].mano[1].puntos, jugadores[i].mano[1].palo);
        printf("1) Me quedo\n2) Retirarse\nSeleccion: ");
        scanf("%i", &elecciones[i]);
        if(elecciones[i] == 2) { 
            printf("\nEl Jugador %i abandona. Gana el Jugador %i!\n", i+1, (i == 0) ? 2 : 1);
            return 0; 
        }
    }

    // --- RONDA 3 (TURN) ---
    printf("\n--- MESA (TURN) ---\n");
    mesa[3] = cartas[sig_carta++]; // CORREGIDO: Se guarda en el arreglo global
    printf("Mesa Carta 4: %i de %c\n", mesa[3].puntos, mesa[3].palo);

    for(i = 0; i < 2; i++) {
        printf("\n=== JUGADOR %i ===\nCarta 1: %d-%c | Carta 2: %d-%c\n", i+1, jugadores[i].mano[0].puntos, jugadores[i].mano[0].palo, jugadores[i].mano[1].puntos, jugadores[i].mano[1].palo);
        printf("1) Me quedo\n2) Retirarse\nSeleccion: ");
        scanf("%i", &elecciones[i]);
        if(elecciones[i] == 2) { 
            printf("\nEl Jugador %i abandona. Gana el Jugador %i!\n", i+1, (i == 0) ? 2 : 1);
            return 0; 
        }
    }

    // --- RONDA 4 (RIVER) ---
    printf("\n--- MESA (RIVER) ---\n");
    mesa[4] = cartas[sig_carta++]; // CORREGIDO: Se guarda en el arreglo global
    printf("Mesa Carta 5: %i de %c\n", mesa[4].puntos, mesa[4].palo);

    // --- EVALUACIÓN FINAL ---
    for(i = 0; i < 2; i++) {
        jugadores[i].score_jerarquia = evaluarMano(jugadores[i].mano, mesa);
        
        // Carta más alta para desempate (Regla 10)
        int m = jugadores[i].mano[0].puntos;
        if(jugadores[i].mano[1].puntos > m) m = jugadores[i].mano[1].puntos;
        jugadores[i].carta_max = m;
    }

    printf("\n\n     RESULTADO FINAL\n\n");
    
    if(jugadores[0].score_jerarquia < jugadores[1].score_jerarquia) {
        printf("GANADOR: JUGADOR 1 con jerarquia %d\n", jugadores[0].score_jerarquia);
    } else if(jugadores[1].score_jerarquia < jugadores[0].score_jerarquia) {
        printf("GANADOR: JUGADOR 2 con jerarquia %d\n", jugadores[1].score_jerarquia);
    } else {
        if(jugadores[0].carta_max > jugadores[1].carta_max) printf("GANADOR: JUGADOR 1 (Carta alta %d)\n", jugadores[0].carta_max);
        else if(jugadores[1].carta_max > jugadores[0].carta_max) printf("GANADOR: JUGADOR 2 (Carta alta %d)\n", jugadores[1].carta_max);
        else printf("EMPATE ABSOLUTO\n");
    }

    return 0;
}
