#ifndef AUTOMATA_H_INCLUDED
#define AUTOMATA_H_INCLUDED

typedef struct {
	vector< vector< pair<char, int> > > connections;
} node;

typedef struct {
    int final; // Final NFA state
    vector< vector<int> > D_states;
    vector< vector< pair<char, int > > > D_trans;

} DFA;


node *createnode(char c)
{
	node *nnode= new node;
	if(nnode==NULL)
	{
		cout<< "Memory Allocation error\n";
		exit(0);
	}
	nnode->connections.resize(2);
	nnode->connections[0].push_back(make_pair(c, 1));
	return nnode;
}


#endif // AUTOMATA_H_INCLUDED
