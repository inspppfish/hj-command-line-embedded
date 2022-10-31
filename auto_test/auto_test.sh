#test1
echo -e "\033[34m\
test1 : copy_to_command_line_input_buffer()\
\033[0m"
var=$(make test1)
if [ "$var" = "" ]; then
  echo "test1 passed"
else
  >&2 echo -e "\033[31m\
test1 not passed\
\033[0m"
fi

#test2
