#ifndef NFA_H_INCLUDED
#define NFA_H_INCLUDED



stack<node *> stck_NFA;
void printNFA(node *nfa);


FILE * makedotNFA(node *nfa)
{
    FILE *dot;
    int i, j;
    if((dot=fopen("NFA.dot", "w"))==NULL)
        cout<<"File Creation Error \n";
    fprintf(dot, "digraph NFA{ \n");
    fprintf(dot, "%d [shape=doublecircle];\n", nfa->connections.size()-1);
    for(i=0; i<nfa->connections.size(); i++)
    {
        for(j=0; j<nfa->connections[i].size(); j++)
        {
            if(nfa->connections[i][j].first== '\0')
                fprintf(dot, "%d -> %d [label=\"epsilon\"]; \n", i, nfa->connections[i][j].second);
            else
                fprintf(dot, "%d -> %d [label=\"%c\"]; \n", i, nfa->connections[i][j].second, nfa->connections[i][j].first);

        }
    }
    fprintf(dot, "}");
    fclose(dot);
    return dot;

}



node * convertToNFA(string post)
{
	int i=0;
	char c;
	while(i<post.length())
	{
		c=post.at(i);
		if(isOperand(c))
		{
			node *op=createnode(c);
			//printNFA(op);
			stck_NFA.push(op);
		}
		else if(c=='|')
		{
			node *or_node=new node;

			node *b= stck_NFA.top();
			stck_NFA.pop();
			node *a= stck_NFA.top();
			stck_NFA.pop();

			int alen, blen, j, k;
			alen=a->connections.size();
			blen=b->connections.size();
			or_node->connections.resize(alen+blen+2);

			for(j=0;j<alen;j++)
			{
				for(k=0;k< a->connections[j].size();k++)
                {
                    or_node->connections[j+1].push_back(make_pair(a->connections[j][k].first, a->connections[j][k].second+1));
                }
			}

            for(j=0;j<blen;j++)
			{
				for(k=0;k< b->connections[j].size();k++)
                {
                    or_node->connections[j+alen+1].push_back(make_pair(b->connections[j][k].first, b->connections[j][k].second+alen+1));
                }
			}

			or_node->connections[0].push_back(make_pair('\0', 1));
			or_node->connections[0].push_back(make_pair('\0', alen+1));
			or_node->connections[alen].push_back(make_pair('\0', alen+blen+1));
			or_node->connections[alen+blen].push_back(make_pair('\0', alen+blen+1));

			delete(a);
			delete(b);

            stck_NFA.push(or_node);

		}
		else if(c==',')
        {
            node *con_node=new node;

			node *b= stck_NFA.top();
			stck_NFA.pop();
			node *a= stck_NFA.top();
			stck_NFA.pop();

			int alen, blen, j, k;
			alen=a->connections.size();
			blen=b->connections.size();
			con_node->connections.resize(alen+blen-1);

			for(j=0; j<alen; j++)
            {
                for(k=0;k<a->connections[j].size();k++)
                    con_node->connections[j].push_back(make_pair(a->connections[j][k].first, a->connections[j][k].second));
            }

            for(j=0; j<blen; j++)
            {
                for(k=0;k<b->connections[j].size();k++)
                    con_node->connections[j+alen-1].push_back(make_pair(b->connections[j][k].first, b->connections[j][k].second+alen-1));
            }

            stck_NFA.push(con_node);

        }

        else if(c=='*'||c=='+'||c=='?')
        {
            node *un_node=new node;

			node *a= stck_NFA.top();
			stck_NFA.pop();

			int alen, j, k;
			alen=a->connections.size();
			un_node->connections.resize(alen+2);
			for(j=0;j<alen;j++)
			{
				for(k=0;k< a->connections[j].size();k++)
                {
                    un_node->connections[j+1].push_back(make_pair(a->connections[j][k].first, a->connections[j][k].second+1));
                }
			}
            un_node->connections[0].push_back(make_pair('\0', 1));
            un_node->connections[alen].push_back(make_pair('\0', alen+1));
            if(c=='*')
            {
                un_node->connections[0].push_back(make_pair('\0', alen+1));
                un_node->connections[alen].push_back(make_pair('\0', 1));
            }
            else if(c=='+')
            {
                un_node->connections[alen].push_back(make_pair('\0', 1));
            }
            else if(c=='?')
            {
                un_node->connections[0].push_back(make_pair('\0', alen+1));
            }

            stck_NFA.push(un_node);
        }



		//cout<<i<<endl;
		i++;
	}
	node *final=stck_NFA.top();
	stck_NFA.pop();

	makedotNFA(final);
	system("dot -Tpng NFA.dot -o NFA.png");
	return final;
}


void printNFA(node *nfa)
{
    int i , j;
    for (i=0; i< nfa->connections.size();i++)
     {
        cout<<"Node "<<i<<'\n';
        for(j=0; j<nfa->connections[i].size();j++)
            cout<<nfa->connections[i][j].first<<"\t"<<nfa->connections[i][j].second<<endl;
        cout<<endl;
     }
}


#endif // NFA_H_INCLUDED
