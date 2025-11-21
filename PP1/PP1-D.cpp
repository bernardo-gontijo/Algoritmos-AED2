#include <iostream>
#include <stdexcept>
#include <vector>
#include <list>
#include <utility>
#include <limits>

typedef unsigned int uint;
typedef unsigned int Vertex;
typedef float Weight;

class WeightedGraphAM{
    private:

    uint num_vertices;
    uint num_edges;
    std::vector<std::vector<Weight>> adj;

    public:

    WeightedGraphAM() : num_vertices(0), num_edges(0){}
    WeightedGraphAM(uint num_vertices) : num_vertices(num_vertices), num_edges(0), adj(num_vertices, std::vector<Weight>(num_vertices, std::numeric_limits<Weight>::infinity())){}

    //destruicao feita automaticamente pela biblioteca

    void add_edge(const Vertex& u, const Vertex& v, const Weight& w){
        if (u >= num_vertices || v >= num_vertices){
            throw std::invalid_argument("Vertice(s) Invalido(s)");
        }

        if (u == v){
            throw std::invalid_argument("Erro de logica");
        }

        if (adj[u][v] == std::numeric_limits<Weight>::infinity() && adj[v][u] == std::numeric_limits<Weight>::infinity()){
            ++num_edges;
        }
        adj[u][v] = w;
        adj[v][u] = w;
        
    }

    void remove_edge(const Vertex& u, const Vertex& v, const Weight& w){
        if (u >= num_vertices || v >= num_vertices){
            throw std::invalid_argument("Vertice(s) Invalido(s)");
        }

        if (u == v){
            throw std::invalid_argument("Erro de logica");
        }

        if (adj[u][v] != std::numeric_limits<Weight>::infinity() && adj[v][u] != std::numeric_limits<Weight>::infinity()){
            --num_edges;
        }
        adj[u][v] = std::numeric_limits<Weight>::infinity();
        adj[v][u] = std::numeric_limits<Weight>::infinity();
    }

    std::list<Vertex> get_adj(const Vertex& u){
        if (u >= num_vertices){
            throw std::invalid_argument("Vertice invalido");
        }

        std::list<Vertex> adjacentes;
        for (uint v = 0; v < num_vertices; v++){
            if (adj[u][v] != std::numeric_limits<Weight>::infinity()){
                adjacentes.push_back(v);
            }
        }
        return adjacentes;
    }

    std::vector<std::vector<Weight>> get_adj_matrix(){
        return adj;
    }

    uint get_num_vertices(){
        return num_vertices;
    }

    uint get_num_edges(){
        return num_edges;
    }

    Weight get_weight(const Vertex& u, const Vertex& v){
        if (u >= num_vertices || v >= num_vertices){
            throw std::invalid_argument("Vertice(s) Invalido(s)");
        }

        if (u == v){
            throw std::invalid_argument("Erro de logica");
        }
        Weight w = adj[u][v];

        return w;            
    }
};

void print_adjancency_matrix(WeightedGraphAM& g){
    std::vector<std::vector<Weight>> adj = g.get_adj_matrix();
    for (uint u = 0; u < g.get_num_vertices(); u++){
        for (uint v = 0; v < g.get_num_vertices(); v++){
            std::cout << adj[u][v] << " ";
        }
        std::cout << "\n";
    }
}

int main(){
    uint n = 0;
    uint m = 0;
    uint u = 0;
    uint v = 0;
    Weight w = 0.0;

    std::cin >> n;
    std::cin >> m;

    WeightedGraphAM graph(n);

  
        for (uint i = 0; i < m; i++){
            std::cin >> u;
            std::cin >> v;
            std::cin >> w;
            try{
                graph.add_edge(u, v, w);
            }catch (const std::invalid_argument& e){
                std::cerr << "Erro: " << e.what() << std::endl;
            }
        }

    std::cout << "num_vertices: " << graph.get_num_vertices() << std::endl;
    std::cout << "num_edges: " << graph.get_num_edges() << std::endl;
    print_adjancency_matrix(graph);

    return 0;
}