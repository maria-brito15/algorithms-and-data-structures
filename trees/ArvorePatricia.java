class NoPatricia {
    String chave;
    String prefixo;
    NoPatricia esquerda, direita;
    boolean ehFolha;
    
    public NoPatricia(String chave, boolean ehFolha) {
        this.chave = chave;
        this.prefixo = "";
        this.esquerda = null;
        this.direita = null;
        this.ehFolha = ehFolha;
    }
    
    public NoPatricia(String prefixo) {
        this.prefixo = prefixo;
        this.chave = null;
        this.esquerda = null;
        this.direita = null;
        this.ehFolha = false;
    }
}

public class ArvorePatricia {
    private NoPatricia raiz;
    
    public ArvorePatricia() {
        raiz = null;
    }
    
    // Calcular o prefixo comum entre duas strings
    private String prefixoComum(String str1, String str2) {
        int minLen = Math.min(str1.length(), str2.length());
        int i = 0;
        
        while (i < minLen && str1.charAt(i) == str2.charAt(i)) {
            i++;
        }
        
        return str1.substring(0, i);
    }
    
    // Inserir uma chave na árvore PATRICIA
    public void inserir(String chave) {
        if (chave == null || chave.isEmpty()) {
            return;
        }
        
        chave = chave.toLowerCase();
        
        if (raiz == null) {
            raiz = new NoPatricia(chave, true);
            return;
        }
        
        raiz = inserirHelper(raiz, chave);
    }
    
    private NoPatricia inserirHelper(NoPatricia no, String chave) {
        // Se é uma folha, precisamos criar um novo nó interno
        if (no.ehFolha) {
            String prefixo = prefixoComum(no.chave, chave);
            
            // Se a chave já existe
            if (prefixo.equals(chave) && prefixo.equals(no.chave)) {
                return no;
            }
            
            // Se a chave é completamente diferente
            if (prefixo.isEmpty()) {
                NoPatricia novoInterno = new NoPatricia("");
                
                if (chave.compareTo(no.chave) < 0) {
                    novoInterno.esquerda = new NoPatricia(chave, true);
                    novoInterno.direita = no;
                } else {
                    novoInterno.esquerda = no;
                    novoInterno.direita = new NoPatricia(chave, true);
                }
                
                return novoInterno;
            }
            
            // Criar novo nó interno com o prefixo comum
            NoPatricia novoInterno = new NoPatricia(prefixo);
            String sufixoExistente = no.chave.substring(prefixo.length());
            String sufixoNovo = chave.substring(prefixo.length());
            
            // Se a chave nova é o prefixo
            if (sufixoNovo.isEmpty()) {
                novoInterno.esquerda = new NoPatricia(chave, true);
                
                NoPatricia noFilho = new NoPatricia("");
                noFilho.esquerda = no;
                no.chave = sufixoExistente;
                novoInterno.direita = noFilho;
            }
            // Se a chave existente é o prefixo
            else if (sufixoExistente.isEmpty()) {
                novoInterno.esquerda = no;
                
                NoPatricia noFilho = new NoPatricia("");
                noFilho.esquerda = new NoPatricia(sufixoNovo, true);
                novoInterno.direita = noFilho;
            }
            // Ambas têm sufixos
            else {
                no.chave = sufixoExistente;
                
                if (sufixoNovo.compareTo(sufixoExistente) < 0) {
                    novoInterno.esquerda = new NoPatricia(sufixoNovo, true);
                    novoInterno.direita = no;
                } else {
                    novoInterno.esquerda = no;
                    novoInterno.direita = new NoPatricia(sufixoNovo, true);
                }
            }
            
            return novoInterno;
        }
        
        // É um nó interno
        String prefixo = prefixoComum(no.prefixo, chave);
        
        // Se o prefixo do nó atual é um prefixo da chave
        if (prefixo.equals(no.prefixo)) {
            String sufixo = chave.substring(no.prefixo.length());
            
            if (sufixo.isEmpty()) {
                // A chave é exatamente o prefixo
                if (no.esquerda == null) {
                    no.esquerda = new NoPatricia(chave, true);
                }
                return no;
            }
            
            // Decidir qual subárvore seguir
            if (no.esquerda != null && no.direita != null) {
                if (no.esquerda.ehFolha && sufixo.charAt(0) < no.esquerda.chave.charAt(0)) {
                    no.esquerda = inserirHelper(no.esquerda, chave.substring(no.prefixo.length()));
                } else if (no.direita.ehFolha && sufixo.charAt(0) >= no.direita.chave.charAt(0)) {
                    no.direita = inserirHelper(no.direita, chave.substring(no.prefixo.length()));
                } else {
                    no.esquerda = inserirHelper(no.esquerda, chave.substring(no.prefixo.length()));
                }
            } else if (no.esquerda != null) {
                no.esquerda = inserirHelper(no.esquerda, chave.substring(no.prefixo.length()));
            } else {
                no.esquerda = new NoPatricia(sufixo, true);
            }
            
            return no;
        }
        
        // Precisa criar um novo nó interno
        NoPatricia novoInterno = new NoPatricia(prefixo);
        String sufixoNo = no.prefixo.substring(prefixo.length());
        String sufixoChave = chave.substring(prefixo.length());
        
        no.prefixo = sufixoNo;
        
        if (sufixoChave.compareTo(sufixoNo) < 0) {
            novoInterno.esquerda = new NoPatricia(sufixoChave, true);
            novoInterno.direita = no;
        } else {
            novoInterno.esquerda = no;
            novoInterno.direita = new NoPatricia(sufixoChave, true);
        }
        
        return novoInterno;
    }
    
    // Buscar uma chave na árvore
    public boolean buscar(String chave) {
        if (chave == null || chave.isEmpty() || raiz == null) {
            return false;
        }
        
        chave = chave.toLowerCase();
        return buscarHelper(raiz, chave, "");
    }
    
    private boolean buscarHelper(NoPatricia no, String chave, String acumulado) {
        if (no == null) {
            return false;
        }
        
        if (no.ehFolha) {
            return (acumulado + no.chave).equals(chave);
        }
        
        String novoAcumulado = acumulado + no.prefixo;
        
        if (!chave.startsWith(novoAcumulado)) {
            return false;
        }
        
        if (novoAcumulado.equals(chave)) {
            return no.esquerda != null && no.esquerda.ehFolha && 
                   (acumulado + no.prefixo + no.esquerda.chave).equals(chave);
        }
        
        boolean encontrado = buscarHelper(no.esquerda, chave, novoAcumulado);
        if (!encontrado) {
            encontrado = buscarHelper(no.direita, chave, novoAcumulado);
        }
        
        return encontrado;
    }
    
    // Listar todas as chaves com um dado prefixo
    public void buscarPorPrefixo(String prefixo) {
        if (raiz == null) {
            System.out.println("Árvore vazia");
            return;
        }
        
        prefixo = prefixo.toLowerCase();
        System.out.println("Palavras com o prefixo '" + prefixo + "':");
        buscarPorPrefixoHelper(raiz, prefixo, "");
    }
    
    private void buscarPorPrefixoHelper(NoPatricia no, String prefixo, String acumulado) {
        if (no == null) {
            return;
        }
        
        if (no.ehFolha) {
            String palavraCompleta = acumulado + no.chave;
            if (palavraCompleta.startsWith(prefixo)) {
                System.out.println("  " + palavraCompleta);
            }
            return;
        }
        
        String novoAcumulado = acumulado + no.prefixo;
        
        buscarPorPrefixoHelper(no.esquerda, prefixo, novoAcumulado);
        buscarPorPrefixoHelper(no.direita, prefixo, novoAcumulado);
    }
    
    // Imprimir todas as chaves na árvore
    public void imprimirTodas() {
        System.out.println("Todas as palavras na árvore PATRICIA:");
        imprimirTodasHelper(raiz, "");
    }
    
    private void imprimirTodasHelper(NoPatricia no, String acumulado) {
        if (no == null) {
            return;
        }
        
        if (no.ehFolha) {
            System.out.println("  " + acumulado + no.chave);
            return;
        }
        
        imprimirTodasHelper(no.esquerda, acumulado + no.prefixo);
        imprimirTodasHelper(no.direita, acumulado + no.prefixo);
    }
    
    // Imprimir a estrutura da árvore
    public void imprimirEstrutura() {
        System.out.println("Estrutura da árvore PATRICIA:");
        imprimirEstruturaHelper(raiz, "", "");
    }
    
    private void imprimirEstruturaHelper(NoPatricia no, String prefixoLinha, String acumulado) {
        if (no == null) {
            return;
        }
        
        if (no.ehFolha) {
            System.out.println(prefixoLinha + "└─ FOLHA: \"" + acumulado + no.chave + "\"");
        } else {
            System.out.println(prefixoLinha + "└─ INTERNO: prefixo=\"" + no.prefixo + "\"");
            if (no.esquerda != null) {
                imprimirEstruturaHelper(no.esquerda, prefixoLinha + "   ├─", acumulado + no.prefixo);
            }
            if (no.direita != null) {
                imprimirEstruturaHelper(no.direita, prefixoLinha + "   └─", acumulado + no.prefixo);
            }
        }
    }
    
    // Contar o número de chaves na árvore
    public int contarChaves() {
        return contarChavesHelper(raiz);
    }
    
    private int contarChavesHelper(NoPatricia no) {
        if (no == null) {
            return 0;
        }
        
        if (no.ehFolha) {
            return 1;
        }
        
        return contarChavesHelper(no.esquerda) + contarChavesHelper(no.direita);
    }

    // =======================
    // MÉTODO DEMONSTRAR (NOVO)
    // =======================
    public void demonstrar() {
        System.out.println("\n=== DEMONSTRAÇÃO DA ÁRVORE PATRICIA ===");
        demonstrarHelper(raiz, "");
    }

    private void demonstrarHelper(NoPatricia no, String acumulado) {
        if (no == null) {
            return;
        }

        if (no.ehFolha) {
            System.out.println("[FOLHA] Palavra = " + acumulado + no.chave);
            return;
        }

        System.out.println("[INTERNO] Prefixo = \"" + no.prefixo + "\" | Caminho = \"" + acumulado + "\"");

        demonstrarHelper(no.esquerda, acumulado + no.prefixo);
        demonstrarHelper(no.direita, acumulado + no.prefixo);
    }
    
    // Exemplo de uso
    public static void main(String[] args) {
        ArvorePatricia patricia = new ArvorePatricia();
        
        System.out.println("=== INSERÇÃO ===");
        String[] palavras = {"romane", "romanus", "romulus", "rubens", "ruber", "rubicon", "rubicundus"};
        for (String palavra : palavras) {
            patricia.inserir(palavra);
            System.out.println("Inseriu: " + palavra);
        }
        
        System.out.println("\n");
        patricia.imprimirEstrutura();

        patricia.demonstrar();
        
        System.out.println("\n=== BUSCA ===");
        String[] buscas = {"romane", "romanus", "roman", "rubens", "teste"};
        for (String busca : buscas) {
            boolean encontrado = patricia.buscar(busca);
            System.out.println("Buscar '" + busca + "': " + (encontrado ? "Encontrado" : "Não encontrado"));
        }
        
        System.out.println("\n=== BUSCA POR PREFIXO ===");
        patricia.buscarPorPrefixo("rom");
        System.out.println();
        patricia.buscarPorPrefixo("rub");
        
        System.out.println("\n=== ESTATÍSTICAS ===");
        System.out.println("Total de chaves: " + patricia.contarChaves());
        
        System.out.println("\n");
        patricia.imprimirTodas();
    }
}
