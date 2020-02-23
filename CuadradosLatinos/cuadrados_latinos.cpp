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
void imprime(const base::matrix<T, 2>& cuadrado) {
   std::cout << "Actual:\n";
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

void lemma_1(base::matrix<int, 2>& cuadrado, std::vector<std::pair<int, int>>& acumulado, int tam) {
   int index = 0;
	for (auto& actual : acumulado) {
      if (actual.first == 0) {
			base::matrix<bool, 2> adyacencia(tam, tam, 1);
         for (int j = 0; j < tam; ++j) {
            for (int i = 0; i < tam; ++i) {
               if (cuadrado[i][j] != 0) {
						adyacencia[cuadrado[i][j] - 1][j] = 0;
					}
				}
			}
         
			std::vector<int> acoplamiento(tam, -1);
			int tam_acoplamiento = maxBPM(adyacencia, acoplamiento, tam);
         for (int i = 0; i < tam; ++i) {
				cuadrado[actual.second][i] = acoplamiento[i] + 1;
			}
			acumulado[index].first = tam;
		}
		index += 1;
	}
}

void lemma_2(base::matrix<int, 2>& cuadrado, std::vector<std::pair<int, int>>& acumulado, int tam) {
   int index = 0;
   for (auto& actual : acumulado) {
		base::matrix<bool, 2> adyacencia(tam, tam, 1);
      if (actual.first != 0) {
         for (int j = 0; j < tam; ++j) {
            if (cuadrado[actual.second][j] != 0) {
               for (int i = 0; i < tam; ++i) {
						adyacencia[i][j] = 0;
					}
               for (int i = 0; i < tam; ++i) {
						adyacencia[cuadrado[actual.second][j] - 1][i] = 0;
					}
				} else {
               for (int i = 0; i < tam; ++i) {
                  if (cuadrado[i][j] != 0) {
							adyacencia[cuadrado[i][j] - 1][j] = 0;
						}
					}
				}
			}

         std::vector<int> acoplamiento(tam, -1);
			int tam_acoplamiento = maxBPM(adyacencia, acoplamiento, tam);
         for (int i = 0; i < tam; ++i) {
            if (acoplamiento[i] != -1) {
					cuadrado[actual.second][i] = acoplamiento[i] + 1;
				}
			}
         acumulado[index].first = tam;
		}
      index += 1;
	}
}

void permuta_filas(base::matrix<int, 2>& cuadrado, std::vector<std::pair<int, int>> filas, int tam) {
	std::reverse(filas.begin( ), filas.end( ));
	for (auto f : filas) {
		for (int i = 0; i < tam; ++i) {
			std::swap(cuadrado[f.first][i], cuadrado[f.second][i]);
		}
	}
}

void permuta_columnas(base::matrix<int, 2>& cuadrado, std::vector<std::pair<int, int>> filas, int tam, std::vector<int>& elementos_vistos) {
	int elemento_unico = 1 + (std::find(elementos_vistos.begin( ), elementos_vistos.end( ), 1) - elementos_vistos.begin( ));
	std::pair<int, int> pos_elemento_unico = busca_elemento(cuadrado, elemento_unico, tam);
	std::cout << "Pos_elem:" << pos_elemento_unico.first << " - " << pos_elemento_unico.second << "\n";

	int siguiente = 0;
	std::vector<bool> fila_ocupada(tam, false);
	for (int i = 0; i < tam; ++i) {

	}
}

void permutar_filas_columnas(base::matrix<int, 2>& cuadrado, std::vector<std::pair<int, int>>& acumulado, int tam, std::vector<int>& elementos_vistos) {   
	int elemento_unico = 1 + (std::find(elementos_vistos.begin( ), elementos_vistos.end( ), 1) - elementos_vistos.begin( ));
	std::pair<int, int> pos_elemento_unico = busca_elemento(cuadrado, elemento_unico, tam);
	
	int anterior = acumulado[pos_elemento_unico.first].first;
	std::vector<std::pair<int, int>> filas = {{ pos_elemento_unico.first, anterior - 1 }};
   for (int i = 0; i < tam; ++i) {
		if (acumulado[i].first != 0 && acumulado[i].second != pos_elemento_unico.first) {
			filas.push_back({ i, anterior + acumulado[i].first - 1 });
         anterior += acumulado[i].first;
		}
	}
	for (int i = 0; i < filas.size( ); ++i) {
		std::cout << filas[i].first << " va a " << filas[i].second << "\n";
	}
	
	permuta_filas(cuadrado, filas, tam);
	permuta_columnas(cuadrado, filas, tam, elementos_vistos);
}

void teorema(base::matrix<int, 2>& cuadrado, std::vector<std::pair<int, int>>& acumulado, int tam, std::vector<int>& elementos_vistos) {
	permutar_filas_columnas(cuadrado, acumulado, tam, elementos_vistos);
}

int main( ) {
	int tam, n;
	std::cin >> tam >> n;

	base::matrix<int, 2> cuadrado(tam, tam);
	std::vector<std::pair<int, int>> acumulado(tam);
	std::partial_sum(acumulado.begin( ), acumulado.end( ), acumulado.begin( ), [](const auto& a, const auto& b) {
		return std::make_pair(a.first, a.second + 1);
	});

	int filas_llenas = 0;
	int numero_elementos = 0, elementos_distintos = 0;
	std::vector<bool> columnas_vistas(tam, false);
	std::vector<int> elementos_vistos(tam, 0);
	for (int i = 0; i < n; ++i) {
		int r, c, e;
		std::cin >> r >> c >> e;

		cuadrado[r - 1][c - 1] = e;
		acumulado[r - 1].first += 1;
		elementos_vistos[e - 1] += 1;

		if (elementos_vistos[e - 1] <= 1) {
			numero_elementos += 1;
		}
		if (columnas_vistas[r - 1] == false) {
			filas_llenas += 1;
			columnas_vistas[r - 1] = true;
		}
	}

	imprime(cuadrado);
	teorema(cuadrado, acumulado, tam, elementos_vistos);
	imprime(cuadrado);

	//imprime(cuadrado);
	//lemma_2(cuadrado, acumulado, tam);
	//imprime(cuadrado);
	//lemma_1(cuadrado, acumulado, tam);
	//imprime(cuadrado);
}
