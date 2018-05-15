#include <stdio.h>
#include "graph.h"
#include "memory.h"

//Create new graph
graph *init_graph() {
	struct graph *g;
	g = NEW(graph);
	g->nodecount = 0;
	g->last = NULL;
	g->nodes = NULL;
	return g;
}

//Insert node into graph
graph_node *insert_node(graph *g, void *data) {
	struct graph_node *node;
	struct graph_nodelist *list;
	node = NEW(graph_node);
	list = make_list(node, NULL, NULL);
	node->graph = g;
	g->nodecount++;
	node->key = g->nodecount;

	if (g->last == NULL) {
		g->nodes = list;
		g->last = list;
	} else {
		g->last->next = list;
		list->prev = g->last;
		g->last = list;
	}

	node->succ = NULL;
	node->pred = NULL;
	node->data = data;
	return node;
}

//Make a nodelist cell
graph_nodelist *make_list(graph_node *head, graph_nodelist *next, graph_nodelist *prev) {
	struct graph_nodelist *list;
	list = NEW(graph_nodelist);
	list->head = head;
	list->next = next;
	list->prev = prev;
	return list;
}

//Get list of nodes in graph
graph_nodelist *get_nodes(graph *g) {
	return g->nodes;
}

//Check if node is in nodelist
int node_in_list(graph_node *node, graph_nodelist *list) {
	struct graph_nodelist *temp;
	temp = list;
	while (temp != NULL) {
		if (temp->head == node) {
			return 1;
		}
		temp = temp->next;
	}

	return 0;
}

//Create edge from node "from", to node "to"
int add_edge(graph_node *from, graph_node *to) {
	if (from->graph != to->graph) {
		return 0;
	}
	if (check_edge(from, to)) {
		return 1;
	}
	to->pred = make_list(from, to->pred, NULL);
	from->succ = make_list(to, from->succ, NULL);
	return 1;
}

//Removes edge from node "from", to node "to"
void rem_edge(graph_node *from, graph_node *to) {

	to->pred = rem_node(from, to->pred);
	from->succ = rem_node(to, from->succ);
}

//Removes node from nodelist
graph_nodelist *rem_node(graph_node *node, graph_nodelist *list) {
	if (node == list->head) {
		return list->next;
	} else {
		return make_list(list->head, rem_node(node, list->next), NULL);
	}
}

//Show all the nodes and edges in a graph, prints to a file
void show_graph(FILE *out, graph_nodelist *list) {
	struct graph_nodelist *temp;
	struct graph_nodelist *succ;
	struct graph_node *node;
	temp = list;
	while (temp != NULL) {
		node = temp->head;
		fprintf(out, " (%d): ", node->key);
		succ = get_succs(node);
		while (succ != NULL) {
			fprintf(out, "%d ", succ->head->key);
			succ = succ->next;
		}
		fprintf(out, "\n");
		temp = temp->next;
	}
}

//Show all the nodes and edges in a graph, prints to a file - reverse order - JUST FOR TESTING
void show_graph_rev(FILE *out, graph_nodelist *list) {
	struct graph_nodelist *temp;
	struct graph_nodelist *succ;
	struct graph_node *node;
	temp = list;
	while (temp != NULL) {
		node = temp->head;
		fprintf(out, " (%d): ", node->key);
		succ = get_succs(node);
		while (succ != NULL) {
			fprintf(out, "%d ", succ->head->key);
			succ = succ->next;
		}
		fprintf(out, "\n");
		temp = temp->prev;
	}
}

//Returns list of succesors to node
graph_nodelist *get_succs(graph_node *node) {

	return node->succ;
}

//Returns list of predecessors to node
graph_nodelist *get_preds(graph_node *node) {

	return node->pred;
}

//Checks if edge exists between node "from", and node "to"
int check_edge(graph_node *from, graph_node *to) {

	return node_in_list(to, get_succs(from));
}

//Gets degree of node
int get_degree(graph_node *node) {

	return get_in_degree(node) + get_out_degree(node);
}

//Gets degree of successors of node
int get_out_degree(graph_node *node) {
	struct graph_nodelist *list;
	int degree;
	list = get_succs(node);
	degree = 0;
	while (list != NULL) {
		degree++;
		list = list->next;
	}

	return degree;
}

//Gets degree of predecessors of node
int get_in_degree(graph_node *node) {
	struct graph_nodelist *list;
	int degree;
	list = get_preds(node);
	degree = 0;
	while (list != NULL) {
		degree++;
		list = list->next;
	}

	return degree;
}

//Returns list of all successors and predecessors of node
graph_nodelist *adjacency(graph_node *node) {

	return concat_list(get_succs(node), get_preds(node));
}

//Puts list first in the back of list second, returns the concatenated list
graph_nodelist *concat_list(graph_nodelist *first, graph_nodelist *second) {
	if (first == NULL) {
		return second;
	} else {
		return make_list(first->head, concat_list(first->next, second), NULL);
	}
}

//Returns data from node
void *get_data(graph_node *node) {

	return node->data;
	
}