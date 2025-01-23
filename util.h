#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>

bool validar_ean8(const char *ean8);
bool arquivo_existe(const char *nome_arquivo);
bool gerar_codigo_barras(const char *ean8, int espacos_laterais, int pixels_por_area, int altura, const char *nome_arquivo);
bool calcular_pixel(int x, const char* ean8, int espacos_laterais, int pixels_por_area);
bool validar_arquivo_pbm(const char *nome_arquivo);
bool decodificar_codigo_barras(int **pixels, int largura, int altura, char *identificador);
bool extrair_codigo_barras(const char *nome_arquivo, char *identificador);
#endif
