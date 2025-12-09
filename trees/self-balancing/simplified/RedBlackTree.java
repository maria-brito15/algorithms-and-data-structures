enum Cor {
    VERMELHO, PRETO
}

class No {
    int valor;
    Cor cor;
    No esquerda, direita;
    
    public No(int valor) {
        this.valor = valor;
        this.cor = Cor.VERMELHO; // Todo novo nó é vermelho
    }
}

public class RedBlackTree {
    private No raiz;
    
    // ==================== MÉTODOS AUXILIARES ====================
    
    private boolean ehVermelho(No no) {
        return no != null && no.cor == Cor.VERMELHO;
    }
    
    // ==================== MÉTODOS DE ROTAÇÃO ====================
    
    private No rotacaoEsquerda(No pivo) {
        No filhoDireita = pivo.direita;
        pivo.direita = filhoDireita.esquerda;
        filhoDireita.esquerda = pivo;
        filhoDireita.cor = pivo.cor;
        pivo.cor = Cor.VERMELHO;
        return filhoDireita;
    }
    
    private No rotacaoDireita(No pivo) {
        No filhoEsquerda = pivo.esquerda;
        pivo.esquerda = filhoEsquerda.direita;
        filhoEsquerda.direita = pivo;
        filhoEsquerda.cor = pivo.cor;
        pivo.cor = Cor.VERMELHO;
        return filhoEsquerda;
    }
    
    // ==================== INSERÇÃO ====================
    
    public void inserir(int valor) {
        raiz = inserirRecursivo(raiz, valor);
        raiz.cor = Cor.PRETO;
    }
    
    private No inserirRecursivo(No no, int valor) {
        if (no == null) {
            return new No(valor);
        }
        
        if (valor < no.valor) {
            no.esquerda = inserirRecursivo(no.esquerda, valor);
        } else if (valor > no.valor) {
            no.direita = inserirRecursivo(no.direita, valor);
        } else {
            return no;
        }
        
        return balancear(no);
    }
    
    private No balancear(No no) {
        // Caso esquerda-direita: requer rotação dupla
        if (ehVermelho(no.esquerda) && ehVermelho(no.esquerda.direita)) {
            no.esquerda = rotacaoEsquerda(no.esquerda);
        }
        
        // Caso direita-esquerda: requer rotação dupla
        if (ehVermelho(no.direita) && ehVermelho(no.direita.esquerda)) {
            no.direita = rotacaoDireita(no.direita);
        }
        
        // Caso esquerda-esquerda
        if (ehVermelho(no.esquerda) && ehVermelho(no.esquerda.esquerda)) {
            no = rotacaoDireita(no);
        }
        
        // Caso direita-direita
        if (ehVermelho(no.direita) && ehVermelho(no.direita.direita)) {
            no = rotacaoEsquerda(no);
        }
        
        // Split de nó 4 (dois filhos vermelhos)
        if (ehVermelho(no.esquerda) && ehVermelho(no.direita)) {
            inverterCores(no);
        }
        
        return no;
    }
    
    private void inverterCores(No no) {
        no.cor = Cor.VERMELHO;
        if (no.esquerda != null) no.esquerda.cor = Cor.PRETO;
        if (no.direita != null) no.direita.cor = Cor.PRETO;
    }
    
    // ==================== OPERAÇÕES DE BUSCA ====================
    
    public No buscar(int valor) {
        No atual = raiz;
        while (atual != null && atual.valor != valor) {
            atual = (valor < atual.valor) ? atual.esquerda : atual.direita;
        }
        return atual;
    }
    
    public No encontrarMinimo() {
        if (raiz == null) return null;
        No atual = raiz;
        while (atual.esquerda != null) {
            atual = atual.esquerda;
        }
        return atual;
    }
    
    public No encontrarMaximo() {
        if (raiz == null) return null;
        No atual = raiz;
        while (atual.direita != null) {
            atual = atual.direita;
        }
        return atual;
    }
    
    // ==================== PERCURSO ====================
    
    public void emOrdem(No no) {
        if (no != null) {
            emOrdem(no.esquerda);
            System.out.print(no.valor + "(" + 
                (no.cor == Cor.VERMELHO ? "V" : "P") + ") ");
            emOrdem(no.direita);
        }
    }
    
    public No getRaiz() {
        return raiz;
    }
    
    // ==================== MÉTODO MAIN ====================
    
    public static void main(String[] args) {
        RedBlackTree arvore = new RedBlackTree();
        
        int[] valores = {10, 20, 30, 15, 25, 5, 1};
        System.out.println("=== INSERÇÃO ===");
        for (int valor : valores) {
            arvore.inserir(valor);
            System.out.print("Após inserir " + valor + ": ");
            arvore.emOrdem(arvore.getRaiz());
            System.out.println();
        }
        
        System.out.println("\n=== BUSCA ===");
        No encontrado = arvore.buscar(15);
        if (encontrado != null) {
            System.out.println("Encontrado: " + encontrado.valor);
        }
        
        System.out.println("\n=== MIN/MAX ===");
        No noMinimo = arvore.encontrarMinimo();
        No noMaximo = arvore.encontrarMaximo();
        System.out.println("Mínimo: " + noMinimo.valor + ", Máximo: " + noMaximo.valor);
    }
}
