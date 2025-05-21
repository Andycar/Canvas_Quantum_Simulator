// Canvas Quantum Simulator – v2.0 "Spaceship edition" 🚀
// -------------------------------------------------------------
//  • Unlimited "models" (unitaries, channels, meta‑nodes)
//  • SceneGraph with Group, Repeat (scale), ParamBind (rotate/param)
//  • JSON plug‑in format extended with arbitrary properties
//  • Compiler flattens graph → gate list for state‑vector engine
//  • ASCII canvas now draws nested groups + repeat loops
// -------------------------------------------------------------
// Build: g++ -std=c++17 -O3 canvas_qsim.cpp -o canvas_qsim
// Dependencies: Eigen3, nlohmann/json
// -------------------------------------------------------------
#include <Eigen/Dense>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <bitset>
#include <optional>
using json=nlohmann::json; using namespace Eigen; using cxd=std::complex<double>; constexpr cxd I{0,1};

//--------------------------------------------------
// Model – fundamental building block
//--------------------------------------------------
struct Model{ std::string name; size_t arity; MatrixXcd U; json meta;};

class ModelRegistry{
    std::map<std::string,Model> reg;
public:
    bool exists(const std::string&k)const{return reg.count(k);} const Model& get(const std::string&k)const{return reg.at(k);} void add(const Model&m){reg[m.name]=m;}
    bool load(const std::string&path){ std::ifstream f(path); if(!f){std::cerr<<"cannot open "<<path<<"\n";return false;} json j; f>>j; Model m; m.name=j["name"]; m.arity=j["arity"]; m.meta=j; auto arr=j["matrix"].get<std::vector<std::vector<std::vector<double>>>>(); size_t n=arr.size(); m.U.resize(n,n); for(size_t r=0;r<n;++r) for(size_t c=0;c<n;++c) m.U(r,c)=cxd(arr[r][c][0],arr[r][c][1]); add(m); return true; }
};

//--------------------------------------------------
// SceneGraph nodes
//--------------------------------------------------
struct Node{
    enum Type{MODEL,GROUP,REPEAT,PARAM} type; Type t; std::string modelName; std::vector<size_t> targets; size_t times=1; double theta=0; std::vector<Node*> kids; json props;};

//--------------------------------------------------
// QuantumCircuit v2 – builds from SceneGraph
//--------------------------------------------------
class QuantumCircuit{ size_t nq; VectorXcd state; ModelRegistry& reg; Node* root; std::vector<std::string> canvas;
    static MatrixXcd kron(const MatrixXcd&a,const MatrixXcd&b){MatrixXcd K(a.rows()*b.rows(),a.cols()*b.cols());for(int i=0;i<a.rows();++i)for(int j=0;j<a.cols();++j)K.block(i*b.rows(),j*b.cols(),b.rows(),b.cols())=a(i,j)*b;return K;}
    MatrixXcd expand(const MatrixXcd&g,const std::vector<size_t>&t){ size_t dim=1ULL<<nq; MatrixXcd op=MatrixXcd::Zero(dim,dim); size_t k=t.size();
        for(size_t b=0;b<dim;++b){ size_t ext=0; for(size_t i=0;i<k;++i) ext|=((b>>t[i])&1ULL)<<i; for(size_t tgt=0;tgt<(1ULL<<k);++tgt){ size_t nb=b; for(size_t i=0;i<k;++i){ size_t mask=1ULL<<t[i]; nb=(nb&~mask)|(((tgt>>i)&1ULL)<<t[i]); } op(nb,b)=g(tgt,ext);} }
        return op; }
    void apply(const MatrixXcd&op){state=op*state;}
    void compile(Node* n){ if(n->t==Node::MODEL){const auto& m=reg.get(n->modelName); MatrixXcd mat=m.U; if(m.meta.contains("param")&&m.meta["param"]=="theta") mat=std::cos(n->theta/2)*MatrixXcd::Identity(mat.rows(),mat.cols())-I*std::sin(n->theta/2)*mat; apply(expand(mat,n->targets)); drawGate(m.meta.value("style","??"),n->targets.front());}
        else if(n->t==Node::REPEAT){ for(size_t i=0;i<n->times;++i) for(auto*k:n->kids) compile(k);} else if(n->t==Node::GROUP){ for(auto*k:n->kids) compile(k);} }
    void drawGate(const std::string&lbl,size_t qubit){ if(canvas.size()<nq) canvas.resize(nq); size_t c=canvas.empty()?0:canvas[0].size(); for(size_t q=0;q<nq;++q){ if(canvas[q].size()<c+3) canvas[q]+=std::string(c+3-canvas[q].size(),' '); canvas[q].replace(c,3,q==qubit?lbl:"───"); } }
public:
    QuantumCircuit(size_t qubits,ModelRegistry&r):nq(qubits),reg(r){state=VectorXcd::Zero(1ULL<<nq);state(0)=1; root=new Node{Node::GROUP};}
    Node* addModel(const std::string&name,const std::vector<size_t>&t){auto*n=new Node{Node::MODEL};n->modelName=name;n->targets=t;root->kids.push_back(n);return n;}
    Node* group(){auto*n=new Node{Node::GROUP};root->kids.push_back(n);return n;}
    Node* repeat(size_t k){auto*n=new Node{Node::REPEAT};n->times=k;root->kids.push_back(n);return n;}
    void build(){for(auto*k:root->kids) compile(k);} 
    void draw()const{for(size_t q=0;q<nq;++q) std::cout<<"q"<<q<<": "<<canvas[q]<<"\n";}
    const VectorXcd& getState() const { return state; }
}; 

//--------------------------------------------------
int main(){
    // Initialize model registry with quantum gates: H, CX, RX gates are defined in JSON files
    ModelRegistry reg;
    reg.load("h.json"); //Hadamard gate
    reg.load("cx.json"); //CNOT gate
    reg.load("rx.json"); //RX gate

    // Create a quantum circuit with 2 qubits
    QuantumCircuit qc(2,reg);
    // Add a Hadamard gate on qubit 0, repeat it 3 times, and add a CNOT gate between qubit 0 and qubit 1
    qc.addModel("H",{0}); auto r=qc.repeat(3); 
    r->kids.push_back(new Node{Node::MODEL, Node::MODEL, "CX", {0,1}});

    // Build and visualize the circuit
    qc.build();
    qc.draw();

    // Print the final state vector
    std::cout << "Final state vector:\n" << qc.getState() << "\n";

    return 0;
}