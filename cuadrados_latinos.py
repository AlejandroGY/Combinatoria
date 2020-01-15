class Graph:
	def __init__(self,graph):
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

	def maxBPM(self, n):
		matchR = [-1] * self.jobs
		result = 0
		for i in range(self.ppl):
			seen = [False] * self.jobs
			if self.bpm(i, matchR, seen):
				result += 1
		return result, matchR

if __name__ == "__main__":
   size_square = int(input( ))
   latin_square = []
   for i in range(0, size_square + 1):
      latin_square.append([0] * (size_square + 1))

   for i in range(0, size_square + 1):
      for j in range(0, size_square + 1):
         latin_square[i][j] = int(input( ))

   for i in range(0, 2 * size_square):
      r = int(input( ))
      c = int(input( ))
      e = int(input( ))


   bpGraph = [
      [0, 1, 1, 0, 0, 0],
      [1, 0, 0, 1, 0, 0],
      [0, 0, 1, 0, 0, 0],
      [0, 0, 1, 1, 0, 0],
      [0, 0, 0, 0, 0, 1],
      [0, 0, 0, 0, 1, 1]
   ]

   g = Graph(bpGraph)
   res = g.maxBPM(6)
   print(res[0])
   print(res[1])
   
