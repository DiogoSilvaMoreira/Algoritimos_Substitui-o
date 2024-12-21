# Escalonamento por Prioridades - Trabalho de Sistemas Operacionais

## Descrição

Este repositório contém a implementação do **Escalonamento por Prioridades**, uma técnica usada para para gerenciar a execução de processos com base em suas prioridades.

### Funcionalidade

O programa simula o escalonamento de processos utilizando a técnica de **Escalonamento por Prioridades**. O usuário pode definir a entrada para os processos, como o **tempo de execução**, o **quantum** do **Round-Robin** para cada fila de prioridade, e o **tempo de aumento de prioridade**. O sistema aloca os processos com base nas suas prioridades, determinando a ordem de execução.

---

Tecnologias Utilizadas
---
    Linguagem de Programação: C++
    Estruturas de controle: Condicionais, loops.
    Entrada e Saída: Leitura e escrita de dados fornecidos pelo usuário. 
---

## Como Executar

apenas clone este repositório e tenha um executavel em python para ele rodar 
O programa solicitará ao usuário os parâmetros de entrada, como o número de processos, o tempo de execução de cada um, os valores de quantum e o tempo de aumento de prioridade. A partir dessas entradas, o programa simula o escalonamento dos processos.

Entrada e Saída
---
Entrada
--

O usuário deve fornecer os seguintes parâmetros:

    Número de processos: Quantos processos serão simulados.
    Tempo de execução de cada processo.
    Quantum de cada fila de prioridades (para o Round-Robin).
    Tempo para aumento de prioridade.

A entrada pode ser gerada manualmente pelo usuário ou aleatoriamente, conforme definido pelo programa.

Saída
---

A saída do programa inclui a ordem de execução dos processos e os tempos de resposta, espera e execução de cada processo, além da visualização do aumento de prioridade durante a simulação.

