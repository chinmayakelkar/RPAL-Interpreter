#ifndef PARSER_H
#define PARSER_H
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<stack>
#include <string>
#include<unordered_map>
#include<queue>
#include<stack>
#include<cctype>
#include<stdlib.h>
#include<cstdio>
#include<sstream>

enum execElementType {EXE_ENV, EXE_INT, EXE_STR, EXE_TUPPLE, EXE_CTRL_STRUCT, EXE_PRIMITIVE_FUNC, EXE_TRUE, EXE_FALSE, EXE_NILL, EXE_DUMMY, EXE_YSTAR, EXE_ITA};

//keep track of all primitive function
enum primitive_function {PRINT, CONC, ORDER, STERN, ISTUPLE, ISINTEGER, STEM, ISTRUTHVALUE, ISSTRING, ITOS };
enum astNodeType {IDT, STR, INT, TAU, LAMBDA, WHERE, LET, AUG, COND, AMP, OR, NOT, GR, GE, LS, LE, EQ, NE, PLUS, MINUS, NEG, MULTI, DIV, EXPO, AT, GAMMA, TRUE, FALSE, NIL, DUMMY, WITHIN, AND, REC, EQUAL, FUN_FORM, PARAN, COMMA, YSTAR, DELTA_THEN, DELTA_ELSE, BETA, ENV };

//declarations
class environment;
class control_st;

class execution_element{
public:
	environment *env_var;
	control_st *ctrl_st_var;
	bool ConcOnce;
	execElementType type; //to keep track of the type
	int int_var;
	std::string string_var;
	std::queue<execution_element*> queue_var; //to handle vars in case of tau
	primitive_function inBuiltFunctions;

};
class AST_node {

		std::string node_value; //will contain the actual values
		astNodeType node_type; //type of node e.g. TERMINAL, TAU etc
	public:
		AST_node(); //initialize everything
		~AST_node();

		AST_node *left_child; //left pointer will point to childern
		AST_node *right_sibling; //this will point to siblings


		void set_node_value(std::string node_value);
		std::string get_node_value();

		void set_node_type(astNodeType node_type);
		astNodeType get_node_type();
};

class environment{
	std::unordered_map<std::string, execution_element*> bound_variable; //this is supposed to be the hash map of all bounded variables in the


public:
	environment *parent; //initialize it to NULL

	environment();
	~environment(); //environment will presist till the end so no need to do anything here

	/* make it return NULL in case key is not found*/
	execution_element* lookup(std::string key); //return the execution_element corresponding to key

	/* in case key already exists just bring down the whole system */
	void insert(std::string key, execution_element *element); //insert into hash table
};

class control_st{
	int num_children; //this is only for TAU node
	public:

		AST_node *node; 
		control_st *next; 
		control_st *delta; 
		AST_node *bound_var; 
		environment *env; 
		
		void setNoofChildren(int num_child);
		int getNoofChildren();
		control_st();
		~control_st();
};

class CSE_machine{
	int i;
	std::stack<control_st*> control_stack; //control stack
	std::stack<execution_element*> execution_stack; //execution stack
	environment *current_env; //current environment

	void print_control_struct(control_st *cnt_st);

public:
	CSE_machine(control_st *cntrl_st);
	void print_tupple(execution_element *tupple); //to print tupples
	void print_execution_element(execution_element *in_element); //print the execution element
	void load(control_st *delta); //load the control stack with
	void execute(); //this will begin the execution
	
};
#endif

