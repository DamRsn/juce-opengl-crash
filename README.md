# Juce OpenGL + Image Caching Crash Repro

Simple reproducible example of a crash in JUCE when using OpenGL and image caching via
`juce::Component::setBufferedToImage`.

What this repo is

- A tiny JUCE audio plugin project that demonstrates a bug when combining OpenGL rendering and component image caching.
- The project uses JUCE `master` as a submodule (see the `JUCE/` folder).
- The Standalone target is the easiest way to test the bug.

Plugin UI

- The plugin UI has two buttons:
    - Left button: connect / disconnect the OpenGL renderer for the main Editor.
    - Right button: toggle `setBufferedToImage(true/false)` on `DummyCachedComponent` (a component that shows a small
      text label at the bottom of the UI).

Reproduction steps (quick)

1. Launch the Standalone build of the plugin (with debugger attached).
2. Click the Left button to enable OpenGL rendering.
3. Click the Right button to turn on image caching for the dummy component.
4. Click the Left button again to disable OpenGL rendering.

Expected / observed behavior

- Windows: If the component was cached while using OpenGL, disabling OpenGL causes a crash (if the component is still
  cached).
- macOS: There is no crash, but the dummy cached component disappears when disabling OpenGL if it was cached while using
  OpenGL. It re-appears if you disable/enable caching.

## Build the plugin

1. Clone this repository with submodules:
   ```bash
   git clone --recurse-submodules <repo-url>
   ```
2. Run cmake
3. Build and run the Standalone target.