#include <iostream>
#include <list>
#include <vector>
#include <utility>
#include <limits>
#include <stdexcept>

typedef float Weight;
typedef std::pair<int, int> VertexPair;

class SetDisjoin {
private:
    int *parent, *rank;
    int n;

public:
    explicit SetDisjoin(int n){
        this->n = n;
        parent = new int[n+1];
        rank = new int[n+1];
        makeSet();
    }
    ~SetDisjoin() {
        delete[] parent;
        delete[] rank;
    }

    void makeSet(){
        for (int x = 0; x < n; x++){
            parent[x] = x;
            rank[x] = 0;
        }
    }

    int findSet(int x){
        if (parent[x] == x){
            return x;
        }

        int result = findSet(parent[x]);
        parent[x] = result;
        return result;
    }

    void unionSet(int x, int y){
        int u = findSet(x);
        int v = findSet(y);

        if (u == v){
            return;
        }
        if (rank[u] < rank[v]){
            parent[u] = v;
        }else if (rank[v] < rank[u]){
            parent[v] = u;
        }else{
            parent[u] = v;
            rank[v]++;
        }
    }
};

class NeuronGraph{
private:
    int num_vertices;
    int num_edges;
    std::list<std::pair<Weight, VertexPair>> edges;

public:
    NeuronGraph(int v, int e) : num_vertices(v), num_edges(e){}
    ~NeuronGraph() = default;

    void add_edge(int u, int v, Weight w){
        edges.push_back({w, {u, v}});
    }

    std::list<std::pair<Weight, VertexPair>> &getEdges(){
        return edges;
    }

    Weight mstKruskal(){
        Weight mst_peso = 0.0;
        edges.sort();
        SetDisjoin d(num_vertices);

        std::list<std::pair<Weight, VertexPair>>::iterator it;
        for (it = edges.begin(); it != edges.end(); ++it){
            int u = it->second.first;
            int v = it->second.second;

            int set_u = d.findSet(u);
            int set_v = d.findSet(v);

            if (set_u != set_v){
                mst_peso += it->first;
                d.unionSet(set_u, set_v);
            }
        }
        return mst_peso;
    }
};

class Vertex{
private:
    int index;
    bool doente = false;
    Weight peso = std::numeric_limits<Weight>::infinity();
    std::list<Vertex*> verticesAdj; //lista de adjacencia
    std::list<Vertex*> minSteps;
    Vertex *pi = nullptr;
    NeuronGraph *subgraph = nullptr;

public:
    Vertex(int indexVertice): index(indexVertice){}
    ~Vertex() = default;

    int getIndex() const{
        return index;
    }

    void setSick() {
        doente = true;
    }

    bool getSick() const{
        return doente;
    }

    void setPeso(Weight newPeso){
        peso = newPeso;
    }

    Weight getPeso() const{
        return peso;
    }

    std::list<Vertex*>& getverticesAdj(){
        return verticesAdj;
    }

    void setPi(Vertex &v){
        pi = &v;
    }

    Vertex* getPi() const{
        return pi;
    }

    void setSubgraph(NeuronGraph &ng){
        subgraph = &ng;
    }

    NeuronGraph &getSubgraph(){
        return *subgraph;
    }

    void adicionarMinSteps(std::list<Vertex*>& caminho){
        if(this != getPi()){
            getPi()->adicionarMinSteps(caminho);
        }
        caminho.push_back(this);
    }
};

class Edge{
private:
    Vertex *v1;
    Vertex *v2;
    Weight peso;

public:
    Edge(Vertex &vertice1, Vertex &vertice2, Weight peso) : v1(&vertice1), v2(&vertice2), peso(peso){}
    ~Edge() = default;
    
    Vertex*  getV1() const{
        return v1;
    }
    Vertex*  getV2() const{
        return v2;
    }
    Weight getPeso() const{
        return peso;
    }
};

class Graph{
private:

    int num_vertices;
    int num_edges;
    std::list<Vertex*> vertices;
    std::list<Edge*> edges;

public:

    Graph() : num_vertices(0), num_edges(0) {}

    void add_vertice(Vertex *v){
        vertices.push_back(v);
        num_vertices++;
    }

    void add_edge(Edge *e){
        e->getV1()->getverticesAdj().push_back(e->getV2());
        e->getV2()->getverticesAdj().push_back(e->getV1());

        edges.push_back(e);
        num_edges++;    
    }

    std::list<Vertex*> &getVertex(){
        return vertices;
    }

    std::list<Edge*> &getEdges(){
        return edges;
    }

    int get_num_vertices(){
        return num_vertices;
    }

    int get_num_edges(){
        return num_edges;
    }

    Edge& getEdge(Vertex &v1, Vertex &v2){
        for (auto &e : edges){
            if ((e->getV1() == &v1 && e->getV2() == &v2) || (e->getV1() == &v2 && e->getV2() == &v1)){
                return *e;
            }
        }
        throw std::invalid_argument("Aresta invalida");
    }

    Vertex& getVertex(int index){
        for(auto &v : vertices){
            if (v->getIndex() == index){
                return *v;
            }
        }
        throw std::invalid_argument("Vertice invalido");
    }

    ~Graph(){
        for (auto &v: vertices) {
            delete v;
        }
        for (auto &e: edges) {
            delete e;
        }
    } 
};

class PriorityQueue{
    std::pair<Vertex*, Weight> *heap;
    int size;
    int heapSize;

public:
    PriorityQueue(int n) {
        heapSize = 0;
        size = n;
        heap = new std::pair<Vertex*, float>[n];
    }
    ~PriorityQueue() {
        delete[] heap;
    }

    static void swap(std::pair<Vertex*, float>&x, std::pair<Vertex*, float>&y) {
        std::pair<Vertex*, float> temp = x;
        x = y;
        y = temp;
    }

    void minHeapify(int i) {
        int e = left(i);
        int d = right(i);
        int menor = i;

        if (e < heapSize && heap[e].second < heap[menor].second){
            menor = e;
        }
        if (d < heapSize && heap[d].second < heap[menor].second){
            menor = d;
        } if (menor != i) {
            swap(heap[i], heap[menor]);
            minHeapify(menor);
        }
    }

    Vertex* extractMin() {
        if (isEmpty() == true) {
            return nullptr;
        }
        if (heapSize == 1) {
            heapSize--;
            return heap[0].first;
        }
        std::pair<Vertex*, float> raiz= heap[0];
        heap[0]= heap[heapSize - 1];
        heapSize--;
        minHeapify(0);
        return raiz.first;
    }

    void insert(std::pair<Vertex*, float> a) {
        if (heapSize == size) {
            return;
        }
        heapSize++;
        int i = heapSize - 1;
        heap[i] = a;
        while (i != 0 && heap[parent(i)].second > heap[i].second) {
            swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }
    void decrescerChave(int a, float n) {
        int i = find(a);
        if (i != -1) {
            heap[i].second = n;
            while (i != 0 && heap[parent(i)].second > heap[i].second) {
                swap(heap[i], heap[parent(i)]);
                i = parent(i);
            }
        }
    }

    void buildMinHeap(){
        for (int i = heapSize / 2; i >= 0; i--) {
            minHeapify(i);
        }
    }

    bool isEmpty() const {
        if (heapSize == 0) {
            return true;
        }
        return false;
    }

    int find(int index) {
        for (int i = 0; i < heapSize; i++) {
            if (heap[i].first->getIndex() == index) {
                return i;
            }
        }
        return -1;
    }

    int parent(int i) {
        return (i) / 2;
    }
    int left (int i) {
        return 2*i;
    }
    int right (int i) {
        return 2*i+1;
    }

};

class Dijkstra{
private:
    Graph *g;
    PriorityQueue q;
    std::list<Vertex*> minSteps;

public:
    explicit Dijkstra(Graph &graph) : g(&graph), q(graph.get_num_vertices()){
        g = &graph;
    }
    void insertHeap(){
        for (auto &v: g->getVertex()) {
            std::pair<Vertex*, Weight> a = std::make_pair(v, v->getPeso());
            q.insert(a);
        }
    }



    void relaxa(Vertex &v1, Vertex &v2, Weight dist){
        if (v2.getPeso() > v1.getPeso() + dist){
            v2.setPeso(v1.getPeso() + dist);
            v2.setPi(v1);
            q.decrescerChave(v2.getIndex(), v2.getPeso());
        }
    }

    void startDijkstra(int index) {
        Vertex &s = g->getVertex(index);
        s.setPeso(0.0);
        s.setPi(s);
        insertHeap();
        while ( q.isEmpty() == false){
            Vertex *u = q.extractMin();
            for (Vertex *v: u->getverticesAdj()){
                Edge e = g->getEdge(*u, *v);
                relaxa(*u, *v, e.getPeso());
            }
        }
    }
    
    void printDijkstra(Graph &g, int index){
        Weight resultado = 0.0;
        g.getVertex(index).adicionarMinSteps(minSteps);
        for (auto *v : minSteps){
            if (v->getSick() == true){
                Weight mst_peso = v->getSubgraph().mstKruskal();
                resultado += mst_peso;
            }
        }
        std::cout << resultado;
    }
};

int main() {
    Graph cerebro;
    int ordemCerebro, tamanhoCerebro, index1, index2, indexEntrada, indexSaida; //Entradas do cerebro
    int ordemBloco, tamanhoBloco, numSickNeurons, indexSickNeuron; //Entrada dos blocos de neuronio
    Weight dist;
    
    std::cin >> ordemCerebro >> tamanhoCerebro;

    for (int i = 1; i <= ordemCerebro; i++){
        auto bloco = new Vertex(i);
        cerebro.add_vertice(bloco);
    }

    for (int i = 1; i <= tamanhoCerebro; i++){
        std::cin >> index1 >> index2 >> dist;
        Vertex &v1 = cerebro.getVertex(index1);
        Vertex &v2 = cerebro.getVertex(index2);

        auto e = new Edge(v1, v2, dist);
        cerebro.add_edge(e);
    }

    std::cin >> indexEntrada >> indexSaida;

    for (auto v : cerebro.getVertex()){
        std::cin >> ordemBloco >> tamanhoBloco;
        auto *ng = new NeuronGraph(ordemBloco, tamanhoBloco);
        v->setSubgraph(*ng);
        std::cin >> numSickNeurons;

        if (numSickNeurons != 0){
            v->setSick();
            for (int i = 0; i < numSickNeurons; i++){
                std::cin >> indexSickNeuron;
            }
        }

        for (int i = 1; i <= tamanhoBloco; i++) {
            std::cin >> index1 >> index2 >> dist;
            v->getSubgraph().add_edge(index1, index2, dist);
        }
    }

    Dijkstra dijkstra(cerebro);
    dijkstra.startDijkstra(indexEntrada);
    dijkstra.printDijkstra(cerebro, indexSaida);

    return 0;
}
