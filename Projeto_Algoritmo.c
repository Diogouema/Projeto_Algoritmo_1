/* Diogo Oliveira Uema
   Gabriel Alves Gomes
   Moises Manoel Dos Santos Filho 
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORDS 10000
#define WORD_LEN 5
#define MAX_TRIES 6
#define DICT_FILE "sem_acentos.txt"
#define SCORE_FILE "scores.txt"

void carregarDicionario(char palavras[][WORD_LEN + 1], int *contagem) {
  FILE *file = fopen(DICT_FILE, "r");
  if (file == NULL) {
    perror("Erro ao abrir o arquivo do dicionário");
    exit(EXIT_FAILURE);
  }

  while (*contagem < MAX_WORDS &&
         fscanf(file, "%5s", palavras[*contagem]) == 1) {
    palavras[*contagem][WORD_LEN] =
        '\0'; // Garante que cada palavra é terminada corretamente com NULL
    (*contagem)++;
  }

  fclose(file);
}

int verificarPalavraNoDicionario(char *palavra, char palavras[][WORD_LEN + 1],
                                 int contagem) {
  for (int i = 0; i < contagem; i++) {
    if (strcmp(palavras[i], palavra) == 0) {
      return 1;
    }
  }
  return 0;
}

void imprimirFeedback(char *tentativa, char *resposta) {
  for (int i = 0; i < WORD_LEN; i++) {
    if (tentativa[i] == resposta[i]) {
      printf("^ ");
    } else if (strchr(resposta, tentativa[i])) {
      printf("! ");
    } else {
      printf("x ");
    }
  }
  printf("\n");
}

void salvarPontuacao(char *nome, char *palavra, int tentativas,
                     double tempoGasto) {
  FILE *file = fopen(SCORE_FILE, "a");
  if (file == NULL) {
    perror("Erro ao abrir o arquivo de pontuações");
    return;
  }

  fprintf(file, "%s - %s - %d tentativas - %.2f segundos\n", nome, palavra,
          tentativas, tempoGasto);
  fclose(file);
}

int main() {
  char palavras[MAX_WORDS][WORD_LEN + 1];
  int contagem = 0, tentativas = 0;
  char tentativa[WORD_LEN + 1], resposta[WORD_LEN + 1], nomeJogador[50];
  time_t inicio, fim;

  carregarDicionario(palavras, &contagem);
  srand(time(NULL));
  strcpy(resposta, palavras[rand() % contagem]);

  printf("Bem-vindo ao jogo! Adivinhe a palavra de 5 letras.\n");
  time(&inicio);

  while (tentativas < MAX_TRIES) {
    printf("Tentativa %d: ", tentativas + 1);
    scanf("%s", tentativa);

    if (!verificarPalavraNoDicionario(tentativa, palavras, contagem) ||
        strlen(tentativa) != WORD_LEN) {
      printf("Palavra inválida ou não encontrada no dicionário. Tente "
             "novamente.\n");
      continue;
    }

    imprimirFeedback(tentativa, resposta);
    tentativas++;

    if (strcmp(tentativa, resposta) == 0) {
      time(&fim);
      printf("Parabéns! Você adivinhou em %d tentativas.\n", tentativas);
      printf("Informe seu nome: ");
      scanf("%49s", nomeJogador); // Previne overflow do buffer
      salvarPontuacao(nomeJogador, resposta, tentativas, difftime(fim, inicio));
      break;
    }
  }

  if (strcmp(tentativa, resposta) != 0) {
    printf("Fim de jogo! A palavra era %s.\n", resposta);
  } // Adicionado chave de fechamento que estava faltando aqui.

  return 0;
}
