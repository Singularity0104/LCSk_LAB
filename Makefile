main: test_point.o MatchList.o test_DP.o test_Sparse.o test_Dense.o
	g++ -o $@ $^
%.o: %.c
	g++ -c -o $@ $^
run: main
	./main
clean:
	rm ./main *.o *.txt
