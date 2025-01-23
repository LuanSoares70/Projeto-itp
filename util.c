#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "util.h"
#include <ctype.h>

bool validar_ean8(const char *ean8) {
    if (strlen(ean8) != 8) return false;
    int soma = 0;
    for (int i = 0; i < 7; i++) {
        if (!isdigit(ean8[i])) return false;
        int digito = ean8[i] - '0';
        soma += (i % 2 == 0) ? digito : digito * 3;
    }
    int verificador = (10 - (soma % 10)) % 10;
    return verificador == (ean8[7] - '0');
}

bool exemplo_funcao(int x) {
    return (x % 2 == 0); 
}

bool calcular_pixel(int x, const char* ean8, int espacos_laterais, int pixels_por_area) {
    return (x % 2 == 0); 
}


bool arquivo_existe(const char *nome_arquivo) {
    FILE *file = fopen(nome_arquivo, "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}


bool gerar_codigo_barras(const char *ean8, int espacos_laterais, int pixels_por_area, int altura, const char *nome_arquivo) {
    FILE *file = fopen(nome_arquivo, "w");
    if (!file) return false;

    fprintf(file, "P1\n"); 
    int largura = espacos_laterais * 2 + 67 * pixels_por_area; 
    fprintf(file, "%d %d\n", largura, altura);

    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            bool pixel = calcular_pixel(x, ean8, espacos_laterais, pixels_por_area);
            fprintf(file, "%d ", pixel ? 1 : 0);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    return true;
}

bool validar_arquivo_pbm(const char *nome_arquivo) {
    FILE *file = fopen(nome_arquivo, "r");
    if (!file) return false;

    char magic_number[3];
    fscanf(file, "%2s", magic_number);
    fclose(file);

    return strcmp(magic_number, "P1") == 0;
}


int l_code[10][7] = {
    {0, 0, 0, 1, 1, 0, 1},  // 0
    {0, 1, 1, 1, 0, 0, 1},  // 1
    {0, 0, 1, 1, 0, 1, 1},  // 2
    {0, 1, 0, 1, 1, 1, 0},  // 3
    {1, 0, 0, 1, 1, 1, 0},  // 4
    {0, 1, 1, 0, 1, 1, 0},  // 5
    {1, 1, 0, 0, 0, 1, 1},  // 6
    {0, 1, 0, 0, 1, 1, 1},  // 7
    {1, 0, 1, 0, 0, 1, 1},  // 8
    {1, 1, 0, 1, 0, 0, 1},  // 9
};

int r_code[10][7] = {
    {1, 1, 1, 0, 0, 1, 0},  // 0
    {1, 0, 0, 0, 1, 1, 0},  // 1
    {1, 1, 0, 0, 1, 0, 0},  // 2
    {0, 0, 1, 0, 1, 1, 0},  // 3
    {0, 1, 0, 0, 0, 1, 1},  // 4
    {1, 1, 0, 1, 0, 0, 0},  // 5
    {0, 0, 1, 1, 0, 1, 0},  // 6
    {0, 1, 0, 1, 1, 0, 0},  // 7
    {1, 0, 0, 1, 1, 0, 0},  // 8
    {0, 1, 1, 0, 0, 0, 1},  // 9
};

bool decodificar_codigo_barras(int **pixels, int largura, int altura, char *identificador) {
    int barras_por_digito = 7;
    int numero_digitos = 8;  
    int largura_codigo = (barras_por_digito * numero_digitos) + 1; 

    int idx = 0;  

    for (int x = 3; x < 31; x += 7) { 
        int barras[7];
        for (int i = 0; i < 7; i++) {
            barras[i] = pixels[altura / 2][x + i];
        }

        bool encontrou = false;
        for (int digito = 0; digito < 10; digito++) {
            bool match = true;
            for (int i = 0; i < 7; i++) {
                if (barras[i] != l_code[digito][i]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                identificador[idx++] = '0' + digito;
                encontrou = true;
                break;
            }
        }


        if (!encontrou) {
            printf("Nenhuma correspondência encontrada para as barras lidas no L-code.\n");
            return false;
        }
    }

    
    for (int x = 37; x < 67; x += 7) {  
        int barras[7];
        for (int i = 0; i < 7; i++) {
            barras[i] = pixels[altura / 2][x + i];
        }

        bool encontrou = false;
        for (int digito = 0; digito < 10; digito++) {
            bool match = true;
            for (int i = 0; i < 7; i++) {
                if (barras[i] != r_code[digito][i]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                identificador[idx++] = '0' + digito;
                encontrou = true;
                break;
            }
        }

        if (!encontrou) {
            printf("Nenhuma correspondência encontrada para as barras lidas no R-code.\n");
            return false;
        }
    }

    identificador[8] = '\0';

    return true;
}

    


bool extrair_codigo_barras(const char *nome_arquivo, char *identificador) {
    FILE *file = fopen(nome_arquivo, "r");
    if (!file) {
        return false;
    }

    char magic_number[3];
    fscanf(file, "%2s", magic_number);
    if (strcmp(magic_number, "P1") != 0) {
        fclose(file);
        return false;
    }

    int largura, altura;
    fscanf(file, "%d %d", &largura, &altura);

    int **pixels = (int **)malloc(altura * sizeof(int *));
    for (int i = 0; i < altura; i++) {
        pixels[i] = (int *)malloc(largura * sizeof(int));
    }

    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            fscanf(file, "%d", &pixels[y][x]);
        }
    }

    bool sucesso = decodificar_codigo_barras(pixels, largura, altura, identificador);

    for (int i = 0; i < altura; i++) {
        free(pixels[i]);
    }
    free(pixels);

    fclose(file);
    return sucesso;
}
