main: test_point.cpp MatchList.cpp test_DP.cpp test_Sparse.cpp test_Dense.cpp test.hpp MatchList.hpp
	g++ -o main test_point.cpp MatchList.cpp test_DP.cpp test_Sparse.cpp test_Dense.cpp test.hpp MatchList.hpp
run: main
	./main
clean:
	rm ./main