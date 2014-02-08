echo "Compiling unit tests..."
g++ main.cpp -o test -L../lib -I../include -std=c++11 -lgtest -lpthread -lcpp3ds -DTESTING
echo "Running unit tests..."
./test -v
result=$?
echo "Unit tests completed. Result: $result"
exit $result