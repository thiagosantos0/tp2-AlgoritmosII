#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <climits>
#include <stack>
#include <sys/resource.h>

#include "helper.h"



void imprimeUsoMemoria() {
  rusage usage;
  getrusage(RUSAGE_SELF, &usage);
  std::cout << "Memória utilizada: " << usage.ru_maxrss << " KB" << std::endl;
}

void imprimePontos(std::vector<Point> points) {
  for (const auto& point : points) {
    std::cout << "ID: "<< point.id << ", X: " << point.x << ", Y: " << point.y << "\n";
  }
}


int distanciaEuclideana(Point a, Point b) {
  return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}


int findRoot(std::vector<int>& parent, int vertex) {
  // Se o pai de i é ele mesmo, então i é a raiz da árvore
  // Caso contrário, i não é a raiz da árvore, então chamamos findRoot novamente
  // até que o pai de i seja ele mesmo e isso significa que i é a raiz da árvore
  if (parent[vertex] != vertex) {
    parent[vertex] = findRoot(parent, parent[vertex]);
  }
  return parent[vertex];
}

// Função para unir os conjuntos
// A palavra "union" é reservada, por isso usei "union_"
void union_(std::vector<int>& parent, int x, int y) {
    int r1 = findRoot(parent, x);
    int r2 = findRoot(parent, y);
    parent[r1] = r2;
}

// Método para comparar custos de duas arestas
bool comparaArestas(const Edge& a, const Edge& b) {
  return a.weight < b.weight;
}

// Encontra a MST usando o algoritmo de Kruskal
std::vector<Edge> kruskalMST(std::vector<Edge>& edges, int V, std::vector<int>& parent) {
  std::sort(edges.begin(), edges.end(), comparaArestas);

  std::vector<Edge> MST;

  for (int i = 0; i < V; i++) {
    parent[i] = i;
  }

  for (Edge& edge : edges) {
    int x = findRoot(parent, edge.u);
    int y = findRoot(parent, edge.v);

    if (x != y) {
      MST.push_back(edge);
      union_(parent, x, y);
    }

  }

  return MST;
}

void dfsPreorder(int u, std::vector<std::vector<int>>& adjMatrix, std::vector<bool>& visited, std::vector<int>& tour) {

  visited[u] = true;
  tour.push_back(u);

  for (int v = 0; v < adjMatrix.size(); v++) {
    if (adjMatrix[u][v] != 0 && !visited[v]) {
      dfsPreorder(v, adjMatrix, visited, tour);
    }
  }
}

std::vector<std::vector<int>> getUnionGraph(std::vector<std::vector<int>>& adjMatrixMST, std::vector<int>& matching) {
  std::vector<std::vector<int>> unionGraph(adjMatrixMST.size(), std::vector<int>(adjMatrixMST.size(), 0));
  // Unindo o matching perfeito com a MST numa matriz de adjacências
  for (int i = 0; i < adjMatrixMST.size(); i++) {
    if (matching[i] != -1) {
      unionGraph[i][matching[i]] = adjMatrixMST[i][matching[i]];
      unionGraph[matching[i]][i] = adjMatrixMST[i][matching[i]];
    }
  }
  return unionGraph;
}


// Combina o MST e o Emparelhamento Mínimo perfeito (O) para encontrar um ciclo euleriano
void eulerianTour(std::vector<std::vector<int>>& adjMatrix2, std::vector<int>& matching, std::vector<int>& tour) {

  std::stack<int> stack;
  stack.push(0);

  while (!stack.empty()) {
    int u = stack.top();
    if (!adjMatrix2[u].empty()) {
      int v = adjMatrix2[u].back();
      adjMatrix2[u].pop_back();
      adjMatrix2[v].erase(std::remove(adjMatrix2[v].begin(), adjMatrix2[v].end(), u), adjMatrix2[v].end());
      stack.push(v);
    } else {
      tour.push_back(u);
      stack.pop();
    }
  }
}


// ----------------------------------------------------------------------------------------------------------------------------//

// **** Branch and Bound****
//
int firstMin(std::vector<std::vector<int>>& adjMatrix, int i) {
  int min = INT_MAX;
  for (int j = 0; j < adjMatrix.size(); j++) {
    //std::cout << "elemento: " << adjMatrix[i][j] << std::endl;
    if (adjMatrix[i][j] < min && i != j) {
      min = adjMatrix[i][j];
    }
  }
//  std::cout << "primeiro min: " << min << std::endl;
  return min;
}

int secondMin(std::vector<std::vector<int>>& adjMatrix, int i) {
  int first = INT_MAX, second = INT_MAX;
  for (int j = 0; j < adjMatrix.size(); j++) {
    // Ignorando o caso de "aresta loop", [0][0], [1][1] e etc.
    if (i == j) {
      continue;
    }
    // Se o custo da aresta for menor do que a mais curta de todas, atualizo
    // first para ter esse valor e second para first, second vai ter o segundo valor
    // mais curto.
    if (adjMatrix[i][j] <= first) {
      second = first;
      first = adjMatrix[i][j];
    // Se o custo da aresta for maior do que o mais curto de todos, mas for menor do que
    // o que temos no second, atualizamos o second.
    } else if (adjMatrix[i][j] <= second && adjMatrix[i][j] != first) {
      second = adjMatrix[i][j];
    }
  }
//  std::cout << "segundo min: " << second << std::endl;
  return second;
}


int calcularLowerBound(std::vector<std::vector<int>>& adjMatrix, Node& u) {
  int lower_bound = 0;
  for (int i = 0; i < adjMatrix.size(); i++) {
    // Se o vértice não está no caminho, então calcula o lower bound
    // Duas arestas mais baratas
    if (!std::count(u.path.begin(), u.path.end(), i)) {
      lower_bound += firstMin(adjMatrix, i) + secondMin(adjMatrix, i);
    }
  }

  lower_bound = ceil(lower_bound/ 2);
  
  return lower_bound;
}



// ----------------------------------------------------------------------------------------------------------------------------//

// **** Christofides ****

// Computando o resultado final dado o circuito euleriano, gerando um hamiltoniano
void hamiltonianTour(std::vector<int>& tour, std::vector<bool>& visited) {
  std::vector<int> finalTour;
  for (int i = 0; i < tour.size(); i++) {
    if (!visited[tour[i]]) {
      finalTour.push_back(tour[i]);
      visited[tour[i]] = true;
    }
  }
  finalTour.push_back(finalTour[0]);
  tour = finalTour;
}

// Algoritmo de Blossom 
int blossomMinWeightPerfectMatching(std::vector<std::vector<int>>& adjMatrix, std::vector<int>& matching) {
  int n = adjMatrix.size();
  std::vector<std::vector<int>> graph;

  graph = adjMatrix; // novo nickname para a adjMatrix dentro da função


  std::vector<int> match(n, -1);
  std::vector<int> slack(n, INT_MAX);
  std::vector<int> label(n, 0);
  std::vector<int> parent(n, -1);

  // Encontrando o label inicial
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
        slack[j] = std::min(slack[j], graph[i][j]);
    }
  }

  // Aumentando o matching
  for (int i = 0; i < n; i++) {
    std::vector<bool> visited(n, false);
    std::vector<int> blossom(n, -1);

    int u = i;
    int v = -1;
    int delta = INT_MAX;

    while (match[u] == -1) {
      v = -1;
      // Encontrando o vértice v que não foi visitado e que tem o menor slack
      for (int j = 0; j < n; j++) {
        if (!visited[j] && (v == -1 || slack[j] < slack[v])) {
          v = j;
        }
      }

      visited[v] = true;
      int w = -1;

      // Atualizando o slack
      for (int j = 0; j < n; j++) {
        if (!visited[j]) {
          int value = graph[u][j] - label[u] - label[j];
          if (slack[j] > value) {
            slack[j] = value;
            parent[j] = u;
          }

          if (slack[j] < delta) {
            w = j;
          }
        }
      }
      // Atualizando o label
      for (int j = 0; j < n; j++) {
        if (visited[j]) {
          label[j] += delta;
        }
        else {
          slack[j] -= delta;
        }
      }
      u = w;
    }

    // Atualizando o matching
    while (u != -1) {
      int p = parent[u];
      int pp = match[p];
      match[u] = p;
      match[p] = u;
      u = pp;
    }
  }
  // Calculando o custo do matching
  int cost = 0;
  for (int i = 0; i < n; i++) {
    cost += graph[i][match[i]];
  }
  return cost / 2;
}

int minKey(std::vector<int>& key, std::vector<bool>& visited) {
  int min = INT_MAX;
  int minIndex;

  for (int v = 0; v < key.size(); v++) {
    if (visited[v] == false && key[v] < min) {
      min = key[v];
      minIndex = v;
    }
  }

  return minIndex;
}


int custoCaminho(std::vector<std::vector<int>>& adjMatrix, std::vector<int>& tour) {
  int custo = 0;
  std::vector<int> repeated(adjMatrix.size(), 0);

  for (int i = 0; i < tour.size() - 1; i++) {
    if (repeated[tour[i]] == 0) {
      //std::cout << tour[i] << " - " << tour[i + 1] << std::endl;
      //std::cout << "custo: " << custo << std::endl;
      custo += adjMatrix[tour[i]][tour[i + 1]];
      repeated[tour[i]] = 1;
    }
  }
  // Adicionando o custo da última aresta
  custo += adjMatrix[tour[tour.size() - 1]][tour[0]];
  return custo;
}

