#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>


struct AdjListNode
{
    char *dest;
    struct AdjListNode* next;
};

struct AdjList
{
    struct AdjListNode *head;  // pointer to head node of list
};

struct Graph
{
    int V;
    struct AdjList* array;
};

struct AdjListNode* newAdjListNode(char *dest)
{
    struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

struct Graph* createGraph(int V)
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;

    // Create an array of adjacency lists.  Size of array will be V
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));

    // Initialize each adjacency list as empty by making head as NULL
    int i;
    for (i = 0; i < V; ++i)
        graph->array[i].head = NULL;

    return graph;
}

void addEdge(struct Graph* graph, char *src, char *dest)
{
    // Add an edge from src to dest.  A new node is added to the adjacency
    // list of src.  The node is added at the beginin

    struct AdjListNode* newNode = newAdjListNode(dest);

    //***Changed. you need to add edge in src node. But you were always adding in 0
    newNode->next = graph->array[src[0]-'a'].head;
    graph->array[src[0]-'a'].head = newNode;

    // Since graph is undirected, add an edge from dest to src also
    newNode = newAdjListNode(src);

    //***Changed. you need to add edge in dest node. But you were always adding in 1
    newNode->next = graph->array[dest[0]-'a'].head;
    graph->array[dest[0]-'a'].head = newNode;
}

void printGraph(struct Graph* graph)
{
    int v;
    for (v = 0; v < graph->V; ++v)
    {
        struct AdjListNode* pCrawl = graph->array[v].head;
        printf("\n Adjacency list of vertex %d\n head %c", v, v + 'a');
        while (pCrawl)
        {
            printf("-> %s", pCrawl->dest);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}

int main()
{
    char *str = "(Hello)";
    printf("%s\n", str);
    return 0;
}
