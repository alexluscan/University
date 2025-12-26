echo "Compiling naive algorithm - naive_sequential.cpp"
g++ -O2 -std=c++11 -pthread -Dhom -Wall naive_sequential.cpp -o naive_1.o
echo "Compiling naive algorithm (threaded) - naive_parallel.cpp"
g++ -O2 -std=c++11 -pthread -Dhome -Wall naive_parallel.cpp -o naive_2.o
echo "Compiling karasuba algorithm - karasuba_sequential.cpp'"
g++ -O2 -std=c++11 -pthread -Dhome -Wall karasuba_sequential.cpp -o karasuba_1.o
echo "Compiling karasuba algorithm (threaded) - karasuba_parallel.cpp'"
g++ -O2 -std=c++11 -pthread -Dhome -Wall karasuba_parallel.cpp -o karasuba_2.o

TEST_FILE="tests/6k.in"

if [ ! -f "$TEST_FILE" ]; then
    echo "Test file $TEST_FILE not found"
    exit 1
fi

echo ""
echo "Running test $TEST_FILE"
head -n1 "$TEST_FILE"
./naive_1.o "$TEST_FILE"
./naive_2.o "$TEST_FILE"
./karasuba_1.o "$TEST_FILE"
./karasuba_2.o "$TEST_FILE"
echo ""

rm *.o








