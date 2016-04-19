/* Ariel Jackson
 * Representing a graph in adjacency list form
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/* =============== STRUCTURES =============== */
/* ========================================== */

typedef struct adjlist adjlist;

/* A structure to represent a linked list - adjacency list */
struct adjlist
{
    unsigned int val; //labels vertex - will be converted to string later
    struct adjlist *next;  // pointer to next node
};

/* A structure to represent a graph, an array of linked adjacency lists.
 * The size of the array is the number of vertices, and each of these is a
 * linked list of edges
 */
typedef struct
{
    unsigned int n_vertices; //number of vertices in graph
    struct adjlist** v_array; //array of adjacency lists
} graph;


/* ======== ADJACENCY LIST OPERATIONS ======= */
/* ========================================== */

/* al_new : creates new adjacency list */
adjlist *al_new(unsigned int x)
{
    adjlist *list = (adjlist *)malloc(sizeof (adjlist));
    if (list == NULL) {
        fprintf(stderr, "error (al_new): malloc failed\n");
        exit(1);
    }
    list->val = x;
    list->next = NULL;
    return list;
}


/* al_cons : prepends given unsigned int
 * to the front of the adjacency list (allocates on heap)
 */
adjlist *al_cons(unsigned int x, adjlist *xs)
{
    adjlist* al_node = (adjlist *)malloc(sizeof (adjlist));
    if (al_node == NULL) {
        fprintf(stderr, "error (al_cons): malloc failed-not enough memory\n");
        exit(1);
    }
    al_node->val = x;
    al_node->next = xs;
    return al_node;
}


/* al_print: prints the given adjacency list */
void al_print(adjlist *xs)
{
    adjlist *tmp = xs;
    while (tmp != NULL) {
        printf("%d, ", tmp->val);
        tmp = tmp->next;
    }
}

/* al_free: frees the adjacency list */
void al_free(adjlist *xs)
{
    adjlist *tmp = xs;
    adjlist *freed = xs;
    while (tmp != NULL){
        freed = tmp;
        tmp = tmp->next;
        free(freed);
    }
}


/* ============= GRAPH OPERATIONS =========== */
/* ========================================== */



/* graph_new : creates new graph with v vertices.
 * mallocs graph->v_array, with each pointer in the array set to NULL.
 * sets graph->n_vertices to v.
 */
graph *graph_new(unsigned int v)
{
    unsigned int i;
    graph *new_graph = (graph *)malloc(sizeof (graph));
    if (new_graph == NULL) {
        fprintf(stderr, "error (graph_new): malloc failed on new_graph\n");
    }
    adjlist **new_alist = (adjlist **)malloc(sizeof (adjlist *) * v);
    if (new_alist == NULL) {
        fprintf(stderr, "error (graph_new): malloc failed on new_alist\n");
    }
    for (i = 0; i < v; i++) {
        new_alist[i] = NULL;
    }
    new_graph->v_array = new_alist;
    new_graph->n_vertices = v;
    return new_graph;
}


/* add_edge : adds edge to given graph. Destination is pre-pended
 * to source's adjacency list. This function assumes a directed graph.
 */
void add_edge(graph* graph, unsigned int source, unsigned int dest)
{
    adjlist *new_alist = al_cons(dest, graph->v_array[source]);
    graph->v_array[source] = new_alist;
}


/* graph_free : graph* -> <void>
 * Frees the graph and all adjacency lists
 */
void graph_free(graph *graph)
{
    if (graph) {
        unsigned int i;
        if (graph->v_array) {
            for (i = 0; i < graph->n_vertices; i++) {
                al_free(graph->v_array[i]);
            }
            free(graph->v_array);
        }
        free(graph);
    }
}


/* ========= PRINTING GRAPH OPERATIONS ======= */
/* ========================================== */


/* vertex_tos : converts unsigned int to string (letter) to be used in
 * graph representation. This function is specific to a specific graph
 */
char *vertex_tos(unsigned int n)
{
    switch (n) {
        case 0:
            return "A";
        case 1:
            return "B";
        case 2:
            return "C";
        case 3:
            return "D";
        case 4:
            return "E";
        case 5:
            return "F";
        case 6:
            return "G";
        default:
            fprintf(stderr,
                  "error (vertex_tos): no string representation for vertex\n");
            exit(1);
    }
}


/* al_graph_print : prints the adjacency list representation of the graph.
 * For vertex V which has destinations at X, Y, Z, this would print :
 * "Adjacency list for vertex V:
 *    [X -> Y -> Z] "
 *
 * NOTE: Inspired by http://www.geeksforgeeks.org/graph-and-its-representations/
 * print function
 */
void al_graph_print(graph *graph)
{
    unsigned int v;
    printf("\n******** ADJACENCY LIST REPRESENTATION ********:\n\n");
    for (v = 0; v < graph->n_vertices; v++) {

        struct adjlist* tmp = graph->v_array[v];
        printf("Adjacency list for vertex %s: \n { ", vertex_tos(v));
        while (tmp) {
            printf(" %s->", vertex_tos(tmp->val));
            tmp = tmp->next;
        }
        printf(" }\n");
    }
}

/* graph_print : prints the graph with vertices and edges.
 * For vertex V which has destinations at X, Y, Z, this would print :
 * Vertex V has these edges:
 *     (V, X) ; (V, Y) ; (V, Z)
 */
void graph_print(graph *graph)
{

    unsigned int v;
    printf("\n******** GRAPH VERTICES & EDGES REPRESENTATION ********:\n\n");
    for (v = 0; v < graph->n_vertices; v++) {

        struct adjlist* tmp = graph->v_array[v];
        printf("Vertex %s has these outgoing edges: \n", vertex_tos(v));
        while (tmp) {
            printf(" (%s, %s) ;", vertex_tos(v), vertex_tos(tmp->val));
            tmp = tmp->next;
        }
        printf("\n");
    }
}


/* ================= MAIN =================== */
/* ========================================== */



/* main : test the above functions with given graph:
 * 7 edges ((A,C),(A,B),(C,D),(D,E),(D,G),(E,F),(G,F))
 * and 7 vertices (A, B, C, D, E, F, G).
 */
int main(int argc, char *argv[])
{
    unsigned int num_vertices = 7;
    // create new graph
    graph *graph = graph_new(num_vertices);

    //add edges
    add_edge(graph, 0, 1); //(A, B)
    add_edge(graph, 0, 2); //(A, C)
    add_edge(graph, 2, 3); //(C, D)
    add_edge(graph, 3, 4); //(D, E)
    add_edge(graph, 3, 6); //(D, G)
    add_edge(graph, 4, 5); //(E, F)
    add_edge(graph, 6, 5); //(G, F)

    // print adjacency representation
    al_graph_print(graph);

    // print graph representation
    graph_print(graph);

    //free graph
    graph_free(graph);
    return 0;
}
