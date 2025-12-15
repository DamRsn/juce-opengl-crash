# JUCE D2D and OpenGL issues demo

Simple reproducible example of graphics issues on latest juce, with multiple instances.

What this repo is:

- A tiny JUCE audio plugin project that demonstrates freezing UI on multiple instances when using Direct2D.
- The project uses JUCE `develop` as a submodule (see the `JUCE/` folder).

Plugin UI

There are 2 buttons: one to enable D2D and one to enable OpenGL.

The bottom component (ExpensiveComponent) draws a lot of paths to simulate a heavy drawing load.

- It has a slider to control the number of paths drawn, and check the responsiveness of the UI to clicks.
- Its paint function is timed, as well as the time between different paint calls. Those two times are displayed in the
  UI ((average on last 10 calls).

## Build the plugin

1. Clone this repository with submodules:
   ```bash
   git clone --recurse-submodules <repo-url>
   ```
2. Run cmake
3. Build plugin target
4. Load it in DAW