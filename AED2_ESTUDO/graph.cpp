#include <iostream>
#include <list>
#include <stdexcept>

typedef unsigned int uint;
typedef unsigned int Vertex;

class GraphAL{
    private:
    uint num_vertices;
    uint num_edges;
    std::list<Vertex> *adj; //vetor a ser alocado

public:
    GraphAL() : num_vertices(0), num_edges(0) {}
    GraphAL(uint num_vertices) : num_vertices(num_vertices), num_edges(0) {
    
        adj = new std::list<Vertex> [num_vertices];
    }
    
    void add_edge(Vertex u, Vertex v){
        if (u >= num_vertices || v >= num_vertices){
            throw std::out_of_range("Vértice(s) inválido(s)");
        }
        if (u == v){
            throw std::invalid_argument("Não é permitido loop");
        }

        adj[u].push_back(v);
        adj[v].push_back(u);
        num_edges++;   
    }

    void remove_edge(Vertex u, Vertex v){
        if (u >= num_vertices || v >= num_vertices){
            throw std::invalid_argument("Vértice(s) inválido(s)");
        }

        if (u == v){
            throw std::invalid_argument("Não é permitido loop");
        }

        adj[u].remove(v);
        adj[v].remove(u);
        num_edges--;   
    }

    std::list<Vertex> get_adj (const Vertex& u){
        return adj[u];
    }

    uint get_num_vertices(){
        return num_vertices;
    }

    ~GraphAL(){
        delete[] adj;
        adj = nullptr;
    }
};

void print_adjacency_list(GraphAL g){
    for (int u; u < g.get_num_vertices(); u++){
        std::list adj_u = g.get_adj(u);
        for (auto v : adj_u){
            std::cout << v << " ";
        }
        std::cout << "\n";
    }
}

int main(){
    
    GraphAL g(5);

    try{
        g.add_edge(1,2);
        g.add_edge(1,3);
        g.add_edge(1,4);
        g.add_edge(1,0);
        g.add_edge(3,4);
        g.add_edge(2,4);
        g.add_edge(2,3);
        g.add_edge(0,4);
        
    } catch(const std::invalid_argument& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
    }
    print_adjacency_list(g);

    return 0;
}