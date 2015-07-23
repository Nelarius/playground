
CC=g++

LINKER_INCLUDES = -LC:/dev/SDL2-devel-2.0.3-mingw/SDL2-2.0.3/lib/x86 -LC:/dev/glew-1.12.0/glew-1.12.0/lib -LC:/dev/lua/build

CFLAGS = -std=gnu++14 -Wall -O2 -DDEBUG -DGLM_FORCE_RADIANS

LDFLAGS =

EXECUTABLE =

TEST_EXECUTABLE = 

ifeq ($(OS),Windows_NT)
	CFLAGS += -I./src -IC:/dev/glm -IC:/dev/glew-1.12.0/glew-1.12.0/include -IC:/dev/SDL2-devel-2.0.3-mingw/SDL2-2.0.3/x86_64-w64-mingw32/include -IC:/dev/boost_1_58_0 -I:C/dev/lua/src -IC:/dev/LuaBridge/Source
	LDFLAGS += -lopengl32 -lglew32 -lmingw32 -lSDL2main -lSDL2 -lyaml-cpp $(LINKER_INCLUDES)
	EXECUTABLE += Build/app.exe
	TEST_EXECTUTABLE += Build/test.exe
else
	UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CFLAGS +=  -I ./src -I /home/muszynsk/dev/LuaBridge/Source
        LDFLAGS +=  -lGL -lGLEW -lSDL2 -lyaml-cpp -lentityx -lassimp -llua
		EXECUTABLE += Build/app
		TEST_EXECUTABLE += Build/test
    endif
endif

OBJ = src/Main.o \
	src/GameState.o \
	src/app/Application.o \
	src/app/Window.o \
	src/app/AppState.o \
	src/app/AppStateStack.o \
	src/ecs/Pool.o \
	src/ecs/Entity.o \
	src/lua/LuaState.o \
	src/opengl/Shader.o \
	src/opengl/Program.o \
	src/opengl/VertexArrayObject.o \
	src/opengl/VertexArrayObjectFactory.o \
	src/opengl/BufferObject.o \
	src/opengl/Texture.o \
	src/opengl/Interface.o \
	src/manager/MeshManager.o \
	src/manager/ShaderManager.o \
	src/system/Collision.o \
	src/system/Debug.o \
	src/system/Physics2d.o \
	src/system/Render.o \
	src/utils/Random.o \
	
TESTOBJ = src/Test.o \
	src/test/EntityManagerTest.o \

all: $(EXECUTABLE)
	cp src/config.lua Build/
	cp -r src/data Build/
	./Build/test

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXECUTABLE): $(OBJ)
	$(CC) -o $(EXECUTABLE) $(OBJ) $(LDFLAGS)
	
test: $(TESTOBJ)
	$(CC) -o $(TEST_EXECUTABLE) $(TESTOBJ) -lUnitTest++


.PHONY: clean

clean:
	rm $(OBJ) $(EXECUTABLE)
	rm Build/config.lua
	rm -r Build/data
