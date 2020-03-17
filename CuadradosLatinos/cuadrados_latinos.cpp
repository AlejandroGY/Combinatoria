#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <numeric>

namespace base {
   template<typename T, int N>
   struct matrix : std::vector<matrix<T, N - 1>> {
      template<typename... P>
      matrix(int n, const P&... param)
      : std::vector<matrix<T, N - 1>>(n, matrix<T, N - 1>(param...)) {
      }
   };

   template<typename T>
   struct matrix<T, 1> : std::vector<T> {
      matrix(int n, const T& v = T( ))
      : std::vector<T>(n, v) {
      }
   };
}

bool bpm(base::matrix<bool, 2>& grafo, int u, std::vector<bool>& visto, std::vector<int>& acoplamiento_, int n) {
   for (int v = 0; v < n; ++v) {
      if (grafo[u][v] && !visto[v]) {
         visto[v] = true;
         if (acoplamiento_[v] < 0 || bpm(grafo, acoplamiento_[v], visto, acoplamiento_, n)) {
            acoplamiento_[v] = u;
            return true;
         }
      }
   }
   return false;
}

int maxBPM(base::matrix<bool, 2>& grafo, std::vector<int>& acoplamiento_, int n) {
   int res = 0;
   for (int u = 0; u < n; ++u) {
      std::vector<bool> visto(n, false);
      if (bpm(grafo, u, visto, acoplamiento_, n)) {
         res++;
      }
   }
   return res;
}

template<typename T>
void imprime(const base::matrix<T, 2>& cuadrado, std::string s = "") {
   std::cout << s << "\n";
   for (const auto& fila : cuadrado) {
      for (const auto& celda : fila) {
         std::cout << celda << " ";
      }
      std::cout << "\n";
   }
}

template<typename T>
void copia_cuadrado(base::matrix<T, 2>& fuente, base::matrix<T, 2>& destino, int tam) {
   for (int i = 0; i < tam; ++i) {
      for (int j = 0; j < tam; ++j) {
         destino[i][j] = fuente[i][j];
      }
   }
}

std::pair<int, int> busca_elemento(base::matrix<int, 2>& cuadrado, int elemento, int tam, int reduccion) {
   for (int i = reduccion; i < tam; ++i) {
      for (int j = 0; j < tam - reduccion; ++j) {
         if (cuadrado[i][j] == elemento) {
            return { i, j };
         }
      }
   }
   return { -1, -1 };
}

void ajusta_adyacencia(base::matrix<int, 2>& cuadrado, base::matrix<bool, 2>& adyacencia, int tam, int reduccion, std::vector<int>& omitir) {
   for (const auto& actual : omitir) {
      for (int j = 0; j < tam; ++j) {
         adyacencia[actual - 1][j] = 0;
      }
   }
   for (int i = 0; i < reduccion; ++i) {
      for (int j = 0; j < tam; ++j) {
         if (cuadrado[i][j] != 0) {
            adyacencia[cuadrado[i][j] - 1][j] = 0;
         }
      }
   }
   for (int i = 0; i < tam; ++i) {
      for (int j = tam - reduccion; j < tam; ++j) {
         adyacencia[i][j] = 0;
      }
   }
}

void arregla_acumulado(base::matrix<int, 2>& cuadrado, std::vector<std::pair<int, int>>& acumulado, std::vector<int>& cantidad_elementos, int tam, int reduccion) {
   std::fill(cantidad_elementos.begin( ), cantidad_elementos.end( ), 0);
   std::partial_sum(acumulado.begin( ), acumulado.end( ), acumulado.begin( ), [](const auto& a, const auto& b) {
      return std::make_pair(a.first, a.second + 1);
   });

   for (int i = reduccion; i < tam; ++i) {
      for (int j = 0; j < tam - reduccion; ++j) {
         if (cuadrado[i][j] != 0) {
            cantidad_elementos[cuadrado[i][j] - 1] += 1;
            acumulado[i].first += 1;
         }
      }
   }
}

void lemma_1(base::matrix<int, 2>& cuadrado, std::vector<std::pair<int, int>>& acumulado, int tam, int reduccion, std::vector<int>& omitir) {
   int index = 0;
   for (auto& actual : acumulado) {
      if (actual.first == 0 && actual.second >= reduccion) {
         base::matrix<bool, 2> adyacencia(tam, tam, 1);
         for (int j = 0; j < tam - reduccion; ++j) {
            for (int i = reduccion; i < tam; ++i) {
               if (cuadrado[i][j] != 0) {
                  adyacencia[cuadrado[i][j] - 1][j] = 0;
               }
            }
         }

         if (reduccion != 0) {
            ajusta_adyacencia(cuadrado, adyacencia, tam, reduccion, omitir);
         }
         imprime(adyacencia, "Matriz adyacencia1:");

         std::vector<int> acoplamiento(tam, -1);
         int tam_acoplamiento = maxBPM(adyacencia, acoplamiento, tam);
         for (int i = 0; i < tam; ++i) {
            cuadrado[actual.second][i] = acoplamiento[i] + 1;
         }
         acumulado[index].first = tam - reduccion;
      }
      index += 1;
   }
}

void lemma_2(base::matrix<int, 2>& cuadrado, std::vector<std::pair<int, int>>& acumulado, int tam, int reduccion, std::vector<int>& omitir) {
   int index = 0;
   for (auto& actual : acumulado) {
      base::matrix<bool, 2> adyacencia(tam, tam, 1);
      if (actual.first != 0 && actual.second >= reduccion) {
         for (int j = 0; j < tam - reduccion; ++j) {
            if (cuadrado[actual.second][j] != 0) {
               for (int i = 0; i < tam; ++i) {
                  adyacencia[i][j] = 0;
               }
               for (int i = 0; i < tam; ++i) {
                  adyacencia[cuadrado[actual.second][j] - 1][i] = 0;
               }
            } else {
               for (int i = reduccion; i < tam; ++i) {
                  if (cuadrado[i][j] != 0) {
                     adyacencia[cuadrado[i][j] - 1][j] = 0;
                  }
               }
            }
         }

         if (reduccion != 0) {
            ajusta_adyacencia(cuadrado, adyacencia, tam, reduccion, omitir);
         }
         imprime(adyacencia, "Matriz adyacencia2:");

         std::vector<int> acoplamiento(tam, -1);
         int tam_acoplamiento = maxBPM(adyacencia, acoplamiento, tam);
         for (int i = 0; i < tam; ++i) {
            if (acoplamiento[i] != -1) {
               cuadrado[actual.second][i] = acoplamiento[i] + 1;
            }
         }
         acumulado[index].first = tam - reduccion;
      }
      index += 1;
   }
}

void intercambia_diagonal(base::matrix<int, 2>& cuadrado, std::vector<std::pair<int, int>>& acumulado, int elemento_unico, std::pair<int, int>& pos_elem_unico, int tam, int reduccion) {
   int ini = pos_elem_unico.first;
   std::vector<bool> visto(tam, false);
   for (int i = ini; i <= tam - reduccion; ++i) {
      acumulado[i].first += 1;
      cuadrado[i][tam - reduccion] = elemento_unico;
      std::swap(cuadrado[i][i], cuadrado[i][tam - reduccion]);
      std::cerr << i << "*\n";
      if (!visto[cuadrado[i][tam - reduccion] - 1]) {
         visto[cuadrado[i][tam - reduccion] - 1] = true;
      } else {
         int actual = cuadrado[i][tam - reduccion];
         std::vector<bool> fila_vista(tam, false); fila_vista[i] = true;
         while (true) {
            bool cambio = false;
            for (int k = reduccion; k <= i; ++k) {
               if (k != i && cuadrado[k][tam - reduccion] == actual && !fila_vista[k]) {
                  std::swap(cuadrado[k][i], cuadrado[k][tam - reduccion]);
                  fila_vista[k] = true;
                  actual = cuadrado[k][tam - reduccion];
                  cambio = true;
               }
            }
            if (cambio == false) {
               break;
            }
         }
      }
      imprime(cuadrado, "Intercambios:");
   }
}

void intercambia_filas(base::matrix<int, 2>& cuadrado, std::vector<std::pair<int, int>> filas, int tam, int reduccion) {
   std::reverse(filas.begin( ), filas.end( ));
   for (const auto& f : filas) {
      for (int i = 0; i < tam - reduccion; ++i) {
         std::swap(cuadrado[f.first][i], cuadrado[f.second][i]);
      }
   }
}

void intercambia_columnas(base::matrix<int, 2>& cuadrado, int i, int j, int tam, int reduccion) {
   for (int k = reduccion; k < tam; ++k) {
      std::swap(cuadrado[k][i], cuadrado[k][j]);
   }
}

void intercambia_columnas(base::matrix<int, 2>& cuadrado, std::vector<std::pair<int, int>> filas, int tam, int reduccion, std::vector<int>& cantidad_elementos) {
   int elemento_unico = 1 + (std::find(cantidad_elementos.begin( ), cantidad_elementos.end( ), 1) - cantidad_elementos.begin( ));
   std::pair<int, int> pos_elemento_unico = busca_elemento(cuadrado, elemento_unico, tam, reduccion);

   int siguiente = 0;
   for (int f = 0; f < filas.size( ); ++f) {
      for (int i = siguiente; i < tam; ++i) {
         if (cuadrado[filas[f].second][i] == elemento_unico) {
            intercambia_columnas(cuadrado, i, filas[0].second, tam, reduccion);
         } else if (cuadrado[filas[f].second][i] != 0) {
            siguiente += (siguiente == filas[0].second);
            intercambia_columnas(cuadrado, i, siguiente++, tam, reduccion);
         }
      }
   }
}

void permuta_filas_columnas(base::matrix<int, 2>& cuadrado, std::vector<std::pair<int, int>>& acumulado, int tam, int reduccion, std::vector<int>& cantidad_elementos) {
   //arregla_acumulado(cuadrado, acumulado, cantidad_elementos, tam, reduccion);
   std::cerr << "\nCantidad de elementos:\n";
   for (int i = 0; i < tam; ++i) {
      std::cerr << "Elemento" << i + 1 << ": " << cantidad_elementos[i] << "\n";
      std::cerr << "Acum" << i << ": " << acumulado[i].first << "\n\n";
   }
   std::cerr << "\n";

   int elemento_unico = 1 + (std::find(cantidad_elementos.begin( ), cantidad_elementos.end( ), 1) - cantidad_elementos.begin( ));
   std::pair<int, int> pos_elemento_unico = busca_elemento(cuadrado, elemento_unico, tam, reduccion);
   std::cerr << "Elem_unico: " << elemento_unico << "\n";
   std::cerr << "Pos elem: " << pos_elemento_unico.first << " " << pos_elemento_unico.second << "\n\n";
   
   int anterior = acumulado[pos_elemento_unico.first].first;
   std::cerr << "Ant: " << anterior << "\n";
   std::vector<std::pair<int, int>> filas = {{ pos_elemento_unico.first, anterior - 1 }};
   
   anterior += 1;
   for (int i = 0; i < tam; ++i) {
      if (acumulado[i].first != 0 && acumulado[i].second != pos_elemento_unico.first) {
         anterior += acumulado[i].first;
         filas.push_back({ i, anterior - 1 });
         std::cerr << "Ant: " << anterior << "\n";
      }
   }
   intercambia_filas(cuadrado, filas, tam, reduccion);
   intercambia_columnas(cuadrado, filas, tam, reduccion, cantidad_elementos);
   arregla_acumulado(cuadrado, acumulado, cantidad_elementos, tam, reduccion);
}

int caso(base::matrix<int, 2>& cuadrado, int tam, int ini, std::vector<std::pair<int, int>>& acumulado, std::vector<int>& cantidad_elementos) {
   std::fill(cantidad_elementos.begin( ), cantidad_elementos.end( ), 0);
   std::partial_sum(acumulado.begin( ), acumulado.end( ), acumulado.begin( ), [](const auto& a, const auto& b) {
      return std::make_pair(a.first, a.second + 1);
   });
   
   int elementos = 0;
   int elementos_distintos = 0;
   for (int i = ini; i < tam; ++i) {
      for (int j = 0; j < tam - ini; ++j) {
         if (cuadrado[i][j] != 0) {
            elementos += 1;
            cantidad_elementos[cuadrado[i][j] - 1] += 1;
            acumulado[i].first += 1;
            elementos_distintos += (cantidad_elementos[cuadrado[i][j] - 1] == 1);
         }
      }
   }
   tam -= ini;
   return ((elementos >= tam || (elementos_distintos <= (tam / 2))) ? 0 : 1);
}

void trivial(base::matrix<int, 2>& cuadrado, std::vector<std::pair<int, int>>& acumulado, int tam, int reduccion) {
   int idx = 0;
   int elementos[2] = { };
   for (int i = tam - reduccion; i < tam; ++i) {
      for (int j = 0; j < tam - reduccion; ++j) {
         if (cuadrado[i][j] != 0) {
            if (idx == 0) {
               elementos[idx++] = cuadrado[i][j];
            } else if (idx == 1 && cuadrado[i][j] != elementos[idx - 1]) {
               elementos[idx] = cuadrado[i][j];
            }
         }
      }
   }
   cuadrado[tam - reduccion][0] = cuadrado[tam - reduccion + 1][1] = elementos[0];
   cuadrado[tam - reduccion][1] = cuadrado[tam - reduccion + 1][0] = elementos[1];
}

void teorema(base::matrix<int, 2>& cuadrado, std::vector<std::pair<int, int>>& acumulado, int tam, int reduccion, std::vector<int>& cantidad_elementos, std::vector<int>& omitir, std::vector<int>& elementos_unicos) {
   std::cerr << "Estoy en el teorema\n";
   if (tam - reduccion <= 2) {
      std::cerr << "entre al trivial\n";
      trivial(cuadrado, acumulado, tam, reduccion);
      arregla_acumulado(cuadrado, acumulado, cantidad_elementos, tam, reduccion);
      return;
   }

   permuta_filas_columnas(cuadrado, acumulado, tam, reduccion, cantidad_elementos);
   imprime(cuadrado, "Cuadrado Permutado:");

   int elemento_unico = 1 + (std::find(cantidad_elementos.begin( ), cantidad_elementos.end( ), 1) - cantidad_elementos.begin( ));
   std::pair<int, int> pos_elemento_unico = busca_elemento(cuadrado, elemento_unico, tam, reduccion);

   cuadrado[pos_elemento_unico.first][pos_elemento_unico.second] = 0;
   acumulado[pos_elemento_unico.first].first -= 1;
   cantidad_elementos[elemento_unico - 1] -= 1;
   elementos_unicos.push_back(elemento_unico);
   omitir.push_back(elemento_unico);

   int caso_act = caso(cuadrado, tam, reduccion + 1, acumulado, cantidad_elementos);
   std::cerr << "Llegue al caso " << caso_act << ":\n";
   if (caso_act == 1) {
      teorema(cuadrado, acumulado, tam, reduccion + 1, cantidad_elementos, omitir, elementos_unicos);
   }

   lemma_2(cuadrado, acumulado, tam, reduccion + 1, omitir);
   lemma_1(cuadrado, acumulado, tam, reduccion + 1, omitir);

   std::cerr << "elemento unico: " << elemento_unico << "\n";
   intercambia_diagonal(cuadrado, acumulado, elemento_unico, pos_elemento_unico, tam, reduccion + 1);
   arregla_acumulado(cuadrado, acumulado, cantidad_elementos, tam, reduccion);
   omitir.pop_back( );

   std::cerr << "antes\n";
   for (auto temporal : omitir) {
      std::cerr << temporal << "*\n";
   }
   lemma_1(cuadrado, acumulado, tam, reduccion, omitir);
   std::cerr << "despues\n";
}

void resuelve(base::matrix<int, 2>& cuadrado, int tam, std::vector<std::pair<int, int>>& acumulado, std::vector<int>& cantidad_elementos, int caso, std::vector<int>& elementos_omitidos, std::vector<int>& elementos_unicos) {
   switch (caso) {
   case 0:
      std::cerr << "Entre a lemma 2\n";
      lemma_2(cuadrado, acumulado, tam, 0, elementos_omitidos);

      std::cerr << "Entre a lemma 1\n";
      lemma_1(cuadrado, acumulado, tam, 0, elementos_omitidos);
      std::cerr << "Sali a lemma 1\n";
      break;
   default:
      teorema(cuadrado, acumulado, tam, 0, cantidad_elementos, elementos_omitidos, elementos_unicos);
      lemma_1(cuadrado, acumulado, tam, 0, elementos_omitidos);
      break;
   }
}

int main( ) {
   int tam, n;
   std::cin >> tam >> n;
   base::matrix<int, 2> cuadrado(tam, tam);

   std::vector<int> elementos_unicos;
   std::vector<int> elementos_omitidos;
   std::vector<int> cantidad_elementos(tam);
   std::vector<std::pair<int, int>> acumulado(tam);
   for (int i = 0; i < n; ++i) {
      int r, c, e;
      std::cin >> r >> c >> e;
      cuadrado[r - 1][c - 1] = e;
   }

   imprime(cuadrado, "Cuadro Inicial:");
   int caso_act = caso(cuadrado, tam, 0, acumulado, cantidad_elementos);
   resuelve(cuadrado, tam, acumulado, cantidad_elementos, caso_act, elementos_omitidos, elementos_unicos);
   imprime(cuadrado, "Cuadrado Resuelto:");
   
   //teorema(cuadrado, acumulado, tam, cantidad_elementos);
   //imprime(cuadrado, "Cuadro Permutado:");
   
   //lemma_2(cuadrado, acumulado, tam, 2, elementos_omitidos);
   //imprime(cuadrado, "Despues del lemma 2:");

   //lemma_1(cuadrado, acumulado, tam, 2, elementos_omitidos);
   //imprime(cuadrado, "Despues del lemma 1:");
}

