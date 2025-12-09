#include <stdio.h>
#include <stdlib.h>

typedef enum {
    VERMELHO,
    PRETO
} Cor;

typedef struct No {
    int valor;
    Cor cor;
    struct No *esquerda, *direita;
} No;

typedef struct {
    No *raiz;
} ArvoreRubroNegra;

// ==================== MÉTODOS AUXILIARES ====================

int ehVermelho(No *no) {
    return no != NULL && no->cor == VERMELHO;
}

No* criarNo(int valor) {
    No *novo = (No*)malloc(sizeof(No));
    novo->valor = valor;
    novo->cor = VERMELHO; 
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// ==================== MÉTODOS DE ROTAÇÃO ====================

No* rotacaoEsquerda(No *pivo) {
    No *filhoDireita = pivo->direita;
    pivo->direita = filhoDireita->esquerda;
    filhoDireita->esquerda = pivo;
    filhoDireita->cor = pivo->cor;
    pivo->cor = VERMELHO;
    return filhoDireita;
}

No* rotacaoDireita(No *pivo) {
    No *filhoEsquerda = pivo->esquerda;
    pivo->esquerda = filhoEsquerda->direita;
    filhoEsquerda->direita = pivo;
    filhoEsquerda->cor = pivo->cor;
    pivo->cor = VERMELHO;
    return filhoEsquerda;
}

// ==================== BALANCEAMENTO ====================

void inverterCores(No *no) {
    no->cor = VERMELHO;
    if (no->esquerda != NULL) no->esquerda->cor = PRETO;
    if (no->direita != NULL) no->direita->cor = PRETO;
}

No* balancear(No *no) {
    if (ehVermelho(no->esquerda) && ehVermelho(no->direita)) {
        inverterCores(no);
    }
    
    if (ehVermelho(no->direita) && !ehVermelho(no->esquerda)) {
        no = rotacaoEsquerda(no);
    }
    
    if (ehVermelho(no->esquerda) && ehVermelho(no->esquerda->esquerda)) {
        no = rotacaoDireita(no);
    }
    
    if (ehVermelho(no->esquerda) && ehVermelho(no->direita)) {
        inverterCores(no);
    }
    
    return no;
}

// ==================== INSERÇÃO ====================

No* inserirRecursivo(No *no, int valor) {
    if (no == NULL) {
        return criarNo(valor);
    }
    
    if (valor < no->valor) {
        no->esquerda = inserirRecursivo(no->esquerda, valor);
    } else if (valor > no->valor) {
        no->direita = inserirRecursivo(no->direita, valor);
    } else {
        return no;
    }
    
    return balancear(no);
}

void inserir(ArvoreRubroNegra *arvore, int valor) {
    arvore->raiz = inserirRecursivo(arvore->raiz, valor);
    arvore->raiz->cor = PRETO; 
}

// ==================== OPERAÇÕES DE BUSCA ====================

No* buscar(ArvoreRubroNegra *arvore, int valor) {
    No *atual = arvore->raiz;
    while (atual != NULL && atual->valor != valor) {
        atual = (valor < atual->valor) ? atual->esquerda : atual->direita;
    }
    return atual;
}

No* encontrarMinimo(ArvoreRubroNegra *arvore) {
    if (arvore->raiz == NULL) return NULL;
    No *atual = arvore->raiz;
    while (atual->esquerda != NULL) {
        atual = atual->esquerda;
    }
    return atual;
}

No* encontrarMaximo(ArvoreRubroNegra *arvore) {
    if (arvore->raiz == NULL) return NULL;
    No *atual = arvore->raiz;
    while (atual->direita != NULL) {
        atual = atual->direita;
    }
    return atual;
}

// ==================== PERCURSO ====================

void emOrdem(No *no) {
    if (no != NULL) {
        emOrdem(no->esquerda);
        printf("%d(%c) ", no->valor, no->cor == VERMELHO ? 'V' : 'P');
        emOrdem(no->direita);
    }
}

// ==================== LIBERAÇÃO DE MEMÓRIA ====================

void liberarArvore(No *no) {
    if (no != NULL) {
        liberarArvore(no->esquerda);
        liberarArvore(no->direita);
        free(no);
    }
}

// ==================== FUNÇÃO MAIN ====================

int main() {
    ArvoreRubroNegra arvore;
    arvore.raiz = NULL;
    
    int valores[] = {10, 20, 30, 15, 25, 5, 1};
    int n = sizeof(valores) / sizeof(valores[0]);
    
    printf("=== INSERCAO ===\n");
    for (int i = 0; i < n; i++) {
        inserir(&arvore, valores[i]);
        printf("Apos inserir %d: ", valores[i]);
        emOrdem(arvore.raiz);
        printf("\n");
    }
    
    printf("\n=== BUSCA ===\n");
    No *encontrado = buscar(&arvore, 15);
    if (encontrado != NULL) {
        printf("Encontrado: %d\n", encontrado->valor);
    }
    
    printf("\n=== MIN/MAX ===\n");
    No *noMinimo = encontrarMinimo(&arvore);
    No *noMaximo = encontrarMaximo(&arvore);
    printf("Minimo: %d, Maximo: %d\n", noMinimo->valor, noMaximo->valor);
    
    liberarArvore(arvore.raiz);
    
    return 0;
}
