/* 
 * File:   main.cpp
 * Author: Souvik-PC
 *
 * Created on March 6, 2014, 12:10 PM
 */
#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>

using namespace std;

/*
 * 
 */

map <string, vector<string> > grammar;
vector<string> nterminals;
map<string, vector<string> > first;


bool isPresent(vector<string> v, string str)
{
    int i;
    for(i=0; i<v.size(); i++)
    {
        if(v[i] == str)
            return true;
    }
    return false;
}

void printvec(vector<string> v)
{
    for(int i =0; i<v.size(); i++)
        cout<<v[i]<<"\t";
    cout<<endl;
}


void printgrammar()
{
    for(int i=0; i<nterminals.size(); i++)
    {
        cout<<nterminals[i]<<endl;
        cout<<"---------------"<<endl;
        printvec(grammar[nterminals[i]]);
    }
}

void getgrammar()
{
    string rule, left;
    int i, j;
    cout<<"Enter Grammar: \n";
    cin>>rule;
    while(rule != "$")
    {
        j=0;
        for(i=0;i<rule.length(); i++)
        {
            if((rule.at(i) == '-')&&(rule.at(i+1) == '>'))
            {
                left=rule.substr(j, i-j);
                i++;
                j=i+1;
                if(! isPresent(nterminals, left))
                    nterminals.push_back(left);
            }
            else if(rule.at(i) == '|')
            {
                grammar[left].push_back(rule.substr(j, i-j));
                j=i+1;
            }
        }
        grammar[left].push_back(rule.substr(j, rule.size() - j));
        cin>>rule;
    }
}

void remImmediateLeftRecursion(int index)
{
    int i;
    bool recursion=false;
    string left = nterminals[index];
    vector<string> right=grammar[left];
    vector<string> rec, nrec;
    for(i=0; i<right.size(); i++)
    {
        if(right[i].at(0) == left.at(0))
        {
            right[i]=right[i].substr(left.size(), right[i].size()-left.size())+left+'\'';
            rec.push_back(right[i]);
            recursion=true;
        }
        else
        {
            nrec.push_back(right[i]);
        }
    }
    if(recursion)
    {
        for(i=0; i<nrec.size(); i++)
        {
            nrec[i]=nrec[i]+left+'\'';
        }
        rec.push_back("#");
    }
    grammar[left]=nrec;
    if(recursion)
    {
        grammar[left+"\'"]=rec;
        nterminals.push_back(left+"\'");
    }
}



int main(int argc, char** argv) {
    int index;
    getgrammar();
    printgrammar();
	cout<<"WELCOME TO A LEFT RECURSION REMOVER !!!";
	cout<<"***************************************";
    cout<<"Enter Rule No. to remove left recursion from: ";
    cin>>index;
    remImmediateLeftRecursion(index);
    printgrammar();
    //cout<<first["a"].size();
    return 0;
}

