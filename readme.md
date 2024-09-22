# Modelo de Segregação Social de Schelling

Este repositório contém uma implementação em C++ do modelo de segregação social de Schelling. Este modelo simula o comportamento de segregação residencial baseado em tolerância entre dois grupos de indivíduos em uma matriz bidimensional.

## Sumário

- [Descrição](#descrição)
- [Funcionalidades](#funcionalidades)
- [Como Usar](#como-usar)
- [Exemplos](#exemplos)
- [Dependências](#dependências)

## Descrição

O **modelo de segregação de Schelling** é uma simulação baseada em agentes que demonstra como interações locais entre vizinhos podem resultar em padrões de segregação mesmo quando os agentes individuais possuem apenas uma leve preferência por vizinhos semelhantes.

Neste código, a matriz representa uma vizinhança, onde cada célula pode estar ocupada por um indivíduo de um dos dois grupos ou pode estar vazia. A simulação itera diversas rodadas, ajustando as posições dos indivíduos com base em sua tolerância à diversidade entre os vizinhos.

![schelling](https://github.com/user-attachments/assets/f8760b68-6430-43b6-a620-2ad1f51de39e)

## Funcionalidades

- Simulação da segregação social com base em uma matriz bidimensional.
- Configuração de parâmetros como o tamanho da matriz, nível de tolerância, percentual de casas vazias, etc.
- Exibição visual da matriz atualizada a cada iteração.
- Seed personalizável para reproduzir resultados aleatórios.
- Controle de atraso entre a atualização das matrizes para melhor visualização.

## Como Usar

Para compilar o código, você pode usar um compilador C++ (como `g++`). Abaixo está o comando básico de compilação e execução:

```bash
make
./bin/SchellingModel [OPÇÕES]
```

### Opções de Linha de Comando

- `-s, --size`: Define o tamanho da matriz. Padrão: `20`.
- `-t, --tolerance`: Nível de tolerância dos agentes (em %). Padrão: `50%`.
- `-e, --empty`: Percentual de casas vazias (em %). Padrão: `10%`.
- `-d, --delay`: Tempo de atraso entre a exibição de uma nova matriz (em milissegundos). Padrão: `0` (sem atraso).
- `--seed`: Define a seed para randomização. Padrão: `time`.
- `-h, --help`: Exibe a mensagem de ajuda.

## Exemplos

### Executando com as configurações padrão:

```bash
./bin/SchellingModel
```

### Definindo uma matriz de 30x30 com 20% de tolerância e 15% de casas vazias:

```bash
./bin/SchellingModel --size 30 --tolerance 20 --empty 15
```

### Adicionando um atraso de 500ms entre as atualizações da matriz:

```bash
./bin/SchellingModel --delay 500
```

## Dependências

Este projeto não possui dependências externas além de um compilador C++ compatível.
