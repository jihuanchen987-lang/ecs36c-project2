.RECIPEPREFIX := >
.DEFAULT_GOAL := all

CXX := g++
CPPFLAGS := -Iinclude -Itests/src
CXXFLAGS := -std=c++20 -O0 -g -Wall -Wextra -MMD -MP
TESTFLAGS := --coverage
TESTLIBS := -lgtest_main -lgtest -pthread
HEADERS := $(wildcard include/*.h) $(wildcard tests/src/*.h)
DATA := $(wildcard data/*.csv)

.PHONY: all dirs test coverage memcheck run clean

all: run coverage

dirs:
>mkdir -p bin obj tests/bin tests/obj tests/htmlcov

obj/FriendGroups.o: src/FriendGroups.cpp $(HEADERS) | dirs
>$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

obj/makefriends.o: src/makefriends.cpp $(HEADERS) | dirs
>$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

bin/makefriends: obj/FriendGroups.o obj/makefriends.o
>$(CXX) $^ -o $@

tests/obj/%.o: tests/src/%.cpp $(HEADERS) | dirs
>$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(TESTFLAGS) -c $< -o $@

tests/obj/FriendGroups.o: src/FriendGroups.cpp $(HEADERS) | dirs
>$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(TESTFLAGS) -c $< -o $@

tests/bin/testmap: tests/obj/MapTest.o
>$(CXX) $(TESTFLAGS) $^ -o $@ $(TESTLIBS)

tests/bin/testunionfind: tests/obj/UnionFindTest.o
>$(CXX) $(TESTFLAGS) $^ -o $@ $(TESTLIBS)

tests/bin/testfriendgroups: tests/obj/FriendGroupsTest.o tests/obj/FriendGroups.o
>$(CXX) $(TESTFLAGS) $^ -o $@ $(TESTLIBS)

test: tests/bin/testmap tests/bin/testunionfind tests/bin/testfriendgroups
>find tests/obj -name '*.gcda' -delete
>./tests/bin/testmap
>./tests/bin/testunionfind
>./tests/bin/testfriendgroups

coverage: test
>lcov --capture --directory tests/obj --output-file tests/coverage.info --ignore-errors inconsistent,source
>lcov --remove tests/coverage.info '/usr/*' '*/tests/src/*' --output-file tests/coverage.info --ignore-errors inconsistent,source
>genhtml tests/coverage.info --output-directory tests/htmlcov --ignore-errors source

memcheck: bin/makefriends test
>valgrind --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=definite,indirect --error-exitcode=1 ./tests/bin/testmap
>valgrind --leak-check=full --errors-for-leak-kinds=definite,indirect --error-exitcode=1 ./bin/makefriends -f data/friendsmed.csv > /dev/null

run: memcheck
>@set -e; for file in $(DATA); do \
>    echo "Running $$file"; \
>    ./bin/makefriends -f "$$file" > /dev/null; \
>done

clean:
>rm -rf bin obj tests/bin tests/obj tests/htmlcov
>rm -f tests/coverage.info

-include $(wildcard obj/*.d tests/obj/*.d)