CFLAGS=-Wall -Wextra

all:	avl_test query.out stdquery.out q_time.out stdq_time.out

avl_test: AVL_test.cpp
	g++ $(CFLAGS) -O2 -g $< -o avl_test.out -lgtest_main -lgtest
	valgrind ./avl_test.out

query.out: main.cpp
	g++ $(CFLAGS) -Og -g $< -o $@

stdquery.out: main.cpp
	g++ $(CFLAGS) -Og -g -DSTD $< -o $@

q_time.out: main.cpp
	g++ $(CFLAGS) -O2 -DSTD -DTIME $< -o $@

stdq_time.out: main.cpp
	g++ $(CFLAGS) -O2 -DSTD -DTIME $< -o $@

clean:
	rm vgcore.*
