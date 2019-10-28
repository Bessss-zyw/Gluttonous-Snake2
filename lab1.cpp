#include "common.h"

using namespace Minisat;

bool lab1(int states[], int n, int button[][2], int m, bool answer[])
{
    Solver solver;
    Var variables[1000];
    int i, j;
    for(i = 0; i<m; ++i)
        variables[i] = solver.newVar();

    //TODO : your solution here.
    //assert(0);
    int buttonIndex[2];
    int buttonNum;
    for(i = 0;i < n;++i)
    {
        buttonNum = 0;
        for(j = 0;j < m;++j){
            if(button[j][0]-1 == i | button[j][1]-1 == i){
                if(buttonNum == 0){buttonIndex[0] = j;buttonNum++;}
                else {buttonIndex[1] = j; buttonNum++; break;}
            }
        }

        if(buttonNum == 1){ 
            if(states[i]) solver.addClause(~mkLit(variables[buttonIndex[0]]));
            else solver.addClause(mkLit(variables[buttonIndex[0]]));
            }
        else
        {
            if(states[i]){
                solver.addClause(~mkLit(variables[buttonIndex[0]]),mkLit(variables[buttonIndex[1]]));
                solver.addClause(mkLit(variables[buttonIndex[0]]),~mkLit(variables[buttonIndex[1]]));
            }
            else
            {
                solver.addClause(mkLit(variables[buttonIndex[0]]),mkLit(variables[buttonIndex[1]]));
                solver.addClause(~mkLit(variables[buttonIndex[0]]),~mkLit(variables[buttonIndex[1]]));
            }
        }
        
    }


    //solve SAT problem
    auto sat = solver.solve();
    if(sat)
    {
        for(i = 0; i < m; ++i)
            answer[i] = (solver.modelValue(variables[i]) == l_True);
        return true;
    }
    return false;
}




