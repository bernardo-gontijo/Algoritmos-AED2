#include <iostream>
#include <list>
#include <vector>
#include <utility>
#include <limits>
#include <stdexcept>

typedef unsigned int uint;
typedef unsigned int Vertex;
typedef unsigned int Weight;
typedef std::pair<Vertex, Weight> VertexWeightPair;


class PriorityQueue {
private:
    std::vector<VertexWeightPair> heap;

    int parent(int i){
        return (i - 1) / 2;
    }
    int left(int i){
        return 2 * i + 1;
    }
    int right(int i){
        return 2 * i + 2;
    }

    void heapifyUp(int i){
        while (i > 0 && heap[i] < heap[parent(i)]){
            std::swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    void heapifyDown(int i){
        int menor = i;
        int l = left(i), r = right(i);

        if (l < (int)heap.size() && heap[l] < heap[menor]){
            menor = l;
        }
        if (r < (int)heap.size() && heap[r] < heap[menor]){
            menor = r;
        }

        if (menor != i){
            std::swap(heap[i], heap[menor]);
            heapifyDown(menor);
        }
    }

public:
    PriorityQueue() = default;

    bool isEmpty() const{
        return heap.empty();
    }

    VertexWeightPair getMin() const{
        if (isEmpty()){
            throw std::runtime_error("Fila vazia");
        }
        return heap[0]; 
    }

    void insert(const VertexWeightPair value){
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }

    VertexWeightPair extractMin(){
        if (isEmpty()){
            throw std::runtime_error("Fila vazia");
        }

        VertexWeightPair minVal = heap[0];
        heap[0] = heap.back();
        heap.pop_back();

        if(!heap.empty()){
            heapifyDown(0);
        }

        return minVal;
    }
};

class Graph{
private:

    uint board_dimension;
    uint num_vertices;
    uint num_edges;
    std::list<VertexWeightPair> *adj;

    static constexpr int moves[8][2] = {
        {2, 1}, {2,-1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };
    
    bool check_inside_board(uint row, uint col) const {
        return row >= 0 && row < board_dimension && col >= 0 && col < board_dimension;
    }

    Vertex get_index(uint row, uint col) const {
        return row * board_dimension + col;
    }

    void build_army_graph() {
        for (uint row = 0; row < board_dimension; row++) {
            for (uint col = 0; col < board_dimension; col++) {
                Vertex u = get_index(row, col);
                for (auto const& m : moves) {
                    int newRow = row + m[0];
                    int newCol = col + m[1];

                    if (check_inside_board(newRow, newCol)) {
                        Vertex v = get_index(newRow, newCol);
                        if (v > u){
                            int row_u = row + 1;
                            char col_u = 'a' + col;
                            int row_v = newRow + 1;
                            char col_v = 'a' + newCol;

                            //Formula
                            Weight w = ((int(col_u) * row_u) + (int(col_v)* row_v)) % 19;

                            add_edge(u, v, w);
                        }

                    }
                }
            }
        }
    }



public:

    Graph(uint n) : board_dimension(n), num_vertices(n*n), num_edges(0) {
        adj = new std::list<VertexWeightPair> [num_vertices];
        build_army_graph();
    }

    Vertex chessToVertex(const std::string& pos, uint board_dimension) {
        char colChar = std::tolower(pos[0]);
        int col = colChar - 'a';
        
        uint row = std::stoi(pos.substr(1)) - 1;

        return row * board_dimension + col;
    }
    
    void add_edge(Vertex u, Vertex v, Weight w){
        if (u >= num_vertices || v >= num_vertices) {
            throw std::invalid_argument("Vertice(s) invalido(s)");
        }
        if (u == v) {
            throw std::invalid_argument("Nao pode ter loop");
        }

        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
        num_edges++;
    }

    std::list<VertexWeightPair> get_adj(const Vertex& u){
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

    ~Graph() {
	delete[] adj;
	adj = nullptr;
	}
};


void print_adjacency_list(Graph g){
    for (uint u = 0; u < g.get_num_vertices(); u++){
        std::list<VertexWeightPair> adj_u = g.get_adj(u);
        std::cout << u << ": ";  
        for (auto const& v : adj_u){
            std::cout << "(" << v.first << ", " << v.second << ")" << ", ";
        }
        std::cout << "\n";
    }
}

//Bubble Sort adaptado

void bubbleSort(std::list<VertexWeightPair>& lista) {
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
            if (it->first > next->first) {
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

std::vector<Weight> Dijkstra(Graph& g, Vertex s, std::vector<Vertex>& pred){
        const int INF = std::numeric_limits<uint>::max();
        Vertex n = g.get_num_vertices();

        //Inicializa
        std::vector<uint> dist(n, INF);
        pred.resize(n, -1);
        dist[s] = 0;

        PriorityQueue q; 
        q.insert({s, 0});

        while(!q.isEmpty()){
            //Extract Min
            VertexWeightPair pair = q.extractMin();
            Vertex u = pair.first; 
            uint dist_u = pair.second;
            

            if (dist_u > dist[u]) continue;

            std::list<VertexWeightPair> vizinhos = g.get_adj(u);

            bubbleSort(vizinhos);

            //Relaxa
            for(auto[v, w] : vizinhos){
                if (dist[v] > dist[u] + w){
                    dist[v] = dist[u] + w;
                    pred[v] = u;
                    q.insert({v, dist[v]});
                }
            }
        }
    return dist;
}

struct Army{
    std::string color;
    Vertex position;
    std::vector<std::string> enemies;
    std::vector<Vertex> path;
    bool preso = false;
    bool winner = false;
    uint moves = 0;
    Weight pathWeight = 0;

    bool isEnemy(const std::string& otherColor){
        for (size_t i = 0; i < enemies.size(); i++){
            if (enemies[i] == otherColor){
                return true;
            }
        }
        return false;   
    }
};


bool hasTormenta(const std::vector<Vertex>& storms, Vertex pos) {
    for (size_t i = 0; i < storms.size(); i++) {
        if (storms[i] == pos) return true;
    }
    return false;
}

void removeTormenta(std::vector<Vertex>& storms, Vertex pos) {
    for (size_t i = 0; i < storms.size(); i++) {
        if (storms[i] == pos) {
            storms.erase(storms.begin() + i);
            break;
        }
    }
}

std::vector<Vertex> buildPath(Vertex origem, Vertex destino, const std::vector<Vertex>& pred) {
    std::vector<Vertex> tmp;

    for (Vertex v = destino; v != (uint)-1; v = pred[v]) {
        tmp.push_back(v);
    }

 
    std::vector<Vertex> path;
    if (!tmp.empty()) {
        path.reserve(tmp.size());
        for (int i = (int)tmp.size() - 1; i >= 0; --i) {
            path.push_back(tmp[i]);
        }
    }
    return path;
}



void simulateBattle(Graph& g, std::vector<Army>& armies, Vertex castle, std::vector<Vertex>& tormentas){
    bool winner = false;

    while (!winner){
        for (size_t i = 0; i < armies.size(); i++){
            Army& army = armies[i];
            if (army.winner){
                continue;
            }

            if(army.preso){
                army.preso = false;
                continue;
            }

            if (army.path.size() <= 1){
                continue;
            }

            Vertex nextPos = army.path[1];

            bool moved = true;

            for(size_t j = 0; j < armies.size(); j++){
                if (i == j){
                    continue;
                }
                if (armies[j].position == nextPos){
                    if (army.isEnemy(armies[j].color)){
                        moved = false; //perder a vez
                    }
                }
            }

            if (hasTormenta(tormentas, nextPos)){
                bool sozinho = true;
                for(size_t j = 0; j < armies.size(); j++){
                    if (i == j){
                        continue;
                    }
                    if (armies[j].position == army.position && !army.isEnemy(armies[j].color)){
                        sozinho = false;
                        break;
                    }
                }

                if (sozinho){
                    army.preso = true;
                    moved = false;
                }else{
                    removeTormenta(tormentas, nextPos);
                }
            }

            if (moved){
                army.position = nextPos;
                army.path.erase(army.path.begin());
                army.moves++;

            }

            if (army.position == castle){
                army.winner = true;
                winner = true;
            }
        }
    }
}

void sortArmiesByColor(std::vector<Army>& armies) {
    size_t n = armies.size();
    for (size_t i = 0; i < n; ++i) {
        size_t minIdx = i;
        for (size_t j = i + 1; j < n; ++j) {
            if (armies[j].color < armies[minIdx].color) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            std::swap(armies[i], armies[minIdx]);
        }
    }
}


int main() {
    
    uint n;
    std::cin >> n;

    Graph g(n);
    
    int num_exercitos;
    std::cin >> num_exercitos;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::vector<Army> armies;


    for (int i = 0; i < num_exercitos; i++) {
        std::string color, pos;
        std::cin >> color >> pos;

        Army army;
        army.color = color;
        army.position = g.chessToVertex(pos, n);

        // lê inimigos até encontrar outro número ou EOF
        std::string enemy;
        while (true) {
            if (!(std::cin >> enemy)) break;
            // se o token for um número (como num_tormentas ou próximo campo), volta no stream
            if (isdigit(enemy[0])) {
                std::cin.putback(enemy[0]);
                break;
            }
            army.enemies.push_back(enemy);
        
        }   
        armies.push_back(army);
    }

    std::string castlePos;
    std::cin >> castlePos;
    Vertex castle = g.chessToVertex(castlePos, n);

    int num_tormentas;
    std::cin >> num_tormentas;

    std::vector<Vertex> tormentas;
    for (int j = 0; j < num_tormentas; j++){
        std::string t;
        std::cin >> t;
        tormentas.push_back(g.chessToVertex(t, n));
    }

    for(uint k = 0; k < armies.size(); k++){
        std::vector<Vertex> pred;
        std::vector<Weight> dist = Dijkstra(g, armies[k].position, pred);
        armies[k].path = buildPath(armies[k].position, castle, pred);
        armies[k].pathWeight = dist[castle];  
    }

    simulateBattle(g, armies, castle, tormentas);

    sortArmiesByColor(armies);

    for (uint i = 0; i < armies.size(); i++){
        if (armies[i].winner){
            std::cout << armies[i].color << " " << armies[i].moves << " " << armies[i].pathWeight;
        }
    }

    return 0;
}
