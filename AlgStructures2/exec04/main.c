#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define Vertex int

typedef struct node_struct *Node; // ponteiro para noh

// noh da lista de adjacencia
struct node_struct {
  Vertex x;
  int weight;
  bool visited;
  Node next; // aponta para o proximo no
};

// o (Di)Grafo armazena o nro de vertices, o nro de arcos/arestas e um ponteiro para a lista de adjacencia
struct graph {
  int V;     // qtde de vertices
  int A;     // qtde de arcos/arestas
  Node *adj; // ponteiro para array de vertices
};

typedef struct graph *Digraph;
#define Graph Digraph

Graph initGraph(int V) {
  Graph G = (Graph)malloc(sizeof(*G));
  G->V = V;
  G->A = 0;
  G->adj = (Node *)malloc(V * sizeof(Node *));
  for (int i = 0; i < V; i++) {
    G->adj[i] = NULL;
  }
  return G;
}

Node newArc(Vertex w, int weight) {
  Node a = (Node)malloc(sizeof(*a));
  a->x = w;
  a->weight = weight;
  a->next = NULL;
  return a;
}

void insertArc(Graph G, Vertex v, Vertex w, int weight) {
  // verifica se o vertice w já existe na lista de nodeacencia de v
  Node last = G->adj[v];
  for (Node v_node = G->adj[v]; v_node != NULL; v_node = v_node->next) {
    if (v_node->x == w) return;
    last = v_node;
  }
  // insere arco no inicio da lista
  if(G->adj[v] == NULL) {
    G->adj[v] = newArc(w, weight);
  } else { // insere arco no final da lista
    last->next = newArc(w, weight);
  }
  G->A++;
}

void insertEdge(Graph G, Vertex v, Vertex w, int weight) {
  insertArc(G, v, w, weight);
  insertArc(G, w, v, weight);
  G->A--;
}

void removeArc(Graph G, Vertex v, Vertex r) {
  if (G->adj[v] == NULL) { // nao existe arco saindo de v
    return;
  }
  Node node = G->adj[v];
  for (Node r_node = G->adj[v]; r_node != NULL; r_node = r_node->next) { // cc, percorre lista
    if (r_node->x == r) {  // se eh o vertice a ser retirado
      G->A--;
      if (G->adj[v] == r_node) { // se for o primeiro da lista
        G->adj[v] = r_node->next;
        return;
      }
      node->next = r_node->next;
      return;
    } else {
      node = r_node; // guarda referencia do anterior
    }
  }
}

void removeEdge(Graph G, Vertex v, Vertex w) {
  removeArc(G, v, w);
  removeArc(G, w, v);
  G->A++;
}

void destroyGraph(Graph G) {
  if (G != NULL) {
    for(int i; i < G->V; i++) {
      free(G->adj[i]);
    }
    free(G->adj);
    free(G);
  }
}

void printGraph(Graph G) {
  printf("\nTotal of vertices: %d, total of arcs/edges: %d\n", G->V, G->A);
  for (int i = 0; i < G->V; i++) {
    printf("Vertex %d, arcs: ", i);
    for (Node print = G->adj[i]; print != NULL; print = print->next) {
      printf("  -> %d (peso %d)", print->x, print->weight);
    }
    printf("\n");
  }
}

void clearVisited(Graph G) {
  for (int i = 0; i < G->V; i++) {
    G->adj[i]->visited = false;
  }
}

void DFS(Graph G, Vertex x) {
  G->adj[x]->visited = true;
  printf("visited %d \n", x);
  for (Node p = G->adj[x]; p != NULL; p = p->next) {
    Vertex y = p->x;
    if (!(G->adj[y]->visited)) {
      DFS(G, y);
    }
  }
}

void startDFS(Graph G, Vertex start) {
  clearVisited(G);
  printf("DFS\n");
  DFS(G, start);
}

void BFS(Graph G, Vertex x) {
  Vertex *queue = (Vertex *)malloc(sizeof(Vertex) * G->V);
  int start = 0, end = 0;
  queue[end++] = x;
  while (start != end) {
    Vertex elem = queue[start++];
    if (!(G->adj[elem]->visited)) {
      G->adj[elem]->visited = true;
      printf("visited %d \n", elem);
      for (Node p = G->adj[elem]; p != NULL; p = p->next) {
        Vertex w = p->x;
        if (!(G->adj[w]->visited)) {
          queue[end++] = w;
        }
      }
    }
  }
  free(queue);
}

void startBFS(Graph G, Vertex start) {
  clearVisited(G);
  printf("BFS\n");
  BFS(G, start);
}

bool simplePath(Graph G, Vertex start, Vertex end) {
  G->adj[start]->visited = true;
  for (Node p = G->adj[start]; p != NULL; p = p->next) {
    if (p->x == end) {
      return true;
    }
    if (G->adj[p->x]->visited == false) {
      if (simplePath(G, p->x, end)) {
        return true;
      }
    }
  }
  return false;
}

void startSimplePath(Graph G, Vertex start, Vertex end) {
  clearVisited(G);
  if (simplePath(G, start, end)) {
    printf("Simple path %d--%d exists!\n", start, end);
  } else {
    printf("Simple path %d--%d does NOT exists!\n", start, end);
  }
}


Graph copyGraph(Graph G) {
  Graph C = initGraph(G->V);
  for(int i = 0; i < G->V; i++) {
      for(Node p = G->adj[i]; p != NULL; p = p->next) {
          insertEdge(C, i, p->x, p->weight);
      }
  }
  return C;
}

int path(Graph C, Vertex v, int *stack, int *index) {
  while (C->adj[v] != NULL) {
      (*index)++;
      stack[*index] = v;
      Vertex w = C->adj[v]->x;
      removeEdge(C, v, w);
      v = w;
  }
  return 1;
}

void eulerPath(Graph G, Vertex v, Vertex end) {
  Graph C = copyGraph(G);
  Vertex *stack = (Vertex *)malloc(sizeof(Vertex)*C->V);
  int index = -1;
  printf("\nEuler Path: -> %d", end);
  while(path(C, v, stack, &index) && index >=0) {
      v = stack[index--];
      printf(" -> %d", v);
  }
  printf("\n");
  //free(stack);
  //destroyGraph(C);
}

void checkEulerPath(Graph G) {
  int *degree = (int *)calloc(G->V, sizeof(int));
  Vertex odds[3];
  int n_odds = 0;
  for(int i = 0; i < G->V; i++) {
      for(Node p = G->adj[i]; p != NULL; p = p->next) {
          degree[i]++;
      }
      if (degree[i] % 2 != 0) {
          odds[n_odds] = i;
          n_odds++;
          if(n_odds > 2) {
              printf("An Euler path does NOT exist.\n");
              free(degree);
              return;
          }
      }
  }
  if (n_odds == 2) {
      printf("An Euler PATH exists.\n");
      eulerPath(G, odds[0], odds[1]);
  } else {
      printf("An Euler CICLE exists.\n");
      eulerPath(G, 0, 0);
  }
  free(degree);
}

bool hamiltonPath(Graph G, Vertex start, Vertex end, int n) {
  if( start == end) {
    if(((n + 1) == G->V)) {
      printf("\n");
      return true;
    } else if(n != 0) {
      return false;
    }
  }
  G->adj[start]->visited = true;
  for(Node p = G->adj[start]; p != NULL; p = p->next) {
    if(!(G->adj[p->x]->visited) || p->x == end) {
      if(hamiltonPath(G, p->x, end, n + 1)) {
        printf("| %d -> %d ", start, p->x);
        return true;
      }
    }
  }
  G->adj[start]->visited = false;
  return false;
}

bool checkHamiltonPath(Graph G, Vertex start, Vertex end) {
  clearVisited(G);
  printf("\nHamilton Path: ");
  return hamiltonPath(G, start, end, 0);
}

void checkEulerPathDigraph(Graph G) {
  int *inDegree = (int *)calloc(G->V, sizeof(int));
  int *outDegree = (int *)calloc(G->V, sizeof(int));

  for (int v = 0; v < G->V; v++) {
    for (Node p = G->adj[v]; p != NULL; p = p->next) {
      outDegree[v]++;
      inDegree[p->x]++;
    }
  }

  int start = -1, end = -1;
  for (int i = 0; i < G->V; i++) {
    if (outDegree[i] - inDegree[i] == 1) {
      if (start == -1) start = i;
      else {
        printf("An Euler path or cicle does NOT exist.\n");
        free(inDegree);
        free(outDegree);
        return;
      }
    } else if (inDegree[i] - outDegree[i] == 1) {
      if (end == -1) end = i;
      else {
        printf("An Euler path or cicle does NOT exist.\n");
        free(inDegree);
        free(outDegree);
        return;
      }
    } else if (inDegree[i] != outDegree[i]) {
      printf("An Euler path or cicle does NOT exist.\n");
      free(inDegree);
      free(outDegree);
      return;
    }
  }

  if (start != -1 && end != -1) {
    printf("An Euler PATH exists.\n");
    eulerPath(G, start, end);
  } else {
    printf("An Euler CICLE exists.\n");
    eulerPath(G, 0, 0);
  }

  free(inDegree);
  free(outDegree);
}


int main() {
  int V, A;
  scanf("%d", &V);
  scanf("%d", &A);
  Graph G = initGraph(V);
  for (int i = 0; i < A; i++) {
    int v, w;
    scanf("%d %d", &v, &w);
    insertArc(G, v, w, 1);
  }

  checkEulerPathDigraph(G);

  destroyGraph(G);
  return 0;
}
