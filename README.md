# MIPT language README

## Description
MIPT language is a study project written in C, where are used some interesting algorithms. Project imitates work of a real compiler of a real programming language. Project consists of three big blocks: frontend, middleend and backend. MIPT language consists of my own keywords, so it in some way characterizes studing at MIPT.

> [!NOTE]
> In this projects are used my own written libraries like:
> * own Stack lib
> * virtual processor CPU
> * Tree with graphic logs

## Frontend
This is a first step of processing code of our language. It consists of lexical analysis part and recursive descent parsing tree constructor. 
### Lexical analisys
**pictures**<br>
###Language EBNF grammar description
1) Programm always starts by defining user functions if needed and start (main) function<br>
**defining:**
```
skill_to func_name [ func param 1 | func param 2 | ... ]
enter_mipt
    code
get_sent_down
```
* if there are no params just leave `[ ]`
* In my language you should write all keywords separated from other.<br>
**Example:**
```
[ ]
( 1 + 2 * 3 )
func_call [ arg1 | arg2 ]
etc.
```

**main function:**
```
born
enter_mipt
    ***code***
get_sent_down
die
```

2) In every function are placed **operators**
* If operator (with else or without)
* Variable operators (initialization or assignment)
* Loop operator
* Base functions:
	- print and scan
	- decrease operator (--)
	- return
	- square root operator
	- operators for quadratic equation
		- all nubers print
		- no roots print
