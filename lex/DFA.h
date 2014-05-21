#ifndef DFA_H_INCLUDED
#define DFA_H_INCLUDED

/*vector<int> getEpsClosure(node *nfa, int state)
{
    int i;
    vector<int> v1, v2;
    cout<<nfa->connections[state].size();
    for(i=0; i<nfa->connections[state].size(); i++)
    {
        if(nfa->connections[state][i].first == '\0')
        {
            v1.push_back(nfa->connections[state][i].second);
            v2=getEpsClosure(nfa , nfa->connections[state][i].second);
            v1.insert(v1.end(), v2.begin(), v2.end());
        }
    }
    return v1;
}*/

void printvec(vector<int> v)
{
    for(int i =0; i<v.size(); i++)
        cout<<v[i]<<"\t";
    cout<<endl;
}



bool isPresent(vector<int> v, int num)
{
    int i;
    for(i=0; i<v.size(); i++)
    {
        if(v[i] == num)
            return true;
    }
    return false;
}

bool isPresent(vector<vector <int> > v, vector<int> c)
{
    vector<int> tmp;
    int length;
    int i, j;
    for(i=0; i<v.size(); i++)
    {
        tmp=v[i];
        if(tmp[tmp.size()-1] == -1)
            length=tmp.size()-1;
        else
            length=tmp.size();
        for(j=0; j<length; j++)
        {
            if(tmp[j]!=c[j])
                break;
        }
        if((j==length)&&(length==c.size()))
            return true;
    }
    return false;
}

int indexOf(vector<vector <int> > v, vector<int> c)
{
    vector<int> tmp;
    int length;
    int i, j;
    for(i=0; i<v.size(); i++)
    {
        tmp=v[i];
        if(tmp[tmp.size()-1] == -1)
            length=tmp.size()-1;
        else
            length=tmp.size();
        for(j=0; j<length; j++)
        {
            if(tmp[j]!=c[j])
                break;
        }
        if((j==length)&&(length==c.size()))
            return i;
    }
    return -1;
}

vector<int> getEpsClosure(node *nfa, vector<int> states)
{
    vector<int> e_clos;
    stack<int> stk;
    for( int i=0; i<states.size(); i++)
    {
        e_clos.push_back(states[i]);
        stk.push(states[i]);
    }
    while(!(stk.empty()))
    {
        int state=stk.top();
        stk.pop();
        for(int j=0; j<nfa->connections[state].size(); j++)
        {
            if((nfa->connections[state][j].first=='\0')&&(!isPresent(e_clos, nfa->connections[state][j].second)))
            {
                e_clos.push_back(nfa->connections[state][j].second);
                stk.push(nfa->connections[state][j].second);
            }

        }
    }
    return e_clos;
}

vector<int> move(node *nfa, const vector<int> &T, char c)
{
    vector<int> v;
    int i, j;
    for(i=0; i<T.size()-1; i++)
    {
        for(j=0; j<nfa->connections[T[i]].size(); j++)
        {
            if(nfa->connections[T[i]][j].first == c)
                if(!isPresent(v, nfa->connections[T[i]][j].second))
                    v.push_back(nfa->connections[T[i]][j].second);
        }
    }
    return v;
}

int unmarked(vector< vector<int> > v)
{
    vector<int> tmp;
    for(int i=0; i<v.size(); i++)
    {
        tmp=v[i];
        if(tmp[tmp.size() - 1] !=-1)
            return i;
    }
    return -1;
}

void printDstates(DFA *dfa)
{
    int i, j;
    for(i=0; i<dfa->D_states.size(); i++)
    {
        cout<<"Node "<<i<<" :\t";
        for(j=0; j<dfa->D_states[i].size(); j++)
            cout<<dfa->D_states[i][j]<<"\t";
        cout<<"\n";
    }
}


bool isFinal(DFA *dfa, vector<int> T)
{
    int i;
    for(i=0; i<T.size(); i++)
    {
        if(T[i] == dfa->final)
            return true;
    }
    return false;
}

FILE * makedotDFA(DFA *dfa)
{
    FILE *dot;
    int i, j;
    if((dot=fopen("DFA.dot", "w"))==NULL)
        cout<<"File Creation Error \n";
    fprintf(dot, "digraph DFA{ \n");

    for(i=0; i<dfa->D_trans.size(); i++)
    {
        if(isFinal(dfa, dfa->D_states[i]))
            fprintf(dot, "%d [shape=doublecircle];\n", i);

        for(j=0; j<dfa->D_trans[i].size(); j++)
        {
            fprintf(dot, "%d -> %d [label=\"%c\"]; \n", i, dfa->D_trans[i][j].second, dfa->D_trans[i][j].first);
        }
    }
    fprintf(dot, "}");
    fclose(dot);
    return dot;

}

DFA * convertToDFA(node *nfa)
{
    DFA *dfa = new DFA;
    vector<int> start(1,0) , U;
    vector<int> st0=getEpsClosure(nfa, start);
    sort(st0.begin(), st0.end());
    //printvec(st0);


    dfa->D_states.push_back(st0);
    //printDstates(dfa);
    int index, i, count=0;

    while(((index=unmarked(dfa->D_states))!=-1)&&count<53)
    {
        vector <int> &mark=dfa->D_states[index];
        mark.push_back(-1);
        count++;
        /*cout<<"Dstates\n";
        printDstates(dfa);
        cout<<"----------------\n";*/
        for(i=0; i<language.size(); i++)
        {
            //cout<<"i= "<<i<<"\n----------\n";
            vector<int> &T=dfa->D_states[index];
            //printvec(T);
            vector<int> tmp= move(nfa, T, language[i]);
            if(tmp.size() == 0)
                continue;
            //cout<<"Move by "<<language[i]<<"is : ";
            //printvec(tmp);
            U=getEpsClosure(nfa, tmp);
            sort(U.begin(), U.end());
            /*cout<<"Eps-Closure: ";
            printvec(U);
            printvec(T);*/
            if(!(isPresent(dfa->D_states, U)))
            {
                /*
                cout<<"State to be Added: \n";
                printvec(U);*/


                dfa->D_states.push_back(U);
            }
            dfa->D_trans.resize(dfa->D_states.size());
            dfa->D_trans[index].push_back(make_pair(language[i] , indexOf(dfa->D_states, U)));

        }
        cout<<count<<endl;

    }
    dfa->final=nfa->connections.size() - 1;
    //printDstates(dfa);
    makedotDFA(dfa);
    system("dot -Tpng DFA.dot -o DFA.png");
    return dfa;
}


void printDFA(DFA *dfa)
{
    int i , j;
    cout<< "FINAL STATE CONTAINS : "<<dfa->final<<"\n";
    cout<<"DSTATES\n--------\n";
    printDstates(dfa);
    cout<<"DTRANS\n--------\n";

    for (i=0; i< dfa->D_trans.size();i++)
     {
        cout<<"Node "<<i<<'\n';
        for(j=0; j<dfa->D_trans[i].size();j++)
            cout<<dfa->D_trans[i][j].first<<"\t"<<dfa->D_trans[i][j].second<<endl;
        cout<<endl;
     }
}


bool checkString(DFA *dfa, string input)
{
    int i, j, cur_state=0, length;
    char c;
    for(i=0; i<input.size(); i++)
    {
        //cout<<cur_state<<"\n";
        c=input.at(i);
        length=dfa->D_trans[cur_state].size();
        for(j=0; j<length; j++)
        {
            if(c==dfa->D_trans[cur_state][j].first)
            {
                cur_state=dfa->D_trans[cur_state][j].second;
                //cout<<"Cur_state changed to : "<<cur_state<<"\n";
                break;
            }
        }
        if(j==length)
            return false;
    }
    //cout<<cur_state<<"\n";


    return isFinal(dfa, dfa->D_states[cur_state]);
}


#endif // DFA_H_INCLUDED
