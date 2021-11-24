
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
