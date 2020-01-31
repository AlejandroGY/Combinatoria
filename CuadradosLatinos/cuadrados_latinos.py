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

   latin_square = [[0] * (size_square) for i in range(0, size_square)]
   for i in range(0, 2 * size_square):
      r, c, e = map(int, input( ).split( ))
      latin_square[r - 1][c - 1] = e
   print(latin_square)

   for t in range(2, size_square):
      bpGraph = [[1] * (size_square) for i in range(0, size_square)]
      for j in range(0, size_square):
         for i in range(0, size_square):
            if latin_square[i][j] != 0:
               bpGraph[latin_square[i][j] - 1][j] = 0

      graph = Graph(bpGraph)
      matching = graph.maxBPM( )
      print(matching)
      for i in range(0, size_square):
         latin_square[t][i] = matching[1][i] + 1

   print()
   for i in range(0, size_square):
      print(latin_square[i])
