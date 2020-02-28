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

std::pair<int, int> busca_elemento(base::matrix<int, 2>& cuadrado, int elemento, int tam) {
   for (int i = 0; i < tam; ++i) {
      for (int j = 0; j < tam; ++j) {
         if (cuadrado[i][j] == elemento) {
            return { i, j };
         }
      }
   }
   return { -1, -1 };
}

template<typename T>
void copia_cuadrado(base::matrix<T, 2>& fuente, base::matrix<T, 2>& destino, int tam) {
   for (int i = 0; i < tam; ++i) {
      for (int j = 0; j < tam; ++j) {
         destino[i][j] = fuente[i][j];
      }
   }
}

void ajusta_adyacencia(base::matrix<int, 2>& cuadrado, base::matrix<bool, 2>& adyacencia, int tam, int tope, std::vector<int>& omitir) {
   for (const auto& actual : omitir) {
      for (int j = 0; j < tam; ++j) {
         adyacencia[actual - 1][j] = 0;
      }
   }
   for (int i = 0; i < tope; ++i) {
      for (int j = 0; j < tam; ++j) {
         if (cuadrado[i][j] != 0) {
            adyacencia[cuadrado[i][j] - 1][j] = 0;
         }
      }
   }
   for (int i = 0; i < tam; ++i) {
      for (int j = tam - tope; j < tam; ++j) {
         adyacencia[i][j] = 0;
      }
   }
}

void lemma_1(base::matrix<int, 2>& cuadrado, std::vector<std::pair<int, int>>& acumulado, int tam, int tope, std::vector<int>& omitir) {
   int index = 0;
   for (auto& actual : acumulado) {
      if (actual.first == 0 && actual.second >= tope) {
         base::matrix<bool, 2> adyacencia(tam, tam, 1);
         for (int j = 0; j < tam - tope; ++j) {
            for (int i = tope; i < tam; ++i) {
               if (cuadrado[i][j] != 0) {
                  adyacencia[cuadrado[i][j] - 1][j] = 0;
               }
            }
         }

         if (tope != 0) {
            ajusta_adyacencia(cuadrado, adyacencia, tam, tope, omitir);
         }
         imprime(adyacencia, "Matriz adyacencia1:");

         std::vector<int> acoplamiento(tam, -1);
         int tam_acoplamiento = maxBPM(adyacencia, acoplamiento, tam);
         for (int i = 0; i < tam; ++i) {
            cuadrado[actual.second][i] = acoplamiento[i] + 1;
         }
         acumulado[index].first = tam - tope;
      }
      index += 1;
   }
}

void lemma_2(base::matrix<int, 2>& cuadrado, std::vector<std::pair<int, int>>& acumulado, int tam, int tope, std::vector<int>& omitir) {
   int index = 0;
   for (auto& actual : acumulado) {
      base::matrix<bool, 2> adyacencia(tam, tam, 1);
      if (actual.first != 0 && actual.second >= tope) {
         for (int j = 0; j < tam - tope; ++j) {
            if (cuadrado[actual.second][j] != 0) {
               for (int i = 0; i < tam; ++i) {
                  adyacencia[i][j] = 0;
               }
               for (int i = 0; i < tam; ++i) {
                  adyacencia[cuadrado[actual.second][j] - 1][i] = 0;
               }
            } else {
               for (int i = tope; i < tam; ++i) {
                  if (cuadrado[i][j] != 0) {
                     adyacencia[cuadrado[i][j] - 1][j] = 0;
                  }
               }
            }
         }

         if (tope != 0) {
            ajusta_adyacencia(cuadrado, adyacencia, tam, tope, omitir);
         }
         imprime(adyacencia, "Matriz adyacencia2:");

         std::vector<int> acoplamiento(tam, -1);
         int tam_acoplamiento = maxBPM(adyacencia, acoplamiento, tam);
         for (int i = 0; i < tam; ++i) {
            if (acoplamiento[i] != -1) {
               cuadrado[actual.second][i] = acoplamiento[i] + 1;
            }
         }
         acumulado[index].first = tam - tope;
      }
      index += 1;
   }
}

void arregla_acumulado(base::matrix<int, 2>& cuadrado, std::vector<std::pair<int, int>>& acumulado, std::vector<int>& cantidad_elementos, int tam) {
   std::fill(cantidad_elementos.begin( ), cantidad_elementos.end( ), 0);
   std::partial_sum(acumulado.begin( ), acumulado.end( ), acumulado.begin( ), [](const auto& a, const auto& b) {
      return std::make_pair(a.first, a.second + 1);
   });

   int elementos = 0;
   int elementos_distintos = 0;
   for (int i = 0; i < tam; ++i) {
      for (int j = 0; j < tam; ++j) {
         if (cuadrado[i][j] != 0) {
            elementos += 1;
            cantidad_elementos[cuadrado[i][j] - 1] += 1;
            acumulado[i].first += 1;
            elementos_distintos += (cantidad_elementos[cuadrado[i][j] - 1] == 1);
         }
      }
   }
}

void intercambia_diagonal(base::matrix<int, 2>& cuadrado, std::vector<std::pair<int, int>>& acumulado, int elemento_unico, std::pair<int, int>& pos_elem_unico, int tam, int tope) {
   int ini = pos_elem_unico.first;
   std::vector<bool> visto(tam, false);
   for (int i = ini; i <= tam - tope; ++i) {
      acumulado[i].first += 1;
      cuadrado[i][tam - tope] = elemento_unico;
      std::swap(cuadrado[i][i], cuadrado[i][tam - tope]);
      std::cerr << i << "*\n";
      if (!visto[cuadrado[i][tam - tope] - 1]) {
         visto[cuadrado[i][tam - tope] - 1] = true;
      } else {
         int actual = cuadrado[i][tam - tope];
         std::vector<bool> fila_vista(tam, false); fila_vista[i] = true;
         while (true) {
            bool cambio = false;
            for (int k = tope; k <= i; ++k) {
               if (k != i && cuadrado[k][tam - tope] == actual && !fila_vista[k]) {
                  std::swap(cuadrado[k][i], cuadrado[k][tam - tope]);
                  fila_vista[k] = true;
                  actual = cuadrado[k][tam - tope];
                  cambio = true;
               }
            }
            if (cambio) {
               break;
            }
         }
      }
      imprime(cuadrado, "Intercambios:");
   }
}

void intercambia_filas(base::matrix<int, 2>& cuadrado, std::vector<std::pair<int, int>> filas, int tam) {
   std::reverse(filas.begin( ), filas.end( ));
   for (const auto& f : filas) {
      for (int i = 0; i < tam; ++i) {
         std::swap(cuadrado[f.first][i], cuadrado[f.second][i]);
      }
   }
}

void intercambia_columnas(base::matrix<int, 2>& cuadrado, int i, int j, int tam) {
   for (int k = 0; k < tam; ++k) {
      std::swap(cuadrado[k][i], cuadrado[k][j]);
   }
}

void intercambia_columnas(base::matrix<int, 2>& cuadrado, std::vector<std::pair<int, int>> filas, int tam, std::vector<int>& cantidad_elementos) {
   int elemento_unico = 1 + (std::find(cantidad_elementos.begin( ), cantidad_elementos.end( ), 1) - cantidad_elementos.begin( ));
   std::pair<int, int> pos_elemento_unico = busca_elemento(cuadrado, elemento_unico, tam);

   int siguiente = 0;
   for (int f = 0; f < filas.size( ); ++f) {
      for (int i = siguiente; i < tam; ++i) {
         if (cuadrado[filas[f].second][i] == elemento_unico) {
            intercambia_columnas(cuadrado, i, filas[0].second, tam);
         } else if (cuadrado[filas[f].second][i] != 0) {
            siguiente += (siguiente == filas[0].second);
            intercambia_columnas(cuadrado, i, siguiente++, tam);
         }
      }
   }
}

void permuta_filas_columnas(base::matrix<int, 2>& cuadrado, std::vector<std::pair<int, int>>& acumulado, int tam, std::vector<int>& cantidad_elementos) {
   std::cerr << "\nCantidad de elementos:\n";
   for (int i = 0; i < tam; ++i) {
      std::cerr << cantidad_elementos[i] << " ";
   }
   std::cerr << "\n";

   int elemento_unico = 1 + (std::find(cantidad_elementos.begin( ), cantidad_elementos.end( ), 1) - cantidad_elementos.begin( ));
   std::pair<int, int> pos_elemento_unico = busca_elemento(cuadrado, elemento_unico, tam);
   
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
   intercambia_filas(cuadrado, filas, tam);
   intercambia_columnas(cuadrado, filas, tam, cantidad_elementos);
   arregla_acumulado(cuadrado, acumulado, cantidad_elementos, tam);
}

void teorema(base::matrix<int, 2>& cuadrado, std::vector<std::pair<int, int>>& acumulado, int tam, int tope, std::vector<int>& cantidad_elementos, std::vector<int>& omitir) {
   permuta_filas_columnas(cuadrado, acumulado, tam, cantidad_elementos);
   imprime(cuadrado, "Cuadrado Permutado:");

   int elemento_unico = 1 + (std::find(cantidad_elementos.begin( ), cantidad_elementos.end( ), 1) - cantidad_elementos.begin( ));
   std::pair<int, int> pos_elemento_unico = busca_elemento(cuadrado, elemento_unico, tam);

   cuadrado[pos_elemento_unico.first][pos_elemento_unico.second] = 0;
   acumulado[pos_elemento_unico.first].first -= 1;
   cantidad_elementos[elemento_unico - 1] -= 1;
   omitir.push_back(elemento_unico);

   lemma_2(cuadrado, acumulado, tam, 1, omitir);
   lemma_1(cuadrado, acumulado, tam, 1, omitir);
   intercambia_diagonal(cuadrado, acumulado, elemento_unico, pos_elemento_unico, tam, 1);
   arregla_acumulado(cuadrado, acumulado, cantidad_elementos, tam);
   omitir.pop_back( );
   lemma_1(cuadrado, acumulado, tam, 0, omitir);
}

int caso(base::matrix<int, 2>& cuadrado, int tam, std::vector<std::pair<int, int>>& acumulado, std::vector<int>& cantidad_elementos) {
   std::fill(cantidad_elementos.begin( ), cantidad_elementos.end( ), 0);
   std::partial_sum(acumulado.begin( ), acumulado.end( ), acumulado.begin( ), [](const auto& a, const auto& b) {
      return std::make_pair(a.first, a.second + 1);
   });
   
   int elementos = 0;
   int elementos_distintos = 0;
   for (int i = 0; i < tam; ++i) {
      for (int j = 0; j < tam; ++j) {
         if (cuadrado[i][j] != 0) {
            elementos += 1;
            cantidad_elementos[cuadrado[i][j] - 1] += 1;
            acumulado[i].first += 1;
            elementos_distintos += (cantidad_elementos[cuadrado[i][j] - 1] == 1);
         }
      }
   }
   return ((elementos >= tam || (elementos_distintos <= (tam / 2))) ? 0 : 1);
}

void resuelve(base::matrix<int, 2>& cuadrado, int tam, std::vector<std::pair<int, int>>& acumulado, std::vector<int>& cantidad_elementos, int caso, std::vector<int>& elementos_omitidos) {
   switch (caso) {
   case 0:
      std::cerr << "Entre a lemma 2\n";
      lemma_2(cuadrado, acumulado, tam, 0, elementos_omitidos);

      std::cerr << "Entre a lemma 1\n";
      lemma_1(cuadrado, acumulado, tam, 0, elementos_omitidos);
      std::cerr << "Sali a lemma 1\n";
      break;
   default:
      teorema(cuadrado, acumulado, tam, 0, cantidad_elementos, elementos_omitidos);
      break;
   }
}

int main( ) {
   int tam, n;
   std::cin >> tam >> n;
   base::matrix<int, 2> cuadrado(tam, tam);

   std::vector<int> elementos_omitidos;
   std::vector<int> cantidad_elementos(tam);
   std::vector<std::pair<int, int>> acumulado(tam);
   for (int i = 0; i < n; ++i) {
      int r, c, e;
      std::cin >> r >> c >> e;
      cuadrado[r - 1][c - 1] = e;
   }

   imprime(cuadrado, "Cuadro Inicial:");
   int caso_act = caso(cuadrado, tam, acumulado, cantidad_elementos);
   resuelve(cuadrado, tam, acumulado, cantidad_elementos, caso_act, elementos_omitidos);
   imprime(cuadrado, "Cuadrado Resuelto:");
   
   //teorema(cuadrado, acumulado, tam, cantidad_elementos);
   //imprime(cuadrado, "Cuadro Permutado:");
   
   //lemma_2(cuadrado, acumulado, tam, 2, elementos_omitidos);
   //imprime(cuadrado, "Despues del lemma 2:");

   //lemma_1(cuadrado, acumulado, tam, 2, elementos_omitidos);
   //imprime(cuadrado, "Despues del lemma 1:");
}

