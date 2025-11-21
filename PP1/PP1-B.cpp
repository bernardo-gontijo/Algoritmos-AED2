#include <iostream>
#include <list>
#include <vector>
#include <stdexcept>

typedef unsigned int uint;
typedef unsigned int Vertex;
typedef float Weight;

class GraphAM{
    private:

    uint num_vertices;
    uint num_edges;
    std::vector<std::vector<Weight>> adj;

    public:

    GraphAM() : num_vertices(0), num_edges(0) {}
    GraphAM(uint num_vertices) : num_vertices(num_vertices), num_edges(0), adj(num_vertices, std::vector<Weight>(num_vertices, 0)){}

    //a destruição é feita automaticamente

    void add_edge(Vertex u, Vertex v){

        if (u >= num_vertices || v >= num_vertices){
            throw std::invalid_argument("Vertice(s) invalido(s)");
        }
        if (u == v){
            throw std::invalid_argument("Nao pode ter loop");
        }

        adj[u][v] = 1;
        adj[v][u] = 1;
        num_edges++;
    }

    std::list<Weight> get_adj(const Vertex& u){
        std::list<Weight> adjacentes;
        if (u >= num_vertices){
            throw std::invalid_argument("Vertice(s) invalido(s)");
        }
        for (uint v = 0; v < num_vertices; v++){
            if (adj[u][v] != 0){
                adjacentes.push_back(v);
            }
        }
        return adjacentes; 
    }

    uint get_num_vertices(){
        return num_vertices;
    }

    uint get_num_edges(){
        return num_edges;
    }

    std::vector<std::vector<Weight>> get_adj_matrix(){
        return adj;
    }

    void print_adjacency_matrix(GraphAM& g){
        std::vector<std::vector<Weight>> matrix = g.get_adj_matrix();
        for (uint u = 0; u < g.get_num_vertices(); u++){
            for(uint v = 0; v < g.get_num_vertices(); v++){
                std::cout << matrix[u][v] << " ";
            }
            std::cout << "\n";
        }
    }
};

int main(){
    std::list<Weight> adjacentes;
    uint n = 0;
    uint m = 0;

    std::cin >> n;
    std::cin >> m;

    GraphAM g(n);

    uint u = 0;
    uint v = 0;

    try{
   
        for (uint i = 0; i < m; i++){
            std::cin >> u;
            std::cin >> v;
            
            g.add_edge(u,v);
            
        }
    }catch (const std::invalid_argument& e){
        std::cerr << "Erro: " << e.what() << std::endl;
    }

    std::cout << "num_vertices: " << g.get_num_vertices() << std::endl;
    std::cout << "num_edges: " << g.get_num_edges() << std::endl;

    std::cout << "\nMATRIZ DE ADJACENCIA: " << "\n";
    g.print_adjacency_matrix(g);

    std::cout << "\nLISTA DE ADJACENCIA: " << "\n";
    for (int i = 0; i < n; i++){
        adjacentes = g.get_adj(i);
        std::cout << i << ": ";   
        for (auto const& a : adjacentes){
            std::cout << a << " ";
        }
        std::cout << "\n";
    }

    return 0;
}