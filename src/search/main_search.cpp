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
#include "debug.hpp"
#include "lsh.hpp"
#include "cube.hpp"
#include "lsh_frechet.hpp"

using namespace std;

void write_file(ofstream &outfile,
				string query_id,vector<string> ids,vector<pair<float,unsigned int>> approximate_Nearest,vector<pair<float,unsigned int>> true_Nearest,
				string algorithm)
{
	outfile << "Query: " << query_id << endl;
	outfile << "Algorithm: " << algorithm << endl;
	
	if(!approximate_Nearest.empty()) outfile << "Approximate Nearest neighbor: " << ids[approximate_Nearest[0].second] << endl;
	if(!true_Nearest.empty()) outfile << "True Nearest neighbor: " << ids[true_Nearest[0].second] << endl;
	if(!approximate_Nearest.empty()) outfile << "distanceApproximate: " << approximate_Nearest[0].first << endl;
	if(!true_Nearest.empty()) outfile << "distanceTrue: " << true_Nearest[0].first << endl;
}

int main(int argc, char *argv[]){
	srand((time(0)));

	string input_file; //-i
	string output_file; //-o
    string query_file; //-q
    int k_lsh=4; //-k
    int L_lsh=5; //-L
    int k_cube=14; //-k
    int M_cube=10; //-M
    int probes_cube=2; //-probes
    string algorithm; //-algorithm
    string metric; //-metric
    double delta=0.01; //-delta

    int k;
    bool k_flag=false;

	//Read given arguments 
	int c;

	struct option long_opt[] =
	{
		{"probes",required_argument,NULL,'p'},
        {"algorithm",required_argument,NULL,'a'},
        {"metric",required_argument,NULL,'m'},
        {"delta",required_argument,NULL,'d'},
		{NULL,0,NULL,0}
	};
	while ((c = getopt_long_only(argc, argv, "i:o:q:k:L:M:p:a:m:d:",long_opt,NULL)) != -1)
	{
		switch (c)
		{
		case 'i':
			input_file = optarg;
			break;
        case 'o':
            output_file = optarg;
            break;
		case 'q':
			query_file = optarg;
			break;
		case 'k':
			k = stoi(optarg);
            k_flag=true;
			break;
		case 'L':
			L_lsh = stoi(optarg);
			break;
		case 'M':
			M_cube = stoi(optarg);
			break;
        case 'p':
            probes_cube = stoi(optarg);
			break;
        case 'a':
            algorithm = optarg;
            break;
        case 'm':
            metric = optarg;
            break;
        case 'd':
            delta = stod(optarg);
            break;
		default:
			exit (1);
		}
	}
    if(k_flag==true)
    {
        if(algorithm=="LSH") k_lsh=k;
        else if(algorithm=="Cube") k_cube=k;
    }

    while (true)
	{

        while(input_file.empty())
        {
            cout<<"Enter input file location: ";
            cin >> input_file;
        }
        while (query_file.empty())
        {
            cout<<"Enter query file location: ";
            cin >> query_file;
        }
        while (output_file.empty())
        {
            cout<<"Enter output file location: ";
            cin >> output_file;
        }

        while (algorithm.empty())
        {
            cout<<"Enter algorithm to be used: ";
            cin >> algorithm;
        }
        while(algorithm=="Frechet" && metric.empty())
        {
            cout<<"Enter metric to be used for LSH Frechet: ";
            cin >> metric;
        }


		struct stat info;
		if (stat("./output",&info) == -1) {
			mkdir("./output", 0700);
		}
		ofstream outfile ("./output/" + output_file, ios::out | ios::trunc);

		double maf=0;
		double time_approximate=0;
		double time_true=0;
		vector<pair<float,unsigned int>> approximate_Nearest;
		vector<pair<float,unsigned int>> true_Nearest;
		unsigned int n_query;

		if(algorithm=="Frechet")
		{
			int dim;
			if(metric=="discrete") dim=2;
			else if(metric=="continuous") dim=1;

			vector<vector<vector<float>>> curves;
			vector<string> ids;
			read_file_curve(input_file,curves,ids,dim);

			vector<vector<vector<float>>> curves_query;
			vector<string> ids_query;
			read_file_curve(query_file,curves_query,ids_query,dim);
			n_query=ids_query.size();
			
			if (metric=="discrete")
			{
				LSH_Frechet lsh_fd(curves,k_lsh,L_lsh,DFD,delta);
				for (unsigned int i=0 ; i<n_query ; i++)
				{
					auto start_lsh = chrono::high_resolution_clock::now();
					approximate_Nearest=lsh_fd.find_N_nearest(curves_query[i],1);
					auto stop_lsh = chrono::high_resolution_clock::now();
					auto elapsed_lsh = stop_lsh - start_lsh ;
					time_approximate += chrono::duration<double>(elapsed_lsh).count();

					auto start_true = chrono::high_resolution_clock::now();
					true_Nearest=exhaustive_search(curves_query[i],curves,1,&getDiscreteFrechetDistance);
					auto stop_true = chrono::high_resolution_clock::now();
					auto elapsed_true = stop_true - start_true ;
					time_true += chrono::duration<double>(elapsed_true).count();


					if(!approximate_Nearest.empty() && !true_Nearest.empty() && true_Nearest[0].first!=0)
					{
						double af=approximate_Nearest[0].first/true_Nearest[0].first;
						if(af>maf) maf=af;
					}
					write_file(outfile,ids_query[i],ids,approximate_Nearest,true_Nearest,"LSH_Frechet_Discrete");
				}
			}
			else if(metric=="continuous")
			{
				LSH_Frechet lsh_fc(curves,k_lsh,L_lsh,CFD,delta);
				ONE_DIM::filter(curves);
				ONE_DIM::filter(curves_query);
				for (unsigned int i=0 ; i<n_query ; i++)
				{
					auto start_lsh = chrono::high_resolution_clock::now();
					approximate_Nearest=lsh_fc.find_N_nearest(curves_query[i],1);
					auto stop_lsh = chrono::high_resolution_clock::now();
					auto elapsed_lsh = stop_lsh - start_lsh ;
					time_approximate += chrono::duration<double>(elapsed_lsh).count();

					auto start_true = chrono::high_resolution_clock::now();
					true_Nearest=exhaustive_search(curves_query[i],curves,1,&continuousFrechetDistance);
					auto stop_true = chrono::high_resolution_clock::now();
					auto elapsed_true = stop_true - start_true ;
					time_true += chrono::duration<double>(elapsed_true).count();

					if(!approximate_Nearest.empty() && !true_Nearest.empty() && true_Nearest[0].first!=0)
					{
						double af=approximate_Nearest[0].first/true_Nearest[0].first;
						if(af>maf) maf=af;
					}
					write_file(outfile,ids_query[i],ids,approximate_Nearest,true_Nearest,"LSH_Frechet_Continuous");
				}
			}
		}
		else
		{
			vector<vector<float>> vectors;
			vector<string> ids;
			read_file_vector(input_file,vectors,ids);

			vector<vector<float>> vectors_query;
			vector<string> ids_query;
			read_file_vector(query_file,vectors_query,ids_query);
			n_query=ids_query.size();

			if(algorithm=="LSH")
			{
				LSH lsh(vectors,k_lsh,L_lsh,L2);
				for (unsigned int i=0 ; i<n_query ; i++)
				{
					auto start_lsh = chrono::high_resolution_clock::now();
					approximate_Nearest=lsh.find_N_nearest(vectors_query[i],1);
					auto stop_lsh = chrono::high_resolution_clock::now();
					auto elapsed_lsh = stop_lsh - start_lsh ;
					time_approximate += chrono::duration<double>(elapsed_lsh).count();

					auto start_true = chrono::high_resolution_clock::now();
					true_Nearest=exhaustive_search(vectors_query[i],vectors,1,&eucledian_distance);
					auto stop_true = chrono::high_resolution_clock::now();
					auto elapsed_true = stop_true - start_true ;
					time_true += chrono::duration<double>(elapsed_true).count();

					if(!approximate_Nearest.empty() && !true_Nearest.empty() && true_Nearest[0].first!=0)
					{
						double af=approximate_Nearest[0].first/true_Nearest[0].first;
						if(af>maf) maf=af;
					}
					write_file(outfile,ids_query[i],ids,approximate_Nearest,true_Nearest,"LSH_Vector");
				}
			}
			else if(algorithm=="Hypercube")
			{
				Cube cube(vectors,k_cube,M_cube,probes_cube,L2);
				for (unsigned int i=0 ; i<n_query ; i++)
				{
					auto start_cube = chrono::high_resolution_clock::now();
					approximate_Nearest=cube.find_N_nearest(vectors_query[i],1);
					auto stop_cube = chrono::high_resolution_clock::now();
					auto elapsed_cube = stop_cube - start_cube ;
					time_approximate += chrono::duration<double>(elapsed_cube).count();

					auto start_true = chrono::high_resolution_clock::now();
					true_Nearest=exhaustive_search(vectors_query[i],vectors,1,&eucledian_distance);
					auto stop_true = chrono::high_resolution_clock::now();
					auto elapsed_true = stop_true - start_true;
					time_true += chrono::duration<double>(elapsed_true).count();

					if(!approximate_Nearest.empty() && !true_Nearest.empty() && true_Nearest[0].first!=0)
					{
						double af=approximate_Nearest[0].first/true_Nearest[0].first;
						if(af>maf) maf=af;
					}
					write_file(outfile,ids_query[i],ids,approximate_Nearest,true_Nearest,"Hypercube");
				}
			}
		}

		time_approximate/=n_query;
		time_true/=n_query;
		outfile << endl;
		outfile << "tApproximateAverage: " << time_approximate << endl;
		outfile << "tTrueAverage: " << time_true << endl;
		outfile << "MAF: " << maf << endl;
		outfile.close();

		return 0;
        string option;
		cout << "Enter /exit to exit program.\n";
		cout << "Enter /rerun to rerun program with new dataset.\n";
		while (true)
		{   
			cin >> option;
			if (option.compare("/rerun")==0)
			{
				input_file.clear();
				query_file.clear();
				output_file.clear();
				break;
			}
			if(option.compare("/exit")==0)
			{
				return 0;
			}
		}
    }
	return 0;
}