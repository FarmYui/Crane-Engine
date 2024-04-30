![Crane](/Resources/Branding/CraneLogo.png?raw=true "Crane")

# CraneEngine
🏗️ Crane Engine is created by following the "Game Engine" tutorial series by the Cherno
Crane Engine was originally born as 2D but has native support for 3D too.

Here is a beatiful render done by the CraneEngine
![CraneEngine](/Resources/Branding/CraneEngineRender.png?raw=true "CraneEngine")

## Editor
📷 The Editor camera supports pitch & yaw rotations, so you can move around the scene with ease

### GUI
🎨 Here is how the editor is looking
![CraneEngine](/Resources/Branding/CraneSceneScreenshot.png?raw=true "CraneEngine")
The scene was entirely made with tools inside the editor

The Editor has: 
- Viewport panel        | It renders the scene from the EditorCamera, you can also select entities directly from here 
- SceneHeirarchy panel  | It shows all the entities in the scene
- Properties panel      | After selecting an entity you will see its components here
- Stats panel           | It shows how many: draw calls, quads, vertices and indices you have in your scene

The Editor has easy learning curve, even for beginners.
Here are all the shortcuts you may want to use to speed up your creation process.
### How to move around
- Alt + MouseButtonLeft    | Rotates around the scene
- Alt + MouseButtonMiddle  | Pans around the scene
- Alt + MouseButtonRight   | Zooms into the scene

### Scene Shortcuts
- Ctrl + N          | Creates a new Empty Scene
- Ctrl + Shift + S  | Saves a Scene into a pre-existing file
- Ctrl + O          | Opens a Scene from file

### Gizmo Shortcuts
- Tab  | Cycles trough Gizmos
- Q    | Gets rid of Gizmos
- W    | Toggles the Translation Gizmo
- E    | Toggles the Rotation Gizmo
- R    | Toggles the Scale Gizmo


## Getting Started
💻 Visual Studio 2022 is recommended, Crane only supports Windows.

You can clone the repository to a local destination using git:

`git clone --recursive https://github.com/FarmYui/CraneEngine`

Make sure that you do a `--recursive` clone to fetch all of the submodules!

Unfortunately the submodules are not maintained anymore so make sure to follow the vid i made to setup correctly 
the dependencies (to avoid headaches)

update: turn on staticruntime in premakefile of yaml-cpp or it will give linking errors ;

In the future obv i will try to remove the submodules and update them but i'm not going to do that any time soon