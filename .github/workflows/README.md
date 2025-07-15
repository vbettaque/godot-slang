# GitHub Actions Workflows

This directory contains GitHub Actions workflows for the Godot Slang project.

## Workflows

### `release.yml` - Release Build
**Triggers:** GitHub releases, Manual dispatch

This workflow creates production-ready releases:

- Builds only the release version for Linux
- Uses Ubuntu 25.04 container (matching the dev container)
- Builds Slang generators and static library
- Builds godot-cpp dependency
- Builds the godot-slang extension
- Packages the complete demo folder as a ready-to-use Godot project
- Creates a zip file for easy distribution
- Automatically uploads to GitHub releases (when triggered by a release)

Todo: move built libraries to addons/godot-slang

**Artifacts:**
- `godot-slang-demo-linux` - Complete demo project package
- `godot-slang-demo.zip` - Zipped demo project for distribution

## Build Process

The workflow follows this process:

1. **Environment Setup**: Install build dependencies matching the dev container
2. **Slang Build**: 
   - Build Slang generators using CMake presets
   - Configure and build Slang static library
3. **godot-cpp Build**: Build the Godot C++ bindings (release mode)
4. **Extension Build**: Build the godot-slang extension using SCons (release mode)
5. **Package**: Package the complete demo folder with built binaries

## Usage

### For Releases
1. Create a GitHub release with a tag
2. The release workflow will automatically build and attach the addon package
3. Or manually trigger the release workflow from the Actions tab

### For Development
To test builds during development, manually trigger the release workflow from the Actions tab.

## Requirements

The workflow requires no special setup - it uses the same dependencies as the dev container and should work out of the box on any repository with this project structure.

## Platform Support

Currently only Linux x86_64 is supported. To add more platforms, update the SCons platform parameters in the workflow.

## Troubleshooting

- Check the workflow logs in the Actions tab
- Manually trigger the release workflow to test builds
- Verify submodules are properly initialized
- Check that all dependencies are available in the container
