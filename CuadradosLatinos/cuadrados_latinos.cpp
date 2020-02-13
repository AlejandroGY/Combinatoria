#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <numeric>

template<typename T>
struct vector2d : public std::vector<std::vector<T>> {
   vector2d(int n)
   : std::vector<std::vector<T>>(n, std::vector<T>(n)) {
   }
	vector2d(int n, int val)
   : std::vector<std::vector<T>>(n, std::vector<T>(n, val)) {
   }
};

bool bpm(vector2d<bool>& grafo, int u, std::vector<bool>& visto, std::vector<int>& acoplamiento_, int n) {
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

int maxBPM(vector2d<bool>& grafo, std::vector<int>& acoplamiento_, int n) {
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
void imprime(const vector2d<T>& cuadrado) {
   std::cout << "Actual:\n";
	for (const auto& fila : cuadrado) {
		for (const auto& celda : fila) {
			std::cout << celda << " ";
		}
		std::cout << "\n";
	}
}

void lemma_1(vector2d<int>& cuadrado, std::vector<std::pair<int, int>>& acumulado, int tam) {
   int index = 0;
	for (auto& actual : acumulado) {
      if (actual.first == 0) {
			vector2d<bool> adyacencia(tam, 1);
         for (int j = 0; j < tam; ++j) {
            for (int i = 0; i < tam; ++i) {
               if (cuadrado[i][j] != 0) {
						adyacencia[cuadrado[i][j] - 1][j] = 0;
					}
				}
			}
         
			//imprime(adyacencia, tam);
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

void lemma_2(vector2d<int>& cuadrado, std::vector<std::pair<int, int>>& acumulado, int tam) {
   int index = 0;
   for (auto& actual : acumulado) {
		vector2d<bool> adyacencia(tam, 1);
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
         
         //imprime(adyacencia, tam);
         for (int i = 0; i < tam; ++i) {
            if (acoplamiento[i] != -1) {
					cuadrado[actual.second][i] = acoplamiento[i] + 1;
				}
			}
         //imprime(cuadrado, tam);
         acumulado[index].first = tam;
		}
      index += 1;
	}
}

void permutar_filas_columnas(vector2d<int>& cuadrado, std::vector<std::pair<int, int>>& acumulado, int tam, int pos_elemento_unico) {
	std::sort(acumulado.begin( ), acumulado.end( ));
	for (auto act : acumulado) {
		std::cout << act.first << " " << act.second << "+\n";
	}
	std::cout << "\n";
	auto it = (std::lower_bound(acumulado.begin( ), acumulado.end( ), std::make_pair(0, pos_elemento_unico), [](const auto& a, const auto& b) {
		return (a.second > b.second);
	}));
   
	int anterior = 1 + it->first;
	std::cerr << "Obtuve:" << it->second << "\n";
	std::vector<int> columnas_finales = { anterior - 1 };
   for (int i = 0; i < tam; ++i) {
		if (acumulado[i].second != it->second && acumulado[i].first != 0) {
			columnas_finales.push_back(anterior + acumulado[i].first);
         anterior += acumulado[i].first;
		}
	}
	for (auto act : columnas_finales) {
		std::cout << act << " ";
	}
	std::cout << "\n";
}

void teorema(vector2d<int>& cuadrado, std::vector<std::pair<int, int>>& acumulado, int tam) {

}

int main( ) {
	int tam, n;
	std::cin >> tam >> n;

	vector2d<int> cuadrado(tam);
	std::vector<std::pair<int, int>> acumulado(tam);
	std::partial_sum(acumulado.begin( ), acumulado.end( ), acumulado.begin( ), [](const auto& a, const auto& b) {
		return std::make_pair(a.first, a.second + 1);
	});

	for (auto act : acumulado) {
		std::cout << act.first << " " << act.second << "*\n";
	}
	std::cout << "\n";
/**/
	int filas_llenas = 0;
	int numero_elementos = 0, elementos_distintos = 0;
	std::vector<bool> filas_vistas(tam, false);
	std::vector<int> elementos_vistos(tam, 0);
	for (int i = 0; i < n; ++i) {
		int r, c, e;
		std::cin >> r >> c >> e;

		cuadrado[r - 1][c - 1] = e;
		acumulado[r - 1].first += 1;

		if (e != 0) {
			numero_elementos += 1;
			elementos_vistos[e - 1] += 1;
		}
		if (filas_vistas[r - 1] == false) {
			filas_llenas += 1;
			filas_vistas[r - 1] = true;
		}
	}

	int pos_elemento_unico = std::find(elementos_vistos.begin( ), elementos_vistos.end( ), 1) - elementos_vistos.begin( );
	std::sort(acumulado.begin( ), acumulado.end( ), std::greater<std::pair<int, int>>( ));

	for (int i = 0; i < tam; ++i) {
		//std::cout << elementos_vistos[i] << " ";
	}
	//std::cout << "\nPos: " << pos_elemento_unico << "\n";

	permutar_filas_columnas(cuadrado, acumulado, tam, pos_elemento_unico);

	//imprime(cuadrado);
	//lemma_2(cuadrado, acumulado, tam);
	//imprime(cuadrado);
	//lemma_1(cuadrado, acumulado, tam);
	//imprime(cuadrado);
/**/
}
