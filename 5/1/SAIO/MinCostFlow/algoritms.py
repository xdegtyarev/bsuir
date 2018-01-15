from collections import deque
from graph import Edge, Vertex, Graph


class MinimalCostFlow(object):

    def __init__(self, graph):
        self.graph = graph

    def calculate(self, base_edges):
        self.iterations = 0
        while True:
            self.iterations += 1

            #calc potentials
            edges_queue = self.get_base_edges_queue(base_edges)

            initial_edge = edges_queue.popleft()
            initial_vertex = Vertex(number=initial_edge.start, potential=0)

            linked_vertex = Vertex(number=initial_edge.end, potential=-initial_edge.price)
            vertexes = [initial_vertex, linked_vertex]

            while len(edges_queue) != 0:
                edge = edges_queue.popleft()
                if filter(lambda x: x.number == edge.start, vertexes):
                    potential = filter(lambda x: x.number == edge.start, vertexes)[0].potential
                    vertexes.append(Vertex(number=edge.end, potential=potential - edge.price))
                elif filter(lambda x: x.number == edge.end, vertexes):
                    potential = filter(lambda x: x.number == edge.end, vertexes)[0].potential
                    vertexes.append(Vertex(number=edge.start, potential=potential + edge.price))
                else:
                    edges_queue.append(edge)

            non_base_edges = filter(lambda x: not filter(lambda y: y.start == x.start and y.end == x.end, base_edges),self.graph.edges)
            delta_edge = filter(lambda x: self.get_delta(vertexes, x) > 0, non_base_edges)

            if not delta_edge:
                return base_edges

            delta_edge = delta_edge[0]

            base_edges.append(delta_edge)

            #get cycle
            temp_graph = Graph(self.graph.vertex_count, base_edges)
            cycle = temp_graph.get_cycle()

            forward_going_edges = self.get_forward_going_edge(cycle, delta_edge)
            backward_going_edges = cycle

            if len(backward_going_edges) == 0:
                raise Exception("Flow isn't bounded from bottom")

            delta = sorted(backward_going_edges, cmp=lambda x, y: x.capacity < y.capacity)[0].capacity

            replacement_edge = filter(lambda x: x.capacity == delta, backward_going_edges)[0]
            print('replacing {} -> {} with {} -> {}'.format(replacement_edge.start, replacement_edge.end,delta_edge.start,delta_edge.end))
            for item in forward_going_edges:
                base_edge = filter(lambda x: x.equals(item), base_edges)[0]
                base_edge.capacity += delta

            for item in backward_going_edges:
                base_edge = filter(lambda x: x.equals(item), base_edges)[0]
                base_edge.capacity -= delta

            base_edges.remove(filter(lambda x: x.equals(replacement_edge), base_edges)[0])

    def get_delta(self, vertexes, edge):
        start_vertex = filter(lambda v: v.number == edge.start, vertexes)[0]
        end_vertex = filter(lambda v: v.number == edge.end, vertexes)[0]
        return start_vertex.potential - end_vertex.potential - edge.price

    def get_base_edges_queue(self, base_edges):
        edges = deque()
        for item in base_edges:
            edges.append(Edge(start=item.start, end=item.end, capacity=item.capacity, price=item.price))
        return edges

    def get_forward_going_edge(self, cycle, delta_edge):
        result = [delta_edge]
        backward = []
        current_edge = delta_edge

        remove_item = filter(lambda x: x.start == delta_edge.start and x.end == delta_edge.end, cycle)
        if remove_item:
            remove_item = remove_item[0]
            cycle.remove(remove_item)

            while cycle:
                item = filter(lambda x: x.start == current_edge.end or x.end == current_edge.start, cycle)

                if not item:
                    item = filter(lambda x: x.start == current_edge.start or x.end == current_edge.end, cycle)[0]
                    backward.append(item)
                    cycle.remove(item)
                    current_edge = item
                    temp = result
                    result = backward
                    backward = temp
                else:
                    item = item[0]
                    result.append(item)
                    cycle.remove(item)
                    current_edge = item

            if filter(lambda x: x.equals(delta_edge), result):
                for item in backward:
                    cycle.append(item)
            else:
                for item in result:
                    cycle.append(item)
                result = backward

            return result
