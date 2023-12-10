// O seguinte programa implementa o algoritmo de Christofides para o problema do tsp métrico.

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <climits>
#include <stack>
#include <ctime>
#include <atomic>

#include "helper.h"

void tspChristofides(std::vector<std::vector<int>>& adjMatrix, std::atomic<bool>& acabouTempo) {

  
  // Computando o tempo
  constexpr int tempoMaximo = 5; // segundos
  std::time_t tempoInicial = std::time(nullptr);


  // Vetor de arestas
  std::vector<Edge> edges;

  // Repare que o custo de (1,2) == (2,1) (isso é pré requisito do tsp-métrico), então só precisamos armazenar metade das arestas
  for (int i = 0; i < adjMatrix.size(); i++) {
    for (int j = i + 1; j < adjMatrix.size(); j++) {
      edges.push_back({i, j, adjMatrix[i][j]});
    }
  }
  
  // Vetor de nós visitados
  std::vector<int> parent(adjMatrix.size(), -1);

  // Gerando a MST
  std::vector<Edge> MST = kruskalMST(edges, adjMatrix.size(), parent);

  // Construindo uma matriz de adjacência para a MST e iniciando ele com zeros
  // Vamos passar essa matriz para a função que encontra o emparelhamento perfeito
  std::vector<std::vector<int>> adjMatrixMST(adjMatrix.size(), std::vector<int>(adjMatrix.size(), 0));
  for (Edge& edge : MST) {
    adjMatrixMST[edge.u][edge.v] = edge.weight;
    adjMatrixMST[edge.v][edge.u] = edge.weight;
  }

  //// Encontrando o matching perfeito na MST
  std::vector<int> matching(adjMatrix.size(), -1);
  int custoPerfeito = 0;

  // Encontrando o "minimum weight perfect matching" usando o algoritmo de Blossom
  // Na verdade eu não utilizei o valor retornado porque passei o matching por referência então já estou
  // alterando ele
  custoPerfeito = blossomMinWeightPerfectMatching(adjMatrixMST, matching);
  std::vector<std::vector<int>> grafoO(adjMatrix.size(), std::vector<int>(adjMatrix.size(), 0));

  grafoO = getUnionGraph(adjMatrixMST, matching);

  std::vector<int> tour;
  std::vector<bool> visited(adjMatrix.size(), false);
  dfsPreorder(0, adjMatrixMST, visited, tour);


  std::cout << "Custo do ciclo hamiltoniano: " << custoCaminho(adjMatrix, tour) << std::endl;

  std::time_t tempoFinal = std::time(nullptr);
  std::cout << "Tempo de execução: " << tempoFinal - tempoInicial << " segundos" << std::endl;

  if (tempoFinal - tempoInicial > tempoMaximo) {
    acabouTempo.store(true);
  }

  acabouTempo.store(true);
}



int main(int argc, char* argv[]) {
  std::atomic<bool> acabouTempo(false);
  
  // Leitura dos dados
  if (argc != 2) {
    std::cout << "Formato: " << argv[0] << " <nome do arquivo>" << std::endl;
    return 1;
  }

  std::string filename = argv[1];

  std::string path = "./datasets/" + filename;


  std::ifstream file(path);
  std::string line;
  std::vector<Point> points;

  if (file.is_open()) {
    while (std::getline(file, line)) {
      std::istringstream iss(line);
      if (line.find("NODE_COORD_SECTION") != std::string::npos) {
        while (std::getline(file, line) && line != "EOF") {
          std::istringstream iss(line);
          Point point;
          iss >> point.id >> point.x >> point.y;
          points.push_back(point);
        }
        break;
      }
  }
  file.close();
  } else {
    std::cout << "Falha na abertura do arquivo\n" << std::endl;
  }
  
  // Representação dos dados em uma matriz de adjacência
  std::vector<std::vector<int>> adjMatrix(points.size(), std::vector<int>(points.size()));
  for (int i = 0; i < points.size(); i++) {
    for (int j = 0; j < points.size(); j++){
      adjMatrix[i][j] = distanciaEuclideana(points[i], points[j]);
    }
  }


  //imprimePontos(points);

  tspChristofides(adjMatrix, acabouTempo);

  // Imprimindo informações de memória
  imprimeUsoMemoria();



  return 0;
}
