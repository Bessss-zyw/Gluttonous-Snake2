#include "common.h"

using namespace z3;

bool func1(int data[4][5], int answer[4])
{
    int i = 0;

    context con;

    expr a = con.int_const("a");
    expr b = con.int_const("b");
    expr c = con.int_const("c");
    expr d = con.int_const("d");

    solver s(con);

    //TODO : your solution here
    //assert(0);
    for(i = 0;i < 4;++i)
        s.add(a*data[i][0]+b*data[i][1]+c*data[i][2]+d*data[i][3] == data[i][4]);

    if(sat == s.check())
    {
        model m = s.get_model();
        answer[0] = m.eval(a).get_numeral_int();
        answer[1] = m.eval(b).get_numeral_int();
        answer[2] = m.eval(c).get_numeral_int();
        answer[3] = m.eval(d).get_numeral_int();

        return true;
    }

    return false;
}

int func2(int data[][2], unsigned n)
{
    unsigned i = 0;

    context ctx;

    expr a = ctx.int_const("a");
    solver s(ctx);

    //TODO : your solution here
    //assert(0);

    for(i = 0;i < n;++i){
        s.add(a%data[i][0] == data[i][1]);
        s.add(a != data[i][1]);
    }

    s.add(a < 1000000);

    if(sat == s.check())
    {
        model m = s.get_model();
        return m.eval(a).get_numeral_int();
    }

    return -1;
}

bool func3(int node_num, int color, int graph[20][20])
{
    int i=0, j=0,k = 0;

    context c;
    expr_vector x(c); // x is a vector of expr.

    for ( i = 0; i < node_num; ++i)
        for ( j = 0; j < node_num; ++j)
        {
            std::stringstream x_name;
            x_name << "x_" << i << '_' << j;// every uninterpreted consts need a unique name
            x.push_back(c.int_const(x_name.str().c_str()));// add a new integer consts
        }
    //x[i * node_num + j] denotes the color of edge between node i and node j

    solver s(c);

    //TODO : your solution here
    //assert(0);
    for(i = 0;i < node_num;++i)
        for(j = 0;j < node_num;++j)
        {
            if(j <= i) 
                s.add(x[i * node_num + j] == 0);
            else{
                s.add(x[i * node_num + j] > 0);
                s.add(x[i * node_num + j] <= color);

                for(k = j+1;k < node_num;++k)
                {
                    s.add(!((x[i * node_num + j] == x[i * node_num + k])&&(x[i * node_num + j] == x[j * node_num + k])));
                }
            }
        }

    if( unsat == s.check() )
        return false;

    model m = s.get_model();
    
    for(i=0; i<node_num; ++i)
    {
        for( j=i+1; j<node_num; ++j)
        {
            //x[i * node_num + j] denotes the color of edge between node i and node j
            graph[i][j] = m.eval( x[i * node_num + j] ).get_numeral_int();
        }
    }
    return true;
}
