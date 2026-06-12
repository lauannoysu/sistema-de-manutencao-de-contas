# Sistema de manutenção de contas

Atividade feita em C usando arquivo binário.

O programa permite cadastrar, consultar, alterar saldo, remover e listar clientes.  
Os dados são salvos no arquivo `clientes.dat`.

## Funções que foram utilizadas

Foram usados:

- `fopen`
- `fread`
- `fwrite`
- `fseek`
- `rewind`

Cada cliente é salvo como um registro fixo usando a estrutura de `struct`.

## Como executar

Compilar:

```bash
gcc main.c -o sistema
