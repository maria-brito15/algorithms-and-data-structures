// ============================================================================
// freq-simbolos.ts
// Métodos de compressão que utilizam a frequência dos símbolos:
//   - RLE (Run‑Length Encoding)
//   - Shannon‑Fano
//   - Huffman
// ============================================================================

// ------------------------------------------------------------
// Classes auxiliares
// ------------------------------------------------------------

// Representa um símbolo e sua frequência (usado em Shannon-Fano)
class SimboloFrequencia {
  constructor(
    public simbolo: string,
    public frequencia: number,
  ) {}
}

// Nó da árvore de Huffman
class NoHuffman {
  public esquerdo: NoHuffman | null = null;
  public direito: NoHuffman | null = null;

  constructor(
    public simbolo: string, // para folhas: o caractere; para internos: string vazia
    public frequencia: number,
    esquerdo?: NoHuffman,
    direito?: NoHuffman,
  ) {
    if (esquerdo !== undefined && direito !== undefined) {
      this.esquerdo = esquerdo;
      this.direito = direito;
    }
  }

  public ehFolha(): boolean {
    return this.esquerdo === null && this.direito === null;
  }
}

// ------------------------------------------------------------
// Classe principal: métodos baseados em frequência
// ------------------------------------------------------------

export class FreqSimbolos {
  // ===================== RLE =====================
  // Codifica substituindo repetições por "quantidade + símbolo"
  // Exemplo: "AAAABBB" -> "4A3B"
  codificarRLE(mensagem: string): string {
    if (mensagem.length === 0) return "";

    let resultado = "";
    let i = 0;

    while (i < mensagem.length) {
      const simboloAtual = mensagem[i];
      let quantidade = 1;

      let j = i + 1;
      while (j < mensagem.length && mensagem[j] === simboloAtual) {
        quantidade++;
        j++;
      }

      if (quantidade > 1) {
        resultado += quantidade + simboloAtual;
      } else {
        resultado += simboloAtual;
      }

      i = j;
    }

    return resultado;
  }

  // Decodifica uma string codificada com RLE
  decodificarRLE(codificado: string): string {
    if (codificado.length === 0) return "";

    let resultado = "";
    let i = 0;

    while (i < codificado.length) {
      const ch = codificado[i];

      if (this._ehDigito(ch)) {
        let inicioNumero = i;
        while (i < codificado.length && this._ehDigito(codificado[i])) {
          i++;
        }
        const quantidade = parseInt(codificado.substring(inicioNumero, i));
        const simbolo = codificado[i];
        resultado += simbolo.repeat(quantidade);
        i++;
      } else {
        resultado += ch;
        i++;
      }
    }

    return resultado;
  }

  private _ehDigito(c: string): boolean {
    return c >= "0" && c <= "9";
  }

  // ===================== SHANNON‑FANO =====================
  codificarShannonFano(mensagem: string): string {
    if (mensagem.length === 0) return "";

    // 1. frequências
    const freq = new Map<string, number>();
    for (const ch of mensagem) {
      freq.set(ch, (freq.get(ch) || 0) + 1);
    }

    // 2. lista ordenada decrescente
    const lista: SimboloFrequencia[] = [];
    for (const [simbolo, f] of freq.entries()) {
      lista.push(new SimboloFrequencia(simbolo, f));
    }
    lista.sort((a, b) => b.frequencia - a.frequencia);

    // 3. gerar códigos recursivamente
    const dicionario = new Map<string, string>();
    this._gerarCodigosShannonFano(lista, "", dicionario);

    // 4. codificar mensagem
    let bits = "";
    for (const ch of mensagem) {
      bits += dicionario.get(ch);
    }

    // 5. cabeçalho: "A:0;B:10;C:110;..."
    let cabecalho = "";
    for (const [simbolo, codigo] of dicionario.entries()) {
      cabecalho += `${simbolo}:${codigo};`;
    }
    if (cabecalho.length > 0) cabecalho = cabecalho.slice(0, -1);

    return `${cabecalho}|${bits}`;
  }

  private _gerarCodigosShannonFano(
    simbolos: SimboloFrequencia[],
    prefixo: string,
    dicionario: Map<string, string>,
  ): void {
    if (simbolos.length === 1) {
      dicionario.set(simbolos[0].simbolo, prefixo);
      return;
    }

    let somaTotal = 0;
    for (const s of simbolos) somaTotal += s.frequencia;
    const metade = somaTotal / 2;

    let somaParcial = 0;
    let pontoDivisao = 0;
    let menorDif = Infinity;

    for (let i = 0; i < simbolos.length; i++) {
      somaParcial += simbolos[i].frequencia;
      
      const dif = Math.abs(somaParcial - metade);

      if (dif < menorDif) {
        menorDif = dif;
        pontoDivisao = i + 1;
      }
    }

    const esquerda = simbolos.slice(0, pontoDivisao);
    const direita = simbolos.slice(pontoDivisao);

    this._gerarCodigosShannonFano(esquerda, prefixo + "0", dicionario);
    this._gerarCodigosShannonFano(direita, prefixo + "1", dicionario);
  }

  decodificarShannonFano(codificado: string): string {
    const sep = codificado.indexOf("|");
    if (sep === -1) return "";

    const cabecalho = codificado.substring(0, sep);
    const bits = codificado.substring(sep + 1);

    const mapaReverso = new Map<string, string>();
    const pares = cabecalho.split(";");

    for (const par of pares) {
      const [simbolo, codigo] = par.split(":");
      if (simbolo && codigo) mapaReverso.set(codigo, simbolo);
    }

    let resultado = "";
    let acumulado = "";

    for (let i = 0; i < bits.length; i++) {
      acumulado += bits[i];

      if (mapaReverso.has(acumulado)) {
        resultado += mapaReverso.get(acumulado);
        acumulado = "";
      }
    }

    return resultado;
  }

  // ===================== HUFFMAN =====================
  codificarHuffman(mensagem: string): string {
    if (mensagem.length === 0) return "";

    // 1. frequências
    const freq = new Map<string, number>();

    for (const ch of mensagem) {
      freq.set(ch, (freq.get(ch) || 0) + 1);
    }

    // 2. criar fila de nós folha
    let fila: NoHuffman[] = [];

    for (const [simbolo, f] of freq.entries()) {
      fila.push(new NoHuffman(simbolo, f));
    }

    fila.sort((a, b) => a.frequencia - b.frequencia);

    // 3. construir árvore (bottom‑up)
    while (fila.length > 1) {
      const menor = fila.shift()!;
      const segundo = fila.shift()!;
      const pai = new NoHuffman(
        "",
        menor.frequencia + segundo.frequencia,
        menor,
        segundo,
      );

      fila.push(pai);
      fila.sort((a, b) => a.frequencia - b.frequencia);
    }

    const raiz = fila[0];
    const dicionario = new Map<string, string>();

    this._percorrerHuffman(raiz, "", dicionario);

    // 4. codificar
    let bits = "";

    for (const ch of mensagem) {
      bits += dicionario.get(ch);
    }

    // 5. cabeçalho
    let cabecalho = "";

    for (const [simbolo, codigo] of dicionario.entries()) {
      cabecalho += `${simbolo}:${codigo};`;
    }
    if (cabecalho.length > 0) cabecalho = cabecalho.slice(0, -1);

    return `${cabecalho}|${bits}`;
  }

  private _percorrerHuffman(
    no: NoHuffman | null,
    codigoAtual: string,
    dicionario: Map<string, string>,
  ): void {
    if (no === null) return;

    if (no.ehFolha()) {
      dicionario.set(no.simbolo, codigoAtual);
      return;
    }

    this._percorrerHuffman(no.esquerdo, codigoAtual + "0", dicionario);
    this._percorrerHuffman(no.direito, codigoAtual + "1", dicionario);
  }

  decodificarHuffman(codificado: string): string {
    const sep = codificado.indexOf("|");
    if (sep === -1) return "";

    const cabecalho = codificado.substring(0, sep);
    const bits = codificado.substring(sep + 1);

    const mapaReverso = new Map<string, string>();
    const pares = cabecalho.split(";");

    for (const par of pares) {
      const [simbolo, codigo] = par.split(":");
      if (simbolo && codigo) mapaReverso.set(codigo, simbolo);
    }

    let resultado = "";
    let acumulado = "";

    for (let i = 0; i < bits.length; i++) {
      acumulado += bits[i];

      if (mapaReverso.has(acumulado)) {
        resultado += mapaReverso.get(acumulado);
        acumulado = "";
      }
    }

    return resultado;
  }
}

// ===================== TESTE SIMPLES =====================

const fs = new FreqSimbolos();
const exemplo = "AAAAABBBCCDDDDD";

console.log("--- freq-simbolos.ts ---");

console.log("Original:", exemplo);
console.log("RLE        :", fs.codificarRLE(exemplo));
console.log("Shannon-Fano:", fs.codificarShannonFano(exemplo));
console.log("Huffman    :", fs.codificarHuffman(exemplo));
