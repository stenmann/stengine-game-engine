# Compiler and flags
CXX = clang++
CXXFLAGS = -std=c++17 -O3 -I. -Iglm -Irapidjson/include -Isrc \
           -ISDL2 -ISDL2_image -ISDL2_ttf -ISDL2_mixer \
           -Ilua \
           -ILuaBridge \
           -Wno-deprecated-declarations \
           -Ibox2d

# Detect operating system
UNAME_S := $(shell uname -s)

# Set linking flags based on OS
ifeq ($(UNAME_S),Darwin)
LDFLAGS = -F./SDL2/lib -F./SDL2_image/lib -F./SDL2_ttf/lib -F./SDL2_mixer/lib \
          -framework SDL2 -framework SDL2_image -framework SDL2_ttf -framework SDL2_mixer \
          -Wl,-rpath,@executable_path/SDL2/lib \
          -Wl,-rpath,@executable_path/SDL2_image/lib \
          -Wl,-rpath,@executable_path/SDL2_ttf/lib \
          -Wl,-rpath,@executable_path/SDL2_mixer/lib \
          -Llua -llua
else
LDFLAGS = -L./SDL2/lib -L./SDL2_image/lib -L./SDL2_ttf/lib -L./SDL2_mixer/lib \
          -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer \
          -Wl,-rpath,'$$ORIGIN/SDL2/lib' \
          -Wl,-rpath,'$$ORIGIN/SDL2_image/lib' \
          -Wl,-rpath,'$$ORIGIN/SDL2_ttf/lib' \
          -Wl,-rpath,'$$ORIGIN/SDL2_mixer/lib' \
          -llua5.4
endif

# Target executable
TARGET = game_engine_linux

# Box2D source files
BOX2D_SOURCES = box2d/collision/b2_broad_phase.cpp \
                box2d/collision/b2_chain_shape.cpp \
                box2d/collision/b2_circle_shape.cpp \
                box2d/collision/b2_collide_circle.cpp \
                box2d/collision/b2_collide_edge.cpp \
                box2d/collision/b2_collide_polygon.cpp \
                box2d/collision/b2_collision.cpp \
                box2d/collision/b2_distance.cpp \
                box2d/collision/b2_dynamic_tree.cpp \
                box2d/collision/b2_edge_shape.cpp \
                box2d/collision/b2_polygon_shape.cpp \
                box2d/collision/b2_time_of_impact.cpp \
                box2d/common/b2_block_allocator.cpp \
                box2d/common/b2_draw.cpp \
                box2d/common/b2_math.cpp \
                box2d/common/b2_settings.cpp \
                box2d/common/b2_stack_allocator.cpp \
                box2d/common/b2_timer.cpp \
                box2d/dynamics/b2_body.cpp \
                box2d/dynamics/b2_chain_circle_contact.cpp \
                box2d/dynamics/b2_chain_polygon_contact.cpp \
                box2d/dynamics/b2_circle_contact.cpp \
                box2d/dynamics/b2_contact.cpp \
                box2d/dynamics/b2_contact_manager.cpp \
                box2d/dynamics/b2_contact_solver.cpp \
                box2d/dynamics/b2_distance_joint.cpp \
                box2d/dynamics/b2_edge_circle_contact.cpp \
                box2d/dynamics/b2_edge_polygon_contact.cpp \
                box2d/dynamics/b2_fixture.cpp \
                box2d/dynamics/b2_friction_joint.cpp \
                box2d/dynamics/b2_gear_joint.cpp \
                box2d/dynamics/b2_island.cpp \
                box2d/dynamics/b2_joint.cpp \
                box2d/dynamics/b2_motor_joint.cpp \
                box2d/dynamics/b2_mouse_joint.cpp \
                box2d/dynamics/b2_polygon_circle_contact.cpp \
                box2d/dynamics/b2_polygon_contact.cpp \
                box2d/dynamics/b2_prismatic_joint.cpp \
                box2d/dynamics/b2_pulley_joint.cpp \
                box2d/dynamics/b2_revolute_joint.cpp \
                box2d/dynamics/b2_weld_joint.cpp \
                box2d/dynamics/b2_wheel_joint.cpp \
                box2d/dynamics/b2_world.cpp \
                box2d/dynamics/b2_world_callbacks.cpp \
                box2d/rope/b2_rope.cpp

# Source files
SOURCES = src/main.cpp \
          src/Actor.cpp \
          $(BOX2D_SOURCES)

# Header files (for dependencies)
HEADERS = src/Actor.hpp \
          src/assets.hpp \
          src/AudioDB.hpp \
          src/AudioHelper.h \
          src/ComponentDB.hpp \
          src/engineUtils.hpp \
          src/EventBus.hpp \
          src/Helper.h \
          src/ImageDB.hpp \
          src/Input.hpp \
          src/main.hpp \
          src/Particle.hpp \
          src/physics.hpp \
          src/Render.hpp \
          src/SceneDB.hpp \
          src/Template.hpp \
          src/TextDB.hpp

# Default target
all: $(TARGET)

# Build liblua.a if missing (macOS only)
lua/liblua.a:
	cd lua && make a

# Build the executable
ifeq ($(UNAME_S),Darwin)
$(TARGET): lua/liblua.a $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)
else
$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)
endif

# Clean build artifacts
clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: all clean