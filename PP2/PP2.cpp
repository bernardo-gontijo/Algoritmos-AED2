#include <iostream>
#include <list>
#include <vector>
#include <utility>
#include <limits>
#include <stdexcept>

typedef unsigned int uint;
typedef unsigned int Vertex;

template <typename T>
class Fila {
private:
    std::list<T> dado;

public:

    void push(const T& elem) {
        dado.push_back(elem);
    }

    void pop() {
        if (dado.empty()){
            std::cout << "Empty Queue" << std::endl;
        }
        dado.pop_front();
    }

    T front() {
        T valor = dado.front();
        return valor;
    }

    bool vazia() const {
        return dado.empty();
    }

    void imprimir() const {
        for (auto const d : dado){
            std::cout << d << std::endl;
        }
        std::cout << std::endl;
    }
};

class Graph{
    private:

    uint num_vertices;
    uint num_edges;
    std::vector<std::list<Vertex>> adj;

    static constexpr int moves[8][2] = {
        {2, 1}, {2,-1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };
    
    bool check_inside_board(int row, int col) const {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }

    Vertex get_index(int row, int col) const {
        return row * 8 + col;
    }

    void build_knight_graph() {
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                Vertex u = get_index(row, col);
                for (auto const& m : moves) {
                    int newRow = row + m[0];
                    int newCol = col + m[1];

                    if (check_inside_board(newRow, newCol)) {
                        Vertex v = get_index(newRow, newCol);
                        if (v > u){
                            add_edge(u, v);
                        }

                    }
                }
            }
        }
        num_edges /= 2; // Cada aresta foi contada duas vezes
    }



    public:

    Graph() : num_vertices(64), num_edges(0), adj(64) {
        build_knight_graph();
    }

    Vertex chessToVertexSimple(const std::string& pos) {
        const char* table[] = {"a1","b1","c1","d1","e1","f1","g1","h1",
                                "a2","b2","c2","d2","e2","f2","g2","h2",
                                "a3","b3","c3","d3","e3","f3","g3","h3",
                                "a4","b4","c4","d4","e4","f4","g4","h4",
                                "a5","b5","c5","d5","e5","f5","g5","h5",
                                "a6","b6","c6","d6","e6","f6","g6","h6",
                                "a7","b7","c7","d7","e7","f7","g7","h7",
                                "a8","b8","c8","d8","e8","f8","g8","h8"};
    
        for (int i = 0; i < 64; i++) {
            if (pos == table[i]){ 
                return i;
            }
        }
        return 0;
    }
    
    void add_edge(Vertex u, Vertex v){
        if (u >= num_vertices || v >= num_vertices) {
            throw std::invalid_argument("Vertice(s) invalido(s)");
        }
        if (u == v) {
            throw std::invalid_argument("Nao pode ter loop");
        }

        adj[u].push_back(v);
        adj[v].push_back(u);
        num_edges++;
    }

    std::list<Vertex> get_adj(const Vertex& u){
        if (u >= num_vertices){
            throw std::invalid_argument("Vertice invalido");
        }
        return adj[u];
    }

    uint get_num_vertices(){
        return num_vertices;
    }

    uint get_num_edges(){
        return num_edges;
    }

    int bfs(std::string king, std::string knight) {
        Vertex s = chessToVertexSimple(king);
        Vertex end = chessToVertexSimple(knight);
        if (s >= num_vertices || end >= num_vertices) {
            throw std::invalid_argument("Vertice invalido");
        }

        std::vector<int> dist(num_vertices, -1);
        Fila<Vertex> q;

        dist[s] = 0;
        q.push(s);

        while (!q.vazia()){
            Vertex u = q.front();
            q.pop();

            if (u == end){
                return dist[u] - 1;
            }

            for (auto v : adj[u]){
                if (dist[v] == -1){
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
        return -1;
    }
};


void print_adjacency_list(Graph g){
    for (uint u = 0; u < g.get_num_vertices(); u++){
        std::list<Vertex> adj_u = g.get_adj(u);
        std::cout << u << ": ";  
        for (auto const& v : adj_u){
            std::cout << v << ", ";
        }
        std::cout << "\n";
    }
}

//Bubble Sort

void bubbleSort(std::list<int>& lista) {
    if (lista.empty()){
        return;
    }

    bool trocou;
    do {
        trocou = false;
        auto it = lista.begin();
        auto next = it;
        next++;
        
        while (next != lista.end()) {
            if (*it > *next) {
                std::swap(*it, *next);
                trocou = true;
            }
            it++;
            next++;
        }
    } while (trocou);
}

void min_steps(std::list<int> list){
    int min = list.front();
    int cont = 0;

    for (int num : list){
        if (num < min){
            min = num;
            cont = 1;
        } else if (num == min){
            cont++;
        }
    } 
    for (int i = 0; i < cont; i++){
        std::cout << min << " ";
    }
}

int main(){

    int n;
    std::cin >> n;
    Graph g;

    std::vector<std::vector<std::string>> tables(n, std::vector<std::string>(5));
    std::vector<std::list<int>> distances(n);
    
    for (int i = 0; i < n; i++){
        for (int j = 0; j < 5; j++){
            std::cin >> tables[i][j];
        } 
    }

    try{
        for (int i = 0; i < n; i++){
            for (int j = 0; j < 4; j++){
                int distance = g.bfs(tables[i][j], tables[i][4]);
                distances[i].push_back(distance);
            }
            bubbleSort(distances[i]); 
        }
    }catch (const std::invalid_argument& e){
        std::cerr << "Erro: " << e.what() << std::endl;
    }

    for (int i = 0; i < n; i++){
        min_steps(distances[i]);
        std::cout << std::endl;
    }
    
    return 0;
}