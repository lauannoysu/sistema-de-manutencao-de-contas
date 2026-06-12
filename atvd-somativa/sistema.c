#include <stdio.h>
#include <string.h>

#define ARQ "clientes.dat"
#define TAM 100
#define TAM_NOME 50

typedef struct
{
    int conta;
    char nome[TAM_NOME];
    float saldo;
    int ocupado;
} Cliente;

void criarArquivo()
{
    FILE *arq;
    Cliente c = {0, "", 0, 0};

    arq = fopen(ARQ, "rb");

    if (arq != NULL)
    {
        fclose(arq);
        return;
    }

    arq = fopen(ARQ, "wb");

    for (int i = 0; i < TAM; i++)
    {
        fwrite(&c, sizeof(Cliente), 1, arq);
    }

    fclose(arq);
}

void cadastrar()
{
    FILE *arq;
    Cliente c;
    int pos;

    arq = fopen(ARQ, "rb+");

    printf("Digite a posicao do cliente: ");
    scanf("%d", &pos);

    if (pos < 0 || pos >= TAM)
    {
        printf("Posicao invalida.\n");
        fclose(arq);
        return;
    }

    fseek(arq, pos * sizeof(Cliente), SEEK_SET);
    fread(&c, sizeof(Cliente), 1, arq);

    if (c.ocupado == 1)
    {
        printf("Ja existe cliente nessa posicao.\n");
        fclose(arq);
        return;
    }

    printf("Numero da conta: ");
    scanf("%d", &c.conta);

    printf("Nome: ");
    getchar();
    fgets(c.nome, 50, stdin);
    c.nome[strcspn(c.nome, "\n")] = '\0';

    printf("Saldo: ");
    scanf("%f", &c.saldo);

    c.ocupado = 1;

    fseek(arq, pos * sizeof(Cliente), SEEK_SET);
    fwrite(&c, sizeof(Cliente), 1, arq);

    fclose(arq);

    printf("Cliente cadastrado.\n");
}

void consultar()
{
    FILE *arq;
    Cliente c;
    int num, achou = 0;

    arq = fopen(ARQ, "rb");

    printf("Digite o numero da conta: ");
    scanf("%d", &num);

    while (fread(&c, sizeof(Cliente), 1, arq) == 1)
    {
        if (c.ocupado == 1 && c.conta == num)
        {
            printf("\nConta: %d\n", c.conta);
            printf("Nome: %s\n", c.nome);
            printf("Saldo: %.2f\n", c.saldo);
            achou = 1;
            break;
        }
    }

    if (achou == 0)
    {
        printf("Conta nao encontrada.\n");
    }

    fclose(arq);
}

void alterarSaldo()
{
    FILE *arq;
    Cliente c;
    int num, achou = 0;

    arq = fopen(ARQ, "rb+");

    printf("Digite o numero da conta: ");
    scanf("%d", &num);

    while (fread(&c, sizeof(Cliente), 1, arq) == 1)
    {
        if (c.ocupado == 1 && c.conta == num)
        {
            printf("Saldo atual: %.2f\n", c.saldo);
            printf("Novo saldo: ");
            scanf("%f", &c.saldo);

            // volta para o registro que foi lido
            fseek(arq, -(long)sizeof(Cliente), SEEK_CUR);
            fwrite(&c, sizeof(Cliente), 1, arq);

            achou = 1;
            printf("Saldo alterado.\n");
            break;
        }
    }

    if (achou == 0)
    {
        printf("Conta nao encontrada.\n");
    }

    fclose(arq);
}

void removerConta()
{
    FILE *arq;
    Cliente c;
    int num, achou = 0;

    arq = fopen(ARQ, "rb+");

    printf("Digite a conta que deseja remover: ");
    scanf("%d", &num);

    while (fread(&c, sizeof(Cliente), 1, arq) == 1)
    {
        if (c.ocupado == 1 && c.conta == num)
        {
            c.ocupado = 0;
            c.conta = 0;
            strcpy(c.nome, "");
            c.saldo = 0;

            fseek(arq, -(long)sizeof(Cliente), SEEK_CUR);
            fwrite(&c, sizeof(Cliente), 1, arq);

            achou = 1;
            printf("Conta removida.\n");
            break;
        }
    }

    if (achou == 0)
    {
        printf("Conta nao encontrada.\n");
    }

    fclose(arq);
}

void listar()
{
    FILE *arq;
    Cliente c;
    int temCliente = 0;

    arq = fopen(ARQ, "rb");

    printf("\n--- Clientes cadastrados ---\n");

    while (fread(&c, sizeof(Cliente), 1, arq) == 1)
    {
        if (c.ocupado == 1)
        {
            printf("Conta: %d | Nome: %s | Saldo: %.2f\n",
                   c.conta, c.nome, c.saldo);
            temCliente = 1;
        }
    }

    if (temCliente == 0)
    {
        printf("Nenhum cliente cadastrado.\n");
    }

    fclose(arq);
}

void listarComRewind()
{
    FILE *arq;
    Cliente c;

    arq = fopen(ARQ, "rb");

    printf("\nListando pela primeira vez:\n");

    while (fread(&c, sizeof(Cliente), 1, arq) == 1)
    {
        if (c.ocupado == 1)
        {
            printf("Conta: %d | Nome: %s | Saldo: %.2f\n",
                   c.conta, c.nome, c.saldo);
        }
    }

    rewind(arq);

    printf("\nListando novamente depois do rewind():\n");

    while (fread(&c, sizeof(Cliente), 1, arq) == 1)
    {
        if (c.ocupado == 1)
        {
            printf("Conta: %d | Nome: %s | Saldo: %.2f\n",
                   c.conta, c.nome, c.saldo);
        }
    }

    fclose(arq);
}

int main()
{
    int op;

    criarArquivo();

    do
    {
        printf("\n===== MENU =====\n");
        printf("1 - Cadastrar cliente\n");
        printf("2 - Consultar cliente\n");
        printf("3 - Atualizar saldo\n");
        printf("4 - Encerrar conta\n");
        printf("5 - Listar clientes\n");
        printf("6 - Repetir listagem com rewind\n");
        printf("7 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &op);

        switch (op)
        {
        case 1:
            cadastrar();
            break;
        case 2:
            consultar();
            break;
        case 3:
            alterarSaldo();
            break;
        case 4:
            removerConta();
            break;
        case 5:
            listar();
            break;
        case 6:
            listarComRewind();
            break;
        case 7:
            printf("Programa encerrado.\n");
            break;
        default:
            printf("Opcao invalida.\n");
        }

    } while (op != 7);

    return 0;
}
