# Unreal Engine Project: ArchViz Explorer Tool

## Overview

This Unreal Engine project focuses on creating and manipulating various architectural and environmental elements within the game world. It includes three main modes: Building Creation Mode, Interior Design Mode, and Road Creation Mode.

### Features

1. **Building Creation Mode:**
   - Allows users to place and manipulate building elements such as walls, floors, and ceilings.
   - **Features:**
     - **WallActor:** Represents vertical building elements.
     - **FloorActor:** Represents horizontal building elements.
     - **CeilingActor:** Represents ceiling elements.
     - **BuildingConstructionWidget:** UI for interacting with building creation functionalities.
     - **Material Selection:** Enables adding materials to individual actors.

2. **Interior Design Mode:**
   - Enables users to furnish and decorate interiors with furniture and decor items.
   - **Features:**
     - **InteriorDesignActor:** Represents interior elements like furniture items.
     - **InteriorDesignWidget:** UI for selecting and placing furniture items.
     - **FurnitureScrollBox:** UI component for browsing available furniture assets.
     - **Placement Restrictions:** Some furniture items have specific placement rules (e.g., beds on floors only).

3. **Road Creation Mode:**
   - Allows users to create and customize roads along spline paths.
   - **Features:**
     - **RoadActor:** Actor for dynamically generating roads based on spline paths.
     - **RoadConstructionWidget:** UI for interacting with road creation functionalities.
     - **ProceduralMeshComponent:** Component used for procedural road mesh generation.

## Setup and Usage

### Prerequisites

- Unreal Engine (version 5.2)
- Visual Studio (for C++ development)

### Installation

1. Clone the repository from [GitHub Repo URL].
2. Open the project in Unreal Engine.
3. Build the project using Visual Studio.
4. Launch the project in Unreal Editor for testing and development.

### Usage

- **Building Creation Mode:**
  - Select the desired building element (wall, floor, ceiling) from the UI.
  - Adjust dimensions using input fields.
  - Place elements in the world by clicking or dragging.

- **Interior Design Mode:**
  - Browse furniture assets in the UI.
  - Click on assets to select and place them within the interior space.
  - Adjust placement using drag-and-drop or precise coordinate inputs.

- **Road Creation Mode:**
  - Use the spline tool to define road paths.
  - Adjust road width and material using UI controls.
  - Roads are generated dynamically based on spline points.

### Key Bindings

- **Right Arrow Key:** Spawn a new actor.
- **Left Arrow Key:** Place the selected actor.
- **R Key:** Rotate the selected actor.
- **I Key:** Display key instructions.
- **M Key:** Toggle actor movement mode.
- **Del Key:** Delete The Selected Actor.

## Demo Video

Watch a demo of this project in action [here](https://zuruinc-my.sharepoint.com/:v:/g/personal/sourabh_pandey_zuru_com/EeGcyDmO_LpGv_h9TbsHXisBmNuGdj67VTS_wpIkoJt_dA?nav=eyJyZWZlcnJhbEluZm8iOnsicmVmZXJyYWxBcHAiOiJPbmVEcml2ZUZvckJ1c2luZXNzIiwicmVmZXJyYWxBcHBQbGF0Zm9ybSI6IldlYiIsInJlZmVycmFsTW9kZSI6InZpZXciLCJyZWZlcnJhbFZpZXciOiJNeUZpbGVzTGlua0NvcHkifX0&e=bocBnh).

### Notes

- Each mode operates independently but can interact within the game world.
- Customization options include material selection, dimension adjustments, and real-time placement feedback.

## Contributions

Contributions to this project are welcome. For major changes, please open an issue first to discuss the proposed changes.

## License

[License details, e.g., MIT License]
