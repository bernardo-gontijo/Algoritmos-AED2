#include <iostream>
#include <stdexcept>
#include <list>
#include <vector>
#include <utility>
#include <optional>

typedef unsigned int uint;
typedef unsigned int Vertex;
typedef float Weight;
typedef std::pair<Vertex, Weight> VertexWeightPair;

class WeightedGraphAL{
    private:

    uint num_vertices;
    uint num_edges;
    std::list<VertexWeightPair> *adj;

    public:

    WeightedGraphAL() : num_vertices(0), num_edges(0) {}
    WeightedGraphAL(uint num_vertices) : num_vertices(num_vertices), num_edges(0){

        adj = new std::list<VertexWeightPair> [num_vertices];
    }

    ~WeightedGraphAL(){
        delete[] adj;
        adj = nullptr;
    }
    
    void add_edge(const Vertex& u, const Vertex& v, const Weight& w){
        if (u >= num_vertices || v >= num_vertices){
            throw std::invalid_argument("Vertice(s) invalido(s)");
        }
        if (u == v){
            throw std::invalid_argument("Erro de logica");
        }
        
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
        ++num_edges;
    }

    std::list<VertexWeightPair> get_adj(const Vertex& u){
        if (u >= num_vertices){
            throw std::invalid_argument("Vertice Invalido");
        }
        return adj[u];
    }

    std::optional<Weight> get_weight(Vertex u, Vertex v){
        if (u >= num_vertices || v >= num_vertices){
            throw std::invalid_argument("Vertice(s) invalido(s)");
        }
        if (u == v){
            throw std::invalid_argument("Erro de logica");
        }
        std::list<VertexWeightPair> pares_vw = get_adj(u);

        for (auto const& p : pares_vw){
            if (p.first == v){
                return p.second;
            }
        }

        return std::nullopt;
    }

    uint get_num_vertices(){
        return num_vertices;
    }

    uint get_num_edges(){
        return num_edges;
    }
};

void print_adjacency_list(WeightedGraphAL& g){
    for (uint u = 0; u < g.get_num_vertices(); u++){
        std::list<VertexWeightPair> list = g.get_adj(u);

        std::cout << u << ": ";
        for (auto const& p : list){
            std::cout << "(" << p.first << ", " << p.second << ")" << ", ";
        }
        std::cout << std::endl;
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

    WeightedGraphAL graph(n);

    try{
        for (uint i = 0; i < m; i++){
            std::cin >> u;
            std::cin >> v;
            std::cin >> w;

            graph.add_edge(u, v, w);
        }
    }catch (const std::invalid_argument& e){
        std::cerr << "Erro: " << e.what() << std::endl;
    }

    std::cout << "num_vertices: " << graph.get_num_vertices() << std::endl;
    std::cout << "num_edges: " << graph.get_num_edges() << std::endl;
    print_adjacency_list(graph);

    return 0;
}