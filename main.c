#include <stdio.h>

#define MAX 10
#define TAM 30



typedef struct {
    char nome[TAM];
    int categoria;      /* 1-Laticinios 2-Carnes 3-Verduras 4-Graos e Secos */
    int local;          /* 1-Geladeira 2-Freezer 3-Despensa */
    int quantidade;
    int dia;
    int mes;
    int ano;
} Alimento;

Alimento estoque[MAX];
int total = 0;

int diaHoje, mesHoje, anoHoje;

int dataValida(int dia, int mes, int ano) {
    if (ano < 2024) {
        return 0;
    }
    if (mes < 1 || mes > 12) {
        return 0;
    }
    if (dia < 1) {
        return 0;
    }

    if (mes == 2) {
        if (dia > 29) return 0;
    } else if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        if (dia > 30) return 0;
    } else {
        if (dia > 31) return 0;
    }

    return 1;
}

/* Aqui vai ter que encaixar a situacao: 1-VENCIDO, 2-VENCE HOJE, 3-EM DIA */
int calcularSituacao(int dia, int mes, int ano) {
    if (ano < anoHoje) return 1;
    if (ano > anoHoje) return 3;

    if (mes < mesHoje) return 1;
    if (mes > mesHoje) return 3;

    if (dia < diaHoje) return 1;
    if (dia > diaHoje) return 3;

    return 2;
}

void mostrarCategoria(int codigo) {
    if (codigo == 1) {
        printf("Laticinios");
    } else if (codigo == 2) {
        printf("Carnes");
    } else if (codigo == 3) {
        printf("Verduras");
    } else {
        printf("Graos e Secos");
    }
}

void mostrarLocal(int codigo) {
    if (codigo == 1) {
        printf("Geladeira");
    } else if (codigo == 2) {
        printf("Freezer");
    } else {
        printf("Despensa");
    }
}

void mostrarSituacao(int codigo) {
    if (codigo == 1) {
        printf("VENCIDO");
    } else if (codigo == 2) {
        printf("VENCE HOJE");
    } else {
        printf("EM DIA");
    }
}


int dadosValidos(Alimento a) {
    if (a.categoria < 1 || a.categoria > 4) {
        printf("Categoria invalida.\n");
        return 0;
    }
    if (a.local < 1 || a.local > 3) {
        printf("Local invalido.\n");
        return 0;
    }
    if (a.quantidade < 0) {
        printf("Quantidade invalida.\n");
        return 0;
    }
    if (!dataValida(a.dia, a.mes, a.ano)) {
        printf("Data invalida.\n");
        return 0;
    }
    return 1;
}


void cadastrarAlimento(void) {
    Alimento novo;

    if (total >= MAX) {
        printf("Estoque cheio!\n");
        return;
    }

    printf("\n--- CADASTRAR ---\n");

    printf("Nome do alimento (sem espacos): ");
    scanf("%29s", novo.nome);

    printf("Categoria (1-Laticinios, 2-Carnes, 3-Verduras, 4-Graos e Secos): ");
    scanf("%d", &novo.categoria);

    printf("Local (1-Geladeira, 2-Freezer, 3-Despensa): ");
    scanf("%d", &novo.local);

    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);

    printf("Dia da validade: ");
    scanf("%d", &novo.dia);

    printf("Mes da validade: ");
    scanf("%d", &novo.mes);

    printf("Ano da validade: ");
    scanf("%d", &novo.ano);

    if (dadosValidos(novo)) {
        estoque[total] = novo;
        total++;
        printf("Alimento cadastrado com sucesso!\n");
    }
}

void listarAlimentos(void) {
    int i;

    if (total == 0) {
        printf("Nenhum alimento cadastrado.\n");
        return;
    }

    printf("\n--- LISTA DE ALIMENTOS ---\n");
    for (i = 0; i < total; i++) {
        printf("[%d] %s\n", i + 1, estoque[i].nome);
        printf("    Categoria : ");
        mostrarCategoria(estoque[i].categoria);
        printf("\n    Local     : ");
        mostrarLocal(estoque[i].local);
        printf("\n    Quantidade: %d\n", estoque[i].quantidade);
        printf("    Validade  : %02d/%02d/%04d\n",
               estoque[i].dia, estoque[i].mes, estoque[i].ano);
        printf("    Situacao  : ");
        mostrarSituacao(calcularSituacao(estoque[i].dia, estoque[i].mes, estoque[i].ano));
        printf("\n\n");
    }
}


void atualizarAlimento(void) {
    int posicao;
    Alimento novo;

    if (total == 0) {
        printf("Nenhum alimento para atualizar.\n");
        return;
    }

    printf("\n--- ATUALIZAR ---\n");
    listarAlimentos();
    printf("Digite o numero do alimento: ");
    scanf("%d", &posicao);

    if (posicao < 1 || posicao > total) {
        printf("Numero invalido.\n");
        return;
    }

    posicao--;

    printf("Novo nome (sem espacos): ");
    scanf("%29s", novo.nome);

    printf("Nova categoria (1-Laticinios, 2-Carnes, 3-Verduras, 4-Graos e Secos): ");
    scanf("%d", &novo.categoria);

    printf("Novo local (1-Geladeira, 2-Freezer, 3-Despensa): ");
    scanf("%d", &novo.local);

    printf("Nova quantidade: ");
    scanf("%d", &novo.quantidade);

    printf("Novo dia: ");
    scanf("%d", &novo.dia);

    printf("Novo mes: ");
    scanf("%d", &novo.mes);

    printf("Novo ano: ");
    scanf("%d", &novo.ano);

    if (dadosValidos(novo)) {
        estoque[posicao] = novo;
        printf("Alimento atualizado com sucesso!\n");
    }
}

/* Aqui vai ser o comando de excluir da tabela  */
void excluirAlimento(void) {
    int posicao;
    int i;

    if (total == 0) {
        printf("Nenhum alimento para excluir.\n");
        return;
    }

    printf("\n--- EXCLUIR ---\n");
    listarAlimentos();
    printf("Digite o numero do alimento para excluir: ");
    scanf("%d", &posicao);

    if (posicao < 1 || posicao > total) {
        printf("Numero invalido.\n");
        return;
    }

    posicao--;

    for (i = posicao; i < total - 1; i++) {
        estoque[i] = estoque[i + 1];
    }

    total--;
    printf("Alimento excluido com sucesso!\n");
}

void verAlertasValidade(void) {
    int i;
    int situacao;

    if (total == 0) {
        printf("Nenhum alimento cadastrado.\n");
        return;
    }

    printf("\n--- ALERTAS DE VALIDADE ---\n");
    for (i = 0; i < total; i++) {
        situacao = calcularSituacao(estoque[i].dia, estoque[i].mes, estoque[i].ano);
        printf("%s - ", estoque[i].nome);
        mostrarSituacao(situacao);
        printf("\n");
    }
}

int main(void) {
    int opcao = -1;

    printf("=== RITA FAZ & SERVE ===\n");
    printf("=== CONTROLE DE VALIDADE DE ALIMENTOS ===\n\n");
    printf("Informe a data de hoje\n");
    printf("Dia: ");
    scanf("%d", &diaHoje);
    printf("Mes: ");
    scanf("%d", &mesHoje);
    printf("Ano: ");
    scanf("%d", &anoHoje);

    do {
        printf("\n=== MENU ===\n");
        printf("1 - Cadastrar alimento\n");
        printf("2 - Listar alimentos\n");
        printf("3 - Atualizar alimento\n");
        printf("4 - Excluir alimento\n");
        printf("5 - Ver alertas de validade\n");
        printf("0 - Sair\n");
        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1) {
            break;
        }

        switch (opcao) {
            case 1:
                cadastrarAlimento();
                break;
            case 2:
                listarAlimentos();
                break;
            case 3:
                atualizarAlimento();
                break;
            case 4:
                excluirAlimento();
                break;
            case 5:
                verAlertasValidade();
                break;
            case 0:
                printf("Programa encerrado.\n");
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}
