#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>

typedef struct ArvoreNo {
    int info;
    int altura;
    struct ArvoreNo* esquerda;
    struct ArvoreNo* direita;
} ArvoreNo;

int altura(ArvoreNo* no) {
    if (no == NULL)
        return 0;
    return no->altura;
}

ArvoreNo* alocar(int info) {
    ArvoreNo* novoNo = (ArvoreNo*)malloc(sizeof(ArvoreNo));
    if (novoNo != NULL) {
        novoNo->info = info;
        novoNo->altura = 1;
        novoNo->esquerda = NULL;
        novoNo->direita = NULL;
    }
    return novoNo;
}

int fatorDeBalanceamento(ArvoreNo* no) {
    if (no == NULL)
        return 0;
    return altura(no->esquerda) - altura(no->direita);
}

ArvoreNo* rotacaoDireita(ArvoreNo* y) {
    ArvoreNo* x = y->esquerda;
    ArvoreNo* T2 = x->direita;

    x->direita = y;
    y->esquerda = T2;

    y->altura = fmax(altura(y->esquerda), altura(y->direita)) + 1;
    x->altura = fmax(altura(x->esquerda), altura(x->direita)) + 1;

    return x;
}

ArvoreNo* rotacaoEsquerda(ArvoreNo* x) {
    ArvoreNo* y = x->direita;
    ArvoreNo* T2 = y->esquerda;

    y->esquerda = x;
    x->direita = T2;

    x->altura = fmax(altura(x->esquerda), altura(x->direita)) + 1;
    y->altura = fmax(altura(y->esquerda), altura(y->direita)) + 1;

    return y;
}

ArvoreNo* Inserir(ArvoreNo* no, int info) {
    if (no == NULL)
        return alocar(info);

    if (info < no->info)
        no->esquerda = Inserir(no->esquerda, info);
    else if (info > no->info)
        no->direita = Inserir(no->direita, info);
    else
        return no;

    no->altura = 1 + fmax(altura(no->esquerda), altura(no->direita));

    int balance = fatorDeBalanceamento(no);

    if (balance > 1 && info < no->esquerda->info)
        return rotacaoDireita(no);

    if (balance < -1 && info > no->direita->info)
        return rotacaoEsquerda(no);

    if (balance > 1 && info > no->esquerda->info) {
        no->esquerda = rotacaoEsquerda(no->esquerda);
        return rotacaoDireita(no);
    }

    if (balance < -1 && info < no->direita->info) {
        no->direita = rotacaoDireita(no->direita);
        return rotacaoEsquerda(no);
    }

    return no;
}

void emOrdem(ArvoreNo* raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esquerda);
        printf("%d ", raiz->info);
        emOrdem(raiz->direita);
    }
}

int Buscar(ArvoreNo* raiz, int info) {
    if (raiz != NULL) {
        if (raiz->info > info) {
            return Buscar(raiz->esquerda, info);
        } else if (raiz->info < info) {
            return Buscar(raiz->direita, info);
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}

void Liberar(ArvoreNo* no) {
    free(no);
}

ArvoreNo* encontrarMinimo(ArvoreNo* no) {
    while (no->esquerda != NULL) {
        no = no->esquerda;
    }
    return no;
}

ArvoreNo* Remover(ArvoreNo* raiz, int info) {
    if (raiz == NULL)
        return raiz;

    if (info < raiz->info)
        raiz->esquerda = Remover(raiz->esquerda, info);
    else if (info > raiz->info)
        raiz->direita = Remover(raiz->direita, info);
    else {
        if ((raiz->esquerda == NULL) || (raiz->direita == NULL)) {
            ArvoreNo* temp = raiz->esquerda ? raiz->esquerda : raiz->direita;

            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else
                *raiz = *temp;

            Liberar(temp);
        } else {
            ArvoreNo* temp = encontrarMinimo(raiz->direita);
            raiz->info = temp->info;
            raiz->direita = Remover(raiz->direita, temp->info);
        }
    }

    if (raiz == NULL)
        return raiz;

    raiz->altura = 1 + fmax(altura(raiz->esquerda), altura(raiz->direita));

    int balance = fatorDeBalanceamento(raiz);

    if (balance > 1 && fatorDeBalanceamento(raiz->esquerda) >= 0)
        return rotacaoDireita(raiz);

    if (balance > 1 && fatorDeBalanceamento(raiz->esquerda) < 0) {
        raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
        return rotacaoDireita(raiz);
    }

    if (balance < -1 && fatorDeBalanceamento(raiz->direita) <= 0)
        return rotacaoEsquerda(raiz);

    if (balance < -1 && fatorDeBalanceamento(raiz->direita) > 0) {
        raiz->direita = rotacaoDireita(raiz->direita);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

void imprimirArvore(ArvoreNo* raiz, int nivel) {
    if (raiz == NULL)
        return;

    imprimirArvore(raiz->direita, nivel + 1);

    for (int i = 0; i < nivel; i++)
        printf("    ");

    printf("%d\n", raiz->info);

    imprimirArvore(raiz->esquerda, nivel + 1);
}

void exibirMenu() {
    printf("\n--- Árvore Binaria ---\n");
    printf("1. Inserir\n");
    printf("2. Remover\n");
    printf("3. Buscar\n");
    printf("4. Exibir em ordem\n");
    printf("5. Exibir em arvore\n");
    printf("6. Sair\n");
    printf("Escolha uma opção: ");
}

int main() {
    ArvoreNo* raiz = NULL;
    int opcao, valor;
    setlocale(LC_ALL, "portuguese");

    do {
        exibirMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o valor a ser inserido: ");
                scanf("%d", &valor);
                raiz = Inserir(raiz, valor);
                printf("Valor %d inserido.\n", valor);
                break;

            case 2:
                printf("Digite o valor a ser removido: ");
                scanf("%d", &valor);
                raiz = Remover(raiz, valor);
                printf("Valor %d removido.\n", valor);
                printf("Elementos da árvore após a remoção: ");
                emOrdem(raiz);
                printf("\n");
                break;

            case 3:
                printf("Digite o valor a ser buscado: ");
                scanf("%d", &valor);
                if (Buscar(raiz, valor)) {
                    printf("Valor %d encontrado na árvore.\n", valor);
                } else {
                    printf("Valor %d não encontrado na árvore.\n", valor);
                }
                break;

            case 4:
                printf("Elementos da árvore em ordem: ");
                emOrdem(raiz);
                printf("\n");
                break;

			case 5:
   				 printf("Árvore em formato de árvore:\n");
   				 imprimirArvore(raiz, 0);
   				 break;
   				 
            case 6:
                printf("Saindo...\n");
                break;
                
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 6);

    return 0;
}

