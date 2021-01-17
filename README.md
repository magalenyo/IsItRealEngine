
# RealDisunity
Game Engine made for the Second Assignment of the Master Degree in Advanced Programming for AAA Games Development, UPC (Barcelona).

This Game Engine called [RealDisunity](https://github.com/magalenyo/IsItRealEngine) was integrated from trhee different Repositories used by its authors:

- [EngineShader](https://github.com/magalenyo/EngineShader): used to create the logic for Shaders compilation and load.
- [Engine](https://github.com/magalenyo/Engine): used to create camera and UI management.
- [RadiantEngine](https://github.com/isaacfdez/RadiantEngine): used to merge several functionalities.

Click on any of the names to access each repository.
## How to use

### Loading models and textures

- Drag model files (.fbx) inside the engine window to load it and replace the current model.

### Menu items

- File
  - Save Scene: Not fully implemented.
  - Load Scene: Not fully implemented.
  - Quit: Exit the engine.

- External Links
  - Go to RealDisunity Engine Github repository.
 
- Help
  - About RealDisunity: Shows information about the engine as well as its licence.

- Windows
  - Lets you toggle on an off the diferent windows of the engine.
  
### Move around the scene and interact with the objects on the scene

Left click outside of the information windows to focus on the scene.

- While Right-clicking the mouse:
  - W to move forward.
  - S to move backwards.
  - A to move to the left.
  - D to move to the right.
  - Q to move downwards.
  - E to move upwards.
  - Move mouse to rotate the camera.
  - Shift to duplicate movement speed.
- Alt + Left-click to orbit around the camera focus
- Mouse wheel to zoom.
- F key to focus in the center of the scene.
- Left Ctrl + Left-click while hovering an object from the scene will automatically select that object on the hierarchy and display its gizmo.
  
### Configuration windows

This window contains engine information and configuration variables for different modules:

- Application
  - Shows a FPS and MS graph.
- Hardware Information
  - Information about CPU, GPU and RAM.
- Libraries' Versions
  - Shows the versions of the libraries used in the engine.
- Renderer
  - Different options regarding the elements drawn in the editor scene, such as the axis grid or the quadtree.
- Window
  - Change window mode
    - Resizable
    - Borderless
    - Fullscreen 
    - Desktop Fullscreen
  - Resize window only if the mode is Resizable or Borderless.
- Texture
  - Information about the texture filters as well as options to modify them.
- Input
  - Information regarding the mouse position.
- Camera
  - Frustum's Front information.
  - Frustum's Up information.
  - Camera position information.
  - Near Plane information.
  - Far Plane information.
  - FOV information.
  - Aspect Ratio information.
  - Movement speed information.
  - Rotation speed information.
  - Zoom speed information.

### Inspector window

This window contains information about the selected object from the scene.

- Checkbox that sets the object's visibility.
- UID of the object.
- The following propierties only appear if the selected object has them:
  - Draw Boxes
    - Checkbox to draw the AABB and OBB of the selected object.
  - Transform
    - Allows to check the transformation properties as well as options to modify them.
  - For each Mesh the selected object has:
    - Checkbox to draw that particular mesh.
    - Information about that particular mesh, such as the number of the vertices, triangles, etc.
  - For each Material the selected object has:
    - Checkbox to load that particular material. If not checked, it will load the default missing texture.
    - Information regarding the material, such as the name, the previews of the loaded material'ls components, etc.
- The following properties only appear id the selected object is the scene camera:
  - Checkbox to activate frustum culling.
  - Parameters to modify the frustum's geometry.
  
### Console window

This window prints all logged information in the engine.

## Libraries used

- Assimp
- Dear ImGui
- DevIL
- GLEW
- ImGuizmo
- MathGeoLib
- RapidJSON
- SDL
  
  
  
## Authors
- Miguel Ángel Bueno Rivera
  - GitHub user: [magalenyo](https://github.com/magalenyo)
  - *contact: magalenyo7@gmail.com*
- Isaac Fernández Fontao
  - GitHub user: [isaacfdez](https://github.com/isaacfdez)
  - *contact: isaacfdez94@gmail.com*


Project License: [MIT](https://github.com/magalenyo/IsItRealEngine/blob/main/LICENSE)


