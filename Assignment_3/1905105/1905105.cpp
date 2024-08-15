#include <bits/stdc++.h>

using namespace std;

double random_d(){
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> distribution(0.00001, 0.99999);
    double random_value = distribution(gen);
    return random_value;
}

class Graph{
public: 
    int v,e;
    int **adj_mat;
    int w_min, w_max;
    int save_w_min, save_w_max;
    // vector<vector<int>> neighbor;
    vector<pair<int,int>> RCL_e;
    int *vertices;
    vector<int> x;
    vector<int> y;
    vector<int> s;
    vector<int> s_;
    Graph(int, int);
    int Grasp(int);
    int semi_greedy();
    int greedy();
    int randomized();
    pair<int,int> local_search();
    int sigma_x(int);
    int sigma_y(int);
    void init();
};

Graph :: Graph(int v, int e){
    this->v= v; this->e = e;
    adj_mat = new int*[v];
    vertices = new int[v];
    w_min = 100000000;
    w_max = -100000000;
    vector<int> n;
    int a,b,w;
    for(int i=0; i<v; i++){
        adj_mat[i] = new int[v];
        // neighbor.push_back(n);
    }
    for(int i=0; i<v; i++){
        for(int j=0; j<v; j++){
            adj_mat[i][j] = 0;
        }
    }
    for(int i=0; i<e; i++){
        cin >> a >> b >> w;
        // inputFile >> a >> b >> w;
        // cout << a <<" " <<  b <<" " << w <<endl;
        adj_mat[a-1][b-1] = w;
        adj_mat[b-1][a-1] = w;
        // neighbor[a-1].push_back(b-1);
        // neighbor[b-1].push_back(a-1);
        if(w<w_min && w!=0){
            w_min = w;
        }
        else if(w>w_max && w!=0){
            w_max = w;
        }
        //cout << adj_mat[a-1][b-1] << endl;
    }
    save_w_max = w_max;
    save_w_min = w_min;
}

void Graph :: init(){
    x.clear();
    y.clear();
    s.clear();
    s_.clear();
    RCL_e.clear();
    vertices = new int[v];
    for(int i=0; i<v;i++){
        vertices[i] = 0;
    }
    w_max = save_w_max;
    w_min = save_w_min;
}

int Graph :: sigma_x(int n){
    int sum = 0;
    for(int i=0; i<y.size(); i++){
        sum += adj_mat[n][y[i]]; 
    }
    return sum;
}

int Graph :: sigma_y(int n){
    int sum = 0;
    for(int i=0; i<x.size(); i++){
        sum += adj_mat[n][x[i]]; 
    }
    return sum;
}

int Graph :: semi_greedy(){
    double alpha = random_d();
    double meu = w_min + alpha*(w_max-w_min);
    // cout << ",in: " << w_min << "max: " <<w_max <<endl;
    // cout << "alpha" << alpha << "mu: " << meu << "v: " << v<< "e: " << e << endl;
    for(int i=0;i<v ; i++){
        for(int j=0; j<v; j++){
            if(adj_mat[i][j]>=meu && adj_mat[i][j]!=0){
                // cout << i << " " << j << endl;
                pair<int,int> a(i,j);
                RCL_e.push_back(a);
            }
        }
    }
    // cout <<"size: " <<  RCL_e.size() << endl;
    int r = rand()%RCL_e.size();
    vertices[RCL_e[r].first] = 1;
    x.push_back(RCL_e[r].first);
    // cout << "X1:" << x[0] << endl;
    vertices[RCL_e[r].second] = 2;
    y.push_back(RCL_e[r].second);
    // cout << "Y1:" << y[0] << endl;
    while(true){
        int* sx = new int[v];
        int* sy = new int[v];
        vector<int> RCL_v;
        bool trace = true;
        for(int i=0; i<v; i++){  
            if(!vertices[i]){
                sx[i] = sigma_x(i);
                sy[i] = sigma_y(i);
                // cout << i << " sx: " << sx[i] << "sy: " << sy[i] <<endl;
                if(trace){
                    w_min = min (sx[i],sy[i]);
                    w_max = max(sx[i],sy[i]);
                    trace = false;
                }
                if(sx[i]<w_min){
                    w_min = sx[i];
                }
                if(sy[i]<w_min){
                    w_min = sy[i];
                }
                if(sx[i]>w_max){
                    w_max = sx[i];
                }
                if(sy[i]>w_max){
                    w_max = sy[i];
                }
            }
        }
        meu = w_min + alpha*(w_max-w_min);
        // cout << "w_min : " << w_min << " w_max: " <<w_max << " mu: " << meu << endl;
        for(int i=0; i<v; i++){
            if(!vertices[i] && max(sx[i],sy[i])>=meu){
                RCL_v.push_back(i);
                // cout << "RCL_v: " << i << endl;
            }
        }
        if(RCL_v.size()==0){
            break;
        }
        r = rand()%RCL_v.size();
        if(sx[RCL_v[r]]>sy[RCL_v[r]]){
            x.push_back(RCL_v[r]);
            vertices[RCL_v[r]]=1;
        }
        else{
            y.push_back(RCL_v[r]);
            vertices[RCL_v[r]]=2;
        }
        int product = 1;
        for(int i=0; i<v; i++){
            product*=vertices[i];
        }
        if(product){
            break;
        }
    }
    // int cx=0, cy=0;
    // for(int l=0; l<x.size(); l++){
    //     cx++;
    // }
    // cout << "cx" << cx << endl;
    // for(int l=0; l<y.size(); l++){
    //    cy++;
    // }
    // cout << "cy" << cy << endl;
    long long int max_cut = 0;
    for(int i=0; i<x.size(); i++){
        for(int j=0; j<y.size(); j++){
            max_cut+=adj_mat[x[i]][y[j]];
        }
    }
    s = x;
    s_ = y;
    // cout << "Semi-Greedy max cut: " << max_cut << endl;
    return max_cut;
}

int Graph :: greedy(){
    // cout << ",in: " << w_min << "max: " <<w_max <<endl;
    // cout << "alpha" << alpha << "mu: " << meu << "v: " << v<< "e: " << e << endl;
    for(int i=0;i<v ; i++){
        for(int j=0; j<v; j++){
            if(adj_mat[i][j]==w_max){
                // cout << i << " " << j << endl;
                pair<int,int> a(i,j);
                RCL_e.push_back(a);
            }
        }
    }
    // cout <<"size: " <<  RCL_e.size() << endl;
    int r = rand()%RCL_e.size();
    vertices[RCL_e[r].first] = 1;
    x.push_back(RCL_e[r].first);
    // cout << "X1:" << x[0] << endl;
    vertices[RCL_e[r].second] = 2;
    y.push_back(RCL_e[r].second);
    // cout << "Y1:" << y[0] << endl;
    while(true){
        int* sx = new int[v];
        int* sy = new int[v];
        vector<int> RCL_v;
        bool trace = true;
        for(int i=0; i<v; i++){  
            if(!vertices[i]){
                sx[i] = sigma_x(i);
                sy[i] = sigma_y(i);
                // cout << i << " sx: " << sx[i] << "sy: " << sy[i] <<endl;
                if(trace){
                    w_min = min (sx[i],sy[i]);
                    w_max = max(sx[i],sy[i]);
                    trace = false;
                }
                if(sx[i]<w_min){
                    w_min = sx[i];
                }
                if(sy[i]<w_min){
                    w_min = sy[i];
                }
                if(sx[i]>w_max){
                    w_max = sx[i];
                }
                if(sy[i]>w_max){
                    w_max = sy[i];
                }
            }
        }
        // cout << "w_min : " << w_min << " w_max: " <<w_max << " mu: " << meu << endl;
        for(int i=0; i<v; i++){
            if(!vertices[i] && max(sx[i],sy[i])==w_max){
                RCL_v.push_back(i);
                // cout << "RCL_v: " << i << endl;
            }
        }
        if(RCL_v.size()==0){
            break;
        }
        r = rand()%RCL_v.size();
        if(sx[RCL_v[r]]>sy[RCL_v[r]]){
            x.push_back(RCL_v[r]);
            vertices[RCL_v[r]]=1;
        }
        else{
            y.push_back(RCL_v[r]);
            vertices[RCL_v[r]]=2;
        }
        int product = 1;
        for(int i=0; i<v; i++){
            product*=vertices[i];
        }
        if(product){
            break;
        }
    }
    long long int max_cut = 0;
    for(int i=0; i<x.size(); i++){
        for(int j=0; j<y.size(); j++){
            max_cut+=adj_mat[x[i]][y[j]];
        }
    }
    s = x;
    s_ = y;
    // cout << "Greedy max cut: " << max_cut << endl;
    return max_cut;
}

int Graph :: randomized(){
    for(int i=0; i<v; i++){
        int r = rand()%2;
        if(!r){
            x.push_back(i);
            vertices[i]=1;
        }
        else{
            y.push_back(i);
            vertices[i]=2;
        }
    }
    // cout << x.size() << " " << y.size() << endl;
    long long int max_cut = 0;
    for(int i=0; i<x.size(); i++){
        for(int j=0; j<y.size(); j++){
            max_cut+=adj_mat[x[i]][y[j]];
        }
    }
    s = x;
    s_ = y;
    // cout << "Randomized max cut: " << max_cut << endl;
    return max_cut;
}

pair<int,int> Graph :: local_search(){
    bool change = true;
    int no_iteration = 0;
    while(change){
        no_iteration++;
        change = false;
        for(int i=0; i<v && (!change); i++){
            if(vertices[i]==1 && (sigma_y(i)-sigma_x(i)>0)){
                vertices[i] = 2;
                s.erase(remove(s.begin(), s.end(), i), s.end());
                s_.push_back(i);
                // cout << i << " S->S\'" << endl;
                change = true;
            }
            else if(vertices[i]==2 && (sigma_x(i)-sigma_y(i)>0)){
                vertices[i] = 1;
                s_.erase(remove(s_.begin(), s_.end(), i), s_.end());
                s.push_back(i);
                // cout << i << " S\'->S" << endl;
                change = true;
            }
        }
    }
    int cx=0, cy=0;
    for(int l=0; l<s.size(); l++){
        cx++;
    }
    // cout << "cx" << cx << endl;
    for(int l=0; l<s_.size(); l++){
       cy++;
    }
    // cout << "cy" << cy << endl;
    long long int max_cut = 0;
    for(int i=0; i<s.size(); i++){
        for(int j=0; j<s_.size(); j++){
            max_cut+=adj_mat[s[i]][s_[j]];
        }
    }
    // cout << "Number of Iteration:" << no_iteration << endl; 
    // cout << "Local Search max cut: " << max_cut << endl;
    pair<int,int> result(max_cut,no_iteration);
    return result;
}

int Graph :: Grasp(int max_it){
    long long int semi_greedy_avg=0, local_search_avg=0, greedy_avg=0,rand_avg=0, grasp_best=0, local_iter=0;
    vector<int> final_s;
    vector<int> final_s_;
    for(int i=0; i<max_it; i++){
        // int sg = semi_greedy();
        int g = greedy();
        pair<int,int> ls = local_search();
        if(i==1){
            grasp_best = ls.first;
        }
        else if(ls.first>grasp_best){
            grasp_best = ls.first;
            final_s = s;
            final_s_ = s_;
        }
        // semi_greedy_avg+=sg;
        greedy_avg+=g;
        local_search_avg+=ls.first;
        local_iter+=ls.second;
        init();
        // int g = greedy();
        int sg = semi_greedy();
        // greedy_avg+=g;
        semi_greedy_avg+=sg;
        init();
        int rg = randomized();
        rand_avg+=rg;
        init();
    }
    // cout << "semi-greedy: " << semi_greedy_avg/max_it << endl;
    // cout << "greedy: " << greedy_avg/max_it << endl; 
    // cout << "Randomized: " << rand_avg/max_it << endl; 
    // cout << "local_search: " << local_search_avg/max_it << endl; 
    // cout << "local_search Iteration: " << local_iter/max_it << endl;
    cout << v << "," << e << "," << rand_avg/max_it << "," << greedy_avg/max_it << "," ;
    cout << semi_greedy_avg/max_it << "," << local_iter/max_it << "," << local_search_avg/max_it << ",";
    cout << max_it << "," <<grasp_best;
    return grasp_best;
}

int main(int argc, char* argv[]){
    int v, e, max_it=50;
    freopen( argv[1] , "r", stdin);
    freopen( argv[2] , "a", stdout);
    cout << argv[1] << ",";
    // inputFile >> v  >> e;
    cin >> v  >> e;
    Graph A(v,e);
    A.Grasp(max_it);
    cout << endl;
    // cout<< "Grasp: " << A.Grasp(max_it) << endl;
    // inputFile.close();
    return 0;
}