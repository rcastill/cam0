# CAM0

Toy project. Open VideoCapture on cam 0 (possibly, webcam) and apply opencv filters.

## Usage

```bash
# Example 1: See camera
./cam0

# Example 2: Apply grayscale
./cam0 ./cam0f_grayscale.so

# Example 3: Apply horizontal flip
./cam0 ./cam0f_fliph.so

# Example 4: Apply both
./cam0 ./cam0f*
```

## Build

Dependencies: opencv4

```bash
# Debug
make -j$(nproc)

# Release
make RELEASE=1 -j$(nproc)
```