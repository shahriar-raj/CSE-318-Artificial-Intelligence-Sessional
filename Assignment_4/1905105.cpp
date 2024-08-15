#include <bits/stdc++.h>

using namespace std;

vector<vector<int>> attr;
int mis_match;

vector<string> split_string(string s){
    vector<string> tokens;
    string token="";
    for(int i=0; s[i]!='\0'; i++){
        if(s[i]==','){
            // token+='/0';
            tokens.push_back(token);
            token="";
            continue;
        }
        token+=s[i];
    }
    tokens.push_back(token);
    return tokens;
}

class Car{
public:
    // int buying, maint, doors, persons, lug_boot, safety, cla;
    vector<int> properties;
    int test_Result;
    Car(int, int, int, int, int, int, int); 
    Car();
};

Car :: Car(int buying, int maint, int doors, int persons, int lug_boot, int safety, int cla){
    properties.push_back(buying);
    properties.push_back(maint);
    properties.push_back(doors);
    properties.push_back(persons);
    properties.push_back(lug_boot);
    properties.push_back(safety);
    properties.push_back(cla); 
} 

Car :: Car(){
  
}

vector<vector<Car>> classify_for_specific_attribute(vector<Car> cars, int attribute){
    vector<vector<Car>> classes;
    for(int i=0; i<attr[attribute].size(); i++){
        vector<Car> sub_cars;
        classes.push_back(sub_cars);
    }
    for(int i=0; i<cars.size();i++){
        classes[cars[i].properties[attribute]].push_back(cars[i]);
    }
    return classes;
} 

double Entropy(vector<Car> cars){
    int count[attr[6].size()] ={0};
    vector<vector<Car>> class_classes = classify_for_specific_attribute(cars, 6);
    double entropy=0;
    for(int i=0; i<class_classes.size(); i++){
        if(class_classes[i].size()>0){ // IF we don't give this condition there can be division by 0 and nan will be answer then 
            double p = -(class_classes[i].size()/(1.0*cars.size()))*log2(class_classes[i].size()/(1.0*cars.size()));
            entropy+=p;
            // cout << entropy << endl;
        }
    } 
    return entropy;
}

double Info_Gain(vector<Car> cars, int attribute){
    double Rem=0, gain;
    double B_e = Entropy(cars);
    vector<vector<Car>> attr_classes = classify_for_specific_attribute(cars, attribute);
    for(int i=0; i<attr_classes.size(); i++){
        Rem+=(attr_classes[i].size()/(cars.size()*1.0))*Entropy(attr_classes[i]);
    }
    gain = B_e - Rem;
    // cout << "GAIN: " << gain << endl;
    return gain;
}

int plurality_value(vector<Car> cars){
    vector<vector<Car>> p_class = classify_for_specific_attribute(cars, 6);
    int p_value = -1;
    int p_result = -1;
    for(int i=0; i<p_class.size(); i++){
        int cmp = p_class[i].size();
        if( cmp > p_value){       
            p_value = p_class[i].size(); 
            p_result = i;
        }
    }
    // cout <<"Inside Result change: " <<  p_result << endl;
    return p_result;
}

pair<bool,int> same_classification(vector<Car> cars){
    bool out = true;
    int class_ = cars[0].properties[6];
    for(int i=0; i<cars.size()-1; i++){
        if(cars[i].properties[6]!=cars[i+1].properties[6]){
            out = false;
            break;
        }
    }
    return pair<bool, int> (out, class_) ;
}

int best_gain(vector<Car> cars, vector<bool> attr_used){
    int bg = -1;
    double g = -1;
    for(int i=0; i<attr_used.size(); i++){
        if(attr_used[i]==false){
            double c_gain = Info_Gain(cars, i);
            // cout << "Cg: " << c_gain << endl;
            if(c_gain>g){
                bg = i;
                g = c_gain;
                // cout << "BG: " << bg << endl;
            }
        }
    }
    return bg;
}

class Decision_Tree{
public:
    int used_attr;
    int result;
    bool hasResult;
    vector<Car> cars;
    vector<Decision_Tree*> subTree;
    Decision_Tree(void);
    void Decision_Tree_Learning(vector<Car> examples, vector<bool> attr_used, vector<Car> parent_examples);
    void test(Car testCar);
};

Decision_Tree :: Decision_Tree(void){
    used_attr = -1;
    result = -1;
    hasResult = false;
}

void Decision_Tree :: test(Car testCar){
    if(this->hasResult){
        testCar.test_Result = result;
        // cout << "R: " << result << endl; 
        if(result!=testCar.properties[6]){
            mis_match++;
        }
    }
    else{
        subTree[testCar.properties[used_attr]]->test(testCar);
    }
}

void Decision_Tree :: Decision_Tree_Learning(vector<Car> examples, vector<bool> attr_used, vector<Car> parent_examples){
    pair<bool,int> r;
    if(examples.size()!=0)
        r = same_classification(examples);
    if(examples.size()==0){
        hasResult = true;
        // cout << "YES1" <<endl;
        // cout << parent_examples.size() << endl;
        result = plurality_value(parent_examples);
        // cout <<"Result Change: " << result << endl;
        cars = examples;
    }
    else if(r.first){
         hasResult = true;
        //  cout << "YEs2" <<endl;
         cars = examples;
        //  cout << "Cars_size" << cars.size() << endl;
         result = r.second;
        //  cout <<"Result Change: " << result << endl;
    }
    else  if(attr_used[0] && attr_used[1] && attr_used[2] && attr_used[3] && attr_used[4] && attr_used[5]){
        hasResult = true;
        // cout << "YEs3" <<endl;
        result = plurality_value(examples);
        // cout <<"Result Change: " << result << endl;
        cars = examples;
    }
    else{
        used_attr = best_gain(examples, attr_used);
        // cout << "YEs4" <<endl;
        // cout <<used_attr << endl;
        // Sleep(500);
        cars = examples;
        vector<vector<Car>> split_class = classify_for_specific_attribute(examples,used_attr);
        attr_used[used_attr] = true;
        // cout << " Total Size:" << cars.size() << endl;

        // for(int i=0; i<attr[used_attr].size(); i++){
        //     cout <<"attr_value" << i << "Cars_size" << split_class[i].size() << endl;
        // }

        for(int i=0; i<attr[used_attr].size(); i++){
             Decision_Tree* Branch = new Decision_Tree();
             Branch->Decision_Tree_Learning(split_class[i],attr_used,examples);
             subTree.push_back(Branch);
        }
        // attr_used[used_attr] == false;
    }
}

int main(){

    // Maps the attribute values to integer to faster computation.
    unordered_map<string, int> b_m_Map = { {"vhigh", 0}, {"high", 1}, {"med", 2}, {"low", 3} };
    unordered_map<string, int> d_Map = { {"2", 0}, {"3", 1}, {"4", 2}, {"5more", 3} };
    unordered_map<string, int> p_Map = { {"2", 0}, {"4", 1}, {"more", 2}};
    unordered_map<string, int> l_Map = { {"small", 0}, {"med", 1}, {"big", 2}};
    unordered_map<string, int> s_Map = { {"low", 0}, {"med", 1}, {"high", 2}};
    unordered_map<string, int> cl_Map = { {"unacc", 0}, {"acc", 1}, {"good", 2}, {"vgood", 3} };

    for(int i=0; i<7; i++){
        vector<int> sa;
        attr.push_back(sa);
    }
    attr[0] = {0,1,2,3}; // 0-> buying, 1-.maint, 2->doors, 3->persons, 4->lug_space, 5->safety, 6->class
    attr[1] = {0,1,2,3};
    attr[2] = {0,1,2,3};
    attr[3] = {0,1,2};
    attr[4] = {0,1,2};
    attr[5] = {0,1,2};
    attr[6] = {0,1,2,3}; // Save attributes and their values

    vector<bool> attr_used;
    for(int i=0; i<6; i++){
        attr_used.push_back(false);
    }

    vector<Car> cars;
    vector<Car> trainCars;
    vector<Car> testCars;
    ifstream inputFile("car.data"); // Open the file for reading
    if (!inputFile) {
        cerr << "Error: Could not open the file." << endl;
        return 1;
    }

    string line;
    
    // Read and process lines until the end of the file is reached
    while (getline(inputFile, line)) {
        // Process each line, tokenize it with "," and set the car attribute values
        vector<string> tokens = split_string(line);
        Car car(b_m_Map[tokens[0]], b_m_Map[tokens[1]], d_Map[tokens[2]], p_Map[tokens[3]], l_Map[tokens[4]], s_Map[tokens[5]], cl_Map[tokens[6]]); 
        cars.push_back(car);
    }

    int sample_size = cars.size();
    int train = 0.8*(sample_size);
    cout << "train: " << train <<endl;
    double mean = 0;
    vector<double> accuracy_set;
    //shuffle the cars list to divide into test and train data and this part will be repeated
    for(int it=0; it<20; it++){
        trainCars.clear();
        testCars.clear();
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        default_random_engine rng(seed);
        shuffle(cars.begin(), cars.end(), rng);

        int i;
        
        for(i=0; i<train; i++){
            trainCars.push_back(cars[i]);
        }
        Decision_Tree tree ;
        tree.Decision_Tree_Learning(trainCars, attr_used, trainCars);
        // cout << "Done" << endl;
        mis_match = 0;
        for(;i<sample_size;i++){
            testCars.push_back(cars[i]);
            tree.test(cars[i]);
            // cout << "test:" << cars[i].properties[6] << endl;
        }
        
        inputFile.close(); // Close the file
        double accuracy = (1-(mis_match*1.0)/(sample_size-train))*100;
        mean += accuracy;
        accuracy_set.push_back(accuracy);
        cout <<"Iteration Number: " << it+1 <<  " Accuracy: " << accuracy << "%" << endl;
    }
    mean = mean/20;
    cout << "Mean accuracy: " << mean <<"%" <<  endl;
    double standard_deviation = 0;
    for(int i=0; i<accuracy_set.size();i++){
        standard_deviation += (mean-accuracy_set[i])*(mean-accuracy_set[i]);
    }
    standard_deviation/=20;
    standard_deviation = sqrt(standard_deviation);
    cout << "Standard Deviation: " << standard_deviation << "%"<< endl;
    return 0;
}