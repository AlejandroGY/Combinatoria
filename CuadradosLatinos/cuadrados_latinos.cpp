#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>

template<typename T>
struct vector2d : public std::vector<std::vector<T>> {
   vector2d(int n)
   : std::vector<std::vector<T>>(n, std::vector<T>(n)) {
   }
	vector2d(int n, int val)
   : std::vector<std::vector<T>>(n, std::vector<T>(n, val)) {
   }
};

bool bpm(vector2d<bool>& grafo, int u, bool visto[], int acoplamiento_[], int n) {
	for (int v = 0; v < n; v++) {
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

int maxBPM(vector2d<bool>& grafo, int n) {
	int acoplamiento_[n];
   std::fill(acoplamiento_, acoplamiento_ + n, -1);

	int result = 0;
	for (int u = 0; u < n; u++) {
		bool visto[n];
		std::fill(visto, visto + n, false);
		if (bpm(grafo, u, visto, acoplamiento_, n)) {
         result++;
      }
	}
	return result;
}

void imprime(vector2d<int>& cuadrado, int tam) {
   std::cout << "Temp:\n";
	for (int i = 0; i < tam; ++i) {
		for (int j = 0; j < tam; ++j) {
			std::cout << cuadrado[i][j] << " ";
		}
		std::cout << "\n";
	}
}

def lemma_1(cuadrado, acumulado, tam):
   for actual in acumulado:
      print(actual)
      if actual[0] == 0:
         adyacencia = [[1] * (tam) for i in range(0, tam)]
         for j in range(0, tam):
            for i in range(0, tam):
               if cuadrado[i][j] != 0:
                  adyacencia[cuadrado[i][j] - 1][j] = 0
         
         #imprime(adyacencia, tam)
         grafo = Grafo(adyacencia)
         acoplamiento = grafo.maxBPM( )
         
         for i in range(0, tam):
            cuadrado[actual[1]][i] = acoplamiento[1][i] + 1
   return cuadrado

void lemma_2(vector2d<int>& cuadrado, std::vector<std::pair<int, int>>& acumulado, int tam) {
   int index = 0
   for (auto actual : acumulado) {
		vector2d<bool> adyacencia(tam);
      std::fill()
      if (actual[0] != 0) {
         for (int j = 0; j < tam; ++j) {
            if (cuadrado[actual[1]][j] != 0) {
               for (int i = 0; i < tam; ++i) {
						adyacencia[i][j] = 0;
					}
               for (int i = 0; i < tam; ++i) {
						adyacencia[cuadrado[actual[1]][j] - 1][i] = 0;
					}
				} else {
               for (int i = 0; i < tam; ++i) {
                  if (cuadrado[i][j] != 0) {
							adyacencia[cuadrado[i][j] - 1][j] = 0;
						}
					}
				}
			}

         grafo = Grafo(adyacencia)
         acoplamiento = grafo.maxBPM( )
         
         #imprime(adyacencia, tam)
         #print(acoplamiento)
         for i in range(0, tam):
            if acoplamiento[1][i] != -1:
               cuadrado[actual[1]][i] = acoplamiento[1][i] + 1
         #imprime(cuadrado, tam)
         acumulado_temp[index][0] = tam;
		}
      index += 1;
	}
}

def permutar_filas_columnas(cuadrado, acumulado, tam, pos_elemento_unico):
   anterior = 0
   for i in range(0, tam):
      if acumulado[i][1] == pos_elemento_unico:
         anterior = acumulado[i][0]
         break
   columnas_finales = [anterior]

   anterior += 1
   for i in range(0, tam):
      if acumulado[i][1] != pos_elemento_unico and acumulado[i][0] != 0:
         columnas_finales.append(anterior + acumulado[i][0])
         anterior += acumulado[i][0]

   print("Columnas")
   print(columnas_finales)
   return cuadrado

void teorema(vector2d<int>& cuadrado, std::vector<std::pair<int, int>>& acumulado, int tam) {

}

int main( ) {
	int tam, n;
	std::cin >> tam >> n;

	vector2d<int> cuadrado(tam);
	std::vector<std::pair<int, int>> acumulado(tam);
	for (int i = 0; i < tam; ++i) {
		acumulado[i].second = i;
	}

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

	int pos_elemento_unico = std::find(elementos_vistos.begin( ), elementos_vistos.end( ), 1) - elementos_vistos.begin( ) + 1;
	std::sort(acumulado.begin( ), acumulado.end( ), std::greater<std::pair<int, int>>( ));

	for (int i = 0; i < tam; ++i) {
		std::cout << elementos_vistos[i] << " ";
	}
	std::cout << "\nPos: " << pos_elemento_unico << "\n";

}
