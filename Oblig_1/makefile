all: compile

compile:
	gcc read_graph_from_file.c PageRank_iterations.c top_n_webpages.c \
	top_n_webpages_omp.c PageRank_iterations_omp.c functions.c main.c \
	-o run_1 -Xpreprocessor -fopenmp -lomp
