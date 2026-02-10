#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#define NUM_TERRITORIOS 5
#define TAM_STRING 30
#define NUM_MISSOES 2

// =====================
// Estrutura de Dados
// =====================
typedef struct {
    char nome[TAM_STRING];
    char cor[TAM_STRING];
    int tropas;
} Territorio;

// =====================
// Protótipos
// =====================
Territorio* alocarMapa(void);
void inicializarTerritorios(Territorio *mapa);
void liberarMemoria(Territorio *mapa);

void exibirMenuPrincipal(void);
void exibirMapa(const Territorio *mapa);
void exibirMissao(int missao, const char *corJogador);

void faseDeAtaque(Territorio *mapa, const char *corJogador);
void simularAtaque(Territorio *atacante, Territorio *defensor, const char *corJogador);

int sortearMissao(void);
int verificarVitoria(const Territorio *mapa, int missao, const char *corJogador);

void limparBufferEntrada(void);

// =====================
// Função Principal
// =====================
int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    Territorio *mapa = alocarMapa();
    if (!mapa) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    inicializarTerritorios(mapa);

    const char corJogador[] = "Azul";
    int missao = sortearMissao();
    int opcao;
    int venceu = 0;

    do {
        exibirMapa(mapa);
        exibirMissao(missao, corJogador);
        exibirMenuPrincipal();

        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, corJogador);
                break;

            case 2:
                venceu = verificarVitoria(mapa, missao, corJogador);
                if (venceu) {
                    printf("\n🎉 PARABÉNS! Você cumpriu a missão!\n");
                } else {
                    printf("\n❌ Missão ainda não concluída.\n");
                }
                break;

            case 0:
                printf("\nSaindo do jogo...\n");
                break;

            default:
                printf("\nOpção inválida!\n");
        }

        if (!venceu && opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }

    } while (opcao != 0 && !venceu);

    liberarMemoria(mapa);
    return 0;
}

// =====================
// Implementações
// =====================
Territorio* alocarMapa(void) {
    return (Territorio *) calloc(NUM_TERRITORIOS, sizeof(Territorio));
}

void inicializarTerritorios(Territorio *mapa) {
    const char *nomes[] = {
        "Brasil", "Argentina", "Chile", "Peru", "Colômbia"
    };

    const char *cores[] = {
        "Azul", "Verde", "Vermelho", "Verde", "Amarelo"
    };

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        strcpy(mapa[i].nome, nomes[i]);
        strcpy(mapa[i].cor, cores[i]);
        mapa[i].tropas = rand() % 5 + 3; // 3 a 7 tropas
    }
}

void liberarMemoria(Territorio *mapa) {
    free(mapa);
}

void exibirMenuPrincipal(void) {
    printf("\n===== MENU =====\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missão\n");
    printf("0 - Sair\n");
}

void exibirMapa(const Territorio *mapa) {
    printf("\n===== MAPA =====\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%d) %-10s | Cor: %-10s | Tropas: %d\n",
               i + 1,
               mapa[i].nome,
               mapa[i].cor,
               mapa[i].tropas);
    }
}

void exibirMissao(int missao, const char *corJogador) {
    printf("\n===== MISSÃO =====\n");

    if (missao == 0) {
        printf("Destruir todos os territórios do exército VERDE.\n");
    } else {
        printf("Conquistar pelo menos 3 territórios com o exército %s.\n", corJogador);
    }
}

void faseDeAtaque(Territorio *mapa, const char *corJogador) {
    int a, d;

    printf("\nTerritório ATACANTE (1 a %d): ", NUM_TERRITORIOS);
    scanf("%d", &a);
    printf("Território DEFENSOR (1 a %d): ", NUM_TERRITORIOS);
    scanf("%d", &d);
    limparBufferEntrada();

    a--; d--;

    if (a < 0 || d < 0 || a >= NUM_TERRITORIOS || d >= NUM_TERRITORIOS || a == d) {
        printf("Escolha inválida!\n");
        return;
    }

    simularAtaque(&mapa[a], &mapa[d], corJogador);
}

void simularAtaque(Territorio *atacante, Territorio *defensor, const char *corJogador) {
    if (strcmp(atacante->cor, corJogador) != 0) {
        printf("Você só pode atacar com seus territórios!\n");
        return;
    }

    if (atacante->tropas < 2) {
        printf("Tropas insuficientes para atacar!\n");
        return;
    }

    int dadoAtaque = rand() % 6 + 1;
    int dadoDefesa = rand() % 6 + 1;

    printf("\n⚔️ BATALHA ⚔️\n");
    printf("Ataque (%s): %d\n", atacante->nome, dadoAtaque);
    printf("Defesa (%s): %d\n", defensor->nome, dadoDefesa);

    if (dadoAtaque >= dadoDefesa) {
        defensor->tropas--;
        printf("Defensor perdeu 1 tropa!\n");

        if (defensor->tropas <= 0) {
            printf("🏴 Território CONQUISTADO!\n");
            strcpy(defensor->cor, corJogador);
            defensor->tropas = 1;
            atacante->tropas--;
        }
    } else {
        atacante->tropas--;
        printf("Atacante perdeu 1 tropa!\n");
    }
}

int sortearMissao(void) {
    return rand() % NUM_MISSOES;
}

int verificarVitoria(const Territorio *mapa, int missao, const char *corJogador) {
    if (missao == 0) {
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, "Verde") == 0) {
                return 0;
            }
        }
        return 1;
    }

    int contador = 0;
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0) {
            contador++;
        }
    }

    return contador >= 3;
}

void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}