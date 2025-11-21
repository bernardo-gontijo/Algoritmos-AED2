#include <iostream>
#include <list>
#include <stdexcept>
#include <string>


typedef unsigned int uint;
typedef unsigned int Vertex;


class GraphAL {

private:
	uint num_vertices;
	uint num_edges;
	std::list<Vertex> *adj;	// vetor dinamicamente alocado

public:
	GraphAL(): num_vertices(0), num_edges(0) {}
	GraphAL(uint num_vertices):num_vertices(num_vertices), num_edges(0) {
		adj = new std::list<Vertex> [num_vertices]; // o vetor adj vai ser uma lista do tipo uint de tamanho igual a num_vertices
	}

	void add_edge(Vertex u, Vertex v) {
		if(u >= num_vertices || v >= num_vertices){
			throw std::invalid_argument("Vertice(s) invalido(s).");
		} else if(u==v) {
			throw std::logic_error("Erro de logica.");
		}
		adj[u].push_back(v);
		adj[v].push_back(u);
		++num_edges;
	}

	std::list<Vertex> get_adj(Vertex const& u) {
		if(u >= num_vertices) {
			throw std::invalid_argument("Parametro invalido.");
		}
		return adj[u];
	}

	uint get_num_vertices() {
		return num_vertices;	// retorna a ordem do grafo
	}

    uint get_num_edges(){
        return num_edges;
    }

	~GraphAL() {
	delete[] adj;
	adj = nullptr;
	}
};

void print_adjacency_list(GraphAL& g) {
	
	for(uint u = 0; u < g.get_num_vertices(); u++) {	// para cada u, retorna a lista de vertices ligados a u	
		std::list<Vertex> adj_u = g.get_adj(u);
        std::cout << u << ": ";
		for(auto const& it : adj_u) {	// para cada u, imprime uma lista de vertices ligados a u
			std::cout << it << ", ";
		}		
		std::cout << std::endl;
	}
}


int main() {
	GraphAL g(9);

    try{
        g.add_edge(0,1);
        g.add_edge(0,2);
        g.add_edge(0,3);
        g.add_edge(1,4);
        g.add_edge(2,4);
        g.add_edge(3,4);
        g.add_edge(5,4);
        g.add_edge(6,4);
        g.add_edge(7,4);
        g.add_edge(8,5);
        g.add_edge(8,6);
        g.add_edge(8,7);

    }catch (const std::invalid_argument& e){
        std::cerr << "Erro" << e.what() << std::endl;
    }

	std::cout << "num_vertices: " << g.get_num_vertices() << std::endl;
	std::cout << "num_edges: " << g.get_num_edges() << std::endl;
	print_adjacency_list(g);

	return 0;
}