---
title: MAC0422 — EP 1
author:
- Andre Souza Abreu
- Theo (??)
credential:
- 11734927
- XXXXXXXX
date: 06/04/2022
---

# INTRODUÇÃO

O presente relatório visa resumir o processo de implementação de uma *shell*
simplificada referente ao Exercício Programa 1 da disciplina *MAC0422 — Sistemas
Operacionais*.

Especifidamente, o objetivo foi gerar um executável `mac422shell` que fosse capaz
de executar os seguintes comandos

- `protegepracaramba <caminho do arquivo>`: faz `000` ser o código de permissão
do arquivo especificado.
- `liberageral <caminho do arquivo>`: faz `777` ser o código de permissão
do arquivo especificado.
- `rodeveja <caminho do programa>`: executa o programa especificado em *foreground*
e emiti uma mensagem sobre o status de saída do programa.
- `rode <caminho do programa>`: executa o programa especificado em *background*.

# ESQUEMA GERAL

O esquema de implementação geral da `shell` pode ser resumido da seguinte forma:
o programa entra em um *loop* infinito onde lê a entrada do usuário e processa essa
entrada, e executa o comando referente à entrada. O loop pode ser divido nas seguintes
etapas:

1. Tenta ler entrada, encerrando o programa em caso de falha.
2. Processa a entrada, lançando mensagens de erro se esta for inválida.
3. Executa o comando dsejado, passando os argumentos para a função em C correspodente.

Eis o trecho programa correspondente:

```c
int main(int argc, char** argv)
{
    char cmd[30], arg[256];

    while (read_cmd(cmd, arg) == 2) {
        /* validate command */
        if (NOT cmd_exists(cmd)) {
            printf("ERROR: invalid command %s\n", cmd);
            continue;
        }
        /* execute command */
        if (call_cmd(cmd, arg) != 0) {
            printf("ERROR: could not run command\n");
            continue;
        }
    }
    printf("\n");

    return 0;
}
```

# IMPLEMENTAÇÃO DOS COMANDOS

Vamos agora fazer uma descrição da implementação dos comandos listados anteriormente.

## protegepracaramba

Neste comando, basicamente utilizamos a *syscall* `chmod`, a qual recebe o caminho
do arquivo e o modo de permissão para o arquivo (que no caso é `000`). Para obter o modo de permissão
compatível com o paramêtro da função, utilizamos a utilidade `strtol` que converte
uma string para o formato especificado pela base provida, que no caso é hexadecimal.

```c
/* change file mode to 000 (very restrictive) */
int protege_pra_caramba(char* filepath)
{
    char mode[] = "0000";
    unsigned int mask = strtol(mode, 0, 8);
    return chmod(filepath, mask);
}
```

## liberageral

A implementação deste comando segue a mesma lógica que no anterior, exceto que
o modo de permissão agora é `777`.

```c
/* change file mode to 000 (very restrictive) */
int libera_geral(char* filepath)
{
    char mode[] = "0000";
    unsigned int mask = strtol(mode, 0, 8);
    return chmod(filepath, mask);
}
```

## rode

Para implementar este comando, utilizamos as *system calls* `fork`,
`execve` e `exit`.

A chamada `fork` é utilizada para criar uma cópia do processo atual. Esta função
retorna 0 para o processo recém criado (processo filho) e retorna o pid do
processo filho para o processo original (processo pai). Por meio deste valor,
podemos distinguir qual processo é qual e executar códigos específicos em cada
um dos casos.

O processo filho executa o programa especificado pelo usuário atráves da
chamada `execve`, a qual tem como paramêtros (1) o caminho do executável, (2)
uma vetor de argumentos para passar ao programa, (3) um vetor com varíaveis de
ambiente para executar o programa. Esta chamada `execve` devolve o *exit status* do
programa executado, então nos precisamos encerrar o processo filho atual por meio
da chamda de sistema `exit`, passando o *exit status* para o `exit`.

O processo pai não faz nada em relação ao filho, visto que neste caso
é desejado que o programa rode no background e que o usuário possa continuar
a usar a *shell*.

```c
/* runs a program on the background */
int rode(char* filepath)
{
    char** argv, ** envv;
    int pid, exit_status;

    pid = fork(); /* fork process */

    if (pid != 0) { /* this is the parent process */
        return pid; /* return the pid to be used elsewhere */
    }
    
    /* this is the child process */

    /* intialize argv vector */
    argv = malloc(sizeof(char)*2);
    argv[0] = filepath;
    argv[1] = NULL;

    /* initialize envv vector */
    envv = malloc(sizeof(char));
    envv[0] = NULL;

    /* execute program and get its status */
    exit_status = execve(filepath, argv, envv);

    /* free allocated memory */
    free(argv);
    free(envv);

    /* exit */
    exit(exit_status);
}
```

## rodeveja

O comando `rodeveja` é quase igual ao `rode`, porém neste caso o programa é
executado em *foreground* e o usuário tem que esperar o programa terminar de
rodar para que possa usar a *shell*. A diferença na implementação está em como
o processo pai lida com o `fork`.

O processo pai aguarda o processo filho terminar de executar, o que é atingido
usando-se a *syscall* `waitpid`, que tem como paramêtros (1) o *process id* do
processo filho, (2) um endereço de memória (ponteiro) onde será guardado o
valor do *exit status* do processo esperado, (3) opções extras. Se houver algum
erro na execução do processo filho, o *exit status* será $-1$, caso contrário o
valor final será o valor real do *exit status* com um *shift* para esquerda de
1 byte (ou seja, multiplicado por 256). É necessário reverter este *shift*.
Após isso, o processo pai deve imprimir o *exit status* na saída padrão.


```c
/* runs a program on foreground and wait for it to complete */
int rode_veja(char* filepath)
{
    int pid, exit_status;

    /* fork the process, run the program, and get the child's pid */
    pid = rode(filepath);

    /* this is the parent process */
    /* wait for child process to terminate, and try to get its exit status */
    waitpid(pid, &exit_status, 0);

    if (exit_status == -1) {
        printf("ERROR: erro ao rodar o programa %s\n", filepath);
    } else {
        /* the status is multiplied by 256, thus needs to convert it */
        exit_status /= 256;
        printf("=> programa '%s' retornou codigo %d\n", filepath, exit_status);
    }

    return 0;
}
```

# CONCLUSÃO E COMENTÁRIOS
