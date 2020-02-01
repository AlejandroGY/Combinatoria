class Graph:
   def __init__(self, graph):
      self.graph = graph
      self.ppl = len(graph)
      self.jobs = len(graph[0])

   def bpm(self, u, matchR, seen):
      for v in range(self.jobs):
         if self.graph[u][v] and seen[v] == False:
            seen[v] = True
            if matchR[v] == -1 or self.bpm(matchR[v], matchR, seen):
               matchR[v] = u
               return True
      return False

   def maxBPM(self):
      matchR = [-1] * self.jobs
      result = 0
      for i in range(self.ppl):
         seen = [False] * self.jobs
         if self.bpm(i, matchR, seen):
            result += 1
      return result, matchR

if __name__ == "__main__":
   size_square = int(input( ))
   number_elements = int(input( ))

   accountant = [[0, i] for i in range(0, size_square)]
   latin_square = [[0] * (size_square) for i in range(0, size_square)]
   for i in range(0, number_elements):
      r, c, e = map(int, input( ).split( ))
      latin_square[r - 1][c - 1] = e
      accountant[r - 1][0] += 1

   accountant.sort(reverse = True)
   print(accountant)
   
   print("\nCuadrado Inicial:")
   for i in range(0, size_square):
      print(latin_square[i])

   for t in accountant:
      if t[0] != 0:
         bpGraph = [[1] * (size_square) for i in range(0, size_square)]
         for j in range(0, size_square):
            if latin_square[t[1]][j] != 0:
               for i in range(0, size_square):
                  bpGraph[i][j] = 0
               for i in range(0, size_square):
                  bpGraph[latin_square[t[1]][j] - 1][i] = 0

         graph = Graph(bpGraph)
         matching = graph.maxBPM( )
         
         print(bpGraph)
         print(matching)
         for i in range(0, size_square):
            if matching[1][i] != -1:
               latin_square[t[1]][i] = matching[1][i] + 1

   print("\nCuadrado Resuelto:")
   for i in range(0, size_square):
      print(latin_square[i])
