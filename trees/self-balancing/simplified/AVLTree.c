#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int valor;
    struct No *esquerda, *direita;
} No;

typedef struct ArvoreAVL {
    No *raiz;
} ArvoreAVL;

// Obtém a altura de um nó
int obterAltura(No *no) {
    if (no == NULL) return 0;
    int alturaEsquerda = obterAltura(no->esquerda);
    int alturaDireita = obterAltura(no->direita);
    return (alturaEsquerda > alturaDireita ? alturaEsquerda : alturaDireita) + 1;
}

// Obtém o fator de balanceamento de um nó
int obterBalanceamento(No *no) {
    return no ? obterAltura(no->esquerda) - obterAltura(no->direita) : 0;
}

// Cria um novo nó
No* criarNo(int valor) {
    No *novoNo = (No*)malloc(sizeof(No));
    novoNo->valor = valor;
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    return novoNo;
}

// Inicializa a árvore
ArvoreAVL* criarArvore() {
    ArvoreAVL *arvore = (ArvoreAVL*)malloc(sizeof(ArvoreAVL));
    arvore->raiz = NULL;
    return arvore;
}

// Rotação à esquerda no nó pivô
No* rotacaoEsquerda(No *noPivo) {
    No *filhoDireita = noPivo->direita;
    No *subarvoreEsquerda = filhoDireita->esquerda;
    
    filhoDireita->esquerda = noPivo;
    noPivo->direita = subarvoreEsquerda;
    
    return filhoDireita;
}

// Rotação à direita no nó pivô
No* rotacaoDireita(No *noPivo) {
    No *filhoEsquerda = noPivo->esquerda;
    No *subarvoreDireita = filhoEsquerda->direita;
    
    filhoEsquerda->direita = noPivo;
    noPivo->esquerda = subarvoreDireita;
    
    return filhoEsquerda;
}

// Balanceia o nó após inserção ou remoção
No* balancear(No *no) {
    int fatorBalanceamento = obterBalanceamento(no);
    
    // Caso Esquerda-Esquerda
    if (fatorBalanceamento > 1 && obterBalanceamento(no->esquerda) >= 0)
        return rotacaoDireita(no);
    
    // Caso Esquerda-Direita
    if (fatorBalanceamento > 1 && obterBalanceamento(no->esquerda) < 0) {
        no->esquerda = rotacaoEsquerda(no->esquerda);
        return rotacaoDireita(no);
    }
    
    // Caso Direita-Direita
    if (fatorBalanceamento < -1 && obterBalanceamento(no->direita) <= 0)
        return rotacaoEsquerda(no);
    
    // Caso Direita-Esquerda
    if (fatorBalanceamento < -1 && obterBalanceamento(no->direita) > 0) {
        no->direita = rotacaoDireita(no->direita);
        return rotacaoEsquerda(no);
    }
    
    return no;
}

// Função auxiliar de inserção
No* inserirNo(No *noAtual, int valor) {
    if (noAtual == NULL)
        return criarNo(valor);
    
    if (valor < noAtual->valor)
        noAtual->esquerda = inserirNo(noAtual->esquerda, valor);
    else if (valor > noAtual->valor)
        noAtual->direita = inserirNo(noAtual->direita, valor);
    else
        return noAtual; // Valores duplicados não são permitidos
    
    return balancear(noAtual);
}

// Insere um nó com o valor fornecido
void inserir(ArvoreAVL *arvore, int valor) {
    arvore->raiz = inserirNo(arvore->raiz, valor);
}

// Encontra o nó com valor mínimo na subárvore
No* encontrarMinimo(No *raizSubarvore) {
    while (raizSubarvore && raizSubarvore->esquerda)
        raizSubarvore = raizSubarvore->esquerda;
    return raizSubarvore;
}

// Encontra o nó com valor máximo na subárvore
No* encontrarMaximo(No *raizSubarvore) {
    while (raizSubarvore && raizSubarvore->direita)
        raizSubarvore = raizSubarvore->direita;
    return raizSubarvore;
}

// Função auxiliar de remoção
No* removerNoAuxiliar(No *noAtual, int valor) {
    if (noAtual == NULL)
        return noAtual;
    
    if (valor < noAtual->valor) {
        noAtual->esquerda = removerNoAuxiliar(noAtual->esquerda, valor);
    } else if (valor > noAtual->valor) {
        noAtual->direita = removerNoAuxiliar(noAtual->direita, valor);
    } else {
        if (noAtual->esquerda == NULL) {
            No *noTemporario = noAtual->direita;
            free(noAtual);
            return noTemporario;
        } else if (noAtual->direita == NULL) {
            No *noTemporario = noAtual->esquerda;
            free(noAtual);
            return noTemporario;
        }
        
        No *sucessor = encontrarMinimo(noAtual->direita);
        noAtual->valor = sucessor->valor;
        noAtual->direita = removerNoAuxiliar(noAtual->direita, sucessor->valor);
    }
    
    return balancear(noAtual);
}

// Remove um nó com o valor fornecido
void remover(ArvoreAVL *arvore, int valor) {
    arvore->raiz = removerNoAuxiliar(arvore->raiz, valor);
}

// Busca por um nó com o valor fornecido
No* buscar(ArvoreAVL *arvore, int valor) {
    No *noAtual = arvore->raiz;
    while (noAtual && noAtual->valor != valor) {
        if (valor < noAtual->valor)
            noAtual = noAtual->esquerda;
        else
            noAtual = noAtual->direita;
    }
    return noAtual;
}

// Encontra o sucessor usando percurso em-ordem
No* sucessorAuxiliar(No *noRaiz, No *noAlvo, No *sucessorAtual) {
    if (noRaiz == NULL)
        return sucessorAtual;
    
    if (noAlvo->valor < noRaiz->valor) {
        return sucessorAuxiliar(noRaiz->esquerda, noAlvo, noRaiz);
    } else if (noAlvo->valor > noRaiz->valor) {
        return sucessorAuxiliar(noRaiz->direita, noAlvo, sucessorAtual);
    } else {
        if (noRaiz->direita)
            return encontrarMinimo(noRaiz->direita);
        return sucessorAtual;
    }
}

No* sucessor(ArvoreAVL *arvore, No *no) {
    return sucessorAuxiliar(arvore->raiz, no, NULL);
}

// Encontra o predecessor usando percurso em-ordem
No* predecessorAuxiliar(No *noRaiz, No *noAlvo, No *predecessorAtual) {
    if (noRaiz == NULL)
        return predecessorAtual;
    
    if (noAlvo->valor < noRaiz->valor) {
        return predecessorAuxiliar(noRaiz->esquerda, noAlvo, predecessorAtual);
    } else if (noAlvo->valor > noRaiz->valor) {
        return predecessorAuxiliar(noRaiz->direita, noAlvo, noRaiz);
    } else {
        if (noRaiz->esquerda)
            return encontrarMaximo(noRaiz->esquerda);
        return predecessorAtual;
    }
}

No* predecessor(ArvoreAVL *arvore, No *no) {
    return predecessorAuxiliar(arvore->raiz, no, NULL);
}

// Encontra o valor mínimo na árvore inteira
No* encontrarMin(ArvoreAVL *arvore) {
    if (arvore->raiz == NULL)
        return NULL;
    return encontrarMinimo(arvore->raiz);
}

// Encontra o valor máximo na árvore inteira
No* encontrarMax(ArvoreAVL *arvore) {
    if (arvore->raiz == NULL)
        return NULL;
    return encontrarMaximo(arvore->raiz);
}

// Realiza percurso em-ordem e imprime os nós
void emOrdem(No *no) {
    if (no) {
        emOrdem(no->esquerda);
        printf("%d ", no->valor);
        emOrdem(no->direita);
    }
}

// Libera a memória da árvore
void liberarArvore(No *no) {
    if (no) {
        liberarArvore(no->esquerda);
        liberarArvore(no->direita);
        free(no);
    }
}

// Exemplo de uso demonstrando todas as operações
int main() {
    ArvoreAVL *arvore = criarArvore();
    
    // Inserir elementos
    int valores[] = {10, 20, 30, 15, 25, 5, 1};
    for (int i = 0; i < 7; i++) {
        inserir(arvore, valores[i]);
        printf("Após inserir %d: ", valores[i]);
        emOrdem(arvore->raiz);
        printf("\n");
    }
    
    // Buscar um nó
    No *encontrado = buscar(arvore, 15);
    if (encontrado)
        printf("\nEncontrado: %d\n", encontrado->valor);
    
    // Encontrar mínimo e máximo
    No *noMinimo = encontrarMin(arvore);
    No *noMaximo = encontrarMax(arvore);
    printf("Mínimo: %d, Máximo: %d\n", noMinimo->valor, noMaximo->valor);
    
    // Encontrar sucessor e predecessor
    No *no = buscar(arvore, 15);
    if (no) {
        No *noSucessor = sucessor(arvore, no);
        No *noPredecessor = predecessor(arvore, no);
        printf("Sucessor de 15: %d\n", noSucessor ? noSucessor->valor : -1);
        printf("Predecessor de 15: %d\n", noPredecessor ? noPredecessor->valor : -1);
    }
    
    // Remover um nó
    remover(arvore, 20);
    printf("\nApós remover 20: ");
    emOrdem(arvore->raiz);
    printf("\n");
    
    // Liberar memória
    liberarArvore(arvore->raiz);
    free(arvore);
    
    return 0;
}
