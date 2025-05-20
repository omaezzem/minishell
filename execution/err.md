===========================================

echo --> echo "-n" Hola
echo " " | cat -e
""''echo hola""'''' que""'' tal""''

--> parsing handle quotes " " you must give me the commande like this echo -n Hola 
whitout quotes .


bash-3.2$ >echo>
bash-3.2$ <echo<
bash-3.2$ >>echo>>
bash: syntax error near unexpected token `newline'  you write ---> minishell: syntax error near unexpected token '>' remplace '>' to 'newline'

echo > < -----------> bash: syntax error near unexpected token `<'

echo | | -----------> bash: syntax error near unexpected token `|'

===========================================

********************************************

bash-3.2$ ./parse 
bash: ./parse: No such file or directory

bash-3.2$ ./parssing/
bash: ./parssing/: is a directory

echo $?

echo $?$

echo $? | echo $? | echo $?

echo $:$= | cat -e

echo $""

echo "$"""

****************************************************

export $?  ------------------------> bash: export: `0': not a valid identifier
export HOLA=bon;jour



int overflow in int 0
int overflow in long 0
== 999 -> ""
> 999 -> 1
> 0 -> 0