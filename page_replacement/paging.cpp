#include <iostream>
#include<bits/stdc++.h>

using namespace std;



int lru(vector< pair<int,int> > &frame_struct)
{
    int min =0;
    for(int i=1;i<frame_struct.size();i++)
        if(frame_struct[i].second< frame_struct[min].second)
            min = i;

    return min;
}

int nru(vector< pair<int,int> > &frame_struct, int *page_table, int counter, int last_use_ptr)
{
    //cout<<"hii i'm inside nru func \n";
    if(counter%1000 == 999)
    {
        cout<<"Inside If \n";
        int j=0;
        while(j < frame_struct.size())
        {
            bitset<32> page_table_entry(page_table[j]);
            page_table_entry.set(0,0);
            int temp_num = (int)(page_table_entry.to_ulong());
            page_table[j] = temp_num;
            j++;
        }
    }

    else
    {
        // cout<<"outside If \n";
        // cout<<"last_use_ptr = "<< last_use_ptr <<"\n";
         int k= last_use_ptr;
         int count = 0;
        // cout<<"frame struct size = "<<frame_struct.size()<<"\n";
        // bitset<32> t(page_table[0]);
        // cout<<"page : "<< t <<"\n";
        while(count < frame_struct.size())
        {
            bitset<32> page_table_entry(page_table[k]);
            //cout<<"page : "<< page_table_entry <<"\n";
            if(page_table_entry[0] == 0)
            {
                cout<<"inside if\n";
                return k;
            }
            else k = (last_use_ptr+count)%frame_struct.size();
            count++;
        }

        return ((k+2)%frame_struct.size());
    }
}


int second_chance(vector< pair<int,int> > &frame_struct, int *page_table, int last_use_ptr)
{
    int k= last_use_ptr;
    int count = 0;

    while(1)
    {
        bitset<32> page_table_entry(page_table[k]);
        if(page_table_entry[0] == 0) return k;
        else
        {
            page_table_entry.set(0,0);
            int temp_num = (int)(page_table_entry.to_ulong());
            page_table[k] = temp_num;
            k = (k+1)%frame_struct.size();

        }
    }

    //return ((k+2)%frame_struct.size());
}


int main(int argc, char const *argv[])
{

    if(argc !=3)
    {
        cout<<"Usage :  " << argv[0]<<" <num_frames>  <input_file.txt>\n";
        exit(0);
    }
    ifstream infile(argv[2]);
    vector< pair<int,int> > vec;

    int frame_count, curr_frame_count = 0;
    int page_table[64];
    int ptr = 0, last_use_ptr = 0, counter =0;
    frame_count = atoi(argv[1]);

    int replacement_policy;
    cout<<"Enter the replaement policy \n";
    cout<<"FIFO : 0\n";
    cout<<"Random : 1\n";
    cout<<"LRU : 2\n";
    cout<<"NRU : 3\n";
    cout<<"Second Chance : 4\n";
    cout<<"\nYour Choice : ";
    cin>>replacement_policy;
    vector< pair<int,int> > frame_struct(frame_count);
    int page_fault = 0, page_transfer = 0, read_count = 0, write_count = 0, map_count =0, unmap_count =0;

    string line;
    while (getline(infile, line))
    {
        istringstream iss(line);
        int a, b;
        if (line[0] == '#') ;
        else
        {
            iss >> a >> b;
            vec.push_back(make_pair(a,b));
        }
    }

    vec.erase(vec.end());
    cout<< "size of vec : "<<vec.size()<<endl<<endl;
    // for(int i = 0; i<vec.size(); i++)
    //     cout<<"a = "<<vec[i].first<<"  b = "<<vec[i].second<<endl;

    int victim_index;
    for(int i = 0; i<vec.size(); i++)
    {
        cout<<"a  =  "<<vec[i].first<<"   b  =  "<<vec[i].second<<endl;
        if(vec[i].second > 64) continue;
        int j =0;
        int flag = 0;
        int dirty;
        while(j < curr_frame_count)
        {
            bitset<32> page_table_entry(page_table[j]);
            bitset<29> temp;
            for(int k = 28; k >=0; k--) temp.set(k, page_table_entry[k+3]);
            dirty = page_table_entry[1];
            int temp_num = (int)(temp.to_ulong());

            if(temp_num == vec[i].second)
            {
                flag =1;
                break;
            }

            j++;
        }


        if(flag == 0)
        {
            //cout<<"flag = "<<flag<<endl;
            page_fault++;
            if(page_fault == 10 && replacement_policy == 2)
            {
                int j=0;
                while(j < frame_struct.size())
                {
                    bitset<32> page_table_entry(page_table[j]);
                    page_table_entry.set(0,0);
                    int temp_num = (int)(page_table_entry.to_ulong());
                    page_table[j] = temp_num;
                    j++;
                }
            }


            if(curr_frame_count < frame_count)
            {
                // cout<<"counter : "<<counter<<endl;
                cout<<i+1<<" : "<<" MAP\t" << vec[i].second << "\t"<<curr_frame_count << endl;
                map_count++;

                if(vec[i].first == 0)
                {
                    cout<<i+1<<" : "<<" READ\t"<< j <<endl<<endl<<endl<<endl;
                    read_count++;
                }
                else
                {
                    cout<<i+1<<" : "<<" WRITE\t"<< j <<endl<<endl<<endl<<endl;
                    write_count++;
                }

                frame_struct[curr_frame_count].first = vec[i].second;
                frame_struct[curr_frame_count].second = counter;
                counter++;
                //cout<<"frame_struct["<<curr_frame_count<<"] = "<<frame_struct[curr_frame_count]<<endl;
                bitset<29> temp_set(vec[i].second);
                bitset<32> t;
                for(int k = 31; k >=3; k--) t.set(k, temp_set[k-3]);
                t.set(2, 1);
                if(vec[i].first == 0) t.set(1, 0);
                else t.set(1, 1);
                t.set(0, 1);
                int number = (int)(t.to_ulong());
                page_table[curr_frame_count] = number;
                curr_frame_count++;

            }

            else
            {
                // cout<<"counter : "<<counter<<endl;
                switch(replacement_policy)
                {
                    case 0: victim_index = ptr;
                    ptr = (ptr + 1)%frame_count ;
                    break;

                    case 1: victim_index = rand()%frame_count;
                    break;

                    case 2 :victim_index = lru(frame_struct);
                    break;

                    case 3://cout<<"hii i'm here\n";
                    victim_index = nru(frame_struct,page_table,counter,last_use_ptr);
                    last_use_ptr = victim_index;
                    break;

                    case 4: victim_index = second_chance(frame_struct, page_table,last_use_ptr);
                    last_use_ptr = victim_index;
                    break;


                }
                cout<<i+1<<" : "<<" UNMAP\t"<<frame_struct[victim_index].first<<"\t"<<victim_index<<endl;
                unmap_count++;
                if(dirty == 1)
                {
                    cout<<i+1<<" : "<<" OUT\t"<<frame_struct[victim_index].first<<"\t"<<victim_index<<endl;
                    page_transfer++ ;
                }
                cout<<i+1<<" : "<<" IN\t"<<vec[i].second<<"\t"<<victim_index<<endl;
                page_transfer++ ;
                cout<<i+1<<" : "<<" MAP\t"<<vec[i].second<<"\t"<<victim_index<<endl;
                map_count++;

                if(vec[i].first == 0)
                {bitset<32> t;
                    cout<<i+1<<" : "<<" READ\t"<<  vec[i].second <<endl<<endl<<endl<<endl;
                    read_count++;
                }
                else
                {
                    cout<<i+1<<" : "<<" WRITE\t"<< vec[i].second <<endl<<endl<<endl<<endl;
                    write_count++;
                }

                bitset<29> temp_set(vec[i].second);
                bitset<32> t;
                ptr = (ptr + 1)%frame_count ;
                for(int k = 31; k >=3; k--) t.set(k, temp_set[k-3]);
                t.set(2, 1);
                if(vec[i].first == 0) t.set(1, 0);
                else t.set(1, 1);
                t.set(0, 1);
                int number = (int)(t.to_ulong());
                page_table[victim_index] = number;
                frame_struct[victim_index].first = vec[i].second;
                frame_struct[victim_index].second = counter;
                counter++;
                // bitset<32> tt(page_table[last_use_ptr]);
                //
                // cout<<"last_use_ptr = "<< last_use_ptr<< " ref_bit = " <<tt[0]<<endl<<endl<<endl<<endl;
            }

        }

        else
        {
            // cout<<"counter : "<<counter<<endl;
            if(vec[i].first == 0)
            {
                cout<<i+1<<" : "<<" READ  : "<< vec[i].second <<endl<<endl<<endl<<endl;
                read_count++;
            }
            else
            {
                cout<<i+1<<" : "<<" WRITE  : "<< vec[i].second <<endl<<endl<<endl<<endl;
                write_count++;
            }

            bitset<32> page_table_entry(page_table[j]);
            page_table_entry.set(0,1);
            int temp_num = (int)(page_table_entry.to_ulong());
            page_table[j] = temp_num;



            frame_struct[j].second = counter;

            counter++;
        }

    }

    cout<< "--------- Summary Statistics----------\n";
    cout<<"\t Number of Page faults =\t"<<page_fault<<endl;
    cout<<"\t Number of Page transfers =\t"<<page_transfer<<endl;
    int t = (map_count +  unmap_count)*250 + (page_transfer)*3000 + (read_count + write_count)*1;
    cout<<"\t Total time taken  for all the operations is =\t"<<t<<" cycles"<<endl;
    return 0;
}
