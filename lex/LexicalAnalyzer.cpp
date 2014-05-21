/*
 * RegexChecker.cpp
 *
 *  Created on: Feb 8, 2014
 *      Author: Souvik-PC
 */

#include <iostream>
#include <stack>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
using namespace std;

#include "automata.h"

bool isOperand(char x);

vector<char> language;


#include "NFA.h"
#include "DFA.h"

#define ISP 's'
#define ICP 'c'

string fixRanges(string regex)
{
    string fixed="", range;
    int i=0, j, k, count=0;
    char min, max;
    for(i=0; i<regex.length(); i++)
    {
        if(regex.at(i)=='[')
        {
            count=0;
            for(j=i+1; j<regex.length(); j++)
            {
                if(regex.at(j)==']')
                    break;
            }
            range=regex.substr(i+1, j-i-1);
            fixed=fixed + '(';
            for(k=0; k<range.length(); k++)
            {
                if(range.at(k)=='-')
                {
                    min=range.at(k-1);
                    max=range.at(k+1);
                    count++;
                    if(count>1)
                        fixed+='|';
                    while(min < max)
                    {
                        fixed=fixed+min;
                        fixed+='|';
                        min++;
                    }
                    fixed+=max;
                }
                
            }
            fixed+= ')';
            //cout<<range;
            i=j;
        }
        else
        {
            fixed=fixed+regex.at(i);
            //cout<<regex.at(i);
        }
        
    }
    
    return fixed;
}



int priority(char x,char mode)
{
	switch(x)
	{
		case '|':return 1;
			break;
		case ',':return 2;
			break;
		case '?':
		case '*':
		case '+':
			if(mode==ISP)
				return 3;
			else
				return 4;

			break;

		case '(':
			if(mode==ISP)
				return 0;
			else
				return 4;
			break;
		default: return 0;

	}
}

bool isOperand(char x)
{
	/*if((x<='Z')&&(x>='A'))
		return true;
	if((x<='z')&&(x>='a'))
		return true;
    if((x<='9')&&(x>='0'))
		return true;

	return false;*/
    
    if(x=='|'||x==','||x=='+'||x=='*'||x=='?'||x=='('||x==')')
        return false;
    return true;
}

bool isUnary(char x)
{
	if(x==')'||x=='*'||x=='+'||x=='?')
		return true;
	return false;
}

bool isPresent(vector<char> v, char c)
{
    int i;
    for(i=0; i<v.size(); i++)
    {
        if(v[i] == c)
            return true;
    }
    return false;
}

void setLanguage(string input)
{
    for(int i=0; i<input.size(); i++)
    {
        if((isOperand(input.at(i)))&&(!(isPresent(language, input.at(i)))))
            language.push_back(input.at(i));
    }
}



string addDot(string input)
{
	string output;
	unsigned int i;
	for(i=0;i<input.length()-1;i++)
	{
		if ((isOperand(input.at(i)) && isOperand(input.at(i+1)))||(isOperand(input.at(i))&&(input.at(i+1)=='('))||(isUnary(input.at(i))&&isOperand(input.at(i+1)))||(isUnary(input.at(i))&&(input.at(i+1) == '(')))
		{
			output+=(input.at(i));
			output+=(',');
		}
		else
		{
			output+=(input.at(i));
		}

	}
	output+=(input.at(i));
	output+='#';
	return output;
}

string postfix(string inp)
{
	string out;
	stack<char> stck;
	stck.push('#');
	char x, y;
	int i=0;
	x=inp.at(i++);
	while(x!='#')
	{
		//cout<<i<<" "<<x<<'\n';
		if(isOperand(x))
			out+=x;
		else if(x==')')
		{
			y=stck.top();
			stck.pop();
			while(y!='(')
			{
				out+=y;
				y=stck.top();
				stck.pop();
			}
		}
		else
		{
			y=stck.top();
			stck.pop();
			while(priority(y, ISP)>=priority(x, ICP))
			{
				out+=y;
				y=stck.top();
				stck.pop();
			}
			stck.push(y);
			stck.push(x);
		}
		x=inp.at(i++);
	}
	y=stck.top();
	stck.pop();
	while(y!='#')
	{
		out+=y;
		y=stck.top();
		stck.pop();

	}

	return(out);


}
int main()
{
	string regex, input;
	string regex_postfix;
	cout<<"Enter input expression : ";
	cin>>regex;
        regex=fixRanges(regex);
        cout<<regex<<endl;
    setLanguage(regex);
    cout<<"Input Language: ";
    for(int i=0; i<language.size();i++)
        cout<<language[i]<<"\t";
    cout<<endl;
	cout<<addDot(regex)<<"\n";
	regex_postfix=postfix(addDot(regex));
	cout<<"Corresponding Postfix expression: "<<regex_postfix<<endl;
	node *nfa=convertToNFA(regex_postfix);
	printNFA(nfa);
    DFA *dfa=convertToDFA(nfa);
    printDFA(dfa);
    //system("cls");
    do
    {
        cout<<"Enter String to check: ";
        cin>>input;
        if(checkString(dfa, input)== true)
            cout<<"Valid Expression\n";
        else
            cout<<"Invalid Expression\n";

    }while(input.at(0)!='$');

	return 0;
}


