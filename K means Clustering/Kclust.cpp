#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>

class point2d{
    public:
        double x;double y;

        point2d(double x2,double y2){
            x=x2;y=y2;
        }

        double distance(point2d that){
            return sqrt( 
                pow( (this->x-that.x) ,2) 
                + 
                pow( (this->y-that.y) ,2) );
        }

        bool sameAs(point2d that){
            if(this->x==that.x && this->y==that.y){
                return true;
            }else{
                return false;
            }
        }

        void print(){
            std::cout<<x<<"  "<<y<<std::endl;
        }
};


//global variable
std::vector<point2d> data;
std::vector<point2d> *cluster1=new std::vector<point2d>;
std::vector<point2d> *cluster2=new std::vector<point2d>;
std::vector<point2d> *cluster3=new std::vector<point2d>;


void readingFile(){
    // open file for reading
    std::ifstream input("iris.data");
    if (!input.is_open()) {
        std::cout << "failed to open " << std::endl;
    } else {

        std::string temp;
        point2d temp2 = point2d(0,0);
        int i=0;

        while(input>>temp){
            //std::cout<<"I am readingfiles input>>temp"<<std::endl;
            //data formating
            switch(i){
                case(0):
                    i++;
                    break;
                case(1):
                    temp2.x=std::stod(temp);
                    i++;
                    break;
                case(2):
                    temp2.y=std::stod(temp);
                    i=0;//reset

                    data.push_back(temp2);//putting into vector
                    break;
                default:
                    break;
            }
            


        }
    }
}

void exportFile(std::vector<point2d> clust,char type){
    std::ofstream output;
    output.open("iris.out",std::ios::app);
    for(auto item: clust){
        output<<type<<" "<<item.x<<" "<<item.y<<std::endl;
    }
    output.close();
}

point2d newcentroid(std::vector<point2d> cluster){
    double sumx=0;
    double sumy=0;
    point2d a=point2d(0,0);
    for(auto item: cluster){
        sumx+=item.x;
        sumy+=item.y;
    }
    a.x=sumx/cluster.size();
    a.y=sumy/cluster.size();
    
    return a;
}


std::vector<point2d> clustering(point2d a,point2d b,point2d c){
    bool nochange=false;
    point2d centroid1=a;
    point2d centroid2=b;
    point2d centroid3=c;
    point2d first1=a;point2d first2=b;point2d first3=c;
    while(!nochange){
        //std::cout<<"I am nochange"<<std::endl;
        for(auto item: data){//find item closest to which centroid
            if( item.distance(centroid1)<=item.distance(centroid2) && item.distance(centroid1)<=item.distance(centroid3) ){
                cluster1->push_back(item);
            }else if( item.distance(centroid2)<=item.distance(centroid1) && item.distance(centroid2)<=item.distance(centroid3) ){
                cluster2->push_back(item);
            }else{
                cluster3->push_back(item);
            }
        }
        point2d prevcent1=centroid1;
        point2d prevcent2=centroid2;
        point2d prevcent3=centroid3;
        if(cluster1->size()==0 || cluster2->size()==0 || cluster3->size()==0){//redo
            return std::vector<point2d>{point2d(0,0),point2d(0,0),point2d(0,0)};
        }
        centroid1=newcentroid(*cluster1);
        centroid2=newcentroid(*cluster2);
        centroid3=newcentroid(*cluster3);

        if(centroid1.sameAs(prevcent1) && centroid2.sameAs(prevcent2) && centroid3.sameAs(prevcent3)){
            nochange=true;
        }else{
            //de-re-allocating memory
            delete cluster1;
            delete cluster2;
            delete cluster3;
            cluster1 = new std::vector<point2d>;
            cluster2 = new std::vector<point2d>;
            cluster3 = new std::vector<point2d>;
        }
    }
    return std::vector<point2d>{centroid1,centroid2,centroid3};

}


int * randStartGenerator(){
    //randomize starting point
    srand (time(NULL));
    int num = rand() % data.size();
    int num2 = rand() % data.size();
    
    while(num2==num){
        num2 = rand() % data.size();
    }
    int num3 = rand() % data.size();
    while(num3==num || num3==num2){
        num3 = rand() % data.size();
    }
    return new int[3]{num,num2,num3};
}



int main(){

    readingFile();
    
    //do once before entering loop
    int *r=randStartGenerator();
    std::vector<point2d> result=clustering(data[r[0]],data[r[1]],data[r[2]]);


    bool consecutivelySameResult=false;
    while(!consecutivelySameResult){
        //std::cout<<"I am consecutivelySameResult"<<std::endl;

        int *r=randStartGenerator();

        std::vector<point2d> prevresult=result;
        std::vector<point2d> result=clustering(data[r[0]],data[r[1]],data[r[2]]);

        if(result[0].sameAs(result[1]) && result[0].sameAs(result[2])){
            //centroid1=centroid2=centroid3 => error
            result=prevresult;
        }
        else if(result[0].sameAs(prevresult[0]) && result[1].sameAs(prevresult[1]) && result[2].sameAs(prevresult[2])){
            //all centroid same as previous
            consecutivelySameResult=true;
        }

    }
    char type='A';
    for(auto item: result){
        std::cout<<type<<" "<<item.x<<" "<<item.y<<std::endl;
        type++;
    }
    
    exportFile(*cluster1,'A');
    exportFile(*cluster2,'B');
    exportFile(*cluster3,'C');

    return 0;
}


