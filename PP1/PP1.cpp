#include <iostream>
#include <list>
#include <vector>
#include <stdexcept>

typedef unsigned int uint;
typedef unsigned int Vertex;

class GraphAL{
    private:

    uint num_vertices;
    uint num_edges;
    std::vector<std::list<Vertex>> adj;

    public:

    GraphAL() : num_vertices(0), num_edges(0) {}
    GraphAL(uint num_vertices) : num_vertices(num_vertices), num_edges(0), adj(num_vertices) {
    }

    void add_edge(Vertex u, Vertex v){

        if (u >= num_vertices || v >= num_vertices){
            throw std::invalid_argument("Vértice(s) inválido(s)");
        }
        if (u == v){
            throw std::invalid_argument("Não pode ter loop");
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
            throw std::invalid_argument("Não pode ter loop");
        }

        adj[u].remove(v);
        adj[v].remove(u);
        num_edges--;
    }

    std::list<Vertex> get_adj(const Vertex& u){
        if (u >= num_vertices){
            throw std::invalid_argument("Vértice inválido");
        }
        return adj[u];
    }

    uint get_num_vertices(){
        return num_vertices;
    }

    uint get_num_edges(){
        return num_edges;
    }
};

void print_adjacency_list(GraphAL g){
    for (uint u = 0; u < g.get_num_vertices(); u++){
        std::list<Vertex> adj_u = g.get_adj(u);
        std::cout << u << ": ";  
        for (auto const& v : adj_u){
            std::cout << v << ", ";
        }
        std::cout << "\n";
    }
}

int main(){
    
    uint u, v;
    std::cin >> u;
    std::cin >> v;

    GraphAL g(u);

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
        
    } catch(const std::invalid_argument& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
    }

    std::cout << "num_vertices: " << g.get_num_vertices() << std::endl;  
    std::cout << "num_edges: " << g.get_num_edges() << std::endl;
    print_adjacency_list(g);

    return 0;
}