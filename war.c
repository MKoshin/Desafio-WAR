
#include <stdio.h>
#include <stdlib.h> //
#include <string.h> // para strcspn()

#define MAX_TERRITORIOS 5
#define TAM_STRING 100


struct territorio {
    char nome[TAM_STRING];
    char cor_do_exercito[TAM_STRING];
    int quantidadeDeTropas;
};

void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        ;
    }
}

int main() {
    struct territorio mapa[MAX_TERRITORIOS];
    int totalTerritorio = 0;
    int opcao;

    // -- Laço principal do menu ---
    do {
        // Exibe o menu de opções.
        printf("\n=============================\n");
        printf("    CADASTRANTO TERRITÓRIO     \n");
        printf("\n=============================\n");
        printf("1. Cadastrar novo território\n");
        printf("2. Listar Mundos existentes\n");
        printf("0. Sair\n");
        printf("=============================\n");
        printf("Escolha uma opcao: ");

        // Lê a opção do usuário.
        if (scanf(" %d", &opcao) != 1) {
            opcao = -1;
        }
        limparBufferEntrada();

        // ---- processamento da opção
        switch (opcao) {
            case 1: // CADASTRO DE MUNDOS
                printf("--- Cadastro de Novo Território ---\n\n");

                if (totalTerritorio < MAX_TERRITORIOS) {
                    printf("Digite o nome do seu mapa: ");
                    fgets(mapa[totalTerritorio].nome, TAM_STRING, stdin);

                    /* ler cor, garantindo unicidade entre territorios existentes */
                    while (1) {
                        printf("Digite a cor ex: Azul, vermelho, Amarelo:  ");
                        if (fgets(mapa[totalTerritorio].cor_do_exercito, TAM_STRING, stdin) == NULL) {
                            mapa[totalTerritorio].cor_do_exercito[0] = '\0';
                        } else {
                            mapa[totalTerritorio].cor_do_exercito[strcspn(mapa[totalTerritorio].cor_do_exercito, "\n")] = '\0';
                        }

                        if (mapa[totalTerritorio].cor_do_exercito[0] == '\0') {
                            printf("Cor invalida. Tente novamente.\n");
                            continue;
                        }

                        int duplicada = 0;
                        for (int j = 0; j < totalTerritorio; j++) {
                            if (strcmp(mapa[j].cor_do_exercito, mapa[totalTerritorio].cor_do_exercito) == 0) {
                                duplicada = 1;
                                break;
                            }
                        }

                        if (duplicada) {
                            printf("Cor ja usada por outro territorio. Escolha outra cor.\n");
                            continue;
                        }

                        break; /* cor ok */
                    }

                    printf("Digite a quantidade de tropas: ");
                    if (scanf(" %d", &mapa[totalTerritorio].quantidadeDeTropas) != 1) {
                        mapa[totalTerritorio].quantidadeDeTropas = 0;
                    }
                    limparBufferEntrada();

                    mapa[totalTerritorio].nome[strcspn(mapa[totalTerritorio].nome, "\n")] = '\0';
                    mapa[totalTerritorio].cor_do_exercito[strcspn(mapa[totalTerritorio].cor_do_exercito, "\n")] = '\0';
                    totalTerritorio++;

                    printf("Território cadastrado com sucesso!\n");

                } else{
                    printf("Não foi possivel cadastrar mais territórios \n");
                }
                break;

            case 2: // LISTAGEM DE MUNDOS
                printf("--- Lista de Mundos - Estato Atual ---\n\n");

                if (totalTerritorio == 0) {
                    printf("Nenhum territorio cadastrado! \n");
                } else{
                    for (int i = 0; i < totalTerritorio; i++){
                        printf("---------------------------------\n");
                        printf("Territorio %d:\n", i + 1);
                        printf("Nome: %s\n", mapa[i].nome);
                        printf("Cor do exercito: %s\n", mapa[i].cor_do_exercito);
                        printf("Quantidade de tropas: %d\n", mapa[i].quantidadeDeTropas);
                    }
                    printf("---------------------------------\n");
                }
                // A pausa é crucial para que o usuário veja a lisa antes
                // do proximo loop limpar a tela

                printf("\n Pressione ENTER para continuar...");
                getchar();
                break;

            case 0: // SAIR
                printf("Saindo do programa. Ate mais!\n");
                break;

            default: // OPÇÃO INVÁLIDA
                printf("Opcao invalida! Tente novamente.\n");
                printf("\n Pressione ENTER para continuar...");
                getchar();
                break;
        }

    } while (opcao != 0);
}

