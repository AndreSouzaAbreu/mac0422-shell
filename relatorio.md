---
title: MAC0422 — EP 1
author:
- Andre Souza Abreu
- Théo Borém Fabris
credential:
- 11734927
- 11735293
date: 06/04/2022
---

# Introdução

Este relatório visa resumir o processo de implementação de uma *shell*
simplificada referente ao Exercício Programa 1 da disciplina *MAC0422 — Sistemas
Operacionais*.

O objetivo do trabalho foi gerar um executável `mac422shell` que fosse capaz de
executar os seguintes comandos utilizando diretamente as chamadas de sistema do
*Minix 3.1.0*:

- `protegepracaramba <caminho do arquivo>`: faz `000` ser o código de permissão
  do arquivo especificado;
- `liberageral <caminho do arquivo>`: faz `777` ser o código de permissão do
  arquivo especificado;
- `rodeveja <caminho do programa>`: executa o programa especificado pelo
  caminho fornecido fazendo que este monopolize a entrada e saída da shell.
  Ao terminar, a shell emite uma mensagem com o status de saída do programa
  executado;
- `rode <caminho do programa>`: executa o programa especificado em
  *background*. Assim, a shell monopoliza a entrada padrão do terminal,
  enquanto a saída padrão é utilizada pela shell e pelo programa em background.
  Além disso, o código de saída do programa não é retornado pela shell.

# Esquema geral

O esquema de implementação geral da `shell` pode ser resumido da seguinte forma:
o programa entra em um *loop* infinito onde lê a entrada do usuário e executa o
comando referente a esta entrada. O loop pode ser divido nas seguintes etapas:

1. Tenta ler entrada, encerrando o programa em caso de falha.
2. Processa a entrada, lançando mensagens de erro se esta for inválida.
3. Executa o comando desejado, passando os argumentos para a função em C correspodente.


# Implementação dos comandos

Nas seguintes seções, as implementações dos comandos da shell são explicadas.

## `protegepracaramba` e `liberageral`

Uma vez que a chamada de sistema `chmod`, chamada por meio da função chmod do
header <sys/stat.h>,  recebe o caminho do arquivo e um número octal para
especificar o modo de permissão para o arquivo. No caso do `protegepracaramba`
este número é `000`, que retira qualquer permissão de leitura, escrita e
execução, e no caso do `liberageral` este número é `777`, que permite qualquer
leitura, escrita e execução do arquivo.

## rode

Para implementar este comando, utilizamos as chamadas de sistema `fork` e
`execve` disponibilizadas no header <unistd.h>.

A chamada `fork` é utilizada para criar uma cópia do processo atual. Esta função
retorna 0 para o processo recém criado (processo filho) e retorna o `pid` do
processo filho para o processo original (processo pai).
Por meio deste valor, é possível distinguir os processos, assim, executa-se
códigos específicos para cada um deles.

O processo filho executa o programa especificado pelo usuário através da
chamada `execve`,
que tem como primeiro parâmetro o caminho do executável,
segundo parâmetro o vetor de argumentos para passar ao programa,
e terceiro paramêtro um vetor com varíaveis de ambiente.
Esta chamada `execve` devolve o *exit status* do
programa executado, então nos precisamos encerrar o processo filho atual por meio
da chamda de sistema `exit`, passando o *exit status* para o `exit`.

Como o programa rodará em background, o processo pai (i.e., a shell) continuará
o loop de leitura dos comandos do usuário.

## rodeveja

O comando `rodeveja` executará o programa em *foreground*, então o usuário
esperará o programa especificado terminar de rodar para que possa usar a *shell*.

Por meio da chamada de sistema `waitpid`, disponibilizada no header
<sys/wait.h>, o processo pai aguardará um dos seus processos filhos terminar de
executar, no caso desta shell só há um processo.
A chamada de sistema recebe como primeiro parâmetro o *process id* do processo
filho que o processo pai esperará, segundo parâmetro um endereço de memória
onde será guardado o *exit status* do processo filho, e terceiro parâmetro
opções extras.
Se houver algum erro na execução do processo filho, o valor devolvido por
`waitpid` será $-1$,
caso contrário o processo executou normalmente e utilizando a macro
WEXITSTATUS(status) é possível extrair o *exit status* devolvido.
Finalmente, o processo pai imprimirá o *exit status* do processo filho na saída
padrão e retornará ao loop de interação com o usuário.
