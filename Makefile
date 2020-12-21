main: test_00.cpp test_01.cpp test_02.cpp test_03.cpp test_04.cpp test_point.cpp test.h
	g++ -o main test_point.cpp test_00.cpp test_01.cpp test_02.cpp test_03.cpp test_04.cpp test.h
run: main
	./main
clean:
	rm ./main