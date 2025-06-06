# Partições Fixas

- Fragmentação Interna - Espaço de sobra em uma partição ocupada

- Fragmentação Externa - A soma dos espaços das partições disponíveis
  é adequada ao tamanho do processo, porém não são contínuas

- Fila Única - A primeira partição em que o processo couber, ele será
  colocado

- Fila Múltipla - Cada partição possui uma fila própria

- A quantidade de informações no **BCP** diminui. Agora a CPU já sabe
  quais partições existem no sistema

# Partições Variáveis

- As partições são alocadas dinamicamente

- Ainda pode haver fragmentação interna, porém o desperdício é baixo
  demais para ser considerado

- Para não haver fragmentação externa é necessário mover partição
  livre até a próxima partição livre, tornando-as uma única partição
  contínua. Esse processo é denominado **compactação**, porém esse
  processo é muito custoso para a CPU

- **Bitmap -** Um vetor com um bit para cada partição, ele serve para
  representar se a partição está livre ou ocupada (0 livre, 1
  ocupada).

- **Lista Encadeada -** Uma lista encadeada indicando o conjunto de
  bits ocupados e desocupados próximos, se há um processo ou não
  nesses bits, onde ele começa e qual o seu tamanho. Ex:
  \[P;0;4\]-\>\[H;5;8\] = 111100000000

# Algoritmos de Alocação de Memória para Processos

- **First Fit -** Aloca o primeiro segmento disponível.

- **Best Fit -** Aloca o segmento de memória que possui menor sobra de
  espaço em relação ao processo.

- **Worst Fit -** Aloca o segmento que tem a maior sobra de memória em
  relação ao processo. Combate fragmentação externa, porém favorece a
  interna.

# Paginação e Segmentação

**Memória Virtual -** Expandir a memória física do sistema,
possibilitando a execução de processos maiores. Dividir o processo em
partes menores com o **SO** sendo responsável por essa divisão. Através
de páginas, segmentação ou paginação segmentada. Paginação, tamanhos
iguais, segmentação tamanho variável.

**Paginação -** Dividir a memória em páginas, pedaços de tamanhos
iguais. No disco terá páginas também.

**Espaço de Endereçamento Virtual -** Endereços do processo, 32 bits = 4
GB, 64 bits = 16 EB.

**Tabela de Páginas -** Mapeamento de um endereço virtual com um endereço
real (espaço de memória real ou físico).

O espaço físico da memória se tornou insuficiente para sustentar
diversos processos de forma eficiente, disso veio a necessidade da
criação de um espaço de memória virtual.

Um processo aloca endereços virtuais, no momento da execução de alguma
operação o seu endereço virtual é traduzido pela MMU para um endereço
real. Há duas formas de atingir isso.

## Paginação

A técnica de paginação utiliza blocos de tamanho fixo na MV e MR
chamados de **páginas e page frames**, o SO mantém uma lista de todas os
**page frames** do sistema.

De forma resumida, a memória virtual é separada em páginas (blocos de
tamanho fixo), enquanto a memória física é separada em **page
frames**, ambos possuem o mesmo tamanho.

Enquanto o SO mantém uma lista de todas os **page frames** do sistema,
cada processo possui a sua própria **tabela de páginas**, que mapeia as
suas **páginas virtuais** com os **page frames.**

A tabela de páginas de um processo é constituída de um número da página
virtual, que vem do endereço virtual do processo, e o número do **page
frame** correspondente na memória física.

Ao acessar o dado de um processo, caso a página virtual deste dado ainda
não esteja mapeada com um **page frame**, o processo deve retornar um
**PAGE_FAULT** e alocar os page frames na memória física.

O tamanho das páginas (atualmente 4Kb) é definido pelo hardware, na
construção do computador, a CPU + MMU define um tamanho suportado para
as páginas. Dentro desse range de valores suportados, o sistema
operacional em seu kernel específico para aquela arquitetura define o
tamanho exato a ser usado. Porém, também há tamanhos alternativos
chamados de **Huge Pages/Large Pages.** Para cargas de trabalho que
manipulam grandes volumes de memória, como bancos de dados, máquinas
virtuais e aplicações científicas, os tamanhos padrão (4 KB) são
ineficientes, porque as tabelas de páginas ficam muito grandes e o tempo
gasto para traduzir endereços aumenta consideravelmente. Dessa forma, o
operador do computador pode configurar para utilizar ou não páginas
maiores (dependendo da arquitetura, pode-se chegar até 1 GB).

O principal problema da paginação é **fragmentação interna:**

- **Páginas maiores:** leitura mais eficiente, tabela menor, mas maior
  fragmentação interna.

- **Páginas menores:** leitura menos eficiente, tabela maior, mas
  menor fragmentação interna.

### Onde armazenar a tabela de páginas?

A tabela pode ser armazenada em 3 lugares diferentes:

- Em um conjunto de registradores (caso a memória seja pequena)

  - Apesar de ser rápido, em todo chaveamento de contexto será
    necessário carregar toda a tabela de páginas do processo neste
    registrador.

- Na própria memória RAM, sendo gerenciada pelo **MMU** usando dois
  registradores

  - O **registrador base da tabela de páginas** (PTBR - page table
    base register), indica o endereço físico de memória onde a
    tabela está alocada.

  - O **registrador limite da tabela de páginas** (PTLR - page table
    limit register), indica o número de entradas da tabela (número
    de páginas).

  - O problema é que é necessário dois acessos a memória sempre que
    for necessário referenciar algum dado da página, um para acessar
    a tabela, outro para acessar a devida posição.

- Em uma memória **cache na MMU**

  - Também conhecido por **TLB (Translation Lookaside Buffer)** ou
    buffer de tradução dinâmica.

  - É um hardware especial para mapear endereços virtuais para
    endereços reais sem ter que passar pela tabela de páginas na
    memória principal (possui o melhor desempenho).

  - O **TLB** armazena um conjunto recente de mapeamentos da tabela
    de páginas.

  - Cada entrada na TLB contém o número da página virtual e o número
    do page frame.

  - O programa solicita um endereço virtual, a **MMU** consulta a
    **TLB**, se encontrar **(TLB Hit)** o endereço físico é
    retornado, caso contrário **(TLB Miss)** um acesso a tabela de
    páginas na memória principal ocorre e um novo mapeamento é
    copiado para a **TLB**, após isso a **MMU** acessa o endereço
    físico correspondente.

  - Para manter a correspondência correta do mapeamento, o SO pode
    executar **TLB Flush**, limpando todo o cache, isso ocorre
    durante uma troca de páginas ou context switch, em alguns
    processadores, apenas a entrada que não é mais válida é apagada.
    Também pode-se manter um PID do processo para cada entrada na
    TLB, evitando que um processador acesse o espaço de memória de
    outro.

### Qual a estrutura de uma entrada na tabela de páginas?

Uma tabela de páginas 32 bits é formada por 6 colunas diferentes. Para
computadores 64 bits, a tabela de páginas pode ser dividida em múltiplas
tabelas diferentes:

- **Número do Page Frame**

- **Número da Página Virtual**

- **Bit de Residência (Present Bit):** Indica se a entrada é válida
  para o uso, se a página virtual está na memória principal ou não.

- **Bit de Proteção (Protection Bit):** Indica os tipos de acessos
  permitidos, leitura, escrita e/ou execução, código deve ter o bit de
  execução ativado, enquanto dados devem ter apenas os de escrita e
  leitura.

- **Bit de Modificação (Dirty Bit):** Controla o uso da página, se ela
  foi modificada desde que foi armazenada no disco. Quando uma página
  é carregada no disco, este bit é zerado, se o processo escrever algo
  nessa página, o sistema ativa este bit. Isso indica se a página
  precisa ser copiada de volta ou não para disco, para que as
  modificações não sejam perdidas. Se for necessário remover essa
  página da RAM, o **MMU** deve consultar esse bit antes.

- **Bit de Referência (Referenced Bit):** Controla o uso da página,
  auxilia o SO na escolha da página que deve deixar a memória
  principal. Se o valor é 1, a página foi referenciada (escrita ou
  leitura), se for zero, ela não foi acessada ainda.

- **Bit de Cache (Cache Disable Bit, CD):** Necessário quando os
  dispositivos de **I/O** são mapeados na memória e não em um
  endereçamento específico **I/O.** Ele define se aquela área pode ou
  não ser armazenada no cache do processador (**Ler sobre memory-mapped
  I/O**).

### Quantas páginas reais serão alocadas a um processo?

Há duas estratégias:

- **Alocação fixa ou estática:** Cada processo terá um número máximo
  de page frames, definidos quando o processo é criado. O limite pode
  ser igual para todos os processos.

  - Possui como vantagem a **simplicidade**, porém um número muito
    pequeno de page frames pode causar muita paginação, por
    consequência, muita troca de páginas.

- **Alocação variável ou dinâmica:** O número máximo de page frames
  varia durante sua execução.

  - Processos com elevada taxa de paginação podem ter seu limite de
    páginas reais ampliado, ou diminuído caso haja pouca troca. O
    problema se dá devido à necessidade de **monitoramento
    constante** para a eventual realocação.

### Quando uma página deve ser carregada para a memória?

Há 3 estratégias:

- **Paginação Simples:** Todas as páginas virtuais são carregadas na
  memória durante a criação do processo, o **bit de residência** de
  todas elas está ativo. Causa desperdício de memória.

- **Paginação por Demanda:** Apenas as páginas referenciadas são
  carregadas na memória principal.

- **Paginação Antecipada:** Carrega para a memória, além das páginas
  referenciadas, outras páginas que podem ou não ser necessárias para
  o processo. O SO, além das principais, carrega páginas próximas ou
  relacionadas, a escolha é baseada em dois princípios, **localidade
  temporal:** páginas acessadas recentemente tendem a ser acessadas de
  novo. E **localidade espacial:** se uma página foi acessada, é
  provável que páginas próximas também.

### Como trazer uma página para a memória?

Acessa a tabela de páginas, verifica se ela está no disco ou na memória.

Se estiver **em disco**, dá **page fault**, o processo é suspenso e
inserido em uma fila especial **([Page Fault Queue](https://zeux.io/2014/12/21/page-fault-queue/))**,
após isso, uma página real livre deve ser alocada. Então a tabela de
páginas é atualizada e a página que estava em disco é carregada na
memória (o processo de carregamento é realizado por um processo
denominado **Pager**), após isso, o processo é retirado da fila especial
e a suspensão é finalizada.

Outra possibilidade é se a página que o processo está tentando acessar,
está fora do seu espaço de endereçamento virtual, isso gera um erro de
segmentação (**Segmentation Fault**), o processo pode ser finalizado
pelo SO ou tratado pelo próprio processo.

### Como liberar espaço na memória?

Há algoritmos específicos para esse swap in/out da memória, so irei citar o **LRU por Aging** e **Clock**

- **LRU Aging** -> Esse algortimo parte do princípio de cada página possuir um contador iniciado em zero e incrementado a cada referência à página, a página com o melhor contador é trocada quando necessário, o problema disso é uma página nova que está sendo referenciada com frequência poderá ser trocada ao invés de uma página antiga que já não é mais referenciada. Para resolver esse problema:

  - Um contador de mais ou menos 8 bits é associada a cada página
  - A cada [**clock tick**](https://superuser.com/a/101202) os contadores de todas as páginas sofrem um shift para a direita (deslocados)
  - Se uma página foi acessada entre um **tick** e outro, o bit mais significativo do contador é setado para 1
  - Ao substituir uma página, é escolhido o que tem o menor valor do contador, o menos recentemente usado.

- **Clock** -> Os **page frames** são listados em uma lista circular, cada page frame possui um **bit R de referência**, que indica se a página foi usada recentemente.
  - Sempre que uma página é acessada por um processo, o hardware ou SO **setam o bit R para 1**
  - Por estar organizado em uma lista circular, um ponteiro percorre todas as páginas multiplas vezes, sempre que é necessário susbtituir uma página, o ponteiro retira a primeira página com bit R = 0 da memória, e põem a nova página.
  - Enquanto ele passa, toda página que possui bit R = 1, possui esse bit zerado, até ser modificado na próxima referência.
