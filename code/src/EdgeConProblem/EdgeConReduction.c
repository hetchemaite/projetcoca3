
#include "EdgeConReduction.h"
#include "Z3Tools.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"
#include <math.h>

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

void getTranslatorSetFromModel(Z3_context ctx, Z3_model model, EdgeConGraph graph)
{
    return;
}



///////////////// Etapes de réduction \\\\\\\\\\\\\\\\\\\

/*  Y1 = ⋀ i ∈ {1,...,N} ⋀m ≠ n ∈ E (¬xm,i ⋁ ¬xn,i)
Chaque convertisseur ne peut être associé à deux différents arêtes  */
Z3_ast translatorHasLessThenTwoEdges(Z3_context ctx, EdgeConGraph edgeGraph, int nbTrans)
{
    int nbNums = getGraph(edgeGraph).numNodes;
    Z3_ast resultTab[getGraph(edgeGraph).numEdges];

    int ind_resultTab = 0;

    for (int i = 0; i < nbTrans; i++)
    {

        for (int node1 = 0; node1 < nbNums; node1++)
        {
            for (int node2 = 0; node2 < nbNums; node2++)
            {

                if (isEdge(getGraph(edgeGraph), node1, node2))
                {

                    for (int node3 = 0; node1 < nbNums; node3++)
                    {
                        for (int node4 = 0; node2 < nbNums; node4++)
                        {

                            if (node1 != node3 || node1 != node4 || node2 != node3 || node2 != node4)
                            {

                                if (isEdge(getGraph(edgeGraph), node3, node4))
                                {

                                    Z3_ast x_mi = getVariableIsIthTranslator(ctx, node1, node2, i);
                                    Z3_ast x_ni = getVariableIsIthTranslator(ctx, node3, node4, i);

                                    Z3_ast neg_x_mi = Z3_mk_not(ctx, x_mi);
                                    Z3_ast negX_x_ni = Z3_mk_not(ctx, x_ni);

                                    Z3_ast orTab[2] = {neg_x_mi, negX_x_ni};

                                    Z3_ast x_mi_Union_x_ni = Z3_mk_or(ctx, 3, orTab);

                                    resultTab[ind_resultTab++] = x_mi_Union_x_ni;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Z3_ast Y_1 = Z3_mk_and(ctx, ind_resultTab, resultTab);
    return Y_1;
}

/* Y2 = ⋀ i ≠ j ∈ {1,...,N} ⋀e ∈ E (¬xe,i ⋁ ¬xe,j)
La même arête ne peut pas avoir 2 convertisseurs. */
Z3_ast edgeHasLessThenTwoTranslator(Z3_context ctx, EdgeConGraph edgeGraph, int nbEdges)
{
    int nbNums = getGraph(edgeGraph).numNodes;
    Z3_ast resultTab[getGraph(edgeGraph).numEdges];

    int ind_resultTab = 0;

    for (int i = 0; i < nbEdges; i++)
    {

        for (int node1 = 0; node1 < nbNums; node1++)
        {
            for (int node2 = 0; node2 < nbNums; node2++)
            {

                if (isTranslator(edgeGraph, node1, node2))
                {

                    for (int node3 = 0; node1 < nbNums; node3++)
                    {
                        for (int node4 = 0; node2 < nbNums; node4++)
                        {

                            if (node1 != node3 || node1 != node4 || node2 != node3 || node2 != node4)
                            {

                                if (isTranslator(edgeGraph, node3, node4))
                                {

                                    Z3_ast neg_x_mi = Z3_mk_not(ctx, getVariableIsIthTranslator(ctx, node1, node2, i));
                                    Z3_ast negX_x_ni = Z3_mk_not(ctx, getVariableIsIthTranslator(ctx, node3, node4, i));

                                    Z3_ast orTab[2] = {neg_x_mi, negX_x_ni};

                                    Z3_ast x_mi_Union_x_ni = Z3_mk_or(ctx, 3, orTab);

                                    resultTab[ind_resultTab++] = x_mi_Union_x_ni;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Z3_ast Y_2 = Z3_mk_and(ctx, ind_resultTab, resultTab);
    return Y_2;
}

/*  Y3 = ⋀ i ∈ {1,...,N} ⋁e ∈ E (xe,i)
Tous les convertisseurs sont attribués à une arête  */
Z3_ast allTranslatorsOnEdge(Z3_context ctx, EdgeConGraph edgeGraph, int nbTrans)
{
    int nbNums = getGraph(edgeGraph).numNodes;
    Z3_ast resultTab[getGraph(edgeGraph).numEdges];

    int ind_resultTab = 0;
    int nbEdges = getGraph(edgeGraph).numEdges;

    for (int i = 0; i < nbTrans; i++)
    {
        Z3_ast unionTab[nbTrans];
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

/*  Y4 = ⋀h≠0 ⋀ j ∈ {1,...,CH} Ꙇj,h  ⇒ (⋁j’ ≠ j  pj,j’)
Chaque composante connexe a au moins un parent (sauf la racine) */
Z3_ast eachComposantHasAtLeastOneParant(Z3_context ctx, EdgeConGraph edgeGraph, int treeDepth)
{

    Z3_ast resultTab[treeDepth * getNumComponents(edgeGraph) + getNumComponents(edgeGraph) * getNumComponents(edgeGraph)];
    int ind_resultTab = 0;

    for (int level = 1; level < treeDepth; level++)
    {
        for (int nbComp_father = 0; nbComp_father < getNumComponents(edgeGraph); nbComp_father++)
        {
            Z3_ast L_jh = getVariableLevelInSpanningTree(ctx, level, nbComp_father);
            Z3_ast neg_L_jh = Z3_mk_not(ctx, L_jh);

            Z3_ast mainOrTab[getNumComponents(edgeGraph)];
            int ind_ouTab = 0;
            for (int nbComp_son = 0; nbComp_son < getNumComponents(edgeGraph); nbComp_son++)
            {

                Z3_ast P_parentSon = getVariableParent(ctx, nbComp_son, nbComp_father);

                Z3_ast orTab[2] = {neg_L_jh, P_parentSon};

                Z3_ast nonL_union_p = Z3_mk_or(ctx, 2, orTab);

                mainOrTab[ind_ouTab++] = nonL_union_p;
            }

            Z3_ast mainOr = Z3_mk_or(ctx, ind_ouTab, mainOrTab);
            resultTab[ind_resultTab++] = mainOr;
        }
    }

    Z3_ast Y_1 = Z3_mk_and(ctx, ind_resultTab, resultTab);
    return Y_1;
}

/*  Y5 = ⋀m≠n≠j ∈  {1,...,CH} ¬pj,m ⋁ ¬pj,n
Chaque composante connexe ne peut avoir qu’un seul parent   */
Z3_ast eachComposantHasAtMostOneParant(Z3_context ctx, EdgeConGraph edgeGraph)
{

    Z3_ast resultTab[getNumComponents(edgeGraph)];
    int ind_resultTab = 0;

    for (int nbComp_father = 0; nbComp_father < getNumComponents(edgeGraph); nbComp_father++)
    {
        for (int nbComp_son1 = 0; nbComp_son1 < getNumComponents(edgeGraph); nbComp_son1++)
        {
            for (int nbComp_son2 = 0; nbComp_son2 < getNumComponents(edgeGraph); nbComp_son2++)
            {

                if (nbComp_father != nbComp_son1 && nbComp_father != nbComp_son2 && nbComp_son2 != nbComp_son1)
                {
                    Z3_ast P_parentSon1 = getVariableParent(ctx, nbComp_son1, nbComp_father);
                    Z3_ast P_parentSon2 = getVariableParent(ctx, nbComp_son2, nbComp_father);

                    Z3_ast neg_P_parentSon1 = Z3_mk_not(ctx, P_parentSon1);
                    Z3_ast neg_P_parentSon2 = Z3_mk_not(ctx, P_parentSon2);

                    Z3_ast orTab[2] = {neg_P_parentSon1, neg_P_parentSon2};

                    Z3_ast nonL_union_p = Z3_mk_or(ctx, 2, orTab);

                    resultTab[ind_resultTab++] = nonL_union_p;
                }
            }
        }
    }

    Z3_ast Y_2 = Z3_mk_and(ctx, ind_resultTab, resultTab);
    return Y_2;
}

/*  Y6 = ⋀j ∈ {1,...,CH}  ⋁h∈ {0,1,...,hauteur} Ꙇj,h
Chaque composante connexe a au moins un niveau. */
Z3_ast eachComponentHasALevel(Z3_context ctx, EdgeConGraph edgeGraph, int depth)
{
    int nbComponents = getNumComponents(edgeGraph);
    Z3_ast resultTab[nbComponents ^ 2];
    int ind_resultTab = 0;

    for (int j = 0; j < depth; j++)
    {
        Z3_ast unionTab[depth];
        int ind_uniontab = 0;
        for (int h; h < depth; h++)
        {
            Z3_ast l_jh = getVariableLevelInSpanningTree(ctx, h, j);
            unionTab[ind_uniontab++] = l_jh;
        }
        Z3_ast union_l_jh = Z3_mk_or(ctx, ind_uniontab, unionTab);
        resultTab[ind_resultTab++] = union_l_jh;
    }
    Z3_ast Y_2 = Z3_mk_and(ctx, ind_resultTab, resultTab);
    return Y_2;
}

/*  Y7 = ⋀m≠n≠j ∈ {1,...,CH} ¬pj,m ⋁ ¬pj,n
Chaque composante connexe ne peut avoir qu’un seul parent   */
Z3_ast eachComposantHasDiffirentLevel(Z3_context ctx, EdgeConGraph edgeGraph, int treeDepth)
{

    Z3_ast resultTab[treeDepth * getNumComponents(edgeGraph) + getNumComponents(edgeGraph) * getNumComponents(edgeGraph)];
    int ind_resultTab = 0;

    for (int level = 1; level < treeDepth; level++)
    {
        for (int nbComp_father = 1; nbComp_father < getNumComponents(edgeGraph); nbComp_father++)
        {
            Z3_ast L_jh = getVariableLevelInSpanningTree(ctx, level, nbComp_father);
            Z3_ast neg_L_jh = Z3_mk_not(ctx, L_jh);

            Z3_ast mainOrTab[getNumComponents(edgeGraph)];
            int ind_ouTab = 0;
            for (int nbComp_son = 1; nbComp_son < getNumComponents(edgeGraph); nbComp_son++)
            {

                Z3_ast neg_L_jy = Z3_mk_not(ctx, getVariableLevelInSpanningTree(ctx, level, nbComp_son));

                Z3_ast orTab[2] = {neg_L_jh, neg_L_jy};
                Z3_ast unionL_L = Z3_mk_or(ctx, 2, orTab);

                mainOrTab[ind_ouTab++] = unionL_L;
            }

            Z3_ast mainOr = Z3_mk_or(ctx, ind_ouTab, mainOrTab);
            resultTab[ind_resultTab++] = mainOr;
        }
    }

    Z3_ast Y_1 = Z3_mk_and(ctx, ind_resultTab, resultTab);
    return Y_1;
}

/*  F4 = ⋁h > k ⋁j ∈ {1,...,CH} Ꙇj,h
Pour que l’arbre ait une profondeur > k , au moins une composante connexe doit avoir une hauteur > k.   */
Z3_ast depthHigherThenK(Z3_context ctx, EdgeConGraph edgeGraph, int k, int treeDepth)
{

    Z3_ast resultTab[getNumComponents(edgeGraph) * getNumComponents(edgeGraph)];
    int ind_resultTab = 0;
    for (int level = k + 1; level < treeDepth; level++)
    {
        for (int nbComp = 0; nbComp < getNumComponents(edgeGraph); nbComp++)
        {

            Z3_ast L_jh = getVariableLevelInSpanningTree(ctx, level, nbComp);
            resultTab[ind_resultTab++] = L_jh;
        }
    }

    Z3_ast form5 = Z3_mk_or(ctx, ind_resultTab, resultTab);
    return form5;
}

/*  F5 = ⋀ u∈Xj ⋀v∈ Xj’ pj,j’ ⇒ {⋁i ∈ {1,...,N} x(u,v),i}  ⋀ {⋁h ∈ {1,...,hauteur} Ꙇj,h ⇒Ꙇj’,h-1} */
int getComponentByNode(EdgeConGraph edgeGraph, int node)
{

    for (int nbComp = 0; nbComp < getNumComponents(edgeGraph); nbComp++)
    {
        if (isNodeInComponent(edgeGraph, node, nbComp))
        {
            return nbComp;
        }
    }
    //not found
    return -1;
}
Z3_ast fatherSon(Z3_context ctx, EdgeConGraph edgeGraph, int nbTrns, int treeDepth)
{
    int tmp = (double)getNumComponents(edgeGraph) * (double)getNumComponents(edgeGraph) * (double)getNumComponents(edgeGraph)* (double)getNumComponents(edgeGraph);
    Z3_ast resultTab[(int)tmp];
    int ind_resultTab = 0;

    for (int node1 = 0; node1 < getGraph(edgeGraph).numNodes; node1++)
    {
        for (int node2 = 0; node2 < getGraph(edgeGraph).numNodes; node2++)
        {

            int nbComp_father = getComponentByNode(edgeGraph, node1);
            int nbComp_son = getComponentByNode(edgeGraph, node2);

            Z3_ast P_parentSon = getVariableParent(ctx, nbComp_son, nbComp_father);
            Z3_ast nonP_parentSon = Z3_mk_not(ctx, P_parentSon);

            //⋁i ∈ {1,...,N} x(u,v),i = a
            Z3_ast X_Tab[nbTrns];
            int ind_X_Tab = 0;

            Z3_ast a;

            if (isEdge(getGraph(edgeGraph), node1, node2))
            {
                for (int tran = 0; tran < nbTrns; tran++)
                {
                    Z3_ast x = getVariableIsIthTranslator(ctx, node1, node2, tran);

                    X_Tab[ind_X_Tab++] = x;
                }

                a = Z3_mk_or(ctx, ind_X_Tab, X_Tab);
            }

            //{⋁h ∈ {1,...,hauteur} Ꙇj,h ⇒Ꙇj’,h-1} = b
            Z3_ast l_Tab[treeDepth];
            int ind_l_Tab = 0;
            for (int level = 0; level < treeDepth; level++)
            {

                Z3_ast L_jh = getVariableLevelInSpanningTree(ctx, level, nbComp_father);
                Z3_ast neg_L_jh = Z3_mk_not(ctx, L_jh);

                Z3_ast L_jh_1 = getVariableLevelInSpanningTree(ctx, level, nbComp_son);

                Z3_ast l_Tab_tmp[2];

                l_Tab_tmp[0] = neg_L_jh;
                l_Tab_tmp[1] = L_jh_1;

                Z3_ast l_tmp = Z3_mk_or(ctx, 2, l_Tab_tmp);

                l_Tab[ind_l_Tab++] = l_tmp;
            }

            Z3_ast b = Z3_mk_or(ctx, ind_l_Tab, l_Tab);

            //a ⋀ b = c

            Z3_ast a_and_b_tab[2];

            a_and_b_tab[0] = a;
            a_and_b_tab[1] = b;

            Z3_ast c = Z3_mk_and(ctx, 2, a_and_b_tab);

            // nonp ⋁ c = d

            Z3_ast nonp_ou_c_tab[2];

            a_and_b_tab[0] = nonP_parentSon;
            a_and_b_tab[1] = c;

            Z3_ast d = Z3_mk_or(ctx, 2, a_and_b_tab);

            resultTab[ind_resultTab++] = d;
        }
    }

    Z3_ast form5 = Z3_mk_and(ctx, ind_resultTab, resultTab);
    return form5;
}


Z3_ast EdgeConReduction(Z3_context ctx, EdgeConGraph edgeGraph, int cost)
{
    int nbTrans = getNumComponents(edgeGraph) - 1;
    int nbEdges = getGraph(edgeGraph).numEdges;
    int treeDepth = getGraph(edgeGraph).numNodes;

    Z3_ast Y1 = translatorHasLessThenTwoEdges(ctx, edgeGraph, nbTrans);
    Z3_ast Y2 = edgeHasLessThenTwoTranslator(ctx, edgeGraph, nbEdges);
    Z3_ast Y3 = allTranslatorsOnEdge(ctx, edgeGraph, nbTrans);
    Z3_ast F1_tab[3] = {Y1, Y2, Y3};
    Z3_ast F1 = Z3_mk_and(ctx, 3, F1_tab);

    Z3_ast Y4 = eachComposantHasAtLeastOneParant(ctx, edgeGraph, treeDepth);
    Z3_ast Y5 = eachComposantHasAtMostOneParant(ctx, edgeGraph);
    Z3_ast F2_tab[2] = {Y4, Y5};
    Z3_ast F2 = Z3_mk_and(ctx, 2, F2_tab);

    Z3_ast Y6 = eachComponentHasALevel(ctx, edgeGraph, treeDepth);
    Z3_ast Y7 = eachComposantHasDiffirentLevel(ctx, edgeGraph, treeDepth);
    Z3_ast F3_tab[2] = {Y6, Y7};
    Z3_ast F3 = Z3_mk_and(ctx, 2, F3_tab);

    Z3_ast F4 = depthHigherThenK(ctx, edgeGraph, cost, treeDepth);

    Z3_ast F5 = fatherSon(ctx, edgeGraph, nbTrans, treeDepth);

    Z3_ast Sat_tab[5] = {F1, F2, F3, F4, F5};
    Z3_ast Sat = Z3_mk_and(ctx, 5, Sat_tab);

    return Sat;
}

/*  Return true s'il y a une solution et modifie le graphe
en ajoutant le nombre de convertisseurs 
*/
bool opt_sat(Z3_context ctx, EdgeConGraph edgeGraph, int cost)
{
    Liste *source; 
    Liste *target;
    Z3_ast formula = EdgeConReduction(ctx, edgeGraph, cost);
    Z3_model model;
    Z3_lbool isFormulaSat = solveFormula(ctx, formula, &model);
    Z3_ast x_ej;

    if (isFormulaSat == Z3_L_TRUE)
    {
        for (int j = 1; j < getGraph(edgeGraph).numEdges; j++)
        {
            for (int node1 = 0; node1 < getGraph(edgeGraph).numNodes; node1++)
            {
                for (int node2 = 0; node2 < getGraph(edgeGraph).numNodes; node2++)
                {
                    if (isEdge(getGraph(edgeGraph), node1, node2))
                    {
                        x_ej = getVariableIsIthTranslator(ctx, node1, node2, j);

                        if (valueOfVarInModel(ctx, model, x_ej))
                        {
                            addTranslator(edgeGraph, node1, node2);
                        }
                    }
                }
            }
        }
        return true;
    }
    return false;
}