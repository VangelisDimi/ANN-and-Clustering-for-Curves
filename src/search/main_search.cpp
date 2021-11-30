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

#include "lsh.hpp"
#include "cube.hpp"

using namespace std;

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
    double delta; //-delta

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


        vector<vector<float>> vectors;
		vector<string> ids;
		read_file(input_file,vectors,ids);

		struct stat info;
		if (stat("./output",&info) == -1) {
			mkdir("./output", 0700);
		}
		ofstream outfile ("./output/" + output_file, ios::out | ios::trunc);

		vector<vector<float>> vectors_query;
		vector<string> ids_query;
		read_file(query_file,vectors_query,ids_query);
		unsigned int n_query=ids_query.size();

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