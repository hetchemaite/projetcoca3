#include "EdgeConResolution.h"
#include "Graph.h"
#include <stdlib.h>
#include <stdio.h>

int BruteForceEdgeCon(EdgeConGraph graph)
{
    return -1;
}

typedef struct
{
    int res;
    int *tab;
} explore_return;

int CalculateMaxCost(EdgeConGraph graph)
{
    int maxCost = 0;

    for (int i = 0; i < getGraph(graph).numNodes; i++)
    {
        int courantCost = 0;
        int *visitedEdges;
        // res = explore(graph, i, 0, [])
        // courantCost = res -> Cost
        // visitedEdges = res -> list
        if (maxCost < courantCost)
        {
            maxCost = courantCost;
        }
        if (len(visitedEdges) != getGraph(graph).numNodes)
        {
            return 0;
        }
    }
    return maxCost;
}

void subsetsUtil(int *A, int **res, int *subset, int index, int sizeOfSubSet)
{
    if (len(subset) == sizeOfSubSet)
    {   
        res[len(res)] = subset;
    }

    for (int i = index; i < len(A); i++)
    {

        // include the A[i] in subset.
        subset[len(subset)] = A[i];
        // move onto the next element.
        subsetsUtil(A, res, subset, i + 1, sizeOfSubSet);

        // exclude the A[i] from subset and triggers
        // backtracking.
        deleteFromListByIndex(subsets, i);
    }

    return;
}

void deleteFromListByIndex(int *list, int ind){

    for(int i=ind; i <= len(list); i++){
        list[i] = list[i+1];
    }
    list[len(list)] = NULL;
}

void subsets(int *A, int sizeOfSubSet, int ** res)
{
    int *subset;

    // keeps track of current element in vector A;
    int index = 0;
    subsetsUtil(A, res, subset, index, sizeOfSubSet);
}

bool isVisitedNode(int node, int *visited_node)
{
    if (visited_node == NULL)
        return false;
    for (int i = 0; i < len(visited_node); i++)
    {
        if (visited_node[i] == node)
            return true;
    }
    return false;
}

explore_return explorer(EdgeConGraph graph, int node, int cost, int *visited_nodes)
{
    explore_return resultat;
    int max_cost = 0;
    if (!isVisitedNode(node, visited_nodes))
    {
        visited_nodes[len(visited_nodes) + 1] = node;
    }

    for (int t = 0; t < getGraph(graph).numNodes; t++)
    {

        if (!isVisitedNode(t, visited_nodes))
        {
            if (isEdgeHeterogeneous(graph, node, t))
            {
                resultat = explorer(graph, t, cost++, visited_nodes);
                max_cost = max(resultat.res, cost);
                for (int s = 0; s < len(resultat.tab); s++)
                {
                    visited_nodes[len(visited_nodes) + 1] = s;
                }
            }
            else
            {
                if (areInSameComponent(graph, node, t))
                {
                    resultat = explorer(graph, t, cost, visited_nodes);
                    max_cost = max(resultat.res, cost);
                    for (int s = 0; s < len(resultat.tab); s++)
                    {
                        visited_nodes[len(visited_nodes) + 1] = s;
                    }
                }
            }
        }
    }
    return resultat;
}
