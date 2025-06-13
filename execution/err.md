minishell$ export EMPTY_VAR
minishell$ echo "[$EMPTY_VAR]"
[$EMPTY_VAR]

bash-3.2$ export EMPTY_VAR
bash-3.2$ echo "[$EMPTY_VAR]"
[]

======================================

minishell$ export QUOTED="hello world"
minishell$ echo "$QUOTED"
$QUOTED

bash-3.2$ export QUOTED="hello world"
bash-3.2$ echo "$QUOTED"
hello world

======================================

minishell$ export SINGLE='single quotes'
minishell$ echo "$SINGLE"
$SINGLE

bash-3.2$ export SINGLE='single quotes'
bash-3.2$ echo "$SINGLE"
single quotes

======================================

minishell$ export EMPTY=""
minishell$ echo "[$EMPTY]"
[$EMPTY]

bash-3.2$ export EMPTY=""
bash-3.2$ echo "[$EMPTY]"
[]

=====================================

minishell$ export PATH="/custom/path:$PATH"
minishell$ echo $PATH
/custom/path:$PATH

bash-3.2$ export PATH="/custom/path:$PATH"
bash-3.2$ echo $PATH
/custom/path:/mnt/homes/omaezzem/.brew/bin:/mnt/homes/omaezzem/.brew/bin:/mnt/homes/omaezzem/.brew/bin:/mnt/homes/omaezzem/brew/bin:/mnt/homes/omaezzem/brew/bin:/mnt/homes/omaezzem/brew/bin:/mnt/homes/omaezzem/brew/bin:/mnt/homes/omaezzem/goinfre/homebrew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/munki:/Library/Apple/usr/bin
bash-3.2$ 

=====================================

minishell$ << a cat
> aasdf
> as
> asdf
> a
1
aasdf
as
asdf
minishell$ 




========================

exit -9223372036854775808

# Expected behavior
- Should exit with status 0 (LONG_MIN % 256)



