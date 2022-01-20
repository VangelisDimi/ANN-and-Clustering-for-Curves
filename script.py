import os
os.system("make search ALGORITHM=Frechet METRIC=continuous OUTPUT_FILE=og_search_algorithm_Frechet_metric_continuous")
os.system("make search ALGORITHM=Frechet METRIC=discrete OUTPUT_FILE=og_search_algorithm_Frechet_metric_discrete")
os.system("make search ALGORITHM=LSH OUTPUT_FILE=og_search_algorithm_LSH")
os.system("make search ALGORITHM=Hypercube OUTPUT_FILE=og_search_algorithm_Hypercube")

os.system("make search ALGORITHM=Frechet METRIC=continuous OUTPUT_FILE=og_search_algorithm_Frechet_metric_continuous INPUT_FILE=./examples/Datasets/encoded_input.csv QUERY_FILE=./examples/Datasets/encoded_query.csv")
os.system("make search ALGORITHM=Frechet METRIC=discrete OUTPUT_FILE=og_search_algorithm_Frechet_metric_discrete INPUT_FILE=./examples/Datasets/encoded_input.csv QUERY_FILE=./examples/Datasets/encoded_query.csv")
os.system("make search ALGORITHM=LSH OUTPUT_FILE=og_search_algorithm_LSH INPUT_FILE=./examples/Datasets/encoded_input.csv QUERY_FILE=./examples/Datasets/encoded_query.csv")
os.system("make search ALGORITHM=Hypercube OUTPUT_FILE=og_search_algorithm_Hypercube INPUT_FILE=./examples/Datasets/encoded_input.csv QUERY_FILE=./examples/Datasets/encoded_query.csv")

os.system("make cluster UPDATE=\"Mean Vector\" ASSIGNMENT=Classic OUTPUT_FILE=og_cluster_update_Mean_Vector_assignment_Classic INPUT_FILE=./examples/Datasets/encoded_input.csv QUERY_FILE=./examples/Datasets/encoded_query.csv")
os.system("make cluster UPDATE=\"Mean Vector\" ASSIGNMENT=LSH OUTPUT_FILE=og_cluster_update_Mean_Vector_assignment_LSH INPUT_FILE=./examples/Datasets/encoded_input.csv QUERY_FILE=./examples/Datasets/encoded_query.csv")
os.system("make cluster UPDATE=\"Mean Vector\" ASSIGNMENT=Hypercube OUTPUT_FILE=og_cluster_update_Mean_Vector_assignment_Hypercube INPUT_FILE=./examples/Datasets/encoded_input.csv QUERY_FILE=./examples/Datasets/encoded_query.csv")
os.system("make cluster UPDATE=\"Mean Frechet\" ASSIGNMENT=Classic OUTPUT_FILE=og_cluster_update_Mean_Frechet_assignment_Classic INPUT_FILE=./examples/Datasets/encoded_input.csv QUERY_FILE=./examples/Datasets/encoded_query.csv")
os.system("make cluster UPDATE=\"Mean Frechet\" ASSIGNMENT=LSH OUTPUT_FILE=og_cluster_update_Mean_Frechet_assignment_LSH INPUT_FILE=./examples/Datasets/encoded_input.csv QUERY_FILE=./examples/Datasets/encoded_query.csv")
os.system("make cluster UPDATE=\"Mean Frechet\" ASSIGNMENT=Hypercube OUTPUT_FILE=og_cluster_update_Mean_Frechet_assignment_Hypercube INPUT_FILE=./examples/Datasets/encoded_input.csv QUERY_FILE=./examples/Datasets/encoded_query.csv")

os.system("make cluster UPDATE=\"Mean Vector\" ASSIGNMENT=Classic OUTPUT_FILE=og_cluster_update_Mean_Vector_assignment_Classic")
os.system("make cluster UPDATE=\"Mean Vector\" ASSIGNMENT=LSH OUTPUT_FILE=og_cluster_update_Mean_Vector_assignment_LSH")
os.system("make cluster UPDATE=\"Mean Vector\" ASSIGNMENT=Hypercube OUTPUT_FILE=og_cluster_update_Mean_Vector_assignment_Hypercube")
os.system("make cluster UPDATE=\"Mean Frechet\" ASSIGNMENT=Classic OUTPUT_FILE=og_cluster_update_Mean_Frechet_assignment_Classic")
os.system("make cluster UPDATE=\"Mean Frechet\" ASSIGNMENT=LSH OUTPUT_FILE=og_cluster_update_Mean_Frechet_assignment_LSH")
os.system("make cluster UPDATE=\"Mean Frechet\" ASSIGNMENT=Hypercube OUTPUT_FILE=og_cluster_update_Mean_Frechet_assignment_Hypercube")
