class Edge(object):

    def __init__(self, start, end, weight=None, double_ended=False, capacity=None, price=None, flow=None):
        self.start = start
        self.end = end
        self.weight = weight
        self.double_ended = double_ended
        self.price = price
        self.capacity = capacity
        self.flow = flow

    def equals(self, other):
        return other.start == self.start and other.end == self.end


class Vertex(object):

    def __init__(self, number=None, potential=None):
        self.number = number
        self.potential = potential


class Graph(object):

        def __init__(self, vertex_count, edges):
            self.edges = sorted(edges, lambda x, y: x.start > y.start)
            self.vertex_count = vertex_count

        def get(self, i, j):
            if i == j:
                return None

            edges = filter(lambda x: x.start == i and x.end == j, self.edges)

            if not edges:
                edges = filter(lambda x: x.double_ended and x.end == i and x.start == j, self.edges)

            return edges[0] if edges else None

        def get_entering_edge(self, vertex):
            return filter(lambda x: x.end == vertex, self.edges)

        def get_leaving_edge(self, vertex):
            return filter(lambda x: x.start == vertex, self.edges)

        def get_cycle(self):
            edges = self.get_edges_copy()
            vertex_array = self.get_vertex_array(edges)

            while len(filter(lambda v: v.potential == 1, vertex_array)) != 0:
                vertex = filter(lambda v: v.potential == 1, vertex_array)[0]
                edge = filter(lambda e: e.start == vertex.number or e.end == vertex.number, edges)[0]
                edges.remove(edge)

                vertex_array = self.get_vertex_array(edges)
            return edges

        def get_vertex_array(self, edges):
            result = []
            for x in xrange(0, self.vertex_count):
                vertex = Vertex(x, len(filter(lambda e: e.start == x or e.end == x, edges)))
                result.append(vertex)

            return result

        def get_edges_copy(self):
            return [Edge(start=x.start, end=x.end, capacity=x.capacity, price=x.price) for x in self.edges]
