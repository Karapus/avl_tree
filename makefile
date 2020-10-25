CFLAGS=-Wall -Wextra
DFLAGS=-ggdb -Og
INCLUDES=AVL_tree.hpp AVL_set.hpp

all:	clean avl_test range.out stdrange.out range_time.out stdrange_time.out order.out order_time.out

avl_test: AVL_test.cpp
	g++ $(CFLAGS) -O2 -g $< -o avl_test.out -lgtest_main -lgtest
	valgrind ./avl_test.out

range.out: range_query.cpp
	g++ $(CFLAGS) $(DFLAGS) $< -o $@

stdrange.out: range_query.cpp
	g++ $(CFLAGS) $(DFLAGS) -DSTD $< -o $@

range_time.out: range_query.cpp
	g++ $(CFLAGS) -O2 -DSTD -DTIME $< -o $@

stdrange_time.out: range_query.cpp
	g++ $(CFLAGS) -O2 -DSTD -DTIME $< -o $@
order.out: order.cpp
	g++ $(CFLAGS) $(DFLAGS) $< -o $@
order_time.out: order.cpp
	g++ $(CFLAGS) -O2 -DTIME $< -o $@
AVL_test.cpp: $(INCLUDES)
range_query.cpp: $(INCLUDES)
order.cpp: $(INCLUDES)

clean:
	rm -f vgcore.*
	rm -f *.out
