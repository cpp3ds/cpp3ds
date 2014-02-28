echo "Compiling unit tests..."
g++ main.cpp -o test -L../lib -I../include -L../../tween-engine/lib -I../../tween-engine/include -std=c++11 -lgtest -lpthread -lcpp3ds-test -ltween-test -DTESTING
echo "Running unit tests..."
./test -v
result=$?
echo "Unit tests completed. Result: $result"
exit $result