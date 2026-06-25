PROJECT_NAME = test-project
ITCHIO_USER = puroprimesouscaprisun

# CXX
CXX = g++
WIN_CXX = x86_64-w64-mingw32-g++
EMCC = em++

# Targets
TARGET = game
WIN_TARGET = publish/windows/game.exe
WEB_TARGET = publish/web/index.html

WINDOWS_DLLS = /usr/x86_64-w64-mingw32/usr/bin/glfw3.dll \
							 /usr/x86_64-w64-mingw32/bin/libgcc_s_seh-1.dll \
							 /usr/x86_64-w64-mingw32/bin/libssp-0.dll \
							 /usr/x86_64-w64-mingw32/bin/libstdc++-6.dll \
							 /usr/x86_64-w64-mingw32/bin/zlib1.dll \
							 /usr/x86_64-w64-mingw32/bin/libwinpthread-1.dll

# Flags
CXXFLAGS = -std=c++17 -Iinclude
WINDOWS_CXXFLAGS = -std=c++17 -Iinclude \
									 -static \
									 -static-libgcc \
									 -static-libstdc++
TEST_CXXFLAGS = -Wall -Wextra

WEB_CXXFLAGS = \
-std=c++17 \
-Iinclude \
-s USE_GLFW=3 \
-s USE_SDL=2 \
-s FULL_ES3=1 \
-s WASM=1 \
-s ALLOW_MEMORY_GROWTH=1 \
-s ASSERTIONS=1 \
-s USE_WEBGL2=1 \
-s STACK_SIZE=100000 \
-s ASSERTIONS=2 \
-s SAFE_HEAP=1 \
-s STACK_OVERFLOW_CHECK=2 \
-s USE_PTHREADS=1 \
-O2

# Libraries
LIBS_LINUX = \
-lglfw \
-lGL \
-ldl \
-lpthread \
-lX11 \
-lXrandr \
-lXi \
-lXxf86vm \
-lXcursor \
-lm

LIBS_WINDOWS = \
-L/usr/x86_64-w64-mingw32/lib \
-lglfw3 \
-lopengl32 \
-lgdi32 \
-lwinmm

# Sources
COMMON_SRC = \
	src/stbImpl.cpp \
	src/main.cpp \
	src/Window.cpp \
	src/Object.cpp \
	src/FileLoader.cpp \
	src/Player.cpp \
	src/Sound.cpp \
	src/UIElement.cpp \
	src/Font.cpp \
	src/TextElement.cpp \
	src/Container.cpp \
	src/Button.cpp \
	src/Particle.cpp \
	src/Enemy.cpp \
	src/UIParticle.cpp

SRC = $(COMMON_SRC) src/glad.c
WEB_SRC = $(COMMON_SRC)

# Web build sources
# Remove glad.c because emscripten provides OpenGL bindings
PUBLISH_SRC = textures \
							fonts \
							shaders

# Object files
OBJ_DIR = build

OBJ = $(SRC:src/%.cpp=$(OBJ_DIR)/src/%.o)
OBJ := $(OBJ:src/glad.c=$(OBJ_DIR)/src/glad.o)

# Build
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LIBS_LINUX)

$(OBJ_DIR)/src/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/src/glad.o: src/glad.c
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

windows:
	mkdir -p build
	mkdir -p publish/windows

	$(WIN_CXX) \
	$(WINDOWS_CXXFLAGS) \
	$(SRC) \
	-o $(WIN_TARGET) \
	$(LIBS_WINDOWS) \
	-mwindows

web:	
	mkdir -p publish/web
 
	$(EMCC) \
	$(WEB_SRC) \
	$(WEB_CXXFLAGS) \
	--preload-file textures \
	--preload-file fonts \
	--preload-file shaders \
	--preload-file sfx \
	-o $(WEB_TARGET)

test: CXXFLAGS += $(TEST_CXXFLAGS)
test: all
	./$(TARGET)

clean:
	rm -rf build
	rm -rf publish
	rm -f $(TARGET)

publish: clean
	mkdir -p publish/linux
	mkdir -p publish/windows
	mkdir -p publish/web

	# Linux
	cp -r $(PUBLISH_SRC) publish/linux
	$(MAKE)

	cp $(TARGET) publish/linux/$(TARGET)

	# Windows
	cp -r $(PUBLISH_SRC) publish/windows
	#cp $(WINDOWS_DLLS) publish/windows

	$(MAKE) windows

	# Web
	$(MAKE) web

	# Zip builds
	cd publish && \
	zip -r -9 linux.zip linux && \
	zip -r -9 windows.zip windows && \
	zip -r -9 game-web.zip web && \
	butler push linux.zip $(ITCHIO_USER)/$(PROJECT_NAME):linux && \
	butler push windows.zip $(ITCHIO_USER)/$(PROJECT_NAME):windows && \
	butler push game-web.zip $(ITCHIO_USER)/$(PROJECT_NAME):game-web

.PHONY: all clean test windows web publish git

