#include "EdgeConResolution.h"
#include "Graph.h"
#include <stdlib.h>
#include <stdio.h>
#include "list.c"



typedef struct
{
    int cost;
    Liste *tab;
} explore_return;

bool isVisitedNode(int node, Liste *visited_node)
{
    if (visited_node == NULL)
        return false;
    for (int i = 0; i < visited_node->size; i++)
    {
        if (getElem(visited_node,i) == node)
            return true;
    }
    return false;
}

explore_return explorer(EdgeConGraph graph, int node, int cost, Liste *visited_nodes)
{
    explore_return resultat;
    int max_cost = 0;
    if (!isVisitedNode(node, visited_nodes))
    {
        insertion(visited_nodes,node);
    }

    for (int t = 0; t < getGraph(graph).numNodes; t++)
    {

        if (!isVisitedNode(t, visited_nodes))
        {
            if (isEdgeHeterogeneous(graph, node, t))
            {
                resultat = explorer(graph, t, cost++, visited_nodes);
                max_cost = max(resultat.cost, cost);
                for (int s = 0; s < resultat.tab->size; s++)
                {
                    insertion(visited_nodes,s);
                }
            }
            else
            {
                if (areInSameComponent(graph, node, t))
                {
                    resultat = explorer(graph, t, cost, visited_nodes);
                    max_cost = max(resultat.cost, cost);
                    for (int s = 0; s < resultat.tab->size; s++)
                    {
                        insertion(visited_nodes,s);
                    }
                }
            }
        }
    }
    return resultat;
}


int CalculateMaxCost(EdgeConGraph graph)
{
    int maxCost = 0;

    for (int i = 0; i < getGraph(graph).numNodes; i++)
    {
        int courantCost = 0;
        Liste *visitedEdges = initialisation();
        explore_return res = explorer(graph, i, 0, visitedEdges);
        courantCost = res.cost;
        if (maxCost < courantCost)
        {
            maxCost = courantCost;
        }
        if (visitedEdges->size != getGraph(graph).numNodes)
        {
            return 0;
        }
    }
    return maxCost;
}




/*
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
        deleteFromListByIndex(subset, i);
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
*/

int nbComb(int nbEdges, int nbTrans){
    //TODO
    return 0;
}

int * subset(int *listEdges, int *data, int start, int end, int index, int nbTrans, int **subSets, int indexSub)
{
    int j, i;
    int currentIndex = indexSub;
    if (index == nbTrans) {
        int eachSubSet[nbTrans];
        for (j = 0; j < nbTrans; j++){
            eachSubSet[j] = data[j];
        }
        return eachSubSet;
    }
    for (i = start; i <= end && end - i + 1 >= nbTrans - index; i++)
    {
        data[index] = listEdges[i];
        int *sub = subset(listEdges, data, i+1, end, index+1, nbTrans,subSets,currentIndex);
        for(int j=0; j<sizeof(sub)/sizeof(sub[0]); j++){
            subSets[i][j] = sub[j];
        }
        
    }
    return NULL;
}

int ** getsubset(int *listEdges, int nbEdges, int nbTrans)
{
    int data[nbEdges];
    int subSets[nbComb(nbEdges,nbTrans)][nbTrans];

    return subset(listEdges, data, 0, nbEdges - 1, 0, nbTrans,subSets,0);
}


int BruteForceEdgeCon(EdgeConGraph graph)
{
    int nbCompo = getNumComponents(graph);
    int nbEdges = getGraph(graph).numEdges;
    int nbTrans = nbCompo -1;
    int nbNodes = getGraph(graph).numNodes;

    //creating the list of edges
    int listEdges[nbEdges];
    int listSources[nbEdges];
    int listTqrgets[nbEdges];
    for(int source=0; source<nbNodes; source++){
        
    }
    
    int ** tout_C = getsubset(listEdges,nbEdges,nbTrans);

    int coutMax = -2147483645;

    int i=0;
    int * sub = tout_C[i++];
    while (sub != NULL)
    {
        resetTranslator(graph);
        int j = 0;
        int edge = sub[j++];
        while (edge != NULL)
        {

            
        }
    
        sub = tout_C[i++];       
    }
    

}






