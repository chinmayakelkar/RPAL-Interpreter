#include <iostream>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>
#include"parser.h"
#include <bits/stdc++.h>
using namespace std;
//global flags
int iflag=0,sflag=0,opflag=0,pucflag=0,numflag=0,comflag=0;
//iflag=identifier, sflag=string, opflag=operator, pucflag=punctuation, numflga=integer, comflag=comment
//structure for tree node
struct Tree_Node {
        std::string data; //data in node
        Tree_Node* left_child; //first child of the node
        Tree_Node* right_sibling;
    };
//create new node method
struct Tree_Node *newTree_Node(string key)
    {
        Tree_Node *temp = new Tree_Node;
        temp->data = key;
        temp->left_child=NULL;
        temp->right_sibling=NULL;
        return (temp);
    }
//stack of trees
stack<struct Tree_Node*> treetop;

stack<AST_node*> AST_stack;
AST_node *ast_tree= NULL;
//prototype declarations
void printPreorder(struct Tree_Node* node, int count1);
void Build_tree(string s, int count);
string Read_token(vector<char>& all_chars, int& nxtcount, string& token, int size1);
bool isWhitespace(char ch);
bool isOperator(char ch);
bool isPunction(char ch);
string analyze(string token);
bool isString(string token);
bool isInteger(string token);
bool isKeyword(string token);
vector<char> all_chars1;
string NT; //global next token variable
string token1; //local variable for token
int nxtcount1=0;
int size1;
int tracker=0;
void Read(string s);
//non terminal function declarations
void Expression ();
void ExpressionW();
void Tuple();
void TupleA();
void TupleC();
void BooleanExp();
void BooleanT();
void BooleanS();
void BooleanP();
void ArithE();
void ArithT();
void ArithF();
void ArithP();
void Operator();
void Operand();
void Definition();
void DefinitionA();
void Defrec();
void  DefinitionB();
void VariableB();

AST_node* standardize_within(AST_node* Within);
AST_node* standardize_where(AST_node *& Where);
AST_node* standardize_let(AST_node *& let_node);
AST_node * standardize_funform(AST_node *& func_form);
AST_node* standardize_at(AST_node *& At_Node);
AST_node* standardize_lambda(AST_node *& lambda_node);
AST_node* standardize_and(AST_node *& And_Node);
void standardize_rec(AST_node *& Rec_Node);

void generate_control_struct(AST_node *Standardized_AST, control_st *Control_Struct);
void standardize_tree(AST_node *node);
void build_tree(astNodeType node_type, string node_value, int num_node);
void print_ast_node(AST_node *in_node);
void print_control_struct(control_st *cnt_st);
void VariableL(int passed);
void print_AST_node(AST_node *node);
void preorder_traversal(AST_node *tree, int num_dots);
control_st::control_st():node(NULL), next(NULL), delta(NULL), bound_var(NULL), env(NULL), num_children(0) {}
control_st::~control_st(){}
void control_st::setNoofChildren(int num_child)
{
    num_children = num_child ;
    }
int control_st::getNoofChildren()
{
    return num_children ; }

environment::environment():parent(NULL) {} //enviroment constructor
/* Initializes AST_node left children and right sibling */
AST_node::AST_node(): left_child(NULL), right_sibling(NULL) {}

/* PENDING */
AST_node::~AST_node() {}

/* set the AST_node's value */
void AST_node::set_node_value(string node_value){ this->node_value = node_value; }

/* return the AST_node's value */
string AST_node::get_node_value() {return node_value ; }

/* set AST node's type */

void AST_node::set_node_type(astNodeType node_type) { this->node_type = node_type ; }

/* return the AST_node's type */

astNodeType AST_node::get_node_type() { return node_type ; }

CSE_machine::CSE_machine(control_st *cntrl_st){
	//create primitive environment
	current_env = new environment();
	i=0;
	//We need to put environment first of all in both stacks
	//create new execution_element of type environment
	execution_element *temp_exe_ele = new execution_element; //this is for EXECUTION ELEMENT
	temp_exe_ele->type = EXE_ENV;
	temp_exe_ele->env_var =  current_env;
	execution_stack.push(temp_exe_ele);
	//create new control struct for CONTROL STACK
	AST_node *new_ast_node = new AST_node;
	new_ast_node->set_node_type(ENV);
	control_st *new_control_st = new control_st;
	new_control_st->node = new_ast_node; //this node will tell the type of control struct in execution
	new_control_st->env = current_env; //this stores the environment
	control_stack.push(new_control_st);

    while(cntrl_st != NULL){
		control_stack.push(cntrl_st);
		cntrl_st = cntrl_st->next;
	}
}
control_st *CS=new control_st;

int main(int argc, char* argv[])
{
    if (argc < 2) {
        //do nothing if args <2
        return 1;
    }
    //second arg
    if(argc==2)
    {
    	ifstream file1 (argv[1]);
    	if (file1.is_open())
    	     {
    	         char ch;
    	         while ((file1.get(ch)))
    	                {
    	                    all_chars1.push_back(ch);
    	                }

    	         all_chars1.push_back(EOF); //append EOF. needed to read last token

    	         size1=all_chars1.size(); //total no of chars in given file
    	         //initialize the NT global variable
    	         NT=Read_token(all_chars1,nxtcount1,token1,size1);
    	         while(comflag==1)
    	         {
    	             comflag=0;
    	             NT=Read_token(all_chars1,nxtcount1,token1,size1);
    	         }
    	         Expression(); //first call to expression
    	            //print the ast
    	        ast_tree = AST_stack.top();

    	        standardize_tree(ast_tree);

    	        generate_control_struct(ast_tree, CS);

    	        CSE_machine cse_machine(CS->next);
    			cse_machine.execute();
    			cout<<endl;

    	            file1.close();
    	         }
    	    else {
    	            cout<<"Could not open file\n";
    	    }
    	     }
    if(argc==3)
    {
    	string s1=argv[1];
    	ifstream file1 (argv[2]);
    	if (file1.is_open())
     {

         char ch;
         while ((file1.get(ch)))
                {
                    all_chars1.push_back(ch);
                }
         if(s1=="-l")
         {
             for(int i=0; i<all_chars1.size();i++)
                cout<<all_chars1[i];
         }
         else if(s1=="-ast")
         {
         all_chars1.push_back(EOF); //append EOF. needed to read last token

         size1=all_chars1.size(); //total no of chars in given file
         //initialize the NT global variable
         NT=Read_token(all_chars1,nxtcount1,token1,size1);
         while(comflag==1)
         {
             comflag=0;
             NT=Read_token(all_chars1,nxtcount1,token1,size1);
         }
         Expression(); //first call to expression
            //print the ast
        ast_tree = AST_stack.top();
        preorder_traversal(ast_tree, 0);


         }
        file1.close();
     }
    	  else {
    	            cout<<"Could not open file\n";
    	    }
    }
    return 0;
}
string Read_token(vector<char>& all_chars, int& nxtcount, string& token, int size1) //lexical analyzer code
{

		string s;
		for(int i=nxtcount; i<size1;i++)
        {
            // char that looks ahead for operators,spaces and symbols
            char skipahead;
            skipahead=all_chars[i];
            //delimitors are space, ", ',operators, puncs, comments
            if(isWhitespace(skipahead))
            {
                if(token.length()>0)
                    {
                        s=analyze(token);
                        token.clear();
                        nxtcount=i+1;
                        break;
                    }
            }
           else if((skipahead=='/') && all_chars[i+1]=='/')
            {
                    if(token.length()>0)
                    {
                     s=analyze(token);
                     token.clear();
                     nxtcount=i;
                     break;
                    }
                    else{
                    int j=i;
                    while(all_chars[j]!='\n' && (j<size1))
                     {
                         j++;
                     }
                    nxtcount=j+1;
                    comflag=1;
                    break;
                    }
            }
            else if((skipahead=='*') && all_chars[i+1]=='*')
            {
                if(token.length()>0)
                    {
                     s=analyze(token);
                     token.clear();
                     nxtcount=i;
                     break;
                    }
                else
                {
                     token.push_back(skipahead);
                     token.push_back(all_chars[i+1]);
                     s=token;
                     token.clear();
                     nxtcount=i+2;
                     opflag=1;
                     break;
                }
            }
            else if((skipahead=='-') && all_chars[i+1]=='>')
            {
                if(token.length()>0)
                    {
                     s=analyze(token);
                     token.clear();
                     nxtcount=i;
                     break;
                    }
                else
                {
                     token.push_back(skipahead);
                     token.push_back(all_chars[i+1]);
                     s=token;
                     token.clear();
                     nxtcount=i+2;
                     opflag=1;
                     break;
                }
            }
            else if((skipahead=='>' || skipahead=='<') && all_chars[i+1]=='=')
            {
                if(token.length()>0)
                    {
                     s=analyze(token);
                     token.clear();
                     nxtcount=i;
                     break;
                    }
                else
                {
                     token.push_back(skipahead);
                     token.push_back(all_chars[i+1]);
                     s=token;
                     token.clear();
                     nxtcount=i+2;
                     opflag=1;
                     break;
                }
            }
            else if(isOperator(skipahead))
            {
                 if(token.length()>0)
                 {
                     s=analyze(token);
                     token.clear();
                     token.push_back(skipahead);
                     if(opflag==1 || pucflag==1)
                        nxtcount=i;
                     else
                        nxtcount=i+1;
                     break;
                 }
                 if(token.length()==0)
                 {
                     token.push_back(skipahead);
                     s=analyze(token);
                     nxtcount=i+1;
                     token.clear();
                     break;
                 }
            }
            else if (isPunction(skipahead))
            {
              if(token.length()>0)
              {
                  s=analyze(token);
                  token.clear();
                  token.push_back(skipahead);
                  if(pucflag==1 || opflag==1)
                    nxtcount=i;
                  else
                    nxtcount=i+1;
                  break;
            }
            if(token.length()==0)
                 {
                     token.push_back(skipahead);
                     s=analyze(token);
                     nxtcount=i+1;
                     token.clear();
                     break;
                 }
            }
           else if(skipahead=='\'')
            {
               //start of a string
                if(token.length()>0)
                {
                    s=analyze(token);
                    token.clear();
                    break;
                }
                 else
                 {
                     int j=i+1;
                     token.push_back(skipahead);
                     while(((all_chars[j]!='\'') ||(all_chars[j-1]=='\\')) && (j<size1))
                    {
                     token.push_back(all_chars[j]);
                     j++;
                }
                token.push_back(all_chars[j]); //add single quote or double quote of string to end
                nxtcount=j+1;
                s=analyze(token);
                break;
                }
            }

            else if(token.length()==1 && (isOperator(token[0])||isPunction(token[0])))
            {
                s=analyze(token);
                token.clear();
                token.push_back(skipahead);
                nxtcount=i;
                break;
            }
            else
            {
                //push only if not a whitespace
                if(!isWhitespace(skipahead))
                  {
                      token.push_back(skipahead);
                  }
            }
        }
        return s;
}
string analyze(string token)
{
    if(token.length()>0)
    {
        if((token.length()==1) && (isOperator(token[0])))
         {
             opflag=1; //operator flag set to 1
             iflag=0,sflag=0,pucflag=0,numflag=0; //all other flags are set to zero
             return token;
             }
        else if((token.length()==1) &&(isPunction(token[0])))
         {
             pucflag=1; //punctuation flag set to 1
             iflag=0,sflag=0,opflag=0,numflag=0; //all other flass set to zero
             return token;
             }
        else if(isInteger(token))
        {
            numflag=1; //integer flag set to 1
            iflag=0,sflag=0,opflag=0,pucflag=0; //all other flags set to zero
            return token;
        }
        else if(isString(token))
        {
            sflag=1; //string flag set to 1
            iflag=0,opflag=0,pucflag=0,numflag=0; //all other flads set to zero
            return token;
        }
         else if(isKeyword(token))
        {
                if(token=="gr"|| token=="ge" || token=="ls"||token=="le"||token=="eq"||token=="ne")
                    opflag=1;
                else
                    opflag=0;
                iflag=0,sflag=0,pucflag=0,numflag=0;
                return token;
        }
        else
        {
            iflag=1;
            sflag=0,opflag=0,pucflag=0,numflag=0; //all other flags set to zero
            return token;
        }
    }
}
bool isKeyword(string token)
{
    //array of keyword strings
    string keywords[]={"let","and","within","where","aug","rec","in", "eq","ne","le","ls","ge","gr","or"};
    for(int i=0; i<14;i++)
    {
        if(token==keywords[i])
            return true;
    }
    return false;
}
bool isString(string token)
{
    //string if first and last char is a '
    int last=token.length()-1;
    if((token[0]=='\'') && (token[last]=='\''))
        {
            return true;
        }
    else
        return false;
}
bool isWhitespace(char ch) {
	if (ch == ' ' || ch == '\t' || ch == '\n' || ch==EOF) {
		return true;
	} else {
		return false;
	}
}

bool isInteger(string token) {
	for(int i=0 ;i<token.length(); i++)
    {
        if(!isdigit(token[i]))
            return false;
    }
    return true;
}
bool isPunction(char ch)
{
    char puncs[]={'(',')',';',','};
    int flag=0;
    for(int i =0; i<4;i++)
    {
        if(ch==puncs[i])
        {
            flag=1;
            break;
        }
    }
    if(flag==1)
        return true;
    else
      return false;
}
bool isOperator(char ch) {
    char Operators[24]={'=','>','<','+','*','/','-','%','^','{','}','&','.','@',':','~','|','$','!','#','[',']','?','"'};
    int flag=0;
    for(int i=0;i<24;i++)
    {
        if(ch==Operators[i])
        {
            flag=1;
            break;
        }
    }
    if(flag==0)
        return false;
    else
        return true;
}
void Expression ()
{
 if(NT=="let")
 {
	Read("let");
	Definition();
	Read("in");
	Expression();
	//Build_tree("let",2);
	build_tree(LET,"let",2);
	}
else if(NT=="fn")
{
	Read("fn");
	int i=0;
	do{
		VariableB();
		i++;
	}while(NT=="(" || iflag==1);

	Read(".");
	Expression();
	//Build_tree("lambda",i+1);
	build_tree(LAMBDA,"lambda",i+1);
}
else{
		ExpressionW();
}
}

void ExpressionW()
{
	Tuple();
	if(NT=="where")
	{
		Read("where");
		Defrec();
		//Build_tree("where",2);
		build_tree(WHERE,"where",2);
	}
	return;
}
void Tuple()
{
	TupleA();
	int i=0;
	while(NT==",")
	{
		Read(",");
		TupleA();
		i++;
	}
	if(i>0)
        {
            //Build_tree("tau",i+1);
            build_tree(TAU, "tau", i+1);
        }
}
void TupleA()
{
	TupleC();
	while(NT=="aug")
	{
		Read("aug");
		TupleC();
		//Build_tree("aug",2);
		build_tree(AUG, "aug", 2);
	}
	return;
}

void TupleC()
{
	BooleanExp();
	if(NT=="->")
	{
		Read("->");
		TupleC();
		Read("|");
		TupleC();
		//Build_tree("->",3);
		build_tree(COND, "->", 3);
	}
}

void BooleanExp()
{
	BooleanT();
	while(NT=="or")
	{
		Read("or");
		BooleanT();
		//Build_tree("or",2);
		build_tree(OR, "or", 2);
	}
	return;
}
void BooleanT()
{
	BooleanS();
	while(NT=="&")
	{
		Read("&");
		BooleanS();
		//Build_tree("&",2);
		build_tree(AMP, "&", 2);
	}
	return;
}

void BooleanS()
{
	if(NT=="not")
		{
			Read("not");
			BooleanP();
			//Build_tree("not",1);
			build_tree(NOT, "not", 1);
			}
	else
		{
			BooleanP();
		}
}
void BooleanP()
{
	ArithE();
	if(NT=="gr"||NT==">")
	{
		Read(NT);
		ArithE();
		//Build_tree("gr",2);
		build_tree(GR, "gr", 2);
		}
	else if(NT=="ge"||NT==">=")
	{
		Read(NT);
		ArithE();
		//Build_tree("gr",2);
		build_tree(GE, "ge", 2);
		}
	else if(NT=="ls"||NT=="<")
	{
		Read(NT);
		ArithE();
		//Build_tree("ls",2);
		build_tree(LS, "ls", 2);
	}
	else if(NT=="le"||NT=="<=")
	{
		Read(NT);
		ArithE();
		//Build_tree("le",2);
        build_tree(LE, "le", 2);
	}
	else if(NT=="eq")
	{
		Read(NT);
		ArithE();
		//Build_tree("eq",2);
		build_tree(EQ, "eq", 2);
	}
	else if(NT=="ne")
	{
		Read(NT);
		ArithE();
		//Build_tree("ne",2);
		build_tree(NE, "ne", 2);
	}
	else
	{
		return;
	}
}
void ArithE()
{
    string temp="";
	if(NT=="-")
	{
		Read(NT);
		ArithT();
		//Build_tree("neg",1);
		build_tree(NEG, "neg", 1);
		}
	else if(NT=="+")
	{
		Read(NT);
		ArithT();
	}
	else
		{
			ArithT();
		}
	while(NT=="-"||NT=="+")
	{
        temp=NT;
		Read(NT);
		ArithT();
		//Build_tree(temp,2);
		if(temp.compare("+") == 0 )
			build_tree(PLUS, temp, 2);
		else
			build_tree(MINUS, temp, 2);
	}
}

void ArithT()
{
	string temp="";
	ArithF();
	while(NT=="*"||NT=="/"){
		temp=NT;
		Read(NT);
		ArithF();
		//Build_tree(temp,2);
		if( temp.compare("*") == 0)
				build_tree(MULTI, temp, 2);
			else
				build_tree(DIV, temp, 2);
	}
	return;
}

void ArithF()
{
	ArithP();
	if(NT=="**")
	{
		Read(NT);
		ArithF();
		//Build_tree("**",2);
		build_tree(EXPO, "**", 2);
	}
	return;
}
void ArithP()
{
	Operator();
	while(NT=="@")
	{
		Read(NT);
		Read("IDENTIFIER");//set iflag to zero and read the next token
		Operator();
		//Build_tree("@",3);
		build_tree(AT, "@", 3);
	}
}
void Operator()
{
	Operand();
	while(iflag==1 || numflag==1 || sflag==1 || NT=="true" || NT=="false" || NT=="nil" || NT=="(" || NT=="dummy")
	{
		Operand();
		//Build_tree("gamma",2);
		build_tree(GAMMA, "gamma", 2);
	}
}

void Operand()
{
	if(iflag==1)
	{
		Read("IDENTIFIER");
        //set iflag=0 and read next token last step in Read function
	}
	else if(sflag==1)
	{
		Read("STRING"); //set sflag=0 and read next token
	}
	else if(numflag==1)
	{
		Read("INTEGER");//set numflag=0 and read next token
	}
	else if(NT=="true")
	{
		Read(NT);
		//Build_tree("true",1);
		build_tree(TRUE, "true", 1);
	}
	else if(NT=="false")
	{
		Read(NT);
		//Build_tree("false",1);
		build_tree(FALSE, "false", 1);
	}
	else if(NT=="nil")
	{
		Read(NT);
		//Build_tree("nil",1);
		build_tree(NIL, "nil", 1);
	}
	else if(NT=="(")
	{
		Read(NT);
		Expression();
		Read(")");
	}
	else if(NT=="dummy")
	{
		Read("dummy");
		//Build_tree("dummy",0);
		build_tree(DUMMY, "dummy", 0);
	}
	else
	{
		exit(EXIT_FAILURE);
	}
}

void Definition()
{
	DefinitionA();
	if(NT=="within")
	{
		Read("within");
		DefinitionA();
		//Build_tree("within",2);
		build_tree(WITHIN, "within", 2);
	}
	return;
}

void DefinitionA()
{
	Defrec();
	int k=0;
	while(NT=="and")
	{
		Read("and"); //
		Defrec();
		k++;
	}
 	if(k>0)
     build_tree(AND, "and", k+1 );
     //Build_tree("and",k+1);
    return;
}
void Defrec()
{
	if(NT=="rec")
	{
		Read("rec");
		DefinitionB();
		//Build_tree("rec",1);
		build_tree(REC, "rec", 1);
	}
	else
	{
	  DefinitionB();
	  }
    return;
}

void  DefinitionB()
{
	int count4=0;
	if(NT=="(")
	{
		Read("(");
		Definition();
		Read(")");
	}
	if(iflag==1)
	{
		Read("IDENTIFIER");
		count4=1;
		//set iflag=0 and read next token;
		if(NT=="=")
		{
			Read("=");
			Expression(); //no call to VariableL needed
			//Build_tree("=",2);
			build_tree(EQUAL, "=", 2);
		}
		else if(NT==",")
		{
		   Read(",");
		   VariableL(1);
		   Read("="); //count+1 since "identifier ," executed inside Db
		   Expression();
		   build_tree(EQUAL, "=", 2);
		   //Build_tree("=",2);
		   }
		else
		{
			do{
				VariableB();
				count4++;
		}while(NT=="(" || iflag==1);
		Read("=");
		Expression();
		//Build_tree("function_form",count4+1);
		build_tree(FUN_FORM, "fcn_form", count4+1);
		}
	}
	return;
}

void VariableB()
{
	if(iflag==1)
		{
		    Read("IDENTIFIER");
		}
	else
	{
		Read("(");
		int flag=0;
			if(iflag==1)
			{
				flag=1;
				VariableL(0);
				}
		Read(")");
		if(flag==0)
		{
		    build_tree(PARAN, "()", 0);
			//Build_tree("()",0);
			}
	}
}

void VariableL(int passed)
{
    int count3;
	if(iflag==1)
		Read("IDENTIFIER");
       //read next token and set iflag=0
	if(passed==1)
	  count3=2;
	else
	  count3=1;
	while(NT==",")
	{
		Read(",");
		Read("IDENTIFIER");
		count3++;
	}
	if(count3>1)
		{
		    //Build_tree(",",count3);
		    build_tree(COMMA, ",", count3);
		}
}

void printPreorder(struct Tree_Node* node, int count1)
    {
         if (node == NULL)
              return;
         int count2=count1;
        /* first print data of node */
         if(count2>0)
            {
                for(int i=0; i<count2; i++)
                    cout<<".";
                cout<<node->data<<endl;
            }
        else
            cout<<node->data<<endl;
        count2++;
         /* then recur on left subtree */

        printPreorder(node->left_child,count2);
         /* now recur on right subtree */
        printPreorder(node->right_sibling,count2-1);

    }

void Read(string s)
{
	if(s=="IDENTIFIER" && iflag==0)
	{
		exit(EXIT_FAILURE); //end the program and send failure message
	}
	if(s=="STRING" && sflag==0)
	{
		exit(EXIT_FAILURE); //end the program and send failure message
	}
	if(s=="INTEGER" && numflag==0)
	{
		exit(EXIT_FAILURE); //end the program and send failure message
	}
	if(s=="IDENTIFIER" && iflag==1)
	{
		//Build_tree(NT,0);
		build_tree(IDT,NT, 0);
		iflag=0;
	}
	else if(s=="STRING" && sflag==1)
		{
		    build_tree(STR,NT,0);
		    //Build_tree(NT,0);
		    }
	else if(s=="INTEGER" && numflag==1)
	{
	    build_tree(INT, NT, 0);
		//Build_tree(NT,0);
		numflag=0;
	}
	else if(s!=NT)
	{
		exit(EXIT_FAILURE); //end the program and send failure message
	}
	if((opflag==1 || pucflag==1) && (all_chars1[nxtcount1]!='\''))
	{
		token1.clear();
	}
    if(sflag==1)
	{
		token1.clear();
		sflag=0;
	}
    NT=Read_token(all_chars1,nxtcount1,token1,size1);
    while(comflag==1)
	 {
		comflag=0;//if comment once again read token to get next valid token & reset comm flag
		NT=Read_token(all_chars1,nxtcount1,token1,size1);
	 }
}

void Build_tree(string s, int j)
{
	if(j==0)
		{
			if(iflag==1)
			{
				string key;
				if(s=="true" || s=="false" || s=="dummy" || s=="nil")
                    key="<"+s+">";
                else
                    key="<ID:"+s+">";
				treetop.push(newTree_Node(key));
			}
			else if (numflag==1)
			{
				string key="<INT:"+s+">";
				treetop.push(newTree_Node(key));
			}
			else if(sflag==1)
			{
				string key="<STR:"+s+">";
				treetop.push(newTree_Node(key));
			}
		}
	if(j>0)
	{
		stack<struct Tree_Node*>temp_st;
		for(int i=0;i<j;i++)
        {
            temp_st.push(treetop.top());
            treetop.pop();
        }
		struct Tree_Node* temp1=temp_st.top();
    	temp_st.pop();
		int k=j-1;
    	struct Tree_Node* temp2=temp1;
    	for(int i=0; i<k && !temp_st.empty();i++)
        {
            struct Tree_Node* temp=temp_st.top();
            temp_st.pop();
            temp2->right_sibling=temp;
            temp2=temp;
            }
        if(s=="nil" || s=="true" || s=="false" || s=="dummy")
            s="<"+s+">";
        struct Tree_Node*newnode=newTree_Node(s);
		newnode->left_child=temp1;
		treetop.push(newnode);
	}
}

void build_tree(astNodeType node_type, string node_value, int num_node) {
		//create new node
	AST_node *new_node = new AST_node;
	//set type & value of new node
	new_node->set_node_type(node_type);

	new_node->set_node_value(node_value);

	if(new_node->get_node_type() == IDT)
    {
        if(new_node->get_node_value().compare("nil")==0)
            {
                new_node->set_node_type(NIL);
            }
        if(new_node->get_node_value().compare("true")==0)
            {
                new_node->set_node_type(TRUE);
            }
        if(new_node->get_node_value().compare("false")==0)
        {
            new_node->set_node_type(FALSE);
        }
        if(new_node->get_node_value().compare("dummy")==0)
        {
            new_node->set_node_type(DUMMY);
        }
    }
    if(new_node->get_node_type()==STR)
    {
        string temp=new_node->get_node_value();
        new_node->set_node_value(temp.substr(1,temp.size()-2));
    }
	//create temp AST_node to hold temp node
	AST_node *temp_node = NULL;
	//for i 1 to n pop and create sibling list
	for(int i=1; i<=num_node; i++){
		AST_node *saved_popped = AST_stack.top(); //save the popped node
		AST_stack.pop();

		if( temp_node != NULL) {
			saved_popped->right_sibling = temp_node;
			temp_node = saved_popped;
		}
		else{ //first popped node
			temp_node = saved_popped;
		}
	}
	//set new node's left children
	new_node->left_child = temp_node;
	//push the new node into stack
	AST_stack.push(new_node);
}

void preorder_traversal(AST_node *tree, int num_dots){
	AST_node *temp = tree;
	for(int i=1;i<=num_dots; i++)
		cout<<".";
    print_AST_node(tree);
	if( temp->left_child != NULL){
		preorder_traversal(temp->left_child, num_dots+1);
	}
	if( temp->right_sibling != NULL)
		preorder_traversal(temp->right_sibling, num_dots);
}

void print_AST_node(AST_node *node) {
	if(node->get_node_type() == STR)
		cout<<"<STR:'"<<node->get_node_value()<<"'>"<<endl;
	else if(node->get_node_type() == IDT)
    {
        if(node->get_node_value().compare("nil")==0)
            {
                cout<<"<nil>"<<endl;
                node->set_node_type(NIL);
            }
        else if(node->get_node_value().compare("true")==0)
        {
            cout<<"<true>"<<endl;
            node->set_node_type(TRUE);
        }
        else if(node->get_node_value().compare("false")==0)
        {
            cout<<"<false>"<<endl;
            node->set_node_type(FALSE);
        }
        else if(node->get_node_value().compare("dummy")==0)
        {
            cout<<"<dummy>"<<endl;
            node->set_node_type(DUMMY);
        }
        else
         cout<<"<ID:"<<node->get_node_value()<<">"<<endl;
    }

	else if(node->get_node_type() == INT)
		cout<<"<INT:"<<node->get_node_value()<<">"<<endl;
	else if(node->get_node_type() == YSTAR)
		cout<<"<"<<node->get_node_value()<<">"<<endl;
	else if(node->get_node_type() == FUN_FORM)
		cout<<"function_form"<<endl;
	else if(node->get_node_type() == TRUE)
		cout<<"<true>"<<endl;
	else if(node->get_node_type() == FALSE)
		cout<<"<false>"<<endl;
	else if(node->get_node_type() == DUMMY)
		cout<<"<dummy>"<<endl;
	else if(node->get_node_type() == NIL)
		cout<<"<nil>"<<endl;
	else
		cout<<node->get_node_value()<<endl;
}


void standardize_tree(AST_node *node){ //this is to standardized let node
	if(node->left_child != NULL )
		standardize_tree(node->left_child);
	if( node->right_sibling != NULL )
		standardize_tree(node->right_sibling);
	if( node->get_node_type() == LET)
		standardize_let(node);

	else if( node->get_node_type() == WHERE)
	     standardize_where(node);

	else if( node->get_node_type() == FUN_FORM)
	     standardize_funform(node);

	else if( node->get_node_type() == WITHIN)
	   standardize_within(node);

	else if( node->get_node_type() == AND)
	    standardize_and(node);

	else if( node->get_node_type() == AT)
	    standardize_at(node);

	else if( node->get_node_type() == REC)
	   standardize_rec(node);
}


AST_node* standardize_let(AST_node *& let_node){

	if(let_node == NULL){
		cout<<"let - node is NULL"<<endl;
		exit(0);
	}
	//set AST_node's value and type to gamma
	let_node->set_node_value("gamma");
	let_node->set_node_type(GAMMA);

	//set AST_node's left child's value and type to lambda
	if( (let_node->left_child) != NULL){
		(let_node->left_child)->set_node_value("lambda");
		(let_node->left_child)->set_node_type(LAMBDA);
	}
	else{
		cout<<"let node's left child is NULL !"<<endl;
		exit(0);
	}

	//save AST_node's right child
	AST_node *prev_right_child = (let_node->left_child)->right_sibling;
	if(prev_right_child == NULL){
		cout<<"let node's right child is NULL !"<<endl;
		exit(0);
	}

	//swap AST_node's right child with AST_node's left child's right child
	if( ((let_node->left_child)->left_child)->right_sibling != NULL ) {
		(let_node->left_child)->right_sibling = ((let_node->left_child)->left_child)->right_sibling;
		((let_node->left_child)->left_child)->right_sibling = prev_right_child;
	}

	else{
		cout<<"Expression is NULL !"<<endl;
		exit(0);
	}
	//return gamma node...previously let node
	return let_node;
}


AST_node* standardize_where(AST_node *& Where){
	if(Where == NULL){
		cout<<"where- node is NULL"<<endl;
		exit(0);
	}

	//set AST_node's value and type to gamma
	Where->set_node_value("gamma");
	Where->set_node_type(GAMMA);

	AST_node *P, *E, *X; //create temps to store org node's childrena

	//First pass - change & save the values of node
	if(Where->left_child != NULL)
		P = Where->left_child;
	else{
		cout<<"where- left_child is NULL"<<endl;
		exit(0);
	}

	//change the equal node to lambda
	if(P->right_sibling != NULL){
		//check if it is equal
		if( (P->right_sibling)->get_node_type() == EQUAL ){
			(P->right_sibling)->set_node_type(LAMBDA);
			(P->right_sibling)->set_node_value("lambda");
		}

		else{
			cout<<"where- right child is not equal as expected"<<endl;
			exit(0);
		}
	}

	else{
		cout<<"where- right child(sibling) is NULL"<<endl;
		exit(0);
	}
	//save X
	X = (P->right_sibling)->left_child ;

	if( X == NULL){
		cout<<"where- X is NULL !"<<endl;
		exit(0);
	}

	//save E

	E = X->right_sibling;
	if( E == NULL){
		cout<<"where- E is NULL !"<<endl;
		exit(0);
	}

	//second pass - change the pointers

	Where->left_child = P->right_sibling; //make lambda a left child of gamma
	P->right_sibling = NULL;
	Where->left_child->right_sibling= E;
	X->right_sibling = P;

	return Where;
}


AST_node* standardize_within(AST_node* Within){
	if(Within == NULL){
			cout<<"within - node is NULL !!"<<endl;
			exit(0);
	}

	AST_node *X1=NULL, *X2=NULL, *E1=NULL, *E2=NULL; //some temps to hold children

	//save X1
	if( Within->left_child != NULL )
		X1 = Within->left_child->left_child;
	else{
		cout<<"within - left child is NULL ! "<<endl;
		exit(0);
	}

	//save X2
	if( Within->left_child->right_sibling != NULL )
		X2 = Within->left_child->right_sibling->left_child; //yeah a little complex
	else{
		cout<<"within - Right children is NULL! "<<endl;
		exit(0);
	}

	//store E1
	if(X1 != NULL)
		E1 = X1->right_sibling;
	else{
		cout<<"within - X1 is NULL ! "<<endl;
		exit(0);
	}

	//store E2
	if( X2 != NULL)
		E2 = X2->right_sibling;
	else{
		cout<<"within - X2 is NULL! "<<endl;
		exit(0);
	}

	Within->set_node_value("=");
	Within->set_node_type(EQUAL);

	Within->left_child = X2; //make X2 left child

	AST_node *gammanode = new AST_node;
	gammanode->set_node_value("gamma");
	gammanode->set_node_type(GAMMA);

	X2->right_sibling = gammanode; //make new gamma node a right children of eq
    AST_node *lambdanode=new AST_node;
	lambdanode->set_node_value("lambda");
	lambdanode->set_node_type(LAMBDA);

	X2->right_sibling->left_child = lambdanode; //make lambda node the left children of gamma
	lambdanode->right_sibling = E1;
	lambdanode->left_child = X1;

	X1->right_sibling = E2;
//Within is now equals node
	return Within;
}


AST_node * standardize_funform(AST_node *& func_form){
	if(func_form == NULL){
		cout<<"standardize_funform- node is NULL !!"<<endl;
		exit(0);
	}
	//convert fcn_form node to equal
	func_form->set_node_type(EQUAL);
	func_form->set_node_value("=");

	AST_node *P=NULL, *V=NULL, *temp_lambda=NULL;

	P = func_form->left_child; //P holds first or left most child of func_form
	if( P == NULL ) {
		cout<<"function form - left child P is NULL ! "<<endl;
		exit(0);
	}

	V = P->right_sibling; //V could be NULL but it shouldn't be
	if( V == NULL ) {
		cout<<"function form - right child V is NULL ! "<<endl;
		exit(0);
	}

	while( V->right_sibling != NULL){
		//create a cascade of lambdas
		temp_lambda = new AST_node;
		temp_lambda->set_node_value("lambda");
		temp_lambda->set_node_type(LAMBDA);

		//set V as right sibling of lambda
		temp_lambda->left_child= V;
		//set newly created lambda node as right sibling of P
		P->right_sibling = temp_lambda;
		P = V;
		V = V->right_sibling;
	}

	//we have Expression here in V
	P->right_sibling = V;
    //function form node is now equals
	return func_form;
}
//this standardizes @ node
AST_node* standardize_at(AST_node *& At_Node){

	if(At_Node == NULL){
		cout<<"standardize_at- node is NULL !!"<<endl;
		exit(0);
	}
	//save E1,N & E2
	AST_node *E1=NULL, *N=NULL, *E2=NULL; //some temps to store E1, N and E2

	E1 = At_Node->left_child; //shouldn't be NULL
	if(E1 == NULL){
		cout<<"@ node - Left child E1 is NULL !"<<endl;
		exit(0);
	}

	N = E1->right_sibling; //shouldn't be NULL
	if(N == NULL){
		cout<<"@ node - Child N is NULL !"<<endl;
		exit(0);
	}

	E2 = N->right_sibling; //shouldn't be NULL
	if(E2 == NULL){
		cout<<"@ node Child E2 is NULL !"<<endl;
		exit(0);
	}
	//change the at node to gamma
	At_Node->set_node_value("gamma");
	At_Node->set_node_type(GAMMA);

	//create a new gamma node
	AST_node *temp_gamma = new AST_node;
	temp_gamma->set_node_type(GAMMA);
	temp_gamma->set_node_value("gamma");

	//make the newly created gamma node a left child of main gamma node
	At_Node->left_child = temp_gamma;

	//make E2 right sibling of new gamma node
	temp_gamma->right_sibling = E2;

	//make N right sibling of new gamma node
	temp_gamma->left_child = N;

	//make E1 right sibling of N prior to this it was E2
	N->right_sibling = E1;

	E1->right_sibling = NULL; //prior to standardization E1's right sibling was N

	return At_Node;
}

AST_node* standardize_lambda(AST_node *& lambda_node){
	if(lambda_node == NULL){
		cout<<"lambda- node is NULL !!"<<endl;
		exit(0);
	}

	AST_node *temp1=NULL, *V_next=NULL; //some temps
	bool standardize = false; //will decide if node needs to be standardized or not

	temp1 = lambda_node->left_child; //this shouldn't be NULL
	if(temp1 == NULL){
		cout<<"lambda - left child V is NULL !"<<endl;
		exit(0);
	}

	V_next = temp1->right_sibling; //this shouldn't be NULL either, either E or another identifier
	if(V_next == NULL){
		cout<<"lambda - right child V or E is NULL !"<<endl;
		exit(0);
	}

	while( V_next->right_sibling != NULL){
		standardize = true; //node has more than one identifier so it needs to be standarized

		//create new lambda node
		AST_node *temp_lambda = new AST_node;
		temp_lambda->set_node_type(LAMBDA);
		temp_lambda->set_node_value("lambda");

		temp1->right_sibling = temp_lambda; //make new lambda node a right sibling of V
		temp_lambda->left_child = V_next;

		//Advance
		temp1 = V_next;
		V_next = V_next->right_sibling;
	}

	if(standardize)
		temp1->right_sibling = V_next;

	return lambda_node;
}

AST_node* standardize_and(AST_node *& And_Node){

	if(And_Node == NULL){
		cout<<"standardize_and- node is NULL !!"<<endl;
		exit(0);
	}

	AST_node *equal=NULL;//
	equal = And_Node->left_child;
	if(equal == NULL){
		cout<<"and - left child EQUAL is null !"<<endl;
		exit(0);
	}

	AST_node *new_equal_node=NULL, *newCommaNode=NULL, *newTauNode=NULL, *temp=NULL;
	new_equal_node = new AST_node;
	new_equal_node->set_node_type(EQUAL);
	new_equal_node->set_node_value("=");

	//change and_node to EQUAL
	And_Node->set_node_type(EQUAL);
	And_Node->set_node_value("=");

	newCommaNode= new AST_node;
	newCommaNode->set_node_type(COMMA);
	newCommaNode->set_node_value(",");

	newTauNode= new AST_node;
	newTauNode->set_node_type(TAU);
	newTauNode->set_node_value("tau");

	And_Node->left_child = newCommaNode;
	newCommaNode->right_sibling = newTauNode;


	newCommaNode->left_child = equal->left_child; //shouldn't be NULL
	newTauNode->left_child= newCommaNode->left_child->right_sibling; //shouldn't be NULL
	if(newCommaNode == NULL || newTauNode == NULL){
		cout<<"one of equal's children is NULL !"<<endl;
		exit(0);
	}
	
	newCommaNode = newCommaNode->left_child;
	newTauNode = newTauNode->left_child;

	temp = equal;
	equal = equal->right_sibling; //go to next equal node
	delete temp;

	while( equal != NULL){
		newCommaNode->right_sibling = equal->left_child; //shouldn't be NULL
		newTauNode->right_sibling = newCommaNode->right_sibling->right_sibling; //shouldn't be NULL
		if(newCommaNode == NULL || newTauNode == NULL){
			cout<<"standardize_and = one of equal's children is NULL !"<<endl;
			exit(0);
		}

		newCommaNode = newCommaNode->right_sibling;
		newTauNode = newTauNode->right_sibling;

		temp = equal;
		equal = equal->right_sibling; //go to next equal node
		delete temp;
	}

	newCommaNode->right_sibling = NULL; //prior to standardization it was pointing to Expression

	return And_Node;

}

void standardize_rec(AST_node *& Rec_Node){
	if(Rec_Node== NULL){
		cout<<"rec- node is NULL !!"<<endl;
		exit(0);
	}

	//change rec to EQUAL
	Rec_Node->set_node_type(EQUAL);
	Rec_Node->set_node_value("=");

	AST_node *eq=NULL, *X=NULL, *E=NULL; //to store temps

	eq = Rec_Node->left_child; //it shouldn't be NULL
	if(eq == NULL){
		cout<<"rec - left child = is NULL!"<<endl;
		exit(0);
	}

	X = eq->left_child; //shouldn't be NULL
	if(X == NULL){
		cout<<"rec - left child X of = is NULL!"<<endl;
		exit(0);
	}

	E = X->right_sibling;
	if(E == NULL){
		cout<<"rec - right child E of = is NULL!"<<endl;
		exit(0);
	}

	//make X left child of main EQUAL
	Rec_Node->left_child = X;

	//create new gamma node
	AST_node *newGammaNode = new AST_node;
	newGammaNode->set_node_type(GAMMA);
	newGammaNode->set_node_value("gamma");

	//put this new node as right children of EQUAL
	X->right_sibling = newGammaNode;

	//create new Y* node
	AST_node *YStar = new AST_node;
	YStar->set_node_type(YSTAR);
	YStar->set_node_value("Y*");

	//shove it in..i mean put it as left children of new gamma
	newGammaNode->left_child = YStar;

	//create new lambda node
	AST_node *newLambdaNode = new AST_node;
	newLambdaNode->set_node_type(LAMBDA);
	newLambdaNode->set_node_value("lambda");

	//put it in as right child of gamma
	YStar->right_sibling = newLambdaNode;

	AST_node *Copy_of_X = new AST_node;
	Copy_of_X->set_node_type( X->get_node_type() );
	Copy_of_X->set_node_value( X->get_node_value() );
	Copy_of_X->left_child = X->left_child;

	newLambdaNode->left_child = Copy_of_X;
	Copy_of_X->right_sibling= E;
}

void generate_control_struct(AST_node *Standardized_AST, control_st *Control_Struct){ //this will recursively generate the control structure
	//standard_ast can't be NULL
	if( Standardized_AST == NULL ){
		cout<<"generate_control_struct - standard_ast is NULL! "<<endl;
		exit(0);
	}
	//if node is lambda. this case won't have any left  child only right sibling
	if( Standardized_AST->get_node_type() == LAMBDA){
		//create new CS node
		control_st *Temp_Control_St = new control_st;

		Temp_Control_St->node = Standardized_AST;  //lambda node
		AST_node *save_right_sibling = Standardized_AST->left_child->right_sibling; //because if there isn't comma next few lines will make it NULL
		if(Standardized_AST->left_child->get_node_type() != COMMA){
			Temp_Control_St->bound_var = Standardized_AST->left_child; //if this is comma it would be left child of left child
			Temp_Control_St->bound_var->right_sibling = NULL; //there could only be one variable
		}
		else
			Temp_Control_St->bound_var = Standardized_AST->left_child->left_child; //if this is comman it would be left child of left child

		Control_Struct->next = Temp_Control_St;
		Control_Struct = Control_Struct->next; //move forward

		control_st *new_cs = new control_st;
		generate_control_struct(save_right_sibling, new_cs); //this  will return the control struct of right child
		//link the new cs
		Control_Struct->delta = new_cs->next;

		delete new_cs;

		if(Standardized_AST->right_sibling != NULL)
			generate_control_struct(Standardized_AST->right_sibling, Control_Struct);
	}

	//else if node is tau. this case won't have any left child only right sibling
	else if( Standardized_AST->get_node_type() == TAU){
		//create new tau node
		control_st *Temp_Control_St = new control_st;
		Temp_Control_St->node = Standardized_AST;

		Control_Struct->next = Temp_Control_St;
		Control_Struct = Control_Struct->next;

		AST_node *temp_ast_node = Standardized_AST->left_child;
		int noOfChildren = 0;

		generate_control_struct(temp_ast_node, Control_Struct);

		while(temp_ast_node != NULL){
			noOfChildren++;
			temp_ast_node = temp_ast_node->right_sibling;
		}
		//set the num of children in tau
		Control_Struct->setNoofChildren(noOfChildren);

		while(Control_Struct->next != NULL)
			Control_Struct = Control_Struct->next; //because generate_control_struct above can generate more than one number of control struct

		if(Standardized_AST->right_sibling != NULL)
			generate_control_struct(Standardized_AST->right_sibling, Control_Struct);
	}

	else if( Standardized_AST->get_node_type() ==  COND){
		//Break the B, T & E and save each of them
		AST_node *BNode=NULL, *TNode=NULL, *ENode=NULL;

		BNode = Standardized_AST->left_child; //shouldn't be NULL
		if(BNode == NULL){
			cout<<"left child (B) is NULL! "<<endl;
			exit(0);
		}

		TNode = BNode->right_sibling; //shoudln't be NULL
		if(TNode == NULL){
			cout<<"middle child (T) is NULL! "<<endl;
			exit(0);
		}

		ENode = TNode->right_sibling; //shouldn't be NULL
		if(ENode == NULL){
			cout<<"right child (E) is NULL! "<<endl;
			exit(0);
		}
		//break all right siblings
		BNode->right_sibling = NULL;
		TNode->right_sibling = NULL;

		AST_node *temp_delta_then = new AST_node;
		temp_delta_then->set_node_type(DELTA_THEN);
		temp_delta_then->set_node_value("delta_then");

		//create new control struct
		control_st *Temp_Control_Struct = new control_st;
		Temp_Control_Struct->node = temp_delta_then;
		Control_Struct->next = Temp_Control_Struct; //link the newly created control struct in the list
		Control_Struct = Control_Struct->next;

		//generate control struct for temp_T;
		control_st *temp_cntst = new control_st;
		generate_control_struct(TNode, temp_cntst); //temp_cntst's next have control struct
		Control_Struct->delta = temp_cntst->next;
		delete temp_cntst;

		AST_node *temp_delta_else = new AST_node;
		temp_delta_else->set_node_type(DELTA_ELSE);
		temp_delta_else->set_node_value("delta_else");

		//create new control struct
		control_st *temp_cs_else= new control_st;
		temp_cs_else->node = temp_delta_else;
		Control_Struct->next = temp_cs_else; //link the newly created control struct in the list
		Control_Struct = Control_Struct->next;
		//generate control struct for temp_T;
		control_st *temp_cstruct= new control_st;

		generate_control_struct(ENode, temp_cstruct); //temp_cntst's next have control struct

		Control_Struct->delta = temp_cstruct->next;
		delete temp_cstruct;

		//create BETA control structure
		AST_node *temp_beta = new AST_node;
		temp_beta->set_node_type(BETA);
		temp_beta->set_node_value("beta");

		control_st *temp_beta_cs = new control_st;
		temp_beta_cs->node = temp_beta;
		Control_Struct->next = temp_beta_cs;
		Control_Struct =Control_Struct->next;
		
		control_st *temp_cstruct_B= new control_st;

		generate_control_struct(BNode, temp_cstruct_B); //temp_cntst's next have control struct
		Control_Struct->next= temp_cstruct_B->next;
		delete temp_cstruct_B;
		while(Control_Struct->next != NULL)
			Control_Struct = Control_Struct->next;

		if(Standardized_AST->right_sibling != NULL)
			generate_control_struct(Standardized_AST->right_sibling, Control_Struct);
	}

	else{
		//create new CS node
		control_st *temp_cs = new control_st;
		//set its values
		temp_cs->node = Standardized_AST; //save the current standardized tree node
		//move forward the CS pointer
		Control_Struct->next = temp_cs;

		Control_Struct = Control_Struct->next;
		//recursively do the same for both left_child and right_sibling
		if(Standardized_AST->left_child != NULL) //will generate only one CS node
			generate_control_struct(Standardized_AST->left_child, Control_Struct);
		while(Control_Struct->next != NULL)
			Control_Struct = Control_Struct->next;
		if(Standardized_AST->right_sibling != NULL)
			generate_control_struct(Standardized_AST->right_sibling, Control_Struct);
	}
}

void print_control_struct(control_st *cnt_st){

	while(cnt_st != NULL){

		print_ast_node(cnt_st->node);
		if( cnt_st->node->get_node_type() == TAU)
			cout<<"Number of children: "<<cnt_st->getNoofChildren()<<endl;

		AST_node *temp_bv = cnt_st->bound_var;
		while(temp_bv != NULL){
			cout<<"\t";
			cout<<" ("<<temp_bv->get_node_value()<<")";
			temp_bv = temp_bv->right_sibling;
		}
		cout<<endl;
        control_st *deltanext=new control_st;
        deltanext=cnt_st->delta;
        cnt_st = cnt_st->next;
		if(deltanext != NULL)
			print_control_struct(deltanext);

	}
}

void print_ast_node(AST_node *in_node){
	cout<<"<";
	switch(in_node->get_node_type()){
		case IDT: cout<<"IDENTIFIER";
			  break;

		case STR: cout<<"STRING";
			  break;

		case INT: cout<<"INTEGER";
			  break;

		case TAU: cout<<"TAU";
			  break;

		case LAMBDA: cout<<"LAMBDA";
			  break;

		case OR: cout<<"OR";
			  break;

		case NOT: cout<<"NOT";
			  break;

		case GR: cout<<"GR";
			  break;

		case GE: cout<<"GE";
			  break;

		case LS: cout<<"LS";
			  break;

		case LE: cout<<"LE";
			  break;

		case EQ: cout<<"EQ"; //this should be binary operator not EQUAL
			  break;

		case NE: cout<<"NE";
			  break;

		case PLUS: cout<<"PLUS";
			  break;

		case MINUS: cout<<"MINUS";
			  break;

		case NEG: cout<<"NEG";
			  break;

		case AMP: cout<<"AMP";
			  break;

		case AUG: cout<<"AUG";
			  break;

		case MULTI: cout<<"MULTI";
			  break;

		case DIV: cout<<"DIV";
			  break;

		case EXPO: cout<<"EXPO";
			  break;

		case GAMMA: cout<<"GAMMA";
			  break;

		case TRUE: cout<<"TRUE";
			  break;

		case FALSE: cout<<"FALSE";
			  break;

		case NIL: cout<<"NIL";
			  break;

		case DUMMY: cout<<"DUMMY";
			  break;

		case YSTAR: cout<<"YSTAR";
			  break;

		case DELTA_THEN: cout<<"DELTA_THEN";
			  break;

		case DELTA_ELSE: cout<<"DELTA_ELSE";
			  break;

		case BETA: cout<<"BETA";
			  break;

		case ENV: cout<<"ENVIRONMENT";
			  break;
		default:
			   cerr<<"Some other type of node !! - "<<in_node->get_node_type()<<endl;
			   exit(0);
	}
	cout<<":"<<in_node->get_node_value()<<">"<<endl;
}

void CSE_machine::execute(){

	//while control stack is not empty
	while( control_stack.empty() != true ){
		//pop 1 element from control stack

		control_st *Popped_Control_Structure = control_stack.top();
		control_stack.pop();

		//first rule is for int...just pass it to execution stack
		if( Popped_Control_Structure->node->get_node_type() == INT){ //if node is INTEGER

			//create new execution_element
			execution_element *new_exe_ele = new execution_element();
			//set type to INT
			new_exe_ele->type = EXE_INT; //set type

			int temp_int = atoi( (Popped_Control_Structure->node->get_node_value() ).c_str() ); //convert string value to int...
			//atoi converts it to string

			new_exe_ele->int_var =  temp_int;

			//push the new execution_element on exeuction stack
			execution_stack.push(new_exe_ele);

		}
		//if nil just pass it on to execution stack
		else if(Popped_Control_Structure->node->get_node_type() == NIL){

			execution_element *new_nil = new execution_element;
			new_nil->type = EXE_NILL;
			execution_stack.push(new_nil);

		}
		//if dummy push it on the stack
		else if(Popped_Control_Structure->node->get_node_type() == DUMMY){

			execution_element *new_dummy= new execution_element;
			new_dummy->type = EXE_DUMMY;
			execution_stack.push(new_dummy);

		}

		//if popped element is TRUE push it to exec stack
		else if(Popped_Control_Structure->node->get_node_type() == TRUE){

			execution_element *new_true= new execution_element;
			new_true->type = EXE_TRUE;
			execution_stack.push(new_true);

		}
		//if poppped element is FALSE push it to exec stack
		else if(Popped_Control_Structure->node->get_node_type() == FALSE){
			//create new exec element and assign type EXE_FALSE
			execution_element *new_false= new execution_element;
			new_false->type = EXE_FALSE;
			execution_stack.push(new_false);

		}

		else if( Popped_Control_Structure->node->get_node_type() == STR){ //if node is STRING

			//create new execution_element and assign type EXE_STR
			execution_element *new_exe_ele = new execution_element();
			new_exe_ele->type = EXE_STR; //set type

			new_exe_ele->string_var = Popped_Control_Structure->node->get_node_value();
			//push the new execution_element on exeuction stack
			execution_stack.push(new_exe_ele);

		}

		//if node is IDENTIFIER...we have to lookup its value in the bound variables of current environment
		else if( Popped_Control_Structure->node->get_node_type() == IDT) {

			//lookup in current environment
			execution_element *idt_exe_ele = current_env->lookup(Popped_Control_Structure->node->get_node_value());

			//if lookup is unsuccessfull it might be primitive function. Primitive functions are implemented later below when a gamma comes in control stack
			if( idt_exe_ele == NULL){
				//check if its primitive function
				if((Popped_Control_Structure->node->get_node_value()).compare("Print") == 0){

					//create new execution element and assign type PRINT
					execution_element *new_exe_ele = new execution_element;
					new_exe_ele->type = EXE_PRIMITIVE_FUNC;
					new_exe_ele->inBuiltFunctions =  PRINT;
					execution_stack.push(new_exe_ele); //push

				}
				else if((Popped_Control_Structure->node->get_node_value()).compare("ItoS") == 0){

					//create new execution element and assign type ItoS
					execution_element *new_exe_ele = new execution_element;
					new_exe_ele->type = EXE_PRIMITIVE_FUNC;
					new_exe_ele->inBuiltFunctions =  ITOS;
					execution_stack.push(new_exe_ele); //push

				}
				else if((Popped_Control_Structure->node->get_node_value()).compare("Istruthvalue") == 0){

					//create new execution element and assign appropriate type
					execution_element *new_exe_ele = new execution_element;
					new_exe_ele->type = EXE_PRIMITIVE_FUNC;
					new_exe_ele->inBuiltFunctions =  ISTRUTHVALUE;
					execution_stack.push(new_exe_ele); //push

				}

				else if((Popped_Control_Structure->node->get_node_value()).compare("Isstring") == 0){

					//create new execution element and assign appropriate type
					execution_element *new_exe_ele = new execution_element;
					new_exe_ele->type = EXE_PRIMITIVE_FUNC;
					new_exe_ele->inBuiltFunctions =  ISSTRING;
					execution_stack.push(new_exe_ele); //push

				}


				else if((Popped_Control_Structure->node->get_node_value()).compare("Isinteger") == 0){

					//create new execution element and assign appropriate type
					execution_element *new_exe_ele = new execution_element;
					new_exe_ele->type = EXE_PRIMITIVE_FUNC;
					new_exe_ele->inBuiltFunctions =  ISINTEGER;
					execution_stack.push(new_exe_ele); //push

				}

				else if((Popped_Control_Structure->node->get_node_value()).compare("Istuple") == 0){

					//create new execution element and assign appropriate type
					execution_element *new_exe_ele = new execution_element;
					new_exe_ele->type = EXE_PRIMITIVE_FUNC;
					new_exe_ele->inBuiltFunctions =  ISTUPLE;
					execution_stack.push(new_exe_ele); //push

				}

				else if((Popped_Control_Structure->node->get_node_value()).compare("Order") == 0){

					//create new execution element and assign Order
					execution_element *new_exe_ele = new execution_element;
					new_exe_ele->type = EXE_PRIMITIVE_FUNC;
					new_exe_ele->inBuiltFunctions =  ORDER;
					execution_stack.push(new_exe_ele); //push

				}
	//Aug is handled separately

				//all string functions primitive ones are handled below- Stern Stem Conc
				else if( ((Popped_Control_Structure->node->get_node_value()).compare("Stern") == 0 )
						|| ((Popped_Control_Structure->node->get_node_value()).compare("Stem") == 0)
					||((Popped_Control_Structure->node->get_node_value()).compare("Conc") == 0) )
				{

					//create new execution element and push appropriate element
					execution_element *new_exe_ele = new execution_element;
					new_exe_ele->type = EXE_PRIMITIVE_FUNC;
					if((Popped_Control_Structure->node->get_node_value()).compare("Stern") == 0 )
						new_exe_ele->inBuiltFunctions =  STERN;
					else if((Popped_Control_Structure->node->get_node_value()).compare("Stem") == 0 )
						new_exe_ele->inBuiltFunctions = STEM;
					else{
						new_exe_ele->inBuiltFunctions = CONC;
						new_exe_ele->ConcOnce = false;
						
						//this boolean will be used to implement Conc S T...Conc is first applied to S and then result is applied to T.
					}
					execution_stack.push(new_exe_ele); //push

				}


				else{
					
					cout<<"\nIDENTIFIER : <"<<(Popped_Control_Structure->node->get_node_value())<<"> is not declared !"<<endl;
					exit(0);
				}
			}

			else{ 
				execution_stack.push(idt_exe_ele);
			}

		}
		//major portion starts here. Handle lambda node...assign the current environment with lambda
		else if (Popped_Control_Structure->node->get_node_type() == LAMBDA){ //if node is LAMBDA
			//current environment
			Popped_Control_Structure->env = current_env;

			execution_element *new_exe_ele = new execution_element();
			new_exe_ele->type = EXE_CTRL_STRUCT; //set type
			new_exe_ele->ctrl_st_var = Popped_Control_Structure;//this has information of the current environment in which lambda was first pushed to exec stack
			//push it on execution stack
			execution_stack.push(new_exe_ele);
		}

		/* TODO - Remember to update the current environment */
		else if(Popped_Control_Structure->node->get_node_type() == ENV) { //if it is ENVIRONMENT

			//pop from execution stack and save it
			execution_element *saved_exe_ele = execution_stack.top(); //This could be IDT, INT or anything before the env var on exec stack
			if(saved_exe_ele != NULL){
				execution_stack.pop(); //pop another element
				execution_element *another_env_ele = execution_stack.top();

				//it should be same environment as control stack's one
				if( !( (another_env_ele->type == EXE_ENV) && ( (another_env_ele->env_var) ==  (Popped_Control_Structure->env) ) )){ //confirm if it is environment and same as the popped one
					cout<<"ERROR - was expecting an ENVIRONMENT element on execution stack !"<<endl;
					exit(0);
				}

				execution_stack.pop(); //ENV was matched so pop it
				//push back the saved element before the env variable on exec stack
				execution_stack.push(saved_exe_ele);

			}
			else{
				//THere has to be something in exec stack on top of env variable or Else Error
				cout<<"ERROR - Was expecting some element on execution stack but got NULL !"<<endl;
				exit(0);
			}

			//We need to update the current environment
			stack<execution_element*> temporaryStack;
			
			while( execution_stack.size() != 0 && ((execution_stack.top())->type != EXE_ENV) ){
				temporaryStack.push( execution_stack.top() );
				execution_stack.pop();
			}

			if( execution_stack.size() != 0){

					current_env = (execution_stack.top())->env_var; //we have current environment
			}

			//put back all the popped elements in the exec stack
			while( temporaryStack.size() != 0){
				execution_stack.push( temporaryStack.top() );
				temporaryStack.pop();
			}

		}

		else if(Popped_Control_Structure->node->get_node_type() == TAU){ //if node is TAU

			//create an execution element of type TAU
			execution_element *new_exec_ele = new execution_element; //it will have queue
			new_exec_ele->type = EXE_TUPPLE;

			//pop n elements from execution stack and make a tuple of them
			int n = Popped_Control_Structure->getNoofChildren();

			for(int i=0; i<n; i++){
				execution_element *temp_ele = execution_stack.top();
				if(temp_ele == NULL) {
					cout<<"\nERROR: Processing TAU node. Expected number of children: "<<n<<" .But Execution Stack has less"<<endl;
					exit(0);
				}

				(new_exec_ele->queue_var).push(temp_ele); //LIFO
				execution_stack.pop();

			}

			//push back the queue into execution stack
			execution_stack.push(new_exec_ele);
		}

		//Handle <YSTAR> node..similar handling to lambda
		else if(Popped_Control_Structure->node->get_node_type() == YSTAR){

			//create a new execution element of type EXE_YSTAR
			execution_element *new_y_star = new execution_element;
			new_y_star->type = EXE_YSTAR;
			execution_stack.push(new_y_star);

		}
		//handle gamma node...function application
		else if(Popped_Control_Structure->node->get_node_type() == GAMMA){ //if node is GAMMA

			//check the element on execution stack
			//first case if there is lambda node on exec stack...most likely
			if( (execution_stack.top())->type == EXE_CTRL_STRUCT ) {

				//pop the element from execution stack
				execution_element *temp_exe_ele = execution_stack.top(); //temp_exe_ele contains control structure
				execution_stack.pop();

				//create new environment
				environment *new_env = new environment();

				//link the new environment to environment in control struct
				new_env->parent = temp_exe_ele->ctrl_st_var->env;

				//bound_var shouldn't be NULL
				if(temp_exe_ele->ctrl_st_var->bound_var == NULL) {
					cout<<"Bound variable of this popped control structure is NULL!"<<endl;
					exit(0);
				}
				//bound the new variable - could have two cases...single bound var or tuple
				//assumption there is single bound var initially...could be a single IDT/variable or a tuple
				execution_element *bind_exe_ele = execution_stack.top();
				execution_stack.pop();
				
				if(bind_exe_ele->type == EXE_TUPPLE){

					//so we have Linked list in temp_exe_ele->ctrl_st_var->bound_var AND TUPPLE in bind_exe_ele
					AST_node *temp_bound_var = temp_exe_ele->ctrl_st_var->bound_var;

					//if bound variable is not linked list bound whole tupple to the variable
					if( temp_bound_var->right_sibling == NULL){

						new_env->insert(temp_bound_var->get_node_value(), bind_exe_ele);
					}

					else{
						//could be a potential bug to expect x no of vars after lambda if lambda is bound to x vars is encountered
						queue<execution_element*> temp_for_tuple;
						while(temp_bound_var != NULL){
							//get the AST_nodes value to use as key
							string temp_key = temp_bound_var->get_node_value();

							execution_element *popped_from_queue = (bind_exe_ele->queue_var).front();
							if(popped_from_queue == NULL){
								cout<<"ERROR - Binding variables - Tupple on execution has less number of elements ! (Yeah I know its not the world's best user friendly error message)"<<endl;
								exit(0);
							}
							temp_for_tuple.push(popped_from_queue); //preserve the tuple
							(bind_exe_ele->queue_var).pop(); //pop the element;
							//insert key and value
							new_env->insert(temp_key, popped_from_queue);
							temp_bound_var = temp_bound_var->right_sibling;

						}

						while( temp_for_tuple.size() != 0){
							(bind_exe_ele->queue_var).push( temp_for_tuple.front() );
							temp_for_tuple.pop();
						}
					}
				}

				else{
					//add this element with key from temp_exe_ele's bound variable to the new environment
					new_env->insert(temp_exe_ele->ctrl_st_var->bound_var->get_node_value(), bind_exe_ele);
				}

				//change the current environment to new environment
				current_env = new_env;
				execution_element *new_exe_ele = new execution_element();
				new_exe_ele->type = EXE_ENV;
				new_exe_ele->env_var = new_env;

				execution_stack.push(new_exe_ele);

				//create new control struct and push it in control stack
				AST_node *new_ast_node = new AST_node;
				new_ast_node->set_node_type(ENV);

				control_st *new_control_st = new control_st;
				new_control_st->node = new_ast_node; //this node will tell the type of control struct in execution
				new_control_st->env = current_env; //this stores the environment
				control_stack.push(new_control_st);

				//load control stack with popped control stack's delta...it contains body of the function
				if(temp_exe_ele->ctrl_st_var->delta == NULL) {
					cout<<"this lambda doesn't have delta !! "<<endl;
					exit(0);
				}
				//load the body of the function on the control stack
				load(temp_exe_ele->ctrl_st_var->delta);

			}
			//Handle <Ystar> node
			else if( (execution_stack.top())->type == EXE_YSTAR ){

				execution_stack.pop();
				//pop <CS> node and change its type from <CS> to <ITA>
				execution_element *temp_cs = execution_stack.top(); // shouldn't be NULL
				if( temp_cs == NULL || temp_cs->type != EXE_CTRL_STRUCT ){
					//there has to be a lambda on execution stack
					cout<<"ERROR -Processing <YSTAR> node. Was expecting <LAMBDA> on execution stack but it is either NULL or some other type !"<<endl;
					exit(0);
				}
				//ctrl_st_var in execution_element will store both
				temp_cs->type = EXE_ITA; //change its type

			}

			else if((execution_stack.top())->type == EXE_ITA){
				i++;

				//create new execution element of type EXE_CTRL_STRUCT
				execution_element *new_ctrl_struct = new execution_element;
				new_ctrl_struct->type = EXE_CTRL_STRUCT;

				//BUG - there couldn't be any top element on stack
				new_ctrl_struct->ctrl_st_var = (execution_stack.top())->ctrl_st_var;
				//push new lambda on the stack

				execution_stack.push(new_ctrl_struct);

				//create new GAMMA ast node and new control stru and push it in CS
				AST_node *new_gamma_push = new AST_node;

				new_gamma_push->set_node_type(GAMMA);

				control_st *another_gamma_cs = new control_st;

				another_gamma_cs->node =new_gamma_push;

				control_stack.push(another_gamma_cs);

				//push two gamma CS in CS Stack

				AST_node *new_gamma_to_push = new AST_node;
				new_gamma_to_push->set_node_type(GAMMA);

				control_st *new_gamma_cs = new control_st;
				new_gamma_cs->node = new_gamma_to_push;

				control_stack.push(new_gamma_cs);
				//new gamma to push
			}
			//handle all primitive function.
			else if( (execution_stack.top())->type == EXE_PRIMITIVE_FUNC) {

				//pop it
				execution_element *popped_primitive_func = execution_stack.top();
				execution_stack.pop();

				if(popped_primitive_func->inBuiltFunctions == PRINT){

					//pop the node and print it
					execution_element *another_ele = execution_stack.top(); //shouldn't be null
					if(another_ele == NULL){
						cout<<"ERROR - Was expecting an element to Print but found NULL please check PRINT again"<<endl;
						exit(0);
					}
					//pop the element
					execution_stack.pop();
					print_execution_element(another_ele); //print it

					//store the DUMMY in execution stack
					execution_element *new_exec_ele = new execution_element;
					new_exec_ele->type = EXE_DUMMY;
					execution_stack.push(new_exec_ele);

				}

				else if(popped_primitive_func->inBuiltFunctions == ITOS){

					//pop the node
					execution_element *another_ele = execution_stack.top(); //shouldn't be null
					if(another_ele == NULL){
						cout<<"ERROR - Was expecting an element to Print but found NULL please check PRINT again"<<endl;
						exit(0);
					}
					//pop the element
					execution_stack.pop();
					if( another_ele->type != EXE_INT){
						cout<<"ERROR - ITOS operands invalied"<<endl;
						exit(0);
					}
					stringstream ss;
					ss << (another_ele->int_var);
					string temp_string = ss.str();

					execution_element *itos_ele = new execution_element;
					itos_ele->type = EXE_STR;
					itos_ele->string_var = temp_string;
					execution_stack.push(itos_ele);

				}

					else if(popped_primitive_func->inBuiltFunctions == ISINTEGER){

					bool isinteger;
					execution_element *check_integer= execution_stack.top();
					if( check_integer== NULL){
						cout<<"ERROR - Please check IsInteger again.."<<endl;
						exit(0);
					}

					execution_stack.pop();
					execution_element *new_check_int= new execution_element;
					if(check_integer->type == EXE_INT)
						new_check_int->type = EXE_TRUE;
					else
						new_check_int->type = EXE_FALSE;

					execution_stack.push(new_check_int);
					//push true or false on the execution stack
				}

				else if(popped_primitive_func->inBuiltFunctions == ISTRUTHVALUE){

					bool istuple ;
					execution_element *check_tuple = execution_stack.top();
					if( check_tuple == NULL){
						cout<<"ERROR - Please check Istruthvalue again.."<<endl;
						exit(0);
					}

					execution_stack.pop();
					execution_element *new_check_tuple = new execution_element;
					if(check_tuple->type == EXE_FALSE || check_tuple->type == EXE_TRUE)
						new_check_tuple->type = EXE_TRUE;
					else
						new_check_tuple->type = EXE_FALSE;

					execution_stack.push(new_check_tuple);

				}

				else if(popped_primitive_func->inBuiltFunctions == ISSTRING){

					bool istuple ;
					execution_element *check_tuple = execution_stack.top();
					if( check_tuple == NULL){
						cout<<"ERROR - Please check Isstring again.."<<endl;
						exit(0);
					}

					execution_stack.pop();
					execution_element *new_check_tuple = new execution_element;
					if(check_tuple->type == EXE_STR)
						new_check_tuple->type = EXE_TRUE;
					else
						new_check_tuple->type = EXE_FALSE;

					execution_stack.push(new_check_tuple);

				}


				else if(popped_primitive_func->inBuiltFunctions == ISTUPLE){

					bool istuple ;
					execution_element *check_tuple = execution_stack.top();
					if( check_tuple == NULL){
						cout<<"ERROR - Please check Istuple again.."<<endl;
						exit(0);
					}

					execution_stack.pop();
					execution_element *new_check_tuple = new execution_element;
					//type can be tuple or nill
					if(check_tuple->type == EXE_TUPPLE || check_tuple->type == EXE_NILL)
						new_check_tuple->type = EXE_TRUE;
					else
						new_check_tuple->type = EXE_FALSE;

					execution_stack.push(new_check_tuple);

				}

				//Handle ORDER
				else if(popped_primitive_func->inBuiltFunctions == ORDER){

					//pop an element
					execution_element *temp_tupple = execution_stack.top();
					//it should be <TUPPLE>
					if( temp_tupple == NULL || (temp_tupple->type != EXE_TUPPLE && temp_tupple->type != EXE_NILL) ){
						cout<<"ERROR - Was expecting a tupple next to ORDER. Please check ORDER again"<<endl;
						exit(0);
					}
					execution_stack.pop();

					//count number of elements in queue.
					int num_elements;
					if(temp_tupple->type == EXE_NILL)
						num_elements = 0;
					else
						num_elements = (temp_tupple->queue_var).size();

					//change the type and value of the execution element
					execution_element *new_int_element = new execution_element;
					new_int_element->type = EXE_INT;
					new_int_element->int_var = num_elements;
					execution_stack.push(new_int_element);

				}

				else if(popped_primitive_func->inBuiltFunctions == STEM){

					execution_element *stem_element = execution_stack.top();
					if( stem_element ==  NULL || stem_element->type != EXE_STR){
						cout<<"ERROR - Stem's operand is invalid.."<<endl;
						exit(0);
					}
					execution_stack.pop();
					string stem_string = stem_element->string_var;
					stem_string = stem_string.substr(0, 1);

					//stem_element->string_var = stem_string;
					execution_element *new_stem_ele = new execution_element;
					new_stem_ele->type = EXE_STR;
					new_stem_ele->string_var = stem_string;
					execution_stack.push(new_stem_ele);

				}

				//CONC needs two gamma on the control stack
				else if (popped_primitive_func->inBuiltFunctions == CONC){
					//pop two strings
					execution_element *first_string = execution_stack.top();
					if(first_string == NULL || first_string->type != EXE_STR){
						cout<<"ERROR - CONC's first operand is invalid"<<endl;
						exit(0);
					}
					execution_stack.pop();

					//if conc_applied is false means conc hasn't been applied yet so just store the string
					if(popped_primitive_func->ConcOnce == false){
						popped_primitive_func->string_var = first_string->string_var;
						popped_primitive_func->ConcOnce = true;
						execution_stack.push(popped_primitive_func);
					}

					else{ //conc had been applied. just concetenat the strings (funny spelling)

						execution_element *conc_string = new execution_element;
						conc_string->type = EXE_STR;
						conc_string->string_var = popped_primitive_func->string_var + first_string->string_var ;
						execution_stack.push(conc_string);

					}

				}
				else if(popped_primitive_func->inBuiltFunctions == STERN){

					//pop the node
					execution_element *stern_element= execution_stack.top(); //shouldn't be null
					if(stern_element== NULL || stern_element->type != EXE_STR){ //should be string
						cout<<"ERROR - Was expecting an element to Stern but found NULL or not string please check STERN again"<<endl;
						exit(0);
					}
					//pop the element
					execution_stack.pop();

					//apply stern
					string stern_string = stern_element->string_var;

					string resultant_string = stern_string.substr(1, stern_string.size() ); //sterning

					execution_element *new_stern_exe = new execution_element;
					new_stern_exe->type = EXE_STR;
					new_stern_exe->string_var = resultant_string;
					//push result string on execution stack
					execution_stack.push(new_stern_exe);
				}
			}

			else if((execution_stack.top())->type == EXE_TUPPLE){ //if there is TUPPLE on Execution Stack
				//pop the tupple
				execution_element *TuppleExecElement = execution_stack.top(); //this contains queue
				execution_stack.pop();

				//pop another element. It should be INTEGER
				execution_element *second_exe_ele = execution_stack.top();
				//could be potential bug if only Integer is expected next
				if(second_exe_ele == NULL || second_exe_ele->type != EXE_INT){ //should be INTEGER only
					cout<<"ERROR - Processing <GAMMA>, <TUPPLE> on Execution Stack - Was expecting an <INTEGER> but either there is nothing or it isn't <INTEGER>"<<endl;
					exit(0);
				}

				execution_stack.pop();

				queue<execution_element*> Temp_Queue; //temp queue
				for(int i=1; i< (second_exe_ele->int_var); i++){
					if((TuppleExecElement->queue_var).size() == 0){
						cout<<"ERROR - Processing <GAMMA>, <TUPPLE> on Execution Stack - There are less number of elements in TUPPLE than the <INTEGER>"<<endl;
						exit(0);
					}

					Temp_Queue.push( (TuppleExecElement->queue_var).front() ); //pop n-1 elements
					(TuppleExecElement->queue_var).pop();
				}

				execution_element *resultant_ele = new execution_element;
				execution_element *temp_ex_el = (TuppleExecElement->queue_var).front(); //could be anything
				while( (TuppleExecElement->queue_var).size() != 0){
					Temp_Queue.push( (TuppleExecElement->queue_var).front() );
					(TuppleExecElement->queue_var).pop();
				}

				resultant_ele->type = temp_ex_el->type; //same type
				resultant_ele->int_var = temp_ex_el->int_var;
				resultant_ele->string_var= temp_ex_el->string_var;
				resultant_ele->queue_var= temp_ex_el->queue_var;
				resultant_ele->ctrl_st_var = temp_ex_el->ctrl_st_var;
				resultant_ele->env_var = temp_ex_el->env_var;
				resultant_ele->inBuiltFunctions= temp_ex_el->inBuiltFunctions;

				if(resultant_ele == NULL){
					cout<<"ERROR - Processing <GAMMA>, <TUPPLE> on Execution Stack..Processing last element- There are less number of elements in TUPPLE than the <INTEGER>"<<endl;
					exit(0);
				}
				
				while( Temp_Queue.size() != 0){
					(TuppleExecElement->queue_var).push( Temp_Queue.front() );
					Temp_Queue.pop();
				}
				//push the resultant element in execution stack
				execution_stack.push(resultant_ele);

			}
			else{
				cout<<"WHILE PROCESSING GAMMA - SOME OTHER TYPE !! "<<endl;
				print_execution_element( execution_stack.top() );
				exit(0);
			}

		}


		else if( (Popped_Control_Structure->node->get_node_type() == BETA) ) { //BETA Node

			execution_element *beta_exe_element = execution_stack.top();
			if(beta_exe_element == NULL){
				cout<<"ERROR - While processing BETA node - Execution Stack is empty ! "<<endl;
				exit(0);
			}

			execution_stack.pop();
			if( !(beta_exe_element->type == EXE_TRUE || beta_exe_element->type == EXE_FALSE) ){
				cout<<"ERROR - While processing BETA node - Expected truth value of exectuion stack but got something else! "<<endl;
				exit(0);
			}

			control_st *load_delta = NULL;
			//we have truth value in beta_exe_element
			if(beta_exe_element->type == EXE_TRUE){

				//pop an element from CONTROL STACK and discard and load next element
				control_st *temp_delta = control_stack.top(); //it should be delta else
				if( temp_delta == NULL || temp_delta->node->get_node_type() != DELTA_ELSE ){
					cout<<"ERROR - Processing <BETA> was expecting DELTA_ELSE but it some other type (may be NULL)"<<endl;
					exit(0);
				}
				control_stack.pop();
				temp_delta = control_stack.top(); //access delta_then
				if(temp_delta == NULL || temp_delta->node->get_node_type() != DELTA_THEN){
					cout<<"ERROR - Processing <BETA> was expecting DELTA_THEN but it some other type (may be NULL)"<<endl;
					exit(0);
				}
				control_stack.pop();
				load_delta = temp_delta;
			}

			else{

				control_st *temp_delta = control_stack.top();
				if( temp_delta == NULL || temp_delta->node->get_node_type() != DELTA_ELSE ){
					cout<<"ERROR - Processing <BETA> was expecting DELTA_ELSE but it some other type (may be NULL)"<<endl;
					exit(0);
				}
				control_stack.pop();

				control_st *temp_delta_discard = control_stack.top(); //access delta_then
				if(temp_delta_discard == NULL || temp_delta_discard->node->get_node_type() != DELTA_THEN){
					cout<<"ERROR - Processing <BETA> was expecting DELTA_THENbut it some other type (may be NULL)"<<endl;
					exit(0);
				}
				control_stack.pop();

				load_delta = temp_delta;
			}

			//load control struct
			load(load_delta->delta);

		}
		else if(Popped_Control_Structure->node->get_node_type() == EQ
				|| Popped_Control_Structure->node->get_node_type() == NE){

			//pop two element
			execution_element *temp_first = execution_stack.top(); //shouldn't be NULL
			if(temp_first == NULL){

				exit(0);
			}
			execution_stack.pop(); //actually pop the element

			execution_element *temp_second = execution_stack.top();
			if(temp_second == NULL){
				exit(0);
			}
			execution_stack.pop(); //actually pop the element

			bool boolResult;
			//both should be of either STRING, INTEGER or BOOL
			if( temp_first->type == EXE_STR && temp_second->type == EXE_STR){
				if(Popped_Control_Structure->node->get_node_type() == EQ)
					boolResult = !((temp_first->string_var).compare(temp_second->string_var));
				else
					boolResult = ((temp_first->string_var).compare(temp_second->string_var));
			}

			else if(temp_first->type == EXE_INT && temp_second->type == EXE_INT){
			//both are INTEGERS

				if(Popped_Control_Structure->node->get_node_type() == EQ)
					boolResult = (temp_first->int_var == temp_second->int_var) ;
				else
					boolResult = (temp_first->int_var != temp_second->int_var);
			}

			else if( (temp_first->type == EXE_TRUE ||temp_first->type== EXE_FALSE) && (temp_second->type == EXE_FALSE || temp_second->type == EXE_TRUE) ){

				if(Popped_Control_Structure->node->get_node_type() == EQ){

					if( temp_first->type == temp_second->type)
						boolResult = true;
					else
						boolResult = false;
				}
				else{
					if( temp_first->type != temp_second->type)
						boolResult = true;
					else
						boolResult = false;
				}
			}

			else{
				exit(0);
			}

			execution_element *newEqExecElement = new execution_element;
			if(boolResult)
				newEqExecElement->type = EXE_TRUE;
			else
				newEqExecElement->type = EXE_FALSE;

			execution_stack.push(newEqExecElement);

		}

		else if(Popped_Control_Structure->node->get_node_type() == AUG){

			execution_element *firstTupple = execution_stack.top();
			if(firstTupple == NULL){
				exit(0);
			}

			execution_stack.pop();

			execution_element *secondTupple= execution_stack.top();
			if(secondTupple == NULL){
				exit(0);
			}

			execution_stack.pop();

			//first element has to be either TUPPLE or NIL
			if( !(firstTupple->type == EXE_TUPPLE || firstTupple->type == EXE_NILL) ){

				exit(0);
			}

			if(!(secondTupple->type == EXE_TUPPLE || secondTupple->type == EXE_NILL || secondTupple->type == EXE_INT || secondTupple->type == EXE_STR || secondTupple->type == EXE_TRUE || secondTupple->type == EXE_FALSE || secondTupple->type == EXE_DUMMY)) {

				exit(0);
			}

			execution_element *newAugmentedTupple = new execution_element;
			newAugmentedTupple->type = EXE_TUPPLE;

			if( (firstTupple->type != EXE_NILL)){
				while( (firstTupple->queue_var).size() != 0 ){
					(newAugmentedTupple->queue_var).push( (firstTupple->queue_var).front() );

					(firstTupple->queue_var).pop(); //BUG changing tupple
				}

			}
			if(secondTupple->type!=EXE_NILL)
				(newAugmentedTupple->queue_var).push( secondTupple);
			
			execution_stack.push(newAugmentedTupple); //push back the first tupple
		}

		else if((Popped_Control_Structure->node->get_node_type() == AMP)
				||(Popped_Control_Structure->node->get_node_type() == OR)){

			//pop two elements from execution stack
			execution_element *first_op = execution_stack.top();
			if( first_op == NULL){
				exit(0);
			}

			execution_stack.pop();

			execution_element *second_op = execution_stack.top();
			if( second_op == NULL){
				exit(0);
			}

			execution_stack.pop();

			if( !(first_op->type == EXE_TRUE || first_op->type == EXE_FALSE) || !(second_op->type == EXE_TRUE || second_op->type == EXE_FALSE)){
				exit(0);
			}
			execution_element *new_amp =  new execution_element;

			if (Popped_Control_Structure->node->get_node_type() == OR){

				if( first_op->type == EXE_FALSE && second_op->type == EXE_FALSE)
					new_amp->type = EXE_FALSE;
				else
					new_amp->type = EXE_TRUE;

			}
			else if(Popped_Control_Structure->node->get_node_type() == AMP){
				if( first_op->type == EXE_TRUE && second_op->type == EXE_TRUE)
					new_amp->type = EXE_TRUE;
				else
					new_amp->type = EXE_FALSE;
			}
			execution_stack.push(new_amp);
		}

		//NEG operator
		else if( Popped_Control_Structure->node->get_node_type() == NEG ){


			execution_element *NegElement = execution_stack.top();
			if(NegElement == NULL){
				exit(0);
			}
			execution_stack.pop();
			if( NegElement->type != EXE_INT){

				exit(0);
			}

			execution_element *neg_ele = new execution_element;
			neg_ele->type = EXE_INT;
			neg_ele->int_var = -(NegElement->int_var);
			execution_stack.push(neg_ele);

		}


		else if( Popped_Control_Structure->node->get_node_type() == NOT){

			execution_element *NotElement = execution_stack.top();
			if(NotElement == NULL){
				cout<<"<NOT>'s operand is NULL !"<<endl;
				exit(0);
			}
			execution_stack.pop();

			execution_element *newNotElement = new execution_element;

			if( NotElement->type == EXE_TRUE)
				newNotElement->type = EXE_FALSE;
			else if( NotElement->type == EXE_FALSE)
				newNotElement->type = EXE_TRUE;
			else{
				exit(0);
			}

			execution_stack.push(newNotElement);

		}

		//PLUS, DIV operator
		else if( (Popped_Control_Structure->node->get_node_type() == DIV) ||
				Popped_Control_Structure->node->get_node_type() == PLUS ||
				(Popped_Control_Structure->node->get_node_type() == MINUS) ||
				(Popped_Control_Structure->node->get_node_type() == GR) ||
				(Popped_Control_Structure->node->get_node_type() == LE) ||
				(Popped_Control_Structure->node->get_node_type() == MULTI)
				|| (Popped_Control_Structure->node->get_node_type() == LS) )  {

			execution_element *TempPop1 = execution_stack.top();
			
			//this should not be NULL and should be integer
			if( TempPop1 == NULL || TempPop1->type != EXE_INT ){
				exit(0);
			}
			execution_stack.pop();

			execution_element *TempPop2 = execution_stack.top();

			if( TempPop2==NULL || TempPop2->type != EXE_INT ){
				exit(0);
			}
			execution_stack.pop();

			int temp_result;
			if(Popped_Control_Structure->node->get_node_type() == DIV)
				temp_result = (TempPop1->int_var)/(TempPop2->int_var);

			else if (Popped_Control_Structure->node->get_node_type() == PLUS)
				temp_result = (TempPop1->int_var)+(TempPop2->int_var);

			else if (Popped_Control_Structure->node->get_node_type() == MINUS)
				temp_result = (TempPop1->int_var)-(TempPop2->int_var);

			else if (Popped_Control_Structure->node->get_node_type() == MULTI)
				temp_result = (TempPop1->int_var)*(TempPop2->int_var);

			else if (Popped_Control_Structure->node->get_node_type() == GR)
				temp_result = (TempPop1->int_var)>(TempPop2->int_var);

			else if (Popped_Control_Structure->node->get_node_type() == LE)
				temp_result = (TempPop1->int_var) <= (TempPop2->int_var);

			else if (Popped_Control_Structure->node->get_node_type() == LS)
				temp_result = (TempPop1->int_var) < (TempPop2->int_var);

			execution_element *new_exe_ele = new execution_element;

			if (Popped_Control_Structure->node->get_node_type() == GR || (Popped_Control_Structure->node->get_node_type() == LE)
					||(Popped_Control_Structure->node->get_node_type() == LS)){
				if (temp_result)
					new_exe_ele->type = EXE_TRUE;
				else
					new_exe_ele->type = EXE_FALSE;
			}
			else{
				new_exe_ele->type = EXE_INT;
				new_exe_ele->int_var = temp_result;
			}

			execution_stack.push(new_exe_ele);

		}

		else{

			print_ast_node(Popped_Control_Structure->node);

		exit(0);
		}

	}

}

void CSE_machine::print_execution_element(execution_element *in_element){
	if(in_element == NULL){
		exit(0);
	}

	switch(in_element->type){
		case EXE_INT: cout<<in_element->int_var;
			  	break;

		case EXE_STR: 
			      for(int i=0; i< (in_element->string_var).size(); i++){
				      if( (in_element->string_var)[i] == '\\') {
					      i++;
					      if( (in_element->string_var)[i] == 'n')
						      cout<<endl;
					      else
						      cout<<"\t";
				      }

				      else
					      cout<<(in_element->string_var)[i];
			      }

			      break;
		case EXE_ENV: cout<<"<ENVIRONMENT>"<<endl;
			      break;

		case EXE_TUPPLE: 
				 print_tupple(in_element);
			      break;
		case EXE_CTRL_STRUCT: cout<<"[lambda closure: ";
				      cout<<in_element->ctrl_st_var->bound_var->get_node_value()<<": 2]"; //possible bug in the code
			      break;
		case EXE_TRUE: cout<<"true";
			       break;

		case EXE_FALSE: cout<<"false";
			       break;

		case EXE_DUMMY: cout<<"dummy";
			       break;
		case EXE_YSTAR: cout<<"<YSTAR>"<<endl;
			       break;
		case EXE_NILL: cout<<"(3)"; //bug in the code...doesn't work for tiny
			       break;
		case EXE_ITA: cout<<"<ITA>"<<endl;
				cout<<"[lambda closure: ";
				      cout<<in_element->ctrl_st_var->bound_var->get_node_value()<<" ]"<<endl;
			      break;

		default:
			      cout<<"print_execution_element - Some other type !! "<<endl;

	}


}

void environment::insert(string key, execution_element *element){ //insert into hash table

	bound_variable[key] = element; //TO-DO check if key already exists
}


void CSE_machine::load(control_st *delta){
	while(delta != NULL){
		control_stack.push(delta);
		delta = delta->next;
	}

}

execution_element* environment::lookup(string key){ //return the execution_element corresponding to key

	//TODO - check in parent environment
	if( bound_variable[key] != NULL)
		return bound_variable[key];
	else if( parent != NULL) //look into parents
		return parent->lookup(key);
	else
		return NULL;
}

//this is to print the tupple
void CSE_machine::print_tupple(execution_element *tupple){
  	queue<execution_element*> temp_tupples;
	cout<<"(";
	while( (tupple->queue_var).size() != 0 ){
		//pop an element from queue
		execution_element *temp_tuple = (tupple->queue_var).front();

		//call print_execution_element
		if( temp_tuple != NULL){
			print_execution_element(temp_tuple);
			if( (tupple->queue_var).size() != 1)
				cout<<", ";
		}

		//save it in another queue
		temp_tupples.push(temp_tuple);
		(tupple->queue_var).pop();
	}

	cout<<")";

}


