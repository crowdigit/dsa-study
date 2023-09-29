CMAKE_MINIMUM_REQUIRED (VERSION 3.27)

PROJECT (Foo)

ADD_EXECUTABLE (
    foo
    ./src/main.cpp
    ./src/expression.cpp
)

SET_PROPERTY (TARGET foo PROPERTY CXX_STANDARD 17)
SET (CMAKE_CXX_FLAGS "-stdlib=libc++")
