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
**pictures**
###Language EBNF grammar description
* Programm always starts by defining user functions if needed and start (main) function
**defining:**
```
skill_to \*func_name\* [ \*func param 1\* | \*func param 2\* | ... ]
enter_mipt
    ***code***
get_sent_down
```
if there are no params just leave `[ ]`
In my language you should write all keywords separated from other. **Example:**
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


