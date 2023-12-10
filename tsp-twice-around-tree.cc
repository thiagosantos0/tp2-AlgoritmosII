// O seguinte programa implementa o algoritmo Twice-Around-The-Tree para o problema do caixeiro viajante métrico.

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

void tspTwiceAroundTree(std::vector<std::vector<int>>& adjMatrix, std::atomic<bool>& acabouTempo) {

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
    std::vector<int> parent(adjMatrix.size());

    // Gerando a MST usando Kruskal
    std::vector<Edge> MST = kruskalMST(edges, adjMatrix.size(), parent);

    // Imprimindo MST
    //std::cout << "MST: " << std::endl;
    //for (Edge& edge : MST) {
    //    std::cout << edge.u << " " << edge.v << " " << edge.weight << std::endl;
    //}

    // Calcular o custo da MST usando o método "multiply by 2"
    int cost = 0;
    for (const Edge& edge : MST) {
        // evitando vérices repetidos
      if (parent[edge.u] != edge.v && parent[edge.v] != edge.u){
        cost += edge.weight;
      }
    }

    // Multiplicar por 2 para completar o ciclo
    cost *= 2;

    std::cout << "Custo: " << cost << std::endl;


    //std::cout << "Custo mínimo: " << min_cost << std::endl;
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


  // DEBUG //

  // Imprime os nós para verificar se leu corretamente
  //imprimePontos(points);

  // Rodando o branch and bound
  tspTwiceAroundTree(adjMatrix, acabouTempo);

  // Imprimindo informações de memória
  imprimeUsoMemoria();

  // [ LEITURA DOS DADOS FORA DO FORMATO PEDIDO PARA O TRABALHO, APENAS PARA TESTES ]
  //std::ifstream file2("./datasets/small15.tsp");

  //if (!file2) {
  //  std::cerr << "Não foi possível abrir o arquivo." << std::endl;
  //  return 1;
  //}
  //
  //int size;
  //file2 >> size;

  //std::vector<std::vector<int>> adj(size, std::vector<int>(size));
  //double aux;

  //for (int i = 0; i < size; ++i) {
  //  for (int j = 0; j < size; ++j) {
  //    file2 >> aux;
  //    adj[i][j] = (int) aux;
  //  }
  //}

  //// Imprimindo matriz
  //for (const auto &linha : adj) {
  //  for (int valor : linha) {
  //    std::cout << valor << ' ';
  //  }
  //  std::cout << '\n';
  //}

  //std::cout << std::endl;

  //tspTwiceAroundTree(adj);

  return 0;
}
