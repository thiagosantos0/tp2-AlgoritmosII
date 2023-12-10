// O seguinte programa implementa o algoritmo Branch and Bound para o problema do caixeiro viajante métrico.

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <climits>
#include <ctime>
#include <atomic>

#include "helper.h"


void tspBranchAndBound(std::vector<std::vector<int>>& adjMatrix, std::atomic<bool>& acabouTempo) {
  
  // Computando o tempo
  constexpr int tempoMaximo = 5; // segundos
  std::time_t tempoInicial = std::time(nullptr);

  // Vou utilizar uma priority queue para armazenar os nós da árvore de busca
  std::vector<Node> Q;

  Node v, u;
  u.path.push_back(0);
  u.lower_bound = 0;
  u.cost = 0;

  Q.push_back(u);


  std::cout << "Lower bound do nó raiz: " << u.lower_bound << std::endl;
  std::cout << "Custo do nó raiz: " << u.cost << std::endl;

  int min_cost = INT_MAX;
  while (!Q.empty()){
    u = Q.front();
    Q.erase(Q.begin());

    if (u.lower_bound < min_cost) {
      // Pegando o último vértice adicionado ao caminho
      int i = u.path[u.path.size() - 1];
      
      for (int j = 0; j < adjMatrix.size(); j++){
        if (!std::count(u.path.begin(), u.path.end(), j)) {
          // Se o nó não está no caminho, então adiciona ele
          v.path = u.path;
          v.path.push_back(j);

          if (v.path.size() == adjMatrix.size()) {
            //std::cout << "ENTROU NO IF DO FINAL\n";
            v.cost = u.cost + adjMatrix[i][j] + adjMatrix[j][0];
            if (v.cost < min_cost)
              min_cost = v.cost;
          }
          // Caso contrário, calculamos o lower bound do vértice que está sendo adicionado
          // e colocamos ele na fila de prioridade
          else {
            // Pegando o custo do caminho que tinhamos antes e somando com o custo da aresta do último vértic
            // do caminho anterior com o vértice que está sendo adicionado
            //std::cout << "u.cost: " << u.cost << std::endl;
            v.cost = u.cost + adjMatrix[i][j];
            // O lower bound é a soma das arestas de menor custo de cada vértice, então estamos subtraindo o lower bound
            // anterior com a soma das arestas de menor custo do vértice anterior e do vértice que está sendo adicionado
            // isso nos dá um lower bound porque estamos subtraindo o lower bound anterior com a soma das arestas de menor, mesmo 
            // que não resulte num caminho válido, ainda é um lower bound.

            v.lower_bound = calcularLowerBound(adjMatrix, v);

            if (v.lower_bound < min_cost)
              Q.push_back(v);
          }
        }
      }
    }
  }
  std::cout << "Custo mínimo: " << min_cost << std::endl;
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

  tspBranchAndBound(adjMatrix, acabouTempo);


  // [ LEITURA DOS DADOS NO FORMATO ALTERNATIVO PARA TESTES ]
  //std::ifstream file2("./datasets/small5.tsp");

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

  
  

  // DEBUG //

  // Imprime os nós para verificar se leu corretamente
  //imprimePontos(points);

  // Rodando o branch and bound
  //tspBranchAndBound(adj, acabouTempo);

  return 0;
}
