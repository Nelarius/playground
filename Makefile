
CC=g++

# there are for windows compilation
SDL_LINK = C:/dev/SDL2-2.0.3/build
SDL_COMP = C:/dev/SDL2-2.0.3/include

GLEW_LINK = C:/dev/glew-1.12.0/lib
GLEW_COMP = C:/dev/glew-1.12.0/include

ASSIMP_LINK = C:/dev/assimp/lib
ASSIMP_COMP = C:/dev/assimp/include

LUA_LINK = C:/dev/lua/build
LUA_COMP = C:/dev/lua/src

LUABRIDGE_COMP = C:/dev/LuaBridge/Source
GLM_COMP = C:/dev/glm

CPPTEST_COMP = C:/dev/unittest-cpp
CPPTEST_LINK = C:/dev/unittest-cpp/build

LINKER_INCLUDES = -L $(SDL_LINK) -L $(GLEW_LINK) -L $(LUA_LINK) -L $(ASSIMP_LINK)

CFLAGS = -std=gnu++14 -Wall -g -DDEBUG -DASSERTION_ENABLED -DGLM_FORCE_RADIANS

LDFLAGS =

EXECUTABLE =

TEST_EXECUTABLE = 

ifeq ($(OS),Windows_NT)
	CFLAGS += -I./src -I $(GLM_COMP) -I $(GLEW_COMP) -I $(SDL_COMP) -I $(LUA_COMP) -I $(LUABRIDGE_COMP) -I $(ASSIMP_COMP) -I $(CPPTEST_COMP)
	LDFLAGS += -lopengl32 -lglew32 -lmingw32 -lSDL2main -lSDL2 -llua -lassimp -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid $(LINKER_INCLUDES)
	EXECUTABLE += Build/app.exe
	TEST_EXECUTABLE += Build/test.exe
else
	UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CFLAGS +=  -I ./src -I /home/muszynsk/dev/LuaBridge/Source
        LDFLAGS +=  -lGL -lGLEW -lSDL2 -lentityx -lassimp -llua 
		EXECUTABLE += Build/app
		TEST_EXECUTABLE += Build/test
    endif
endif

OBJ = src/Main.o \
	src/3rdparty/imgui.o \
	src/3rdparty/imgui_draw.o \
	src/3rdparty/json11/json11.o \
	src/app/GameState.o \
	src/app/Application.o \
	src/app/Window.o \
	src/app/KeyboardManager.o \
	src/app/AppState.o \
	src/app/AppStateStack.o \
	src/ecs/Event.o \
	src/ecs/Entity.o \
	src/ecs/Component.o \
	src/ecs/System.o \
	src/lua/LuaState.o \
	src/lua/BindLua.o \
	src/opengl/Shader.o \
	src/opengl/Program.o \
	src/opengl/VertexArrayObject.o \
	src/opengl/VertexArrayObjectFactory.o \
	src/opengl/BufferObject.o \
	src/opengl/Texture.o \
	src/manager/MeshManager.o \
	src/manager/ShaderManager.o \
	src/math/Angle.o \
	src/system/Render.o \
	src/system/Debug.o \
	src/system/Scripter.o \
	src/system/Ui.o \
	src/system/ImGuiRenderer.o \
	src/utils/Random.o \
	src/utils/Pool.o \
	
TESTOBJ = src/Test.o \
	src/test/EntityManagerTest.o \
	src/ecs/Component.o \
	src/ecs/Entity.o \
	src/ecs/Event.o \
	src/ecs/System.o \
	src/utils/Pool.o \

all: $(EXECUTABLE)
	make test
	cp src/config.lua Build/
	cp -r src/data Build/
	./Build/test

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXECUTABLE): $(OBJ)
	$(CC) -o $(EXECUTABLE) $(OBJ) $(LDFLAGS)
	
test: $(TESTOBJ)
	$(CC) -o $(TEST_EXECUTABLE) $(TESTOBJ) -L $(CPPTEST_LINK) -lUnitTest++


.PHONY: clean

clean:
	rm $(OBJ) $(EXECUTABLE) $(TEST_EXECUTABLE) $(TESTOBJ)
	rm Build/config.lua
	rm -r Build/data
