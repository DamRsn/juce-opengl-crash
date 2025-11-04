# Juce OpenGL + Image Caching Crash Repro

Simple reproducible example of a crash in JUCE when using OpenGL, image caching via
`juce::Component::setBufferedToImage` and `juce::Component::getCachedComponentImage()->invalidateAll()`.

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

1. Launch the Standalone build of the plugin (with debugger attached). (Note that OpenGL and Image Caching are on).
2. Click the Invalidate button and see that there's a crash because `getCachedComponentImage()` returns a `nullptr`

Also one can get a crash by clicking "Invalidate" with OpenGL off, if the component was cached with OpenGL on.

## Build the plugin

1. Clone this repository with submodules:
   ```bash
   git clone --recurse-submodules <repo-url>
   ```
2. Run cmake
3. Build and run the Standalone target.