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

![Screenshot from 2023-08-31 12-02-29](https://github.com/2APetrin/MIPT_language/assets/75694757/cbd146a3-4d1f-461c-9404-57efea383638)<br><br>
Frontend block makes lexical tokens from our text of programm. It looks like this<br>
![Screenshot from 2023-09-01 17-44-36](https://github.com/2APetrin/MIPT_language/assets/75694757/95c05791-5662-4a3a-8bfa-e0477e40f4c3)<br>
Then frontend runs recursive descent parser making codetree based on EBNF grammar.

### Language EBNF grammar description

Programm always starts by defining user functions if needed and start (main) function<br><br>
**defining functions:**
```
skill_to func_name [ *func param 1* | *func param 2* | ... ]
enter_mipt
    *code*
get_sent_down
```
* if there are no params just leave `[ ]`
* In my language you should write all keywords separated from other exept $.

**Example:**
```
[ ]
( 1 + 2 * 3 )
func_call [ arg1 | arg2 ]
etc...
```

**main function:**
```
born
enter_mipt
    *code*
get_sent_down
die
```

In every function are placed **operators**
### If operator (with else or without)<br>
**If operator:**
```
when ( x + 1 ) <-> 3 + 2 * 1 togda
enter_mipt
    *code*
get_sent_down
```
`==` is `<->`<br>
`>`  is `>`<br>
`<`  is `<`<br>
`>=` is `->`<br>
`<=` is `->`<br>
`!=` is `<!>`<br>

**Else:**
```
when_nepravilno
enter_mipt
    *code*
get_sent_down
```
### Variable operators (initialization or assignment)<br>
**Variable initialization:**
```
invent *variable* *number*$
```

**Var assignment:**
```
*variable* podgon_pod (expression or function call)$
```
**Loop:**<br>
loop checks if variable is true or not (while in c)
```
skat *variable* nulevok_untill_they_run_out
enter_mipt
	*code*
get_sent_down
```
* Base functions:
	- print and scan
	- decrease operator (--)
	- return
	- square root operator
	- operators for quadratic equation:
		- all nubers print
		- no roots print
<br>

**print:**<br>
number print<br>
```
whisper_to_the_professor *expression* in_the_ear$
```

**scan:**<br>
number input in my language
```
take_note *variable*$
```

**decrease operator:**
decreases value of variable for 1<br>
```
solve_nulevka *variable*$
```

**return from function:**
```
hand_in *expression* zadach$
```

**square root:**
```
*variable* podgon_pod izvlech *variable* koren$
```

**all numbers fit:**
```
what_ugodno$
```

**no roots print:**
```
vihoda_net$
```

### Programm example
```
skill_to fact [ num ]
enter_mipt
    when num <- 0 togda
    enter_mipt
        hand_in 1 zadach$
    get_sent_down

    invent fact_temp 0$

    fact_temp podgon_pod fact [ num - 1 ]$
    fact_temp podgon_pod fact_temp * num$

    hand_in fact_temp zadach$
get_sent_down


born
enter_mipt
    invent x 0$
    take_note x$

    x podgon_pod fact [ x ]$

    whisper_to_the_professor x in_the_ear$

get_sent_down
die
```


### Recursive descent parsing and base tree
Recursive descent parser makes AST tree with it's standart from code tokens like this<br>
![tree_dump0](https://github.com/2APetrin/MIPT_language/assets/75694757/6064ff21-8e9f-4222-b4d5-5cdad28d1521)

## Middleend
Midlleend gets AST tree and simplifies it for code optimization. Then it makes temporary file with virtual CPU assembly code. (My CPU project)<br>
![tree_dump101](https://github.com/2APetrin/MIPT_language/assets/75694757/4a177953-a1fd-464b-b2fb-7ec2928a9298)

## Backend
Backend is responsible for bytecode generration for my virtual CPU. Backend gets asm code from midleend and generates codes for CPU and then CPU executes them.<br>
![image](https://github.com/2APetrin/MIPT_language/assets/75694757/96aaf041-4384-4da7-9c05-7452b3647281)

## Build
```
git clone https://github.com/2APetrin/MIPT_language.git

cd MIPT_language

make

./mipt_language *codefile path*
```
*Last command example*
```
./mipt_language examples/factorial.mipt
```
