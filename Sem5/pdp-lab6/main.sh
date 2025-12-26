echo "Compiling C++ - hamiltonean_cycle.cpp"
g++ -O2 -std=c++11 -pthread -Dhom -Wall hamiltonean_cycle.cpp -o hamiltonean_cycle.o

echo "Compiling Java - HamiltonianCycleForkJoin.java"
javac HamiltonianCycleForkJoin.java
echo ""

for i in `ls input/*.txt`
do
    echo "Running C++ test $i"
    head -n1 $i
    ./hamiltonean_cycle.o $i
    echo ""

    echo "Running Java test $i"
    head -n1 $i
    java HamiltonianCycleForkJoin $i
    echo ""
done

rm -f *.o *.class








