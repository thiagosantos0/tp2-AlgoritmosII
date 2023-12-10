#ifndef HELPER_H
#define HELPER_H

#include <vector>

struct Point{
  int id;
  double x;
  double y;
};

// Struct para representar um nó da árvore
struct Node {
  int cost;
  int lower_bound;
  std::vector<int> path;
};

// Struct para representar uma aresta
struct Edge {
  int u;
  int v;
  int weight;
};


void imprimeUsoMemoria();
void imprimePontos(std::vector<Point> points);
int distanciaEuclideana(Point a, Point b);
int findRoot(std::vector<int>& parent, int vertex);
void union_(std::vector<int>& parent, int x, int y);
std::vector<Edge> kruskalMST(std::vector<Edge>& edges, int V, std::vector<int>& parent);
void dfsPreorder(int u, std::vector<std::vector<int>>& adjMatrix, std::vector<bool>& visited, std::vector<int>& tour);

// Branch and Bound

// Encontra a aresta de menor custo que poder ser conectada a um nó "i"
// Lookup na linha do vértice "i"
int firstMin(std::vector<std::vector<int>>& adjMatrix, int i);

// Ignora a primeira aresta de menor custo -> Encontra a segunda de menor custo
int secondMin(std::vector<std::vector<int>>& adjMatrix, int i);

// Computa lower bound para o nó u
int calcularLowerBound(std::vector<std::vector<int>>& adjMatrix, Node& u);
 


// Christofides

// Recebe a matriz de adjacência do grafo O = MST união M (onde MST é a árvore geradora mínima e M é o matching perfeito)
// e computa o ciclo euleriano
void eulerianTour(std::vector<std::vector<int>>& adjMatrixMST, std::vector<int>& matching, std::vector<int>& tour);

// Faz o ciclo hamiltoniano a partir do ciclo euleriano
void hamiltonianTour(std::vector<int>& tour, std::vector<bool>& visited);

// Faz o grafo O = MST união M
std::vector<std::vector<int>> getUnionGraph(std::vector<std::vector<int>>& adjMatrixMST, std::vector<int>& matching);

// Algoritmo de Blossom para encontrar o "minimum weight perfect matching"
int blossomMinWeightPerfectMatching(std::vector<std::vector<int>>& adjMatrix, std::vector<int>& matching);

// Função para computar o custo do tour
int custoCaminho(std::vector<std::vector<int>>& adjMatrix, std::vector<int>& tour);

// minKey retorna o vértice com a menor chave que não está na árvore geradora mínima
int minKey(std::vector<int>& key, std::vector<bool>& visited);


#endif
