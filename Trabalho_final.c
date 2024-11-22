#include <stdio.h>
#include <string.h>
#include <Windows.h>

typedef struct reg 
{
    char titulo[100];
    char autor[50];
    char ano[20];
    char status;
} reg;

int total(FILE *); // total de registros
void cadastrar(FILE*);
void consultar(FILE*);
void geraarqtxt(FILE*);
void excluir(FILE*);
int main() 
{
    int op;
    FILE *arq;

    if ((arq = fopen("biblioteca1.dat", "rb+")) == NULL) 
    {
        if ((arq = fopen("biblioteca1.dat", "wb+")) == NULL)
        {
            printf("Falha ao abrir o arquivo!\n");
            system("pause");
        }
    }

    do { // menu de opcoes
        system("CLS");
        printf("\n============ Biblioteca ============\n");
        printf("1. Cadastrar Livro\n");
        printf("2. Consultar por codigo\n");
        printf("3. Gerar relatorio\n");
        printf("4. Excluir livro\n");
        printf("5. Sair\n");
        printf("=========== Livros: %d ============\n", total(arq));
        printf("Opcao: ");
        scanf("%d", &op);
        switch (op)
        {
            case 1: // cadastrar novo livro
                cadastrar(arq);
                break;
            case 2: // consultar por codigo
                consultar(arq);
                break;
            case 3: // gerar arquivo texto com todos os contatos
                geraarqtxt(arq);
                break;
            case 4: // exclui um registro do arq
                excluir(arq);
                break;
            case 5: fclose(arq);
        }
    } while (op != 5);
}

void cadastrar(FILE* arq)
{
    reg livro;
    char confirma;
    livro.status = ' ';
    fflush(stdin);

    printf("Cadastrar novo livro:\n");
    printf("\nNumero do registro: %d\n", total(arq) + 1);
    printf("Titulo.......:");
    fgets(livro.titulo, sizeof(livro.titulo), stdin);
    livro.titulo[strcspn(livro.titulo, "\n")] = '\0'; 

    printf("Autor.......:");
    fgets(livro.autor, sizeof(livro.autor), stdin);
    livro.autor[strcspn(livro.autor, "\n")] = '\0'; 

    printf("Ano.........:");
    fgets(livro.ano, sizeof(livro.ano), stdin);
    livro.ano[strcspn(livro.ano, "\n")] = '\0'; 

    printf("\nConfirma <s/n>: ");
    fflush(stdin); 
    scanf("%c", &confirma);

    if (toupper(confirma) == 'S') {
        printf("\ngravando...\n\n");
        fseek(arq, 0, SEEK_END);
        fwrite(&livro, sizeof(reg), 1, arq); 
    }
    system("pause");
}

void consultar(FILE* arq)
{
    reg livro;
    int nr;
    printf("\nConsulta pelo codigo\n");
    printf("\nInforme o Codigo....:");
    scanf("%d", &nr);
    if ((nr <= total(arq)) && (nr > 0)) {
        fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
        fread(&livro, sizeof(reg), 1, arq);
        if (livro.status == ' ') {
            printf("\nTitulo.....:%s", livro.titulo);
            printf("\nAutor.....:%s", livro.autor);
            printf("\nAno.....:%s\n\n", livro.ano);
        }
        else
            printf("\nRegistro excluido! \n");
    }
    else {
        printf("\nNumero de registro invalido!\n");
    }
    system("pause");
}

void geraarqtxt(FILE* arq)
{
    char nomearq[20];
    printf("Nome do arquivo texto: ");
    scanf("%s", nomearq);
    strcat(nomearq, ".txt");
    FILE* arqtxt = fopen(nomearq, "w");
    if (!arqtxt) {
        printf("Nao foi possivel criar esse arquivo!\n");
        system("pause");
        return;
    }

    fprintf(arqtxt, "Titulo                 Autor                     Ano        Status\n");
    fprintf(arqtxt, "===================================================================\n");

    int nr;
    reg livro;
    for (nr = 0; nr < total(arq); nr++) {
        fseek(arq, nr * sizeof(reg), SEEK_SET);
        fread(&livro, sizeof(reg), 1, arq);
        fprintf(arqtxt, "%-20s%-25s%-10s- %c\n", livro.titulo, livro.autor, livro.ano, livro.status);
    }
    fprintf(arqtxt, "====================================================================\n");
    fclose(arqtxt);
}

void excluir(FILE* arq)
{
    reg livro;
    char confirma;
    int nr;
    printf("\nInforme o codigo do registro para excluir\n");
    scanf("%d", &nr);
    if ((nr <= total(arq)) && (nr > 0)) {
        fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
        fread(&livro, sizeof(reg), 1, arq);
        if (livro.status == ' ') {
            printf("\nTitulo....:%s", livro.titulo);
            printf("\nAutor....:%s", livro.autor);
            printf("\nAno....:%s", livro.ano);
            printf("\nConfirma a exclusao: <s/n>\n");
            getchar();
            scanf("%c", &confirma);

            if (toupper(confirma) == 'S') {
                printf("\nexcluindo...\n\n");
                fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
                livro.status = '*';
                fwrite(&livro, sizeof(reg), 1, arq);
            }
        }
        else
            printf("Registro inexistente! \n");
    }
    else {
        printf("\nNumero de registro invalido!\n");
    }
    system("pause");
}

int total(FILE* arq)
{
    fseek(arq, 0, SEEK_END);
    return ftell(arq) / sizeof(reg);
}
