main: test_point.cpp MATCHLIST.cpp test_DP.cpp test_Sparse.cpp test_Dense.cpp test.hpp MATCHLIST.hpp
	g++ -o main test_point.cpp MATCHLIST.cpp test_DP.cpp test_Sparse.cpp test_Dense.cpp test.hpp MATCHLIST.hpp
run: main
	./main
clean:
	rm ./main