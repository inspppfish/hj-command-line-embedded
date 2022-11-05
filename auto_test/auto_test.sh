#test
echo -e "\033[34m\
test1 : buffer
test2 : util
test3 : parse\
\033[0m"
var=$(make test_all)
make clean
if [ "$var" = "" ]; then
  echo "test passed"
else
  >&2 echo -e "\033[31m\
test not passed\
\033[0m"
fi
