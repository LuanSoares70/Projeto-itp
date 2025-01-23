#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "util.h" 

int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo PBM>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *nome_arquivo = argv[1];

    if (!arquivo_existe(nome_arquivo) || !validar_arquivo_pbm(nome_arquivo)) {
        fprintf(stderr, "Erro: Arquivo inválido ou inexistente.\n");
        return EXIT_FAILURE;
    }

    char identificador[9];

    if (extrair_codigo_barras(nome_arquivo, identificador)) {
        printf("Identificador extraído: %s\n", identificador);
    } else {
        printf("Falha ao extrair o identificador.\n");
    }

    return EXIT_SUCCESS;
}