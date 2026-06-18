# miniRT

**miniRT** is a compact 3D ray tracer written in C. It renders simple scenes using the ray tracing protocol, implementing spheres, planes, and cylinders with ambient and diffuse lighting, hard shadows, and object transformations. The project emphasizes mathematical modeling, intersection calculations, vectorized SIMD performance, and memory-safe execution using [**MLX42**](https://github.com/codam-coding-college/MLX42).

## Features

- **Ray tracing engine** using MLX42 (OpenGL) for real-time rendering
- **Scene parsing** from `.rt` files
- **Supported objects**: sphere, plane, cylinder
- **Camera system** with position and orientation control
- **Lighting model**:
  - Ambient lighting
  - Diffuse shading (Lambert)
  - Hard shadows
- **Transformations**:
  - Translation (all objects)
  - Rotation (except spheres and lights)
- **Real-time window rendering**
- **Robust error handling and cleanup**
- **Optional bonuses**: specular highlights, multiple lights, patterns

## Collaborators

- [RJW-db](https://github.com/RJW-db)
- [JonatanDobos](https://github.com/JonatanDobos)

## Table of Contents

- [Project Scope](#project-scope)
- [Instructions](#instructions)
  - [Requirements](#requirements)
  - [Usage](#usage)
    - [Running the program](#running-the-program)
    - [Interactive controls](#interactive-controls)
      - [Movement](#movement)
      - [Rotation and view](#rotation-and-view)
      - [Object selection](#object-selection)
      - [Object modification](#object-modification)
      - [Render settings](#render-settings)
      - [General](#general)
- [Build](#build)
  - [Compilation](#compilation)
  - [Quick test scenes](#quick-test-scenes)
  - [Build system details](#build-system-details)
- [Architecture and Implementation](#architecture-and-implementation)
  - [Scene parsing and validation](#scene-parsing-and-validation)
  - [Window and event flow](#window-and-event-flow)
  - [Interaction and scene editing](#interaction-and-scene-editing)
  - [Resolution scaling and threading](#resolution-scaling-and-threading)
  - [Scene export and runtime diagnostics](#scene-export-and-runtime-diagnostics)
  - [Performance optimizations](#performance-optimizations)
  - [Error handling and cleanup](#error-handling-and-cleanup)
- [Resources](#resources)
- [License](#license)

## Project Scope

<details>
<summary><strong>Subject rules and scope</strong></summary>

### Core requirements

The program must implement a functional ray tracer with the following behavior:

- Render a 3D scene from a `.rt` file  
- Display the result in a window using [**MiniLibX**](https://harm-smits.github.io/42docs/libs/minilibx)  
- At Codam, [**MLX42**](https://github.com/codam-coding-college/MLX42) is also allowed  
- Handle window events:
  - `ESC` key closes the window and exits cleanly  
  - Window close button exits cleanly  
- Support required geometric objects:
  - Sphere  
  - Plane  
  - Cylinder  
- Compute correct ray-object intersections, including inside objects  
- Apply object transformations:
  - Translation for all objects  
  - Rotation for applicable objects (excluding spheres and lights)  
- Implement lighting:
  - Ambient lighting  
  - Diffuse reflection  
  - Hard shadows  
- Render the computed image to the window  
- Exit with `Error\n` followed by a clear message on invalid input  
- Free all allocated memory and properly destroy MLX resources on exit  

### Allowed functions/libraries

The mandatory part allows the following:

- `open`, `close`, `read`, `write`  
- `malloc`, `free`  
- `printf`, `perror`, `strerror`, `exit`  
- `gettimeofday()`  
- Math library functions (`-lm`)  
- [**Libftx**](https://github.com/RJW-db/libftx#submodules)
- All [**MiniLibX**](https://harm-smits.github.io/42docs/libs/minilibx)/[**MLX42**](https://github.com/codam-coding-college/MLX42) functions  

### Project rules

The project must follow these rules:

- The project must be written in C  
- The code must follow the Norm  
- The program must not crash unexpectedly (segfault, bus error, double free, etc.)  
- All allocated memory must be freed correctly  
- The `"Makefile"` must use `cc` with `-Wall -Wextra -Werror`  
- The `"Makefile"` must include `$(NAME)`, `all`, `clean`, `fclean`, `re`  
- The Makefile must not perform unnecessary relinking  
- If libft is used, it must be compiled via its own Makefile  
- The project must be submitted to the assigned Git repository  

### Scene requirements

A valid `.rt` scene must follow these rules:

- Elements can appear in any order  
- Elements can be separated by one or more spaces or line breaks  
- Identifiers must be valid:
  - `A` (ambient light)  
  - `C` (camera)  
  - `L` (light)  
  - `sp`, `pl`, `cy` (objects)  
- Elements defined by a capital letter can only appear once:
  - Ambient light (`A`)  
  - Camera (`C`)  
- The scene must contain:
  - Exactly one camera  
  - Exactly one ambient light  
  - At least one light source  
- Values must respect constraints:
  - Colors: `[0–255]`  
  - Ratios: `[0.0–1.0]`  
  - Normalized vectors: `[-1,1]` per axis  
- Invalid or malformed scenes must trigger:
  - `Error\n` followed by a descriptive message  

### Allowed functions

- `open`, `close`, `read`, `write`
- `malloc`, `free`
- `printf`, `perror`, `strerror`, `exit`
- Math library (`-lm`)
- MiniLibX functions
- `gettimeofday()`
- Libft (if included in `"libft/"`)

### Project rules

- Written in C
- Must follow the Norm
- No crashes (segfault, double free, etc.)
- No memory leaks
- Makefile requirements:
  - Rules: `$(NAME)`, `all`, `clean`, `fclean`, `re`, `bonus`
  - Flags: `-Wall -Wextra -Werror`
  - No unnecessary relinking
- Submission via assigned Git repository

### Scene requirements

A valid `.rt` file must:

- Allow flexible ordering of elements
- Use correct identifiers:
  - `A` (ambient)
  - `C` (camera)
  - `L` (light)
  - `sp`, `pl`, `cy` (objects)
- Include:
  - Exactly one camera
  - Exactly one ambient light
  - At least one light source
- Respect value constraints:
  - Colors: `[0–255]`
  - Ratios: `[0.0–1.0]`
  - Normalized vectors: `[-1,1]`
- Be properly formatted with valid spacing and line breaks

### Example scene

A minimal valid `.rt` scene:

```sh
A   0.2            255,255,255  
C   -50,0,20       0,0,1        70  
L   -40,0,30       0.7          255,255,255  
pl  0,0,0          0,1.0,0      255,0,225  
sp  0,0,20         20 255,0,0  
cy  50.0,0.0,20.6  0,0,1.0      14.2         21.42     10,0,255  
```
### Bonus features

Optional improvements:

- Specular reflection (Phong model)
- Multiple light sources **(implemented)**  
- Checkerboard patterns
- Additional objects (cone, etc.)
- Texture or bump mapping

Bonuses are evaluated only if the mandatory part is fully correct.

</details>

## Instructions

### Requirements

- C compiler compatible with the Makefile
- MiniLibX/MLX42 installed
- Math library (`-lm`)
- Memory debugging tools:
  - Valgrind (Linux)
  - leaks (macOS)

Depending on your setup, you may also need:

- `git` for submodule initialization (MLX42, Libftx)  
- `cmake` (required for MLX42 build)  
- GLFW (required for MLX42)  
- OpenGL (macOS, typically included by default) 

### Usage

### Running the program

Run the program with a scene file:

```sh
./miniRT scenes/white_spheres.rt
```

### Interactive controls

The renderer supports real-time object manipulation, camera movement, and render settings. Keys move the **selected object** if one is selected; otherwise they move the **camera**.

#### Movement

| Key | Action |
|-----|--------|
| `W` | Move forward |
| `S` | Move backward |
| `A` | Move left |
| `D` | Move right |
| `Space` | Move up |
| `Left Shift` | Move down |

#### Rotation and view

| Key | Action |
|-----|--------|
| `Left / Right / Up / Down` arrows | Rotate selected object or camera |
| `Mouse wheel up/down` | Increase / decrease camera FOV |

#### Object selection

| Key | Action |
|-----|--------|
| `Left click` | Select object under cursor |
| `V` | Select previous object |
| `N` | Select next object |
| `B` | Deselect object |

#### Object modification

| Key | Action |
|-----|--------|
| `,` | Decrease radius (sphere/light/cylinder) |
| `.` | Increase radius (sphere/light/cylinder) |
| `M` | Decrease cylinder height |
| `/` | Increase cylinder height |
| `-` | Decrease ambient ratio or selected light brightness |
| `=` | Increase ambient ratio or selected light brightness |

#### Render settings

| Key | Action |
|-----|--------|
| `1` | Toggle automatic resolution scaling |
| `2` | Decrease manual starting render ratio |
| `3` | Increase manual starting render ratio |
| `Right Shift` | Toggle soft shadows (multithreaded only) |
| `K` | Toggle performance stats (multithreaded only) |

#### General

| Key | Action |
|-----|--------|
| `P` | Print object info |
| `Enter` | Start filename input / file creation mode |
| `Esc` | Close window (with threaded cleanup if enabled) |

## Build

### Compilation

```sh
make          # Build default release
make clean    # Remove build artifacts
make fclean   # Remove build artifacts and binary
make re       # Rebuild from scratch
make debug    # Build with sanitizers and debug symbols
make valgrind # Build for Valgrind memory testing
```

### Quick test scenes

```sh
make test     # Run test scene
make pillars  # Run pillars scene
make sunshine # Run sunshine scene
make white    # Run white scene
```

### Build system details

The `"Makefile"` uses advanced optimizations for faster compilation and execution:

- **Parallel builds**: `MAKEFLAGS += -j` enables multi-threaded compilation using all available CPU cores  
- **Compiler optimizations**: `-O2 -march=native` with architecture-specific flags for maximum performance  
- **Security flags**: `-fstack-protector-strong` and fortified source protection  
- **Link-time optimization**: `-flto=auto` (GCC) or `-flto=thin` (Clang) for optimized linking  
- **Compiler passthrough**: `COMPILER=$(COMPILER)` ensures the same compiler is used across all submodules  
- **Auto-vectorization**: Compiler automatically vectorizes SIMD operations with `-O2` 

The parallel build flag (`-j`) is also applied inside submodules:

- [**Libftx**](https://github.com/RJW-db/libftx) (extended libft with submodules)  
  - [get_next_line](https://github.com/RJW-db/get_next_line)  
  - [dbltoa](https://github.com/RJW-db/dbltoa)  
  - [dynarr](https://github.com/RJW-db/dynarr)  
- [**MLX42**](https://github.com/codam-coding-college/MLX42): Built with CMake and parallel compilation (`--parallel`)

## Architecture and Implementation

### Scene parsing and validation

- [Parse](src/parsing/parse/parsing.c) `.rt` files and build the scene structure with shared [parse utilities](src/parsing/parse/parse_utils.c)
- Validate [line structure and identifiers](src/parsing/line_validate/line_val.c), and [numeric value ranges](src/parsing/line_validate/number_check.c)
- Load [cameras, lights, and primitives](src/parsing/init_input/parse_objects.c) into the internal scene representation through [primitive initialization](src/parsing/init_input/init_primitives.c)

### Window and event flow

- Initialize the [window](src/mlx/setup/window_setup.c), image buffers, and runtime state with additional [setup helpers](src/mlx/setup/window_setup2.c)
- Register keyboard, mouse, scroll, and [loop callbacks](src/mlx/hooks/hooks.c)
- Drive redraws, [render updates](src/mlx/hooks/loop_hooks.c), and frame timing through the MLX42 event loop
- Handle [object picking](src/mlx/hooks/mouse_hook.c) and FOV scrolling from mouse input
- These runtime interactions are declared through [RTmlx.h](include/RTmlx.h) and use shared scene types from [scene.h](include/scene.h)

### Interaction and scene editing

- Route continuous [input](src/mlx/hooks/loop_move_hooks.c) through the movement loop
- Move the [camera](src/mlx/transform/cam/camera_move.c) and rotate its view through [camera rotation](src/mlx/transform/cam/camera_rotate.c)
- Move selected [objects](src/mlx/transform/obj/obj_move.c) and rotate them through [object rotation](src/mlx/transform/obj/obj_rotate.c)
- Change [radius, height, brightness, and editable values](src/mlx/transform/obj/obj_modification.c)
- Toggle manual and automatic [scaling behavior](src/mlx/transform/resolution/scaling.c)
- These controls are exposed through [RTmlx.h](include/RTmlx.h) and rely on the scene structures in [scene.h](include/scene.h)

### Resolution scaling and threading

- Automatic and manual [render scaling](src/render/rendering/upscale_manager.c) adjust resolution during rendering
- Threaded [synchronization](src/mlx/hooks/loop_hooks.c) and image switching keep the display responsive
- Create worker [threads](src/threads/handling/thread_setup.c) and release them through [thread termination](src/threads/handling/thread_terminate.c)
- Run per-thread [render work](src/threads/routine/routine.c), duplicate [scene state](src/threads/routine/read_scene.c), and keep shared [thread helpers](src/threads/routine/utils_thread.c)
- The threading model is defined in [threadsRT.h](include/threadsRT.h) and integrated into the main render loop and event flow

### Scene export and runtime diagnostics

- Create `.rt` [output files](src/create_scene_file/create_rt_file.c) from the current scene
- Serialize [scene elements](src/create_scene_file/scene_elements.c) such as cameras, lights, and ambient data
- Serialize [geometric primitives](src/create_scene_file/geometric_primitives.c) with object-specific formatting
- Print selected [object data](src/mlx/print/print_objs.c), [primitive details](src/mlx/print/print_primitives.c), and [frame statistics](src/mlx/print/print_perf_stats.c)
- Scene formatting and parser-facing structures are defined in [scene.h](include/scene.h) and [parsing.h](include/parsing.h)

### Performance optimizations

- Vector math and SIMD-friendly data structures are defined in [threadsRT.h](include/threadsRT.h) and [mathRT.h](include/mathRT.h)
- Core [arithmetic](src/math_vector/vec/vec_arithmetic.c), [geometry](src/math_vector/vec/vec_geometry.c), and [transform](src/math_vector/vec/vec_transform.c) helpers are grouped in `"src/math_vector/"`
- This reduces the cost of intersection, lighting, and camera-space calculations

### Error handling and cleanup

- Centralized [error handling](src/error/error.c) is paired with formatted [error output](src/error/print.c)
- Program [initialization](src/setup_cleanup/init.c) prepares shared runtime state before rendering starts
- Free allocated memory and runtime [resources](src/setup_cleanup/cleanup.c) on shutdown
- Exit cleanly on parse failures, render shutdown, close events, and threaded termination paths
- Error codes and helpers are declared in [RTerror.h](include/RTerror.h) and cleanup entry points are declared in [setup_clean.h](include/setup_clean.h)

## Resources

### Graphics libraries

- [MiniLibX documentation](https://harm-smits.github.io/42docs/libs/minilibx/introduction.html)
- [MLX42 repository](https://github.com/codam-coding-college/MLX42)
- [MLX42 documentation](https://github.com/codam-coding-college/MLX42/tree/master/docs)

### Ray tracing

- [Scratchapixel - Introduction to Ray Tracing](https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/how-does-it-work.html)
- [Ray Tracing in One Weekend series](https://raytracing.github.io/)
- [42 cub3d docs](https://harm-smits.github.io/42docs/projects/cub3d)

### Math and 3D graphics

- [Vector Math for 3D Computer Graphics](https://chortle.ccsu.edu/vectorlessons/vectorindex.html)
- [Practical 3D Math for Programmers](https://stunlock.gg/posts/linear_algebra/)
- [3D Modelling for Programmers - Math Review](https://cathyatseneca.gitbooks.io/3d-modelling-for-programmers/content/mathematical_background/index.html)


## License

This project is licensed under the [MIT License](LICENSE).
