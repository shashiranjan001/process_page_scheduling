#include <bits/stdc++.h>
#include <time.h>
#include <fstream>
using namespace std;

float fcfs (int N,vector<pair<float,float> > &job ){
    vector<pair<float,float> > tep(job.begin(),job.end());
    vector<float>fin;
    sort(tep.begin(),tep.end());
    for(int i=0;i<N;i++){
        //cout<<"job arr time: "<<tep[i].first<<"\tjob burst time: "<<tep[i].second<<endl;
    }
    fin.push_back(tep[0].second);
    int ti=fin[0];
    for(int i=1;i<N;i++){
       fin.push_back(max(ti+tep[i].second,tep[i].first+tep[i].second));
       ti=max(ti+tep[i].second,tep[i].first+tep[i].second);
    }
    for(int i=0;i<N;i++){
           // cout<<"job fin time: "<<fin[i]<<endl;
    }
    float sum=0;
    for(int i=0;i<N;i++){
            sum+=(fin[i]-tep[i].first);
    }
    return sum/N;
}

float sjf (int N,vector<pair<float,float> > &A ){
    vector<pair<float,float> > job(A.begin(),A.end());
    priority_queue<pair<float,int> > que;
    priority_queue<pair<float,int> >pq;
    int trt=0;

    for(int i=0;i<N;i++){
        pq.push(make_pair(-1*job[i].first,i));
    }

    float time=0;
    pair<float,int> temp1,temp2;
    do{
        if(!pq.empty()){
            temp1=pq.top();
            pq.pop();
            que.push(make_pair(-1*(job[temp1.second].second),temp1.second));
          //  cout<<"job: "<<temp1.second<<"arrined at "<<-1*temp1.first<<endl;
            if(!pq.empty()){temp2=pq.top();
            while(temp2.first==temp1.first){
                pq.pop();
                que.push(make_pair(-1*(job[temp2.second].second),temp2.second));
         //       cout<<"jo0b: "<<temp2.second<<"arrined at "<<-1*temp2.first<<endl;
                if(pq.empty())break;
                temp2 = pq.top();
            }}
        time = -1*temp1.first;
      //  cout<<"time"<<time<<endl ;
        }
        float en = (pq.empty())?(float)INT_MAX:(-1*(pq.top().first));
      //  cout<<"en"<<en<<endl;
        if(!que.empty()){
                temp1 = que.top();
                while(((-1*temp1.first)<=en-time)){
                    que.pop();
                    time+=(-1*temp1.first);
               //     cout<<"job: "<<temp1.second<<"finished at "<<time<<endl;
                    trt+=(time-job[temp1.second].first);
                    if(que.empty())break;
                    temp1 = que.top();

                }
        }
        if((en>time)&&(!que.empty())){
                temp1 = que.top();
            que.pop();
        temp1.first+=(en-time);
        //cout<<"job: "<<temp1.second<<"remaining  "<<-1*temp1.first<<endl;
            time=en;
            que.push(temp1);

        }


    }while(!pq.empty());
    return trt/N;
}

float rr (int N,float quant,vector<pair<float,float> > &A ){
    vector<pair<float,float> > job(A.begin(),A.end());
    priority_queue<pair<float,int> >pq;
    int trt=0;

    for(int i=0;i<N;i++){
        pq.push(make_pair(-1*job[i].first,i));
    }
    queue<pair<float,int> > que;
    float time=0;
    pair<float,int> temp1,temp2;
    do{

        if(!que.empty()){
            temp1 = que.front();
                while(((-1*temp1.first)<=quant)){
                    que.pop();
                    time+=(-1*temp1.first);
                   // cout<<"job: "<<temp1.second<<"\tfinished at "<<time<<endl;
                    trt+=(time-job[temp1.second].first);
                    if(que.empty())break;
                    temp1 = que.front();

                }

        }
        if(!pq.empty()){
            temp1=pq.top();
            while(-1*temp1.first<=time){
                pq.pop();
                que.push(make_pair(-1*(job[temp1.second].second),temp1.second));
              //  cout<<"job: "<<temp1.second<<"\tarrived at "<<-1*temp1.first<<endl;
                if(pq.empty())break;
                    temp1=pq.top();

            }
        }
        if(!que.empty()){
                    temp1 = que.front();
                    if(((-1*temp1.first)>quant)){
                        que.pop();
                        temp1.first+=quant;
                        time+=quant;
                       // cout<<"job: "<<temp1.second<<"\tremaining  "<<-1*temp1.first<<"\ttime: "<<time<<endl;
                        que.push(temp1);
                    }
                }





    }while(!pq.empty()||!que.empty());
    return trt/N;

}

double ran_expo(double lambda){
    double u;
    u = rand() / (RAND_MAX + 1.0);
    return -log(1- u) / lambda;
}

int main()
{
    int f[3],s[3],r1[3],r2[3],r3[3],n[3];
    float av[15]={0};

    n[0]=10;
    n[1]=50;
    n[2]=100;
    int t=10;
    ofstream myfile;
      myfile.open ("output2.csv");
      myfile << "n=10,fcfs,sjf,rr-1,rr-2,rr-5,n=50,fcfs,sjf,rr-1,rr-2,rr-5,n=100,fcfs,sjf,rr-1,rr-2,rr-5\n";
    while(t--){
    srand(t*(unsigned)time(NULL));
            for(int j=0;j<3;j++){
               // cout<<"N : "<<n[j]<<endl;
            vector<pair<float,float> > job;
            job.clear();
            float arr,bur;
            double Lam=0.1;
            pair<float,float> pa;
            arr =0.000000;
            bur = rand()%20 +1 ;
            pa = make_pair(arr,bur);
            job.push_back(pa);
            for(int i=1;i<n[j];i++){

                arr=ran_expo(Lam)*Lam*10/log(RAND_MAX+1.0);
                bur = rand()%20 +1 ;
                pa = make_pair(arr,bur);
                job.push_back(pa);
            }
            /*cout<<"jobs\n";
            for(int i=0;i<n[j];i++){
                cout<<"job no: " <<i<<"\tarr time: "<<job[i].first<<"\tburst time: "<<job[i].second<<endl;
            }*/
            //cout<<endl;
           // cout<<"\nFirst come first serve\n\n";
            f[j]=fcfs(n[j],job);
            //cout<<"\nShortest job first\n\n";
            s[j]=sjf(n[j],job);
            //cout<<"\nRound Robin time quantum =1\n\n";
            r1[j]=rr(n[j],1,job);
            //cout<<"\nRound Robin time quantum =2\n\n";
            r2[j]=rr(n[j],2,job);
            //cout<<"\nRound Robin time quantum =5\n\n";
            r3[j]=rr(n[j],5,job);
        }
       /* for(int j=0;j<3;j++){
                cout<<"\n\n\nN : "<<n[j]<<endl;

            cout<<endl;
            cout<<"\nFirst come first serve\n\n";
            cout<<f[j];
            cout<<"\nShortest job first\n\n";
            cout<<s[j];
            cout<<"\nRound Robin time quantum =1\n\n";
            cout<<r1[j];
            cout<<"\nRound Robin time quantum =2\n\n";
            cout<<r2[j];
            cout<<"\nRound Robin time quantum =5\n\n";
            cout<<r3[j];
        }*/
        av[0]+=f[0];
        av[1]+=s[0];
        av[2]+=r1[0];
        av[3]+=r2[0];
        av[4]+=r3[0];
        av[5]+=f[1];
        av[6]+=s[1];
        av[7]+=r1[1];
        av[8]+=r2[1];
        av[9]+=r3[1];
        av[10]+=f[2];
        av[11]+=s[2];
        av[12]+=r1[2];
        av[13]+=r2[2];
        av[14]+=r3[2];
        myfile<<","<<f[0]<<","<<s[0]<<","<<r1[0]<<","<<r2[0]<<","<<r3[0]<<",,"<<f[1]<<","<<s[1]<<","<<r1[1]<<","<<r2[1]<<","<<r3[1]<<",,"<<f[2]<<","<<s[2]<<","<<r1[2]<<","<<r2[2]<<","<<r3[2]<<"\n";
    }
    for(int i=0;i<15;i++){
        av[i]=av[i]/10;
    }
    myfile<<"\nAverage,"<<av[0]<<","<<av[1]<<","<<av[2]<<","<<av[3]<<","<<av[4]<<",,"<<av[5]<<","<<av[6]<<","<<av[7]<<","<<av[8]<<","<<av[9]<<",,"<<av[10]<<","<<av[11]<<","<<av[12]<<","<<av[13]<<","<<av[14]<<"\n";

}
