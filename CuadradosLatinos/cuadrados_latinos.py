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

def imprime(cuadrado, tam):
   print("Temp:")
   for i in range(0, tam):
      print(cuadrado[i])

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

def lemma_2(cuadrado, acumulado, tam):
   index = 0
   acumulado_temp = acumulado
   for actual in acumulado:
      adyacencia = [[1] * (tam) for i in range(0, tam)]
      if actual[0] != 0:
         for j in range(0, tam):
            if cuadrado[actual[1]][j] != 0:
               for i in range(0, tam):
                  adyacencia[i][j] = 0
               for i in range(0, tam):
                  adyacencia[cuadrado[actual[1]][j] - 1][i] = 0
            else:
               for i in range(0, tam):
                  if cuadrado[i][j] != 0:
                     adyacencia[cuadrado[i][j] - 1][j] = 0

         grafo = Grafo(adyacencia)
         acoplamiento = grafo.maxBPM( )
         
         #imprime(adyacencia, tam)
         #print(acoplamiento)
         for i in range(0, tam):
            if acoplamiento[1][i] != -1:
               cuadrado[actual[1]][i] = acoplamiento[1][i] + 1
         #imprime(cuadrado, tam)
         acumulado_temp[index][0] = tam
      index += 1
   return cuadrado, acumulado_temp

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

def teorema(cuadrado, acumulado, tam):
   pass

if __name__ == "__main__":
   tam, n = map(int, input( ).split( ))
   acumulado = [[0, i] for i in range(0, tam)]
   cuadrado = [[0] * (tam) for i in range(0, tam)]

   filas_llenas = 0
   numero_elementos = 0
   elementos_distintos = 0

   filas_vistas = [False for i in range(0, tam)]
   elementos_vistos = [0 for i in range(0, tam)]
   for i in range(0, n):
      r, c, e = map(int, input( ).split( ))
      cuadrado[r - 1][c - 1] = e
      acumulado[r - 1][0] += 1

      if e != 0:
         numero_elementos += 1
         elementos_vistos[e - 1] += 1
      if filas_vistas[r - 1] == False:
         filas_llenas += 1
         filas_vistas[r - 1] = True

   pos_elemento_unico = 0
   for i in range(0, tam):
      if elementos_vistos[i] == 1:
         pos_elemento_unico = i + 1
         break

   acumulado.sort(reverse = True)
   print(acumulado)
   
   print("\nCuadrado Inicial:")
   for i in range(0, tam):
      print(cuadrado[i])

   #cuadrado_latino, acumulado = lemma_2(cuadrado, acumulado, tam)
   #print(acumulado)
   #cuadrado_latino = lemma_1(cuadrado_latino, acumulado, tam)

   print("Pos elemento unico:")
   print(pos_elemento_unico)
   cuadrado_latino = permutar_filas_columnas(cuadrado, acumulado, tam, pos_elemento_unico)

   print("\nCuadrado Resuelto:")
   for i in range(0, tam):
      print(cuadrado_latino[i])
