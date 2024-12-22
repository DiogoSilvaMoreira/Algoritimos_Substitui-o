# Algoritmos de Substituição de Página - NUR (Não Usada Recentemente)

## Descrição

Este repositório contém a implementação da técnica de **Não Usada Recentemente (NUR)** para gerência de memória, especificamente voltada para o gerenciamento de processos em sistemas operacionais.

### Funcionalidade

O programa simula o gerenciamento de memória utilizando a técnica de **NUR (Não Usada Recentemente)** para atualizar os bits de referência e modificação dos processos em um ciclo de clock. O sistema gerencia uma memória de tamanho variável, onde novos processos são inseridos utilizando a técnica **First-Fit**. A cada 10 unidades de tempo, os bits de referência (R) dos processos são atualizados para 0 e, após 10 unidades de tempo desde a última modificação, os bits de modificação (M) também são atualizados.

---

## Requisitos

- Linguagem de Programação: **C++**
- Dependências: Não há dependências externas. A execução requer um interpretador de C++

---

Tecnologias Utilizadas
---
    Linguagem de Programação: C++
    Estruturas de controle: Condicionais, loops.
    Entrada e Saída: Leitura e escrita de dados fornecidos pelo usuário. 
---

## Como Executar

somente clone este repositorio e tenha um executavel em C++
O programa pedirá ao usuário para definir a memória de tamanho variável e inserir os processos com tamanhos aleatórios ou definidos pelo usuário. O programa irá gerenciar a memória utilizando a técnica de First-Fit e atualizar os bits de referência (R) e modificação (M) a cada ciclo de clock.

---
Entrada e Saída
---
Entrada
--

O programa aceita os seguintes parâmetros de entrada:

    Tamanho da memória: Definido pelo usuário em tempo de execução.
    Novos processos: Cada processo terá um tamanho e será inserido na memória utilizando a técnica First-Fit.
    O ciclo de clock ocorrerá automaticamente a cada 10 unidades de tempo.

Saída
---

A saída será o estado da memória com a atualização dos bits R e M a cada ciclo de tempo, conforme o processo é referenciado e modificado. O estado da memória será impresso a cada novo ciclo.
