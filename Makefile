NAME            :=  miniRT

MAKEFLAGS       +=  -j
COMPILER        :=  gcc

BASE_FLAGS      :=  -std=c99 -Wall -Wextra -Werror
PEDANTIC        :=  -Wpedantic -pedantic-errors -Wundef -Wstrict-prototypes
# WARNINGS        :=  -Wshadow -Wconversion -Wsign-conversion         \
#                     -Wformat=2 -Wuninitialized -Wunreachable-code
WARNINGS        :=  -Wshadow -Wsign-conversion

CAST_WARNINGS   :=  -Wbad-function-cast
ifeq ($(shell $(COMPILER) --version | grep -c "gcc"),1)
CAST_WARNINGS   +=  -Wcast-function-type
endif

DEPFLAGS        :=  -MMD -MP

# OPTIMIZATION    :=  -O2 -march=native -fno-math-errno              \
#                     -freciprocal-math -fno-signed-zeros            \
#                     -fno-trapping-math
# SECURITY        :=  -fstack-protector-strong
THREAD_FLAGS    :=  -pthread
N_JOBS          :=  $(shell nproc || sysctl -n hw.logicalcpu || echo 1)
THREADS         :=  $(if $(filter-out 1,$(N_JOBS)),2,1)

OS              :=  $(shell uname -s)
ifeq ($(OS),Linux)
OPTIMIZATION    +=  -fsingle-precision-constant -flto=auto -fuse-linker-plugin
SECURITY        +=  -D_FORTIFY_SOURCE=2
FSANITIZE       :=  leak,
else ifeq ($(OS),Darwin)
OPTIMIZATION    +=  -flto
endif

SANITIZERS      :=  -fsanitize=$(FSANITIZE)address,undefined,null,integer-divide-by-zero,signed-integer-overflow,bounds,alignment
DEBUG_FLAGS     :=  -fno-omit-frame-pointer

SCREEN_RES      :=
ifeq ($(OS),Linux)
SCREEN_RES      :=  $(shell xrandr 2>/dev/null | grep '*' | uniq | awk '{print $$1}' | head -1)
else ifeq ($(OS),Darwin)
SCREEN_RES      :=  $(shell system_profiler SPDisplaysDataType 2>/dev/null | grep Resolution | head -n 1 | awk '{print $$2"x"$$4}')
endif

ifeq ($(SCREEN_RES),)
SCREEN_RES      :=  1920x1080
endif

SCREEN_WIDTH    :=  $(shell echo $(SCREEN_RES) | cut -d 'x' -f 1)
SCREEN_HEIGHT   :=  $(shell echo $(SCREEN_RES) | cut -d 'x' -f 2)

CFLAGS          :=  $(BASE_FLAGS) $(PEDANTIC) $(WARNINGS) $(CAST_WARNINGS)  \
                    $(DEPFLAGS) $(OPTIMIZATION) $(SECURITY) $(THREAD_FLAGS)  \
                    -DTHREADS=$(THREADS)                                      \
                    -DSCREEN_WIDTH=$(SCREEN_WIDTH)                            \
                    -DSCREEN_HEIGHT=$(SCREEN_HEIGHT)

CFLAGS += -no-pie

ifneq ($(filter valgrind,$(MAKECMDGOALS)),)
CFLAGS          +=  -g $(DEBUG_FLAGS)
else ifneq ($(filter debug,$(MAKECMDGOALS)),)
CFLAGS          +=  -g2 $(SANITIZERS) $(DEBUG_FLAGS) -fno-sanitize-recover=all
endif

PRINT_NO_DIR    :=  --no-print-directory
RM              :=  rm -rf

# MLX42
ifeq ($(OS),Darwin)
GLFW_PREFIX     :=  $(shell brew --prefix glfw 2>/dev/null)
MLX_LINK        :=  -lglfw -L$(GLFW_PREFIX)/lib
else
MLX_LINK        :=  -lglfw -lm -ldl -lpthread
endif

# Directories
INC_DIR         :=  include
SRC_DIR         :=  src
BUILD_DIR       :=  .build
EXT_DIR         :=  extern_libraries

# Libftx
LIBFTX_A         :=  libftx.a
LIBFTX_DIR       :=  $(EXT_DIR)/libftx
LIBFTX           :=  $(LIBFTX_DIR)/$(LIBFTX_A)
LIBFTX_INC       :=  $(LIBFTX_DIR)/$(INC_DIR)
LIBFTX_SENTINEL  :=  $(LIBFTX_DIR)/.git

# MLX42
MLX42_DIR       :=  $(EXT_DIR)/MLX42
MLX42_SENTINEL  :=  $(MLX42_DIR)/.git
MLXLIB          :=  $(MLX42_DIR)/build/libmlx42.a
MLX42_INC       :=  -I$(MLX42_DIR)/include

MAIN            :=  main.c
PARSE           :=  parse/parsing.c             parse/parse_utils.c                                             \
                    line_validate/line_val.c    line_validate/number_check.c                                    \
                    init_input/parse_objects.c  init_input/init_primitives.c
THREADING       :=  handling/thread_setup.c     handling/thread_terminate.c                                     \
                    routine/routine.c           routine/utils_thread.c          routine/read_scene.c            \
                    routine/performance_stats.c
MLX             :=  setup/window_setup.c        setup/window_setup2.c                                           \
                    hooks/hooks.c               hooks/loop_hooks.c              hooks/mouse_hook.c              \
                    hooks/loop_move_hooks.c                                                                     \
                    transform/cam/camera_move.c transform/cam/camera_rotate.c                                   \
                    transform/obj/obj_move.c    transform/obj/obj_rotate.c      transform/obj/obj_modification.c\
                    transform/resolution/scaling_resolution.c                                                   \
                    print/print_objs.c          print/print_primitives.c        print/print_perf_stats.c
SCENE           :=  set_filename.c              create_rt_file.c                scene_elements.c                \
                    geometric_primitives.c
RENDER          :=  rendering/render.c          rendering/trace_ray.c                                           \
                    rendering/set_pixel.c       rendering/upscale_manager.c                                     \
                    intersect/obj_intersect.c   intersect/cylinder.c            intersect/cylinder_parts.c      \
                    intersect/lighting.c
MATH_VEC        :=  vec/vec_arithmetic.c        vec/vec_geometry.c              vec/vec_transform.c             \
                    math/clamp.c
SETUP_CLEAN     :=  init.c                      cleanup.c
ERROR           :=  error.c                     print.c

SRCS            :=  $(addprefix $(SRC_DIR)/, $(MAIN))                                                           \
                    $(addprefix $(SRC_DIR)/parsing/, $(PARSE))                                                  \
                    $(addprefix $(SRC_DIR)/threads/, $(THREADING))                                              \
                    $(addprefix $(SRC_DIR)/mlx/, $(MLX))                                                        \
                    $(addprefix $(SRC_DIR)/create_scene_file/, $(SCENE))                                        \
                    $(addprefix $(SRC_DIR)/render/, $(RENDER))                                                  \
                    $(addprefix $(SRC_DIR)/math_vector/, $(MATH_VEC))                                           \
                    $(addprefix $(SRC_DIR)/setup_cleanup/, $(SETUP_CLEAN))                                      \
                    $(addprefix $(SRC_DIR)/error/, $(ERROR))

OBJS            :=  $(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS            :=  $(OBJS:.o=.d)

INCLUDES        :=  -I$(INC_DIR) -I$(LIBFTX_INC) $(MLX42_INC)

DELETE          :=  *.out           **/*.out        .DS_Store   \
                    **/.DS_Store    .dSYM/          **/.dSYM/

all: $(NAME)

$(NAME): $(OBJS) $(MLXLIB) $(LIBFTX)
	@$(COMPILER) $(CFLAGS) $(OBJS) $(MLXLIB) $(LIBFTX) $(MLX_LINK) -o $(NAME)
	@printf "$(CREATED)" $@ $(CUR_DIR)

$(BUILD_DIR)/%.o: %.c | $(LIBFTX) $(MLXLIB)
	@mkdir -p $(@D)
	@$(COMPILER) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(MLX42_SENTINEL):
	git submodule update --init $(MLX42_DIR)

$(MLXLIB): | $(MLX42_SENTINEL)
	cmake -B $(MLX42_DIR)/build $(MLX42_DIR)
	cmake --build $(MLX42_DIR)/build --parallel
	@printf "$(CREATED)" libmlx42.a $(CUR_DIR)

$(LIBFTX_SENTINEL): | $(MLX42_SENTINEL)
	git submodule update --init $(LIBFTX_DIR)
	git -C $(LIBFTX_DIR) checkout $(shell git config -f .gitmodules submodule.$(LIBFTX_DIR).branch || echo main)
	git -C $(LIBFTX_DIR) submodule update --remote --merge
	git -C $(LIBFTX_DIR) submodule update --init src/printf
	git -C $(LIBFTX_DIR) submodule update --init src/get_next_line
	git -C $(LIBFTX_DIR) submodule update --init src/dbltoa
	git -C $(LIBFTX_DIR) submodule update --init src/dynarr
	git -C $(LIBFTX_DIR)/src/printf checkout $$(git config -f $(abspath $(LIBFTX_DIR))/.gitmodules submodule.src/printf.branch || echo main)
	git -C $(LIBFTX_DIR)/src/get_next_line checkout $$(git config -f $(abspath $(LIBFTX_DIR))/.gitmodules submodule.src/get_next_line.branch || echo main)
	git -C $(LIBFTX_DIR)/src/dbltoa checkout $$(git config -f $(abspath $(LIBFTX_DIR))/.gitmodules submodule.src/dbltoa.branch || echo main)
	git -C $(LIBFTX_DIR)/src/dynarr checkout $$(git config -f $(abspath $(LIBFTX_DIR))/.gitmodules submodule.src/dynarr.branch || echo main)

$(LIBFTX): | $(LIBFTX_SENTINEL)
	@$(MAKE) $(PRINT_NO_DIR) -C $(LIBFTX_DIR) SUBMODULES_CMD= all printf gnl dbltoa dynarr $(firstword $(filter debug valgrind,$(MAKECMDGOALS)) all)

clean:
	@$(RM) $(BUILD_DIR) $(DELETE)
	@printf "$(REMOVED)" $(BUILD_DIR)/ $(CUR_DIR)$(BUILD_DIR)/

fclean: clean
	@[ ! -e "$(LIBFTX_SENTINEL)" ] || $(MAKE) $(PRINT_NO_DIR) -C $(LIBFTX_DIR) fclean
	@[ ! -e "$(MLX42_SENTINEL)" ] || $(RM) "$(MLX42_DIR)/build"
	@$(RM) $(NAME)
	@printf "$(REMOVED)" $(NAME) $(CUR_DIR)

re:
	@$(MAKE) $(PRINT_NO_DIR) fclean
	@$(MAKE) $(PRINT_NO_DIR) all

test: all
	./$(NAME) ./scenes/test.rt

pillars: all
	./$(NAME) ./scenes/pillars.rt

sunshine: all
	./$(NAME) ./scenes/sunshine.rt

white: all
	./$(NAME) ./scenes/white.rt

debug: $(if $(filter-out debug,$(MAKECMDGOALS)),,all)

valgrind: $(if $(filter-out valgrind,$(MAKECMDGOALS)),,all)

print-%:
	$(info $($*))

-include $(DEPS)

.PHONY: all clean fclean re       \
        test pillars sunshine white \
        debug valgrind print-%

# ----------------------------------- colors --------------------------------- #

BOLD        := \033[1m
DIM         := \033[2m
ITALIC      := \033[3m
UNDERLINE   := \033[4m
BLACK       := \033[30m
RED         := \033[31m
GREEN       := \033[32m
YELLOW      := \033[33m
BLUE        := \033[34m
MAGENTA     := \033[35m
CYAN        := \033[36m
WHITE       := \033[37m
RESET       := \033[0m

R_MARK_UP   := $(MAGENTA)$(BOLD)
CA_MARK_UP  := $(GREEN)$(BOLD)

# ----------------------------------- messages ------------------------------- #

CUR_DIR     :=  $(dir $(abspath $(firstword $(MAKEFILE_LIST))))
REMOVED     :=  $(R_MARK_UP)REMOVED $(CYAN)%s$(MAGENTA) (%s) $(RESET)\n
CREATED     :=  $(CA_MARK_UP)CREATED $(CYAN)%s$(GREEN) (%s) $(RESET)\n
