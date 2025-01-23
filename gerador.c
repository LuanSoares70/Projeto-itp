#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "util.h" 

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <identificador EAN-8> [opções]\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *identificador = argv[1];

    if (!validar_ean8(identificador)) {
        fprintf(stderr, "Erro: Identificador inválido. Certifique-se de que é um EAN-8 válido.\n");
        return EXIT_FAILURE;
    }

    int espacos_laterais = 10;
    int pixels_por_area = 2;
    int altura = 50;
    char nome_arquivo[256] = "codigo_de_barras.pbm";

    if (argc > 2) espacos_laterais = atoi(argv[2]);
    if (argc > 3) pixels_por_area = atoi(argv[3]);
    if (argc > 4) altura = atoi(argv[4]);
    if (argc > 5) strncpy(nome_arquivo, argv[5], sizeof(nome_arquivo) - 1);

    if (arquivo_existe(nome_arquivo)) {
        printf("O arquivo %s já existe. Deseja sobrescrevê-lo? (s/n): ", nome_arquivo);
        char resposta;
        scanf(" %c", &resposta);
        if (resposta != 's' && resposta != 'S') {
            fprintf(stderr, "Erro: Arquivo já existe.\n");
            return EXIT_FAILURE;
        }
    }

    if (!gerar_codigo_barras(identificador, espacos_laterais, pixels_por_area, altura, nome_arquivo)) {
        fprintf(stderr, "Erro ao gerar o código de barras.\n");
        return EXIT_FAILURE;
    }

    printf("Código de barras gerado com sucesso: %s\n", nome_arquivo);
    return EXIT_SUCCESS;
}
