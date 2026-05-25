// ============================================================================
// rep-sequencias.ts
// Métodos de compressão que utilizam repetição de sequências (dicionários):
//   - LZ77 (janela deslizante)
//   - LZ78 (dicionário de prefixos)
//   - LZW (dicionário inicializado com alfabeto)
// ============================================================================

export class RepSequencias {
  // ------------------------------------------------------------
  // LZ77
  // ------------------------------------------------------------
  // Codifica usando janela deslizante.
  // Parâmetros: tamanho do buffer de busca (busca) e look‑ahead.
  // Tokens no formato: (offset,length,proximoChar)
  // Resultado: string com tokens separados por ';'
  codificarLZ77(
    mensagem: string,
    tamBusca: number = 12,
    tamLookAhead: number = 6,
  ): string {
    if (mensagem.length === 0) return "";

    let pos = 0;
    const tokens: string[] = [];

    while (pos < mensagem.length) {
      const inicioBusca = Math.max(0, pos - tamBusca);
      const bufferBusca = mensagem.substring(inicioBusca, pos);
      const lookAhead = mensagem.substring(
        pos,
        Math.min(pos + tamLookAhead, mensagem.length),
      );

      let melhorOffset = 0;
      let melhorLen = 0;

      // Procura a maior correspondência
      for (let offset = 1; offset <= bufferBusca.length; offset++) {
        let tamanho = 0;

        while (
          tamanho < lookAhead.length &&
          pos - offset + tamanho < mensagem.length &&
          mensagem[pos - offset + tamanho] === mensagem[pos + tamanho]
        ) {
          tamanho++;
        }

        if (tamanho > melhorLen) {
          melhorLen = tamanho;
          melhorOffset = offset;
        }
      }

      if (melhorLen > 0) {
        const proxChar =
          pos + melhorLen < mensagem.length ? mensagem[pos + melhorLen] : "";

        tokens.push(`(${melhorOffset},${melhorLen},${proxChar})`);
        pos += melhorLen + (proxChar ? 1 : 0);
      } else {
        tokens.push(`(0,0,${mensagem[pos]})`);
        pos++;
      }
    }

    return tokens.join(";");
  }

  // Decodifica uma string LZ77
  decodificarLZ77(codificado: string): string {
    if (codificado.length === 0) return "";

    const regex = /\((\d+),(\d+),(.*?)\)/g;
    const tokens: Array<[number, number, string]> = [];

    let match: RegExpExecArray | null;

    while ((match = regex.exec(codificado)) !== null) {
      tokens.push([parseInt(match[1]), parseInt(match[2]), match[3]]);
    }

    let resultado = "";

    for (const [offset, length, char] of tokens) {
      if (offset === 0) {
        resultado += char;
      } else {
        const inicio = resultado.length - offset;

        for (let i = 0; i < length; i++) {
          resultado += resultado[inicio + i];
        }

        if (char) resultado += char;
      }
    }

    return resultado;
  }

  // ------------------------------------------------------------
  // LZ78
  // ------------------------------------------------------------
  // Token: (índice, caractere). Dicionário começa vazio.
  codificarLZ78(mensagem: string): string {
    if (mensagem.length === 0) return "";

    const dicionario = new Map<string, number>(); // string -> índice
    let proxIndice = 1; // 0 é reservado para string vazia
    const tokens: string[] = [];

    let prefixo = "";

    for (let i = 0; i < mensagem.length; i++) {
      const c = mensagem[i];
      const candidato = prefixo + c;

      if (dicionario.has(candidato)) {
        prefixo = candidato;
      } else {
        const indicePrefixo = prefixo === "" ? 0 : dicionario.get(prefixo)!;

        tokens.push(`(${indicePrefixo},${c})`);
        dicionario.set(candidato, proxIndice++);

        prefixo = "";
      }
    }

    // Se sobrou algo no prefixo (caso termine com string que está no dicionário)
    if (prefixo !== "") {
      const indicePrefixo = dicionario.get(prefixo)!;
      tokens.push(`(${indicePrefixo},)`);
    }

    return tokens.join(";");
  }

  decodificarLZ78(codificado: string): string {
    if (codificado.length === 0) return "";

    const regex = /\((\d+),(.*?)\)/g;
    const tokens: Array<[number, string]> = [];

    let match: RegExpExecArray | null;

    while ((match = regex.exec(codificado)) !== null) {
      tokens.push([parseInt(match[1]), match[2]]);
    }

    const dicionario = new Map<number, string>(); // índice -> string
    let resultado = "";
    let proxIndice = 1;

    for (const [indice, caractere] of tokens) {
      let entrada: string;

      if (indice === 0) {
        entrada = caractere;
      } else {
        const prefixo = dicionario.get(indice)!;
        entrada = prefixo + caractere;
      }

      resultado += entrada;

      if (caractere !== "") {
        dicionario.set(proxIndice++, entrada);
      }
    }

    return resultado;
  }

  // ------------------------------------------------------------
  // LZW
  // ------------------------------------------------------------
  // Dicionário inicial: todos os caracteres individuais que aparecem na mensagem.
  // O cabeçalho envia esses caracteres na ordem em que foram adicionados.
  // Formato: "ABC|0 1 2 4" onde "ABC" são os caracteres iniciais.
  codificarLZW(mensagem: string): string {
    if (mensagem.length === 0) return "";

    // 1. Descobre os caracteres únicos
    const caracteresUnicos = new Set<string>();
    for (const ch of mensagem) caracteresUnicos.add(ch);
    const listaChars = Array.from(caracteresUnicos).sort();

    // 2. Inicializa dicionário
    const dicionario = new Map<string, number>();
    let proxIndice = 0;

    for (const ch of listaChars) {
      dicionario.set(ch, proxIndice++);
    }

    const indices: number[] = [];
    let prefixo = "";

    for (let i = 0; i < mensagem.length; i++) {
      const c = mensagem[i];
      const candidato = prefixo + c;

      if (dicionario.has(candidato)) {
        prefixo = candidato;
      } else {
        indices.push(dicionario.get(prefixo)!);
        dicionario.set(candidato, proxIndice++);
        prefixo = c;
      }
    }

    if (prefixo !== "") {
      indices.push(dicionario.get(prefixo)!);
    }

    const cabecalho = listaChars.join("");
    return `${cabecalho}|${indices.join(" ")}`;
  }

  decodificarLZW(codificado: string): string {
    const sep = codificado.indexOf("|");
    if (sep === -1) return "";

    const cabecalho = codificado.substring(0, sep);
    const numeros = codificado
      .substring(sep + 1)
      .split(" ")
      .map(Number);

    if (numeros.length === 0) return "";

    // 1. Reconstrói dicionário inicial a partir do cabeçalho
    const dicionario = new Map<number, string>();
    let proxIndice = 0;

    for (const ch of cabecalho) {
      dicionario.set(proxIndice++, ch);
    }

    let resultado = "";
    let anterior = "";

    for (let i = 0; i < numeros.length; i++) {
      const idx = numeros[i];
      let atual: string;

      if (dicionario.has(idx)) {
        atual = dicionario.get(idx)!;
      } else if (idx === proxIndice) {
        // Caso especial: a string é anterior + primeiro caractere de anterior
        atual = anterior + anterior[0];
      } else {
        throw new Error(`Índice inválido na decodificação LZW: ${idx}`);
      }

      resultado += atual;

      // Adiciona nova entrada ao dicionário (exceto no primeiro símbolo)
      if (i > 0) {
        const novaEntrada = anterior + atual[0];
        dicionario.set(proxIndice++, novaEntrada);
      }

      anterior = atual;
    }

    return resultado;
  }
}

// ===================== TESTE SIMPLES =====================

const rs = new RepSequencias();
const exemplo = "ABABABCABABABC";

console.log("\n--- rep-sequencias.ts ---");
console.log("Original :", exemplo);
console.log("LZ77     :", rs.codificarLZ77(exemplo));
console.log("LZ78     :", rs.codificarLZ78(exemplo));
console.log("LZW      :", rs.codificarLZW(exemplo));

console.log("\n--- Verificações ---");
console.log("LZ77 dec :", rs.decodificarLZ77(rs.codificarLZ77(exemplo)));
console.log("LZ78 dec :", rs.decodificarLZ78(rs.codificarLZ78(exemplo)));
console.log("LZW dec  :", rs.decodificarLZW(rs.codificarLZW(exemplo)));
