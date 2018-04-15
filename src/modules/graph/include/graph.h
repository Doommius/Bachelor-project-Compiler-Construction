#ifndef __graph_h
#define __graph_h

#include <stdio.h>

/**
 * Based on the implementation in the book, page 230
 * Also from: http://www.cs.princeton.edu/~appel/modern/c/chap10/graph.c
 * and: http://www.cs.princeton.edu/~appel/modern/c/chap10/graph.h
 * General purpose, so using void pointers
 */

typedef struct graph{
    int nodecount;
    struct graph_nodelist *nodes;
    struct graph_nodelist *last;

} graph;

typedef struct graph_node{
    struct graph *graph;
    int key;
    struct graph_nodelist *succ;
    struct graph_nodelist *pred;
    void *data;


} graph_node;


typedef struct graph_nodelist{
    struct graph_node *head;
    //Keeping track of the prev in nodelist, to make it easier to do the liveness analysis
    struct graph_nodelist *next;
    struct graph_nodelist *prev;

} graph_nodelist;

//Create new graph
graph *init_graph();

//Insert node into graph
graph_node *insert_node(graph *g, void *data);

//Make a nodelist cell
graph_nodelist *make_list(graph_node *head, graph_nodelist *next, graph_nodelist *prev);

//Get list of nodes in graph
graph_nodelist *get_nodes(graph *g);

//Check if node is in nodelist
int node_in_list(graph_node *node, graph_nodelist *list);

//Create edge from node "from", to node "to"
int add_edge(graph_node *from, graph_node *to);

//Removes edge from node "from", to node "to"
void rem_edge(graph_node *from, graph_node *to);

//Removes node from nodelist
graph_nodelist *rem_node(graph_node *node, graph_nodelist *list);

//Show all the nodes and edges in a graph, prints to a file
void show_graph(FILE *out, graph_nodelist *list);

//Show all the nodes and edges in a graph, prints to a file - reverse order
void show_graph_rev(FILE *out, graph_nodelist *list);

//Returns list of succesors to node
graph_nodelist *get_succs(graph_node *node);

//Returns list of predecessors to node
graph_nodelist *get_preds(graph_node *node);

//Checks if edge exists between node "from", and node "to"
int check_edge(graph_node *from, graph_node *to);

//Gets degree of node
int get_degree(graph_node *node);

//Gets degree of successors of node
int get_out_degree(graph_node *node);

//Gets degree of predecessors of node
int get_int_degree(graph_node *node);

//Returns list of all successors and predecessors of node
graph_nodelist *adjacency(graph_node *node);

//Puts list first in the back of list second, returns the concatenated list
graph_nodelist *concat_list(graph_nodelist *first, graph_nodelist *second);

//Returns data from node
void *get_data(graph_node *node);


#endif
