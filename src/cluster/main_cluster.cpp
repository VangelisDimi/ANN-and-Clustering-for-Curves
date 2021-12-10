#include <iostream>
#include <ctime>
#include <cstring>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <chrono>

#include "cluster.hpp"
//#include "cluster_ANN.hpp"
#include "utils.hpp"

using namespace std;

void write_file(string output_file,
				vector<cluster::centroid> centroids,pair<vector<float>,float> silhouettes,vector<string> ids,
				double time_cluster,string algorithm,bool complete,bool silhouette,string update)
{
	struct stat info;
	if (stat("./output",&info) == -1) {
		mkdir("./output", 0700);
	}
	ofstream outfile ("./output/" + output_file, ios::out | ios::trunc);

	outfile << "Algorithm: " << algorithm << endl;
	for (int i = 0; i < (int) centroids.size(); i++)
	{
		outfile<<"CLUSTER-"<<i+1;
		outfile<<" {size: "<<centroids[i].vectors.size();
		outfile<<", centroid: ";
		
		outfile<<"[";
		int y=0;
		for(auto it = centroids[i].coordinates.begin(); it != centroids[i].coordinates.end(); ++it)
		{
			if(update==MEAN_V)
			{
				outfile<<*it;
				if(next(it,1)!=centroids[i].coordinates.end()) outfile<<",";
			}
			else if(update==MEAN_F)
			{
				outfile<<"("<<y<<",";
				outfile<<*it;
				outfile<<")";
				if(next(it,1)!=centroids[i].coordinates.end()) outfile<<",";
			}
			y++;
		}
		outfile<<"]}"<<endl;
	}

	outfile << "clustering_time: " << time_cluster << endl;

	if(silhouette)
	{
		outfile<<endl;
		outfile << "Silhouette: [";
		for(auto it = silhouettes.first.begin(); it < silhouettes.first.end(); ++it)
		{
			outfile<<*it<<",";
		}
		outfile<<silhouettes.second<<"]"<<endl;
	}

	if(complete)
	{
		outfile<<endl;
		for (int i = 0; i < (int) centroids.size(); i++)
		{
			outfile<<"CLUSTER-"<<i+1;
			outfile<<" { ";
			outfile<<"[";
			int y=0;
			for(auto it = centroids[i].coordinates.begin(); it != centroids[i].coordinates.end(); ++it)
			{
				if(update==MEAN_V)
				{
					outfile<<*it;
					if(next(it,1)!=centroids[i].coordinates.end()) outfile<<",";
				}
				else if(update==MEAN_F)
				{
					outfile<<"("<<y<<",";
					outfile<<*it;
					outfile<<")";
					if(next(it,1)!=centroids[i].coordinates.end()) outfile<<",";
				}
				y++;
			}
			outfile<<"]";

			for(auto it = centroids[i].vectors.begin(); it != centroids[i].vectors.end(); ++it)
			{
				outfile<<", "<<ids[it->index];
			}
			outfile<<"}"<<endl;
		}
	}

	outfile.close();
}

int main(int argc, char *argv[]){
	srand((time(0)));

	string input_file; //-i
	string configuration_file; //-c
	string output_file; //-o

	string update; //-update
	string assignment; //-assignment
    bool complete = false;//-complete
    bool silhouette = false; //-silhouette
	
    int K_cluster;
    int L_lsh=3;
    int k_lsh=4;
    int M_cube=10;
    int k_cube=3;
    int probes_cube=2;

    bool K_flag=false;

	//Read given arguments 
	int c;

	struct option long_opt[] =
	{
		{"update",required_argument,NULL,'u'},
		{"assignment",required_argument,NULL,'a'},
		{"complete",no_argument,NULL,'l'},
		{"silhouette",no_argument,NULL,'s'},
		{NULL,0,NULL,0}
	};
	while ((c = getopt_long_only(argc, argv, "i:c:o:u:a:ls",long_opt,NULL)) != -1)
	{
		switch (c)
		{
		case 'i':
			input_file = optarg;
			break;
		case 'c':
			configuration_file = optarg;
			break;
		case 'o':
			output_file = optarg;
			break;
		case 'u':
			update= optarg;
			break;
		case 'a':
			assignment = optarg;
			break;
		case 'l':
			complete=true;
			break;
		case 's':
			silhouette=true;
			break;
		default:
			exit (1);
		}
	}

	while(input_file.empty())
	{
		cout<<"Enter input file location: ";
		cin >> input_file;
	}
	while (configuration_file.empty())
	{
		cout<<"Enter configuration file location: ";
		cin >> configuration_file;
	}
	while (output_file.empty())
	{
		cout<<"Enter output file location: ";
		cin >> output_file;
	}

    //Read configuration file
    ifstream  file(configuration_file);
	string  line;
	while(getline(file, line))
	{
        string key;
        istringstream is_line(line);
        if(getline(is_line, key, ':'))
        {
            int value;
            is_line >> value;
            
            if(key=="number_of_clusters")
            {
                K_cluster=value;
                K_flag=true;
            }
            else if (key=="number_of_vector_hash_tables")
                L_lsh=value;
            else if (key=="number_of_vector_hash_functions")
                k_lsh=value;
            else if (key=="max_number_M_hypercube")
                M_cube=value;
            else if (key=="number_of_hypercube_dimensions")
                k_cube=value;
            else if (key=="number_of_probes")
                probes_cube=value;
        }
	}

	if(K_flag==false)
	{
		cout<<"Enter number of clusters to be created: ";
		cin>>K_cluster;
	}

	vector<vector<float>> vectors;
	vector<string> ids;
	read_file(input_file,vectors,ids);
	if(assignment=="Classic" && (update==MEAN_V || update==MEAN_F))
	{
		pair<vector<float>,float> silhouettes;
		auto start_cluster = chrono::high_resolution_clock::now();
		cluster_lloyds cluster(K_cluster,vectors,update);
		auto stop_cluster = chrono::high_resolution_clock::now();
		auto elapsed_cluster = stop_cluster - start_cluster ;
		double time_cluster = chrono::duration<double>(elapsed_cluster).count();

		if(silhouette) silhouettes=cluster.get_silhouettes_average();
		write_file(output_file,cluster.get_clusters(),silhouettes,ids,time_cluster,"Lloyds with "+update,complete,silhouette,update);
	}

	
	return 0;
}