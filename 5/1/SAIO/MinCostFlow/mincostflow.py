from algoritms import MinimalCostFlow
from graph import Graph, Edge

def main():
    graph = Graph(10, [
        Edge(1, 2, price=9, capacity=2),
        Edge(1, 8, price=5, capacity=7),
        Edge(2, 3, price=1, capacity=4),
        Edge(2, 6, price=3, capacity=0),
        Edge(2, 7, price=5, capacity=3),
        Edge(3, 9, price=-2, capacity=0),
        Edge(4, 3, price=-3, capacity=0),
        Edge(5, 4, price=6, capacity=3),
        Edge(6, 5, price=8, capacity=4),
        Edge(7, 3, price=-1, capacity=0),
        Edge(7, 4, price=4, capacity=0),
        Edge(7, 5, price=7, capacity=5),
        Edge(7, 9, price=1, capacity=0),
        Edge(8, 7, price=2, capacity=0),
        Edge(8, 9, price=2, capacity=0),
        Edge(9, 6, price=6, capacity=2),
        ])

    base_edges = [
        Edge(1, 2, price=9, capacity=2),
        Edge(1, 8, price=5, capacity=7),
        Edge(2, 3, price=1, capacity=4),
        Edge(2, 7, price=5, capacity=3),
        Edge(5, 4, price=6, capacity=3),
        Edge(6, 5, price=8, capacity=4),
        Edge(7, 5, price=7, capacity=5),
        Edge(9, 6, price=7, capacity=2)
        ]

    calculator = MinimalCostFlow(graph)
    edges = calculator.calculate(base_edges)
    sumprice = 0;

    for edge in edges:
        print('{} -> {} = {}'.format(edge.start, edge.end, edge.price))
        sumprice += edge.price

    print('\nSumprice: {}'.format(sumprice))

if __name__ == '__main__':
    main()
