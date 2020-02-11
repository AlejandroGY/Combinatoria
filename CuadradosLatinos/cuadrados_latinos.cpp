#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>

#define M 6
#define N 6

template<typename T>
struct vector2d : public std::vector<std::vector<T>> {
   vector2d(int n)
   : std::vector<std::vector<T>>(n, std::vector<T>(n)) {
   }
};

bool bpm(bool grafo[M][N], int u, bool visto[], int acoplamiento_[]) {
	for (int v = 0; v < N; v++) {
		if (grafo[u][v] && !visto[v]) {
			visto[v] = true;
			if (acoplamiento_[v] < 0 || bpm(grafo, acoplamiento_[v], visto, acoplamiento_)) {
				acoplamiento_[v] = u;
				return true;
			}
		}
	}
	return false;
}

int maxBPM(bool grafo[M][N]) {
	int acoplamiento_[N];
   std::fill(acoplamiento_, acoplamiento_ + N, -1);

	int result = 0;
	for (int u = 0; u < M; u++) {
		bool visto[N];
		std::fill(visto, visto + N, false);
		if (bpm(grafo, u, visto, acoplamiento_)) {
         result++;
      }
	}
	return result;
}

int main( ) {
	
}

/*
#include <iostream>
#include <vector>

template<typename T>
struct vector2d : public std::vector<std::vector<T>> {
   vector2d(int n)
   : std::vector<std::vector<T>>(n, std::vector<T>(n)) {
   }
};

void f(vector2d<int>& por_ref, vector2d<int> por_val) {

}

int main( ) {
   vector2d<int> matriz(10);
   std::cout << matriz[0][1];
   f(matriz, matriz);
}


*/