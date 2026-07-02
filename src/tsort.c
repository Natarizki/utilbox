/* tsort.c — topological sort. Input: pairs of items per line (A B means
 * A must come before B). Uses Kahn's algorithm. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utilbox.h"

#define MAX_NODES 1024
#define MAX_EDGES 4096

static char *names[MAX_NODES];
static int nnodes = 0;
static int edges_from[MAX_EDGES], edges_to[MAX_EDGES];
static int nedges = 0;
static int indegree[MAX_NODES];

static int find_or_add(const char *name)
{
    for (int i = 0; i < nnodes; i++) {
        if (strcmp(names[i], name) == 0) return i;
    }
    names[nnodes] = strdup(name);
    return nnodes++;
}

int tsort_main(int argc, char **argv)
{
    FILE *in = stdin;
    if (argc >= 2) {
        in = fopen(argv[1], "r");
        if (!in) return die("tsort", "cannot open", argv[1]);
    }

    char a[256], b[256];
    while (fscanf(in, "%255s %255s", a, b) == 2) {
        int ia = find_or_add(a);
        int ib = find_or_add(b);
        edges_from[nedges] = ia;
        edges_to[nedges] = ib;
        nedges++;
        indegree[ib]++;
    }
    if (in != stdin) fclose(in);

    int queue[MAX_NODES], qhead = 0, qtail = 0;
    for (int i = 0; i < nnodes; i++) {
        if (indegree[i] == 0) queue[qtail++] = i;
    }

    int output_count = 0;
    while (qhead < qtail) {
        int n = queue[qhead++];
        printf("%s\n", names[n]);
        output_count++;

        for (int e = 0; e < nedges; e++) {
            if (edges_from[e] == n) {
                if (--indegree[edges_to[e]] == 0) {
                    queue[qtail++] = edges_to[e];
                }
            }
        }
    }

    if (output_count < nnodes) {
        fprintf(stderr, "utilbox: tsort: cycle detected\n");
        return 1;
    }

    return 0;
}
