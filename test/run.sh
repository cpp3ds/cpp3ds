echo "Compiling unit tests..."
g++ main.cpp -lgtest -std=c++11 -o test
echo "Running unit tests..."
./test -v
result=$?
echo "Unit tests completed. Result: $result"
exit $result