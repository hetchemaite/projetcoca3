
#include "EdgeConReduction.h"
#include "Z3Tools.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

Z3_ast getVariableIsIthTranslator(Z3_context ctx, int node1, int node2, int number)
{
    char name[40];
    if (node1 < node2)
        snprintf(name, 40, "[(%d,%d),%d]", node1, node2, number);
    else
        snprintf(name, 40, "[(%d,%d),%d]", node2, node1, number);
    return mk_bool_var(ctx, name);
}

Z3_ast getVariableParent(Z3_context ctx, int child, int parent)
{
    char name[40];
    snprintf(name, 40, "p_[%d,%d]", child, parent);
    return mk_bool_var(ctx, name);
}

Z3_ast getVariableLevelInSpanningTree(Z3_context ctx, int level, int component)
{
    char name[40];
    snprintf(name, 40, "l_[%d,%d]", component, level);
    return mk_bool_var(ctx, name);
}

Z3_ast EdgeConReduction(Z3_context ctx, EdgeConGraph edgeGraph, int cost)
{
    return Z3_mk_false(ctx);
}

void getTranslatorSetFromModel(Z3_context ctx, Z3_model model, EdgeConGraph graph)
{
    return;
}

Z3_ast translatorHasLessThenTwoEdges(Z3_context ctx,EdgeConGraph edgeGraph, int nbTrans){
    int nbNums = getGraph(edgeGraph).numNodes;
    Z3_ast resultTab[getGraph(edgeGraph).numEdges];

    int ind_resultTab = 0;

    for(int i=0; i<nbTrans; i++){


        for(int node1 = 0; node1 < nbNums; node1++){
            for(int node2 = 0; node2 < nbNums; node2++){


                if(isEdge(getGraph(edgeGraph),node1,node2)){

                    for(int node3 = 0; node1 < nbNums; node3++){
                        for(int node4 = 0; node2 < nbNums; node4++){

                            if(node1 != node3 || node1 != node4 || node2 != node3 || node2 != node4){

                                if(isEdge(getGraph(edgeGraph),node3,node4)){

                                    Z3_ast x_mi = getVariableIsIthTranslator(ctx,node1,node2,i);
                                    Z3_ast x_ni = getVariableIsIthTranslator(ctx,node3,node4,i);

                                    Z3_ast neg_x_mi = Z3_mk_not(ctx,x_mi);
                                    Z3_ast negX_x_ni = Z3_mk_not(ctx,x_ni);

                                    Z3_ast orTab[2] = {neg_x_mi,negX_x_ni};

                                    Z3_ast x_mi_Union_x_ni = Z3_mk_or(ctx,3,orTab);

                                    resultTab[ind_resultTab++] = x_mi_Union_x_ni;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Z3_ast Y_1 = Z3_mk_and(ctx,ind_resultTab,resultTab);
    return Y_1;

}


Z3_ast eachComposantHasAtLeastOneParant(Z3_context ctx,EdgeConGraph edgeGraph, int treeDepth){

    Z3_ast resultTab[treeDepth*getNumComponents(edgeGraph)+ getNumComponents(edgeGraph) * getNumComponents(edgeGraph)];
    int ind_resultTab = 0;


    for(int level =0; level<treeDepth; level++){
        for(int nbComp_father=0; nbComp_father< getNumComponents(edgeGraph); nbComp_father++){
            Z3_ast L_jh = getVariableLevelInSpanningTree(ctx,  level, nbComp_father);
            Z3_ast neg_L_jh = Z3_mk_not(ctx,L_jh);
            
            Z3_ast mainOrTab[getNumComponents(edgeGraph)];
            int ind_ouTab = 0;
            for(int nbComp_son=0; nbComp_son< getNumComponents(edgeGraph); nbComp_son++){

                Z3_ast P_parentSon = getVariableParent(ctx, nbComp_son, nbComp_father);

                Z3_ast orTab[2] = {neg_L_jh,P_parentSon};

                Z3_ast nonL_union_p = Z3_mk_or(ctx,2,orTab);

                mainOrTab[ind_ouTab++] = nonL_union_p;
            }

            Z3_ast mainOr = Z3_mk_or(ctx,ind_ouTab,mainOrTab);
            resultTab[ind_resultTab++] = mainOr;

        }
    }

    Z3_ast Y_1 = Z3_mk_and(ctx,ind_resultTab,resultTab);
    return Y_1;
}


Z3_ast eachComposantHasAtMostOneParant(Z3_context ctx,EdgeConGraph edgeGraph){

    Z3_ast resultTab[getNumComponents(edgeGraph)];
    int ind_resultTab = 0;

    for(int nbComp_father=0; nbComp_father< getNumComponents(edgeGraph); nbComp_father++){
        for(int nbComp_son1=0; nbComp_son1< getNumComponents(edgeGraph); nbComp_son1++){
            for(int nbComp_son2=0; nbComp_son2< getNumComponents(edgeGraph); nbComp_son2++){

                if(nbComp_father != nbComp_son1 && nbComp_father !=  nbComp_son2 && nbComp_son2 != nbComp_son1){
                    Z3_ast P_parentSon1 = getVariableParent(ctx, nbComp_son1, nbComp_father);
                    Z3_ast P_parentSon2 = getVariableParent(ctx, nbComp_son2, nbComp_father);

                    Z3_ast neg_P_parentSon1 = Z3_mk_not(ctx,P_parentSon1);
                    Z3_ast neg_P_parentSon2 = Z3_mk_not(ctx,P_parentSon2);

                    Z3_ast orTab[2] = {neg_P_parentSon1,neg_P_parentSon2};

                    Z3_ast nonL_union_p = Z3_mk_or(ctx,2,orTab);

                    resultTab[ind_resultTab++] = nonL_union_p;


                }
            }
        }
    }

    Z3_ast Y_2 = Z3_mk_and(ctx,ind_resultTab,resultTab);
    return Y_2;
}


Z3_ast depthHigherThenK(Z3_context ctx,EdgeConGraph edgeGraph, int k,int treeDepth){

    Z3_ast resultTab[getNumComponents(edgeGraph)* getNumComponents(edgeGraph)];
    int ind_resultTab = 0;
    for(int level = k+1; level<treeDepth; level++){
        for(int nbComp=0; nbComp< getNumComponents(edgeGraph); nbComp++){

            Z3_ast L_jh = getVariableLevelInSpanningTree(ctx,  level, nbComp);
            resultTab[ind_resultTab++] = L_jh;

        }
    }

    Z3_ast form5 = Z3_mk_or(ctx,ind_resultTab,resultTab);
    return form5;

}

int getComponentByNode(EdgeConGraph edgeGraph, int node){

    for(int nbComp=0; nbComp< getNumComponents(edgeGraph); nbComp++){
        if(isNodeInComponent(edgeGraph,node,nbComp)){
            return nbComp;
        }
    }
    //not found
    return -1;

}



Z3_ast fatherSon(Z3_context ctx,EdgeConGraph edgeGraph, int k,int treeDepth){

}


/*  Y3 = ⋀ i ∈ {1,...,N} ⋁e ∈ E (xe,i)
Tous les convertisseurs sont attribués à une arête  */
Z3_ast allTranslatorsOnEdge(Z3_context ctx, EdgeConGraph edgeGraph, int nbTrans)
{
    int nbNums = getGraph(edgeGraph).numNodes;
    Z3_ast resultTab[getGraph(edgeGraph).numEdges];

    int ind_resultTab = 0;
    int nbEdges = getGraph(edgGraph).numEdges;

    for (int i = 0; i < nbTrans; i++)
    {
        Z3_ast unionTab[depth];
        int ind_uniontab = 0;

        for (int node1 = 0; node1 < nbNums; node1++)
        {
            for (int node2 = 0; node2 < nbNums; node2++)
            {
                if (isEdge(getGraph(edgeGraph), node1, node2))
                {
                    Z3_ast x_ei = getVariableIsIthTranslator(ctx, node1, node2, i);
                    unionTab[ind_uniontab++] = x_ei;
                }
            }
        }
        Z3_ast union_x_ei = Z3_mk_or(ctx, ind_uniontab, unionTab);
        resultTab[ind_resultTab++] = union_x_ei;
    }
    Z3_ast Y_2 = Z3_mk_and(ctx, ind_resultTab, resultTab);
    return Y_2;
}

/*  Y2 = ⋀j ∈ {1,...,CH}  ⋁h∈ {0,1,...,hauteur} Ꙇj,h
Chaque composante connexe a au moins un niveau. */
Z3_ast eachComponentHasALevel(Z3_context ctx, EdgeConGraph edgeGraph, int depth)
{
    int nbComponents = getNumComponents(edgeGraph);
    Z3_ast resultTab[nbComponents ^ 2];
    int ind_resultTab = 0;

    for (int i = 0; i < depth; i++)
    {
        Z3_ast unionTab[depth];
        int ind_uniontab = 0;
        for (int h; h < depth; h++)
        {
            Z3_ast l_jh = getVariableLevelInSpanningTree(ctx,level,component);
            unionTab[ind_uniontab++] = l_jh;
        }
        Z3_ast union_l_jh = Z3_mk_or(ctx, ind_uniontab, unionTab);
        resultTab[ind_resultTab++] = union_l_jh;
    }
    Z3_ast Y_2 = Z3_mk_and(ctx, ind_resultTab, resultTab);
    return Y_2;
}

Z3_ast edgeHasLessThenTwoTranslator(Z3_context ctx,EdgeConGraph edgeGraph, int nbEdges){
    int nbNums = getGraph(edgeGraph).numNodes;
    Z3_ast resultTab[getGraph(edgeGraph).numEdges];

    int ind_resultTab = 0;

    for(int i = 0; i < nbEdges; i++){


        for(int node1 = 0; node1 < nbNums; node1++){
            for(int node2 = 0; node2 < nbNums; node2++){


                if(isTranslator(edgeGraph,node1,node2)){

                    for(int node3 = 0; node1 < nbNums; node3++){
                        for(int node4 = 0; node2 < nbNums; node4++){

                            if(node1 != node3 || node1 != node4 || node2 != node3 || node2 != node4){

                                if(isTranslator(edgeGraph,node3,node4)){

                                    Z3_ast neg_x_mi = Z3_mk_not(ctx, getVariableIsIthTranslator(ctx,node1,node2,i));
                                    Z3_ast negX_x_ni = Z3_mk_not(ctx, getVariableIsIthTranslator(ctx,node3,node4,i));

                                    Z3_ast orTab[2] = {neg_x_mi,negX_x_ni};

                                    Z3_ast x_mi_Union_x_ni = Z3_mk_or(ctx,3,orTab);

                                    resultTab[ind_resultTab++] = x_mi_Union_x_ni;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Z3_ast Y_2 = Z3_mk_and(ctx,ind_resultTab,resultTab);
    return Y_2;    

}

Z3_ast eachComposantHasDiffirentLevel(Z3_context ctx, EdgeConGraph edgeGraph, int treeDepth){

    Z3_ast resultTab[treeDepth*getNumComponents(edgeGraph)+ getNumComponents(edgeGraph) * getNumComponents(edgeGraph)];
    int ind_resultTab = 0;


    for(int level = 1; level < treeDepth; level++){
        for(int nbComp_father =1; nbComp_father < getNumComponents(edgeGraph); nbComp_father++){
            Z3_ast L_jh = getVariableLevelInSpanningTree(ctx,  level, nbComp_father);
            Z3_ast neg_L_jh = Z3_mk_not(ctx,L_jh);
            
            Z3_ast mainOrTab[getNumComponents(edgeGraph)];
            int ind_ouTab = 0;
            for(int nbComp_son=1; nbComp_son < getNumComponents(edgeGraph); nbComp_son++){


                Z3_ast neg_L_jy = Z3_mk_not(ctx, getVariableLevelInSpanningTree(ctx,  level, nbComp_son));

                Z3_ast orTab[2] = {neg_L_jh,neg_L_jy};
                Z3_ast unionL_L = Z3_mk_or(ctx,2,orTab);

                mainOrTab[ind_ouTab++] = unionL_L;
            }

            Z3_ast mainOr = Z3_mk_or(ctx,ind_ouTab, mainOrTab);
            resultTab[ind_resultTab++] = mainOr;

        }
    }

    Z3_ast Y_1 = Z3_mk_and(ctx,ind_resultTab,resultTab);
    return Y_1;
}