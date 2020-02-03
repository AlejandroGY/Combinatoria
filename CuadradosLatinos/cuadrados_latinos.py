class Grafo:
   def __init__(self, grafo):
      self.grafo = grafo
      self.casillas = len(grafo)
      self.numeros = len(grafo[0])

   def bpm(self, u, acoplamiento_, visto):
      for v in range(self.numeros):
         if self.grafo[u][v] and visto[v] == False:
            visto[v] = True
            if acoplamiento_[v] == -1 or self.bpm(acoplamiento_[v], acoplamiento_, visto):
               acoplamiento_[v] = u
               return True
      return False

   def maxBPM(self):
      acoplamiento_ = [-1] * self.numeros
      resultado = 0
      for i in range(self.casillas):
         visto = [False] * self.numeros
         if self.bpm(i, acoplamiento_, visto):
            resultado += 1
      return resultado, acoplamiento_

def resuelve_1(matriz, acumulado, tam):
   for actual in acumulado:
      adyacencia = [[1] * (tam) for i in range(0, tam)]
      if actual[0] != 0:
         for j in range(0, tam):
            if matriz[actual[1]][j] != 0:
               for i in range(0, tam):
                  adyacencia[i][j] = 0
               for i in range(0, tam):
                  adyacencia[matriz[actual[1]][j] - 1][i] = 0
      else:
         for j in range(0, tam):
            for i in range(0, tam):
               if matriz[i][j] != 0:
                  adyacencia[matriz[i][j] - 1][j] = 0

      grafo = Grafo(adyacencia)
      acoplamiento = grafo.maxBPM( )
      
      print()
      print(adyacencia)
      print(acoplamiento)
      for i in range(0, tam):
         if acoplamiento[1][i] != -1:
            matriz[actual[1]][i] = acoplamiento[1][i] + 1
   return matriz

if __name__ == "__main__":
   tam, n = map(int, input( ).split( ))
   acumulado = [[0, i] for i in range(0, tam)]
   matriz = [[0] * (tam) for i in range(0, tam)]
   for i in range(0, n):
      r, c, e = map(int, input( ).split( ))
      matriz[r - 1][c - 1] = e
      acumulado[r - 1][0] += 1

   acumulado.sort(reverse = True)
   print(acumulado)
   
   print("\nCuadrado Inicial:")
   for i in range(0, tam):
      print(matriz[i])

   cuadrado_latino = resuelve_1(matriz, acumulado, tam)

   print("\nCuadrado Resuelto:")
   for i in range(0, tam):
      print(cuadrado_latino[i])
