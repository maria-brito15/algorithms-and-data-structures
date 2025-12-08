class NoTrie {
    static final int TAMANHO_ALFABETO = 26;
    
    NoTrie[] filhos;
    boolean fimDePalavra;
    int frequencia;
    
    public NoTrie() {
        filhos = new NoTrie[TAMANHO_ALFABETO];
        fimDePalavra = false;
        frequencia = 0;
    }
}

public class ArvoreTrie {
    private NoTrie raiz;
    
    public ArvoreTrie() {
        raiz = new NoTrie();
    }
    
    // Inserir uma palavra na Trie
    public void inserir(String palavra) {
        if (palavra == null || palavra.isEmpty()) {
            return;
        }
        
        palavra = palavra.toLowerCase();
        NoTrie atual = raiz;
        
        for (char c : palavra.toCharArray()) {
            if (c < 'a' || c > 'z') continue;
            
            int indice = c - 'a';
            if (atual.filhos[indice] == null) {
                atual.filhos[indice] = new NoTrie();
            }
			
            atual = atual.filhos[indice];
        }
        
        atual.fimDePalavra = true;
        atual.frequencia++;
    }
    
    // Buscar uma palavra na Trie
    public boolean buscar(String palavra) {
        NoTrie no = buscarNo(palavra);
        return no != null && no.fimDePalavra;
    }
    
    // Buscar um nó correspondente a uma palavra
    private NoTrie buscarNo(String palavra) {
        if (palavra == null || palavra.isEmpty()) {
            return null;
        }
        
        palavra = palavra.toLowerCase();
        NoTrie atual = raiz;
        
        for (char c : palavra.toCharArray()) {
            if (c < 'a' || c > 'z') {
                continue;
            }
            
            int indice = c - 'a';
            if (atual.filhos[indice] == null) {
                return null;
            }
			
            atual = atual.filhos[indice];
        }
        
        return atual;
    }
    
    // Verificar se existe alguma palavra com o prefixo dado
    public boolean comecaCom(String prefixo) {
        return buscarNo(prefixo) != null;
    }
    
    // Deletar uma palavra da Trie
    public boolean deletar(String palavra) {
        if (palavra == null || palavra.isEmpty()) {
            return false;
        }
        
        palavra = palavra.toLowerCase();
        return deletarHelper(raiz, palavra, 0);
    }
    
    private boolean deletarHelper(NoTrie atual, String palavra, int indice) {
        if (indice == palavra.length()) {
            if (!atual.fimDePalavra) return false; // Palavra não existe
            
            atual.fimDePalavra = false;
            atual.frequencia = 0;
            
            // Retorna true se o nó pode ser deletado (não tem filhos)
            return !temFilhos(atual);
        }
        
        char c = palavra.charAt(indice);
        if (c < 'a' || c > 'z') {
            return false;
        }
        
        int idx = c - 'a';
        NoTrie filho = atual.filhos[idx];
        
        if (filho == null) {
            return false; // Palavra não existe
        }
        
        boolean deveDeletarFilho = deletarHelper(filho, palavra, indice + 1);
        
        if (deveDeletarFilho) {
            atual.filhos[idx] = null;
            // Retorna true se o nó atual pode ser deletado
            return !atual.fimDePalavra && !temFilhos(atual);
        }
        
        return false;
    }
    
    // Verificar se o nó tem filhos
    private boolean temFilhos(NoTrie no) {
        for (NoTrie filho : no.filhos) {
            if (filho != null) {
                return true;
            }
        }
		
        return false;
    }
    
    // Obter todas as palavras com um dado prefixo
    public void autocompletar(String prefixo) {
        NoTrie no = buscarNo(prefixo);
        
        if (no == null) {
            System.out.println("Nenhuma palavra encontrada com o prefixo: " + prefixo);
            return;
        }
        
        System.out.println("Palavras com o prefixo '" + prefixo + "':");
        coletarPalavras(no, prefixo.toLowerCase());
    }
    
    private void coletarPalavras(NoTrie no, String prefixo) {
        if (no.fimDePalavra) {
            System.out.println(prefixo + " (frequência: " + no.frequencia + ")");
        }
        
        for (int i = 0; i < 26; i++) {
            if (no.filhos[i] != null) {
                char c = (char) ('a' + i);
                coletarPalavras(no.filhos[i], prefixo + c);
            }
        }
    }
    
    // Contar o número total de palavras na Trie
    public int contarPalavras() {
        return contarPalavrasHelper(raiz);
    }
    
    private int contarPalavrasHelper(NoTrie no) {
        int count = 0;
        
        if (no.fimDePalavra) {
            count = 1;
        }
        
        for (NoTrie filho : no.filhos) {
            if (filho != null) {
                count += contarPalavrasHelper(filho);
            }
        }
        
        return count;
    }
    
    // Obter a palavra mais longa na Trie
    public String palavraMaisLonga() {
        StringBuilder resultado = new StringBuilder();
        StringBuilder atual = new StringBuilder();
        encontrarPalavraMaisLonga(raiz, atual, resultado);
        return resultado.toString();
    }
    
    private void encontrarPalavraMaisLonga(NoTrie no, StringBuilder atual, StringBuilder resultado) {
        if (no.fimDePalavra && atual.length() > resultado.length()) {
            resultado.setLength(0);
            resultado.append(atual);
        }
        
        for (int i = 0; i < 26; i++) {
            if (no.filhos[i] != null) {
                atual.append((char) ('a' + i));
                encontrarPalavraMaisLonga(no.filhos[i], atual, resultado);
                atual.deleteCharAt(atual.length() - 1);
            }
        }
    }
    
    // Imprimir todas as palavras na Trie
    public void imprimirTodasPalavras() {
        System.out.println("Todas as palavras na Trie:");
        imprimirHelper(raiz, "");
    }
    
    private void imprimirHelper(NoTrie no, String prefixo) {
        if (no.fimDePalavra) {
            System.out.println(prefixo);
        }
        
        for (int i = 0; i < 26; i++) {
            if (no.filhos[i] != null) {
                imprimirHelper(no.filhos[i], prefixo + (char) ('a' + i));
            }
        }
    }

    // Método auxiliar para iniciar a visualização a partir da raiz
	public void mostrar() {
		System.out.println("\n=== VISUALIZAÇÃO DA TRIE ===");
		mostrarHelper(raiz, "");
	}

	// Método de visualização recursivo adaptado (como solicitado)
	private void mostrarHelper(NoTrie no, String prefixo) {
		if (no == null) {
			return;
		}

		if (no.fimDePalavra) {
			System.out.println("Palavra: " + prefixo + " (FIM DE PALAVRA)");
		}

		// Percorre todos os possíveis filhos
		for (int i = 0; i < NoTrie.TAMANHO_ALFABETO; i++) {
			NoTrie proximoNo = no.filhos[i];
			
			if (proximoNo != null) {
				char caractere = (char) ('a' + i);
				String proximoPrefixo = prefixo + caractere;

				// Imprime a transição (adaptado para o estilo da imagem)
				if (prefixo.isEmpty()) {
					System.out.println("[ESTOU NA RAIZ] E VOU PARA (" + proximoPrefixo + ")");
				} else {
					System.out.println("[ESTOU EM (" + prefixo + ")] E VOU PARA (" + proximoPrefixo + ")");
				}

				// Chamada recursiva para o próximo nó
				mostrarHelper(proximoNo, proximoPrefixo);
			}
		}
	}
    
    // Exemplo de uso
    public static void main(String[] args) {
        ArvoreTrie trie = new ArvoreTrie();
        
        System.out.println("=== INSERÇÃO ===");
        String[] palavras = {"casa", "carro", "cachorro", "gato", "cadeira", "cafe", "computador"};
        for (String palavra : palavras) {
            trie.inserir(palavra);
            System.out.println("Inseriu: " + palavra);
        }
        
        System.out.println("\n--- TESTES ---");
        
        System.out.println("\n=== BUSCA ===");
        String[] buscas = {"casa", "carro", "cavalo", "ca"};
        for (String busca : buscas) {
            boolean encontrado = trie.buscar(busca);
            System.out.println("Buscar '" + busca + "': " + (encontrado ? "Encontrado" : "Não encontrado"));
        }
        
        System.out.println("\n=== PREFIXO ===");
        String[] prefixos = {"ca", "com", "gat", "co"};
        for (String prefixo : prefixos) {
            boolean existe = trie.comecaCom(prefixo);
            System.out.println("Começa com '" + prefixo + "': " + (existe ? "Sim" : "Não"));
        }
        
        System.out.println("\n=== AUTOCOMPLETAR ===");
        trie.autocompletar("ca");
        
        System.out.println("\n=== ESTATÍSTICAS ===");
        System.out.println("Total de palavras: " + trie.contarPalavras());
        System.out.println("Palavra mais longa: " + trie.palavraMaisLonga());
        
        System.out.println("\n=== DELETAR ===");
        trie.deletar("cachorro");
        System.out.println("Deletou 'cachorro'");
        System.out.println("Buscar 'cachorro': " + (trie.buscar("cachorro") ? "Encontrado" : "Não encontrado"));
        
        System.out.println("\n=== IMPRIMIR TODAS PALAVRAS ===");
        trie.imprimirTodasPalavras();
        
        // Chamada do novo método de visualização
        trie.mostrar();
    }
}