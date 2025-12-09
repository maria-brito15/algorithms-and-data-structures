class No {
    int valor;
    No esquerda, direita;
    
    public No(int valor) {
        this.valor = valor;
        this.esquerda = null;
        this.direita = null;
    }
}

public class AVLTree {
    private No raiz;
    
    public ArvoreAVL() {
        raiz = null;
    }
    
    // Obtém a altura de um nó
    private int obterAltura(No no) {
        if (no == null) return 0;
        int alturaEsquerda = obterAltura(no.esquerda);
        int alturaDireita = obterAltura(no.direita);
        return Math.max(alturaEsquerda, alturaDireita) + 1;
    }
    
    // Obtém o fator de balanceamento de um nó
    private int obterBalanceamento(No no) {
        return no != null ? obterAltura(no.esquerda) - obterAltura(no.direita) : 0;
    }
    
    // Rotação à esquerda no nó pivô
    private No rotacaoEsquerda(No noPivo) {
        No filhoDireita = noPivo.direita;
        No subarvoreEsquerda = filhoDireita.esquerda;
        
        filhoDireita.esquerda = noPivo;
        noPivo.direita = subarvoreEsquerda;
        
        return filhoDireita;
    }
    
    // Rotação à direita no nó pivô
    private No rotacaoDireita(No noPivo) {
        No filhoEsquerda = noPivo.esquerda;
        No subarvoreDireita = filhoEsquerda.direita;
        
        filhoEsquerda.direita = noPivo;
        noPivo.esquerda = subarvoreDireita;
        
        return filhoEsquerda;
    }
    
    // Balanceia o nó após inserção ou remoção
    private No balancear(No no) {
        int fatorBalanceamento = obterBalanceamento(no);
        
        // Caso Esquerda-Esquerda
        if (fatorBalanceamento > 1 && obterBalanceamento(no.esquerda) >= 0)
            return rotacaoDireita(no);
        
        // Caso Esquerda-Direita
        if (fatorBalanceamento > 1 && obterBalanceamento(no.esquerda) < 0) {
            no.esquerda = rotacaoEsquerda(no.esquerda);
            return rotacaoDireita(no);
        }
        
        // Caso Direita-Direita
        if (fatorBalanceamento < -1 && obterBalanceamento(no.direita) <= 0)
            return rotacaoEsquerda(no);
        
        // Caso Direita-Esquerda
        if (fatorBalanceamento < -1 && obterBalanceamento(no.direita) > 0) {
            no.direita = rotacaoDireita(no.direita);
            return rotacaoEsquerda(no);
        }
        
        return no;
    }
    
    // Função auxiliar de inserção
    private No inserirNo(No noAtual, int valor) {
        if (noAtual == null)
            return new No(valor);
        
        if (valor < noAtual.valor)
            noAtual.esquerda = inserirNo(noAtual.esquerda, valor);
        else if (valor > noAtual.valor)
            noAtual.direita = inserirNo(noAtual.direita, valor);
        else
            return noAtual; // Valores duplicados não são permitidos
        
        return balancear(noAtual);
    }
    
    // Insere um nó com o valor fornecido
    public void inserir(int valor) {
        raiz = inserirNo(raiz, valor);
    }
    
    // Encontra o nó com valor mínimo na subárvore
    private No encontrarMinimo(No raizSubarvore) {
        while (raizSubarvore != null && raizSubarvore.esquerda != null)
            raizSubarvore = raizSubarvore.esquerda;
        return raizSubarvore;
    }
    
    // Encontra o nó com valor máximo na subárvore
    private No encontrarMaximo(No raizSubarvore) {
        while (raizSubarvore != null && raizSubarvore.direita != null)
            raizSubarvore = raizSubarvore.direita;
        return raizSubarvore;
    }
    
    // Função auxiliar de remoção
    private No removerNoAuxiliar(No noAtual, int valor) {
        if (noAtual == null)
            return noAtual;
        
        if (valor < noAtual.valor) {
            noAtual.esquerda = removerNoAuxiliar(noAtual.esquerda, valor);
        } else if (valor > noAtual.valor) {
            noAtual.direita = removerNoAuxiliar(noAtual.direita, valor);
        } else {
            if (noAtual.esquerda == null)
                return noAtual.direita;
            else if (noAtual.direita == null)
                return noAtual.esquerda;
            
            No sucessor = encontrarMinimo(noAtual.direita);
            noAtual.valor = sucessor.valor;
            noAtual.direita = removerNoAuxiliar(noAtual.direita, sucessor.valor);
        }
        
        return balancear(noAtual);
    }
    
    // Remove um nó com o valor fornecido
    public void remover(int valor) {
        raiz = removerNoAuxiliar(raiz, valor);
    }
    
    // Busca por um nó com o valor fornecido
    public No buscar(int valor) {
        No noAtual = raiz;
        while (noAtual != null && noAtual.valor != valor) {
            if (valor < noAtual.valor)
                noAtual = noAtual.esquerda;
            else
                noAtual = noAtual.direita;
        }
        return noAtual;
    }
    
    // Encontra o sucessor usando percurso em-ordem
    private No sucessorAuxiliar(No noRaiz, No noAlvo, No sucessorAtual) {
        if (noRaiz == null)
            return sucessorAtual;
        
        if (noAlvo.valor < noRaiz.valor) {
            return sucessorAuxiliar(noRaiz.esquerda, noAlvo, noRaiz);
        } else if (noAlvo.valor > noRaiz.valor) {
            return sucessorAuxiliar(noRaiz.direita, noAlvo, sucessorAtual);
        } else {
            if (noRaiz.direita != null)
                return encontrarMinimo(noRaiz.direita);
            return sucessorAtual;
        }
    }
    
    public No sucessor(No no) {
        return sucessorAuxiliar(raiz, no, null);
    }
    
    // Encontra o predecessor usando percurso em-ordem
    private No predecessorAuxiliar(No noRaiz, No noAlvo, No predecessorAtual) {
        if (noRaiz == null)
            return predecessorAtual;
        
        if (noAlvo.valor < noRaiz.valor) {
            return predecessorAuxiliar(noRaiz.esquerda, noAlvo, predecessorAtual);
        } else if (noAlvo.valor > noRaiz.valor) {
            return predecessorAuxiliar(noRaiz.direita, noAlvo, noRaiz);
        } else {
            if (noRaiz.esquerda != null)
                return encontrarMaximo(noRaiz.esquerda);
            return predecessorAtual;
        }
    }
    
    public No predecessor(No no) {
        return predecessorAuxiliar(raiz, no, null);
    }
    
    // Encontra o valor mínimo na árvore inteira
    public No encontrarMin() {
        if (raiz == null)
            return null;
        return encontrarMinimo(raiz);
    }
    
    // Encontra o valor máximo na árvore inteira
    public No encontrarMax() {
        if (raiz == null)
            return null;
        return encontrarMaximo(raiz);
    }
    
    // Realiza percurso em-ordem e imprime os nós
    public void emOrdem(No no) {
        if (no != null) {
            emOrdem(no.esquerda);
            System.out.print(no.valor + " ");
            emOrdem(no.direita);
        }
    }
    
    public No getRaiz() {
        return raiz;
    }
    
    // Exemplo de uso demonstrando todas as operações
    public static void main(String[] args) {
        ArvoreAVL arvore = new ArvoreAVL();
        
        // Inserir elementos
        int[] valores = {10, 20, 30, 15, 25, 5, 1};
        for (int valor : valores) {
            arvore.inserir(valor);
            System.out.print("Após inserir " + valor + ": ");
            arvore.emOrdem(arvore.getRaiz());
            System.out.println();
        }
        
        // Buscar um nó
        No encontrado = arvore.buscar(15);
        if (encontrado != null)
            System.out.println("\nEncontrado: " + encontrado.valor);
        
        // Encontrar mínimo e máximo
        No noMinimo = arvore.encontrarMin();
        No noMaximo = arvore.encontrarMax();
        System.out.println("Mínimo: " + noMinimo.valor + ", Máximo: " + noMaximo.valor);
        
        // Encontrar sucessor e predecessor
        No no = arvore.buscar(15);
        if (no != null) {
            No sucessor = arvore.sucessor(no);
            No predecessor = arvore.predecessor(no);
            System.out.println("Sucessor de 15: " + (sucessor != null ? sucessor.valor : "null"));
            System.out.println("Predecessor de 15: " + (predecessor != null ? predecessor.valor : "null"));
        }
        
        // Remover um nó
        arvore.remover(20);
        System.out.print("\nApós remover 20: ");
        arvore.emOrdem(arvore.getRaiz());
        System.out.println();
    }
}
