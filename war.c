#include <stdio.h>
#include <stdlib.h> //
#include <string.h> // para strcspn()
#include <time.h>   // para srand/time

#define MAX_TERRITORIOS 5
#define TAM_STRING 100

// strcpy altera string [<destino>, <origem>]
// strcmp compara duas strings se forem iguais resultado == 0 se nao == 1
// locale.h para usar acentos
// strlen retorna tamanho da string
// strcspn retorna o indice do primeiro caractere de uma string que corresponde a qualquer caractere de outra string
// FGETS (variavel, tamanho, stdin) reserva automatico o caractere \0

/* struct territorio {
    char nome[TAM_STRING];
    char cor_do_exercito[TAM_STRING];
    int quantidadeDeTropas;
}; */

typedef struct{ /* usando typedef elimino uso de struct no chamado da struct*/
    char nome[TAM_STRING];
    char cor_do_exercito[TAM_STRING];
    int quantidadeDeTropas;
    char missaoRecebida[TAM_STRING];
    int vitoriaPts;
    int defesaPts;

} territorio;

/* Sistema de missões será criado*/

typedef struct{
    char nome[TAM_STRING]; // nome da missão
    char objetivo[TAM_STRING]; // conquistar X territorios
    char status[TAM_STRING]; // ativo ou concluido
    char missaoDestinado[TAM_STRING]; // jogador que recebeu a missão
} missao;

void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        ;
    }
}

/* ============== FUNÇÕES DE BATALHA COM PONTEIROS ==============
 * 
 * EXPLICAÇÃO DE PONTEIROS:
 * - Um ponteiro armazena o ENDEREÇO de memória de uma variável
 * - Declaração: tipo *nomePonteiro;
 * - Operador &: obtém o endereço de uma variável (ex: &variavel)
 * - Operador *: acessa o valor no endereço (desreferência)
 * - Com structs, usamos -> para acessar membros via ponteiro
 *   Exemplo: ponteiro->campo é equivalente a (*ponteiro).campo
 */

/* Função para rolar dados de ataque (1-6)
 * Retorna um array alocado dinamicamente com malloc */
int* rolarDadosAtaque(int numDados) {
    /* malloc aloca memória dinamicamente no HEAP
     * sizeof(int) * numDados = tamanho total em bytes
     * Retorna um ponteiro para o bloco de memória alocado */
    int *dados = (int*) malloc(sizeof(int) * numDados);
    
    if (dados == NULL) {
        printf("Erro ao alocar memoria para dados de ataque!\n");
        return NULL;
    }
    
    /* Preenche o array com valores aleatórios de 1 a 6 */
    for (int i = 0; i < numDados; i++) {
        dados[i] = (rand() % 6) + 1;  /* dados[i] é equivalente a *(dados + i) */
    }
    
    return dados;  /* Retorna o ponteiro para o array */
}

/* Função para rolar dados de defesa (1-6) */
int* rolarDadosDefesa(int numDados) {
    int *dados = (int*) malloc(sizeof(int) * numDados);
    
    if (dados == NULL) {
        printf("Erro ao alocar memoria para dados de defesa!\n");
        return NULL;
    }
    
    for (int i = 0; i < numDados; i++) {
        dados[i] = (rand() % 6) + 1;
    }
    
    return dados;
}

/* Função auxiliar para ordenar array em ordem decrescente (bubble sort)
 * Recebe ponteiro para o array e seu tamanho */
void ordenarDecrescente(int *arr, int tamanho) {
    /* arr é um ponteiro para o primeiro elemento do array */
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = 0; j < tamanho - i - 1; j++) {
            if (arr[j] < arr[j + 1]) {
                /* Troca usando variável temporária */
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

/* Função principal de batalha
 * Recebe PONTEIROS para os territórios atacante e defensor
 * Isso permite modificar os valores originais! */
void realizarBatalha(territorio *atacante, territorio *defensor) {
    /* 
     * IMPORTANTE: atacante e defensor são PONTEIROS para structs
     * Usamos -> para acessar os campos: atacante->quantidadeDeTropas
     * Isso é equivalente a: (*atacante).quantidadeDeTropas
     */
    
    printf("\n=== BATALHA INICIADA ===\n");
    printf("%s (Cor: %s) ATACA %s (Cor: %s)\n", 
           atacante->nome, atacante->cor_do_exercito,
           defensor->nome, defensor->cor_do_exercito);
    printf("Tropas atacantes: %d | Tropas defensoras: %d\n\n",
           atacante->quantidadeDeTropas, defensor->quantidadeDeTropas);
    
    /* Loop de batalha - continua até alguém ficar sem tropas */
    while (atacante->quantidadeDeTropas > 1 && defensor->quantidadeDeTropas > 0) {
        /* Atacante pode usar até 3 dados (precisa manter 1 tropa no território) */
        int dadosAtaque = (atacante->quantidadeDeTropas - 1 > 3) ? 3 : atacante->quantidadeDeTropas - 1;
        /* Defensor pode usar até 3 dados */
        int dadosDefesa = (defensor->quantidadeDeTropas > 3) ? 3 : defensor->quantidadeDeTropas;
        
        /* Aloca memória dinamicamente para os dados */
        int *dadosAtk = rolarDadosAtaque(dadosAtaque);
        int *dadosDef = rolarDadosDefesa(dadosDefesa);
        
        if (dadosAtk == NULL || dadosDef == NULL) {
            /* Libera memória se houve erro */
            if (dadosAtk) free(dadosAtk);
            if (dadosDef) free(dadosDef);
            return;
        }
        
        /* Ordena os dados em ordem decrescente */
        ordenarDecrescente(dadosAtk, dadosAtaque);
        ordenarDecrescente(dadosDef, dadosDefesa);
        
        /* Mostra os dados rolados */
        printf("Dados do atacante: ");
        for (int i = 0; i < dadosAtaque; i++) {
            printf("[%d] ", dadosAtk[i]);
        }
        printf("\nDados do defensor: ");
        for (int i = 0; i < dadosDefesa; i++) {
            printf("[%d] ", dadosDef[i]);
        }
        printf("\n");
        
        /* Compara os dados (quantidade mínima entre ataque e defesa) */
        int comparacoes = (dadosAtaque < dadosDefesa) ? dadosAtaque : dadosDefesa;
        
        for (int i = 0; i < comparacoes; i++) {
            if (dadosAtk[i] > dadosDef[i]) {
                /* Atacante vence - defensor perde 1 tropa */
                defensor->quantidadeDeTropas--;
                printf("  Combate %d: Atacante vence! (%d > %d)\n", i+1, dadosAtk[i], dadosDef[i]);
                atacante->vitoriaPts++;
                
            } else {
                /* Empate ou defensor vence - atacante perde 1 tropa */
                atacante->quantidadeDeTropas--;
                printf("  Combate %d: Defensor vence! (%d <= %d)\n", i+1, dadosAtk[i], dadosDef[i]);
                defensor->defesaPts++;
            }
        }
        
        printf("Tropas restantes - Atacante: %d | Defensor: %d\n\n",
               atacante->quantidadeDeTropas, defensor->quantidadeDeTropas);
        
        /* IMPORTANTE: Libera a memória alocada com malloc!
         * Sempre que usar malloc, use free para evitar vazamento de memória */
        free(dadosAtk);
        free(dadosDef);
        
        /* Pequena pausa entre rodadas */
        printf("Pressione ENTER para proxima rodada...");
        getchar();
    }
    
    /* Resultado final */
    printf("\n=== RESULTADO DA BATALHA ===\n");
    if (defensor->quantidadeDeTropas == 0) {
        printf("VITORIA de %s!\n", atacante->nome);
        printf("%s conquistou o territorio %s!\n", atacante->cor_do_exercito, defensor->nome);
        
        /* Vencedor se apossa do território:
         * - Muda a cor do exército do território conquistado
         * - Move tropas para o novo território */
        strcpy(defensor->cor_do_exercito, atacante->cor_do_exercito);
        
        /* Move metade das tropas restantes para o território conquistado */
        int tropasMovidas = atacante->quantidadeDeTropas / 2;
        if (tropasMovidas < 1) tropasMovidas = 1;
        
        defensor->quantidadeDeTropas = tropasMovidas;
        atacante->quantidadeDeTropas -= tropasMovidas;
        
        printf("Tropas movidas para %s: %d\n", defensor->nome, tropasMovidas);
        printf("Tropas restantes em %s: %d\n", atacante->nome, atacante->quantidadeDeTropas);
    } else {
        printf("DEFESA bem sucedida de %s!\n", defensor->nome);
        printf("%s manteve seu territorio!\n", defensor->cor_do_exercito);
    }
}

int main() {
    territorio mapa[MAX_TERRITORIOS];
    int totalTerritorio = 0;
    int opcao;
    int escolhaMissao = 0;

    missao missoes[5] = {
        {"Primeira vitória", "Conquistar 1 territorios inimigos", "Ativo", ""},
        {"Defensor Implacável", "Defender com sucesso 2 ataques", "Ativo", ""},
        {"Vencedor supremo", "Ter total de 4 vitórias", "Ativo", ""},
        {"Dominação", "Controlar 3 territórios ao mesmo tempo", "Ativo", ""},
        {"Atacante implacável", "Vencer 2 batalhas seguidas", "Ativo", ""}
    };

    /* inicializa gerador de numeros aleatorios para batalhas */
    srand((unsigned) time(NULL));

    // -- Laço principal do menu ---
    do {
        // Exibe o menu de opções.
        printf("\n=============================\n");
        printf("    CADASTRANTO TERRITÓRIO     \n");
        printf("\n=============================\n");
        printf("1. Cadastrar novo território\n");
        printf("2. Listar Mundos existentes\n");
        printf("3. Iniciar batalha entre territorios\n");
        printf("4. Exibir missões (são predefinidas) \n");
        printf("5. Verificar missão concluida\n");
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
                    /* Ler nome com validação de duplicata */
                    while (1) {
                        printf("Digite o nome do seu mapa: ");

                        if (fgets(mapa[totalTerritorio].nome, TAM_STRING, stdin) == NULL) {
                            mapa[totalTerritorio].nome[0] = '\0';
                        } else {
                            mapa[totalTerritorio].nome[strcspn(mapa[totalTerritorio].nome, "\n")] = '\0';
                        }

                        if (mapa[totalTerritorio].nome[0] == '\0') {
                            printf("Nome invalido. Tente novamente. \n");
                            continue;
                        }

                        int duplicado1 = 0;
                        for (int j = 0; j < totalTerritorio; j++) {
                            if (strcmp(mapa[j].nome, mapa[totalTerritorio].nome) == 0) {
                                duplicado1 = 1;
                                break;
                            }
                        }

                        if (duplicado1) {
                            printf("Nome ja usado por outro territorio. Escolha outro nome.\n");
                            continue;
                        }
                        break; /* nome ok */
                    }

                    /* Ler cor com validação de duplicata */
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

                    /* Ler quantidade de tropas */
                    printf("Digite a quantidade de tropas: ");
                    if (scanf(" %d", &mapa[totalTerritorio].quantidadeDeTropas) != 1) {
                        mapa[totalTerritorio].quantidadeDeTropas = 0;
                    }
                    limparBufferEntrada();

                    mapa[totalTerritorio].missaoRecebida[0] = '\0';
                    mapa[totalTerritorio].vitoriaPts = 0;   // ADICIONAR
                    mapa[totalTerritorio].defesaPts = 0;    // ADICIONAR
                    
                    totalTerritorio++;
                    printf("Território cadastrado com sucesso!\n");

                } else {
                    printf("Não foi possivel cadastrar mais territórios \n");
                }
                break;

            case 2: // LISTAGEM DE MUNDOS
                printf("--- Lista de Mundos - Estado Atual ---\n\n");

                if (totalTerritorio == 0) {
                    printf("Nenhum territorio cadastrado! \n");
                } else{
                    for (int i = 0; i < totalTerritorio; i++){
                        printf("---------------------------------\n");
                        printf("Territorio %d:\n", i + 1);
                        printf("Nome: %s\n", mapa[i].nome);
                        printf("Cor do exercito: %s\n", mapa[i].cor_do_exercito);
                        printf("Quantidade de tropas: %d\n", mapa[i].quantidadeDeTropas);
                        printf("Missão recebida: %s\n", mapa[i].missaoRecebida);
                    }
                    printf("---------------------------------\n");
                }
                // A pausa é crucial para que o usuário veja a lista antes
                // do proximo loop limpar a tela


                printf("\n Pressione ENTER para continuar...");
                getchar();
                break;

            case 3: // BATALHA
                printf("--- Iniciar Batalha ---\n\n");
                
                if (totalTerritorio < 2) {
                    printf("Voce precisa de pelo menos 2 territorios para batalhar!\n");
                    printf("\n Pressione ENTER para continuar...");
                    getchar();
                    break;
                }
                
                /* Lista territórios disponíveis */
                printf("Territorios disponiveis:\n");
                for (int i = 0; i < totalTerritorio; i++) {
                    printf("%d. %s (Cor: %s, Tropas: %d)\n", 
                           i + 1, mapa[i].nome, mapa[i].cor_do_exercito, 
                           mapa[i].quantidadeDeTropas);
                }
                
                int indiceAtacante, indiceDefensor;
                
                /* Seleciona território atacante */
                printf("\nDigite o numero do territorio ATACANTE: ");
                if (scanf(" %d", &indiceAtacante) != 1) {
                    printf("Entrada invalida!\n");
                    limparBufferEntrada();
                    printf("\n Pressione ENTER para continuar...");
                    getchar();
                    break;
                }
                limparBufferEntrada();
                indiceAtacante--;  /* Ajusta para índice 0-based */
                
                /* Valida índice do atacante */
                if (indiceAtacante < 0 || indiceAtacante >= totalTerritorio) {
                    printf("Territorio atacante invalido!\n");
                    printf("\n Pressione ENTER para continuar...");
                    getchar();
                    break;
                }
                
                /* Verifica se atacante tem tropas suficientes (mínimo 2) */
                if (mapa[indiceAtacante].quantidadeDeTropas < 2) {
                    printf("Territorio atacante precisa de pelo menos 2 tropas!\n");
                    printf("\n Pressione ENTER para continuar...");
                    getchar();
                    break;
                }
                
                /* Seleciona território defensor */
                printf("Digite o numero do territorio DEFENSOR: ");
                if (scanf(" %d", &indiceDefensor) != 1) {
                    printf("Entrada invalida!\n");
                    limparBufferEntrada();
                    printf("\n Pressione ENTER para continuar...");
                    getchar();
                    break;
                }
                limparBufferEntrada();
                indiceDefensor--;  /* Ajusta para índice 0-based */
                
                /* Valida índice do defensor */
                if (indiceDefensor < 0 || indiceDefensor >= totalTerritorio) {
                    printf("Territorio defensor invalido!\n");
                    printf("\n Pressione ENTER para continuar...");
                    getchar();
                    break;
                }
                
                /* Verifica se não está atacando a si mesmo */
                if (indiceAtacante == indiceDefensor) {
                    printf("Nao pode atacar o proprio territorio!\n");
                    printf("\n Pressione ENTER para continuar...");
                    getchar();
                    break;
                }
                
                /* Verifica se são da mesma cor (aliados) */
                if (strcmp(mapa[indiceAtacante].cor_do_exercito, 
                          mapa[indiceDefensor].cor_do_exercito) == 0) {
                    printf("Nao pode atacar territorios da mesma cor (aliados)!\n");
                    printf("\n Pressione ENTER para continuar...");
                    getchar();
                    break;
                }
                
                /* 
                 * AQUI ESTÁ O USO DE PONTEIROS!
                 * Passamos o ENDEREÇO dos elementos do array usando &
                 * &mapa[indice] retorna um ponteiro (territorio*)
                 * A função recebe esses ponteiros e pode modificar os valores originais
                 */
                realizarBatalha(&mapa[indiceAtacante], &mapa[indiceDefensor]);

                printf("\n Pressione ENTER para continuar...");
                getchar();
                break;
            
            case 4: // sistema de missão //

                printf("--- Sistema de Missões ---\n\n");
            for(int j = 0; j < 5; j++) {
                printf("Missão numero %d:\n", j + 1);
                printf("Nome: %s\n", missoes[j].nome);
                printf("Objetivo: %s\n", missoes[j].objetivo);
                printf("Status: %s\n", missoes[j].status);
                printf("Destinado para: %s\n", missoes[j].missaoDestinado);
                printf("\n");
                printf("-----------------------\n ");
            };
                
                // ATRIBUIR MISSÃO A UM TERRITÓRIO

                printf("Digite o numero do territorio para receber a missão (1 a %d): ", totalTerritorio);
                scanf(" %d", &opcao);
                limparBufferEntrada();

                // atribuir missão aleatória para o território selecionado
                //for (int i = 0; i < 6; i++){
                //     escolhaMissao = rand() % 6;
                //}

                if (opcao < 1 || opcao > totalTerritorio){
                    printf(" Numero de territorio inválido \n");
                    printf("\n pressione ENTER para continuar...");
                    getchar();
                    break;
                };

                // Atribuir a missao disponível para o territorio selecionado

                printf("Digite o numero da missão para atribuir ao territorio (1 a 5) (OBS: Missões só podem ser atribuidas a um territorio.): \n");
                scanf(" %d", &escolhaMissao);
                limparBufferEntrada();
                
                if (escolhaMissao < 1 || escolhaMissao > 5){
                    printf(" Numero de missão inválido \n");
                    printf("\n pressione ENTER para continuar...");
                    getchar();
                    break;
                };

                int duplicado2 = 0;

                printf("DEBUG: missaoDestinado='%s' (len=%zu)\n", 
                    missoes[escolhaMissao - 1].missaoDestinado, 
                    strlen(missoes[escolhaMissao - 1].missaoDestinado));
                printf("DEBUG: missaoRecebida='%s' (len=%zu)\n", 
                    mapa[opcao - 1].missaoRecebida, 
                    strlen(mapa[opcao - 1].missaoRecebida));

                // Missão já foi atribuída a algum território?
                if (strlen(missoes[escolhaMissao - 1].missaoDestinado) > 0) {
                    duplicado2 = 1;
                }

                // Território já possui uma missão?
                if (strlen(mapa[opcao - 1].missaoRecebida) > 0) {
                    duplicado2 = 1;
                }

                if (duplicado2) {
                    printf("Missão já atribuída a outro territorio ou territorio já tem uma missão! Escolha outro territorio.\n");
                    printf("\n Pressione ENTER para continuar...");
                    getchar();
                    break;
                } else{

                strcpy(mapa[opcao - 1].missaoRecebida, missoes[escolhaMissao - 1].nome);
                strcpy(missoes[escolhaMissao - 1].missaoDestinado, mapa[opcao - 1].nome);

                printf("Missão '%s' atribuída ao territorio '%s' com sucesso!\n", 
                       missoes[escolhaMissao - 1].nome, mapa[opcao - 1].nome);

                printf("\n Pressione ENTER para continuar...");
                getchar();
                };

                break;

            case 5: // verificar missão concluida

                // atualizar as missoes defesa ataque e conquista
                // verificar se o atacante conquistou o territorio defensor
                
                // Missão 1: "Primeira vitória" - Conquistar 1 território inimigo
                if (strlen(missoes[0].missaoDestinado) == 0){
                    printf("Missão 'Primeira vitória' ainda nao foi atribuida a nenhum território.\n");
                } else {
                    for (int i = 0; i < totalTerritorio; i++){
                        if(strcmp(mapa[i].nome, missoes[0].missaoDestinado) == 0){
                            if (mapa[i].vitoriaPts >= 1){
                                strcpy(missoes[0].status, "Concluido");
                                printf("Missão 'Primeira vitória' concluída por %s!\n", mapa[i].nome);
                                printf("DEBUG: vitoriaPts=%d\n", mapa[i].vitoriaPts);
                            } else{
                                printf("Missão 'Primeira vitória' ainda nao foi concluida por %s. (Vitórias: %d/1\n",
                                       mapa[i].nome, mapa[i].vitoriaPts);
                            }
                        }
                    }
                }

                // Missão 2: "Defensor Implacável"

                if (strlen(missoes[1].missaoDestinado) == 0){
                    printf("Missão 'Defensor Implacável' ainda nao foi atribuida a nenhum território.\n");
                } else {
                    for (int i = 0; i < totalTerritorio; i++){
                        if(strcmp(mapa[i].nome, missoes[1].missaoDestinado) == 0){
                            if (mapa[i].defesaPts >= 1){
                                strcpy(missoes[1].status, "Concluido");
                                printf("Missão 'Defensor Implacável' concluída por %s!\n", mapa[i].nome);
                                printf("DEBUG: defesaPts=%d\n", mapa[i].defesaPts);
                            } else{
                                printf("Missão 'Defensor Implacável' ainda nao foi concluida por %s. (Defesas: %d/1\n",
                                       mapa[i].nome, mapa[i].defesaPts);
                            }
                        }
                    }
                }

                // Missão 3: "Vencedor supremo" - Ter total de 4 vitórias

                if (strlen(missoes[2].missaoDestinado) == 0){
                    printf("Missão 'Vencedor supremo' ainda nao foi atribuida a nenhum território.\n");
                } else {
                    for (int i = 0; i < totalTerritorio; i++){
                        if(strcmp(mapa[i].nome, missoes[2].missaoDestinado) == 0){
                            if (mapa[i].vitoriaPts >= 4){
                                strcpy(missoes[2].status, "Concluido");
                                printf("Missão 'Vencedor supremo' concluída por %s!\n", mapa[i].nome);
                                printf("DEBUG: vitoriaPts=%d\n", mapa[i].vitoriaPts);
                            } else{
                                printf("Missão 'Vencedor supremo' ainda nao foi concluida por %s. (Vitórias: %d/4\n",
                                       mapa[i].nome, mapa[i].vitoriaPts);
                            }
                        }
                    }
                }

                // Missão 4: "Dominação" - Controlar 2 territórios ao mesmo tempo

                if (strlen(missoes[3].missaoDestinado) == 0){
                    printf("Missão 'Dominação' ainda nao foi atribuida a nenhum território.\n");
                } else {
                    for (int i = 0; i < totalTerritorio; i++){
                        if(strcmp(mapa[i].nome, missoes[3].missaoDestinado) == 0){
                            // Contar quantos territorios tem a mesma cor do exercito
                            int territoriosControlados = 0;
                            for (int j = 0; j < totalTerritorio; j++){
                                if(strcmp(mapa[j].cor_do_exercito, mapa[i].cor_do_exercito) == 0){
                                    territoriosControlados++;
                                }
                            }

                            if (territoriosControlados >= 2){
                                strcpy(missoes[3].status, "Concluido");
                                printf("Missão 'Dominação' concluída por %s!\n", mapa[i].nome);
                            } else{
                                printf("Missão 'Dominação' ainda nao foi concluida por %s. (Territórios: %d/2\n",
                                       mapa[i].nome, territoriosControlados);
                            }
                        }
                    }
                }
                // Missão 5: "Atacante implacável" - Vencer 3 batalhas

                if (strlen(missoes[4].missaoDestinado) == 0){
                    printf("Missão 'Atacante implacável' ainda nao foi atribuida a nenhum território.\n");
                } else {
                    for (int i = 0; i < totalTerritorio; i++){
                        if(strcmp(mapa[i].nome, missoes[4].missaoDestinado) == 0){
                            if (mapa[i].vitoriaPts >= 3){
                                strcpy(missoes[4].status, "Concluido");
                                printf("Missão 'Atacante implacável' concluída por %s!\n", mapa[i].nome);
                            } else{
                                printf("Missão 'Atacante implacável' ainda nao foi concluida por %s. (Vitórias: %d/1\n",
                                       mapa[i].nome, mapa[i].vitoriaPts);
                            }
                        }
                    }
                }

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

