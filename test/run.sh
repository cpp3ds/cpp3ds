echo "Compiling unit tests..."
g++ main.cpp -lgtest -lpthread -o test
echo "Running unit tests..."
./test -v
result=$?
echo "Unit tests completed. Result: $result"
exit $result