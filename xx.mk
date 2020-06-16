0->
gcc -o lock lockw.cpp
1->
gcc --std=c++11 -o slock ScopedLock.cpp  -lpthread -lstdc++
