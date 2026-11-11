What we currently have is :
    main.cpp
    │
    ├── SDL initialization
    ├── OpenGL initialization
    ├── shader loading
    ├── pipeline creation
    ├── mesh creation
    ├── rendering
    ├── input
    └── cleanup

Next Task = update function that update the scene/ camera on deltatime + input

- [X] add glm
- [X] uniform setter in pipeline class
- [ ] Mesh importer
- [X] coordinate system
- [X] Mesh collection renderer
- [ ] camera system + movement ( get input from sdl3)
- [ ] choose and apply code convention, especially on how to pass data between class 

Cpp Core Guideline to read:
    - [ ] P — Philosophy
    - [ ] I — Interfaces
    - [ ] F — Functions
    - [ ] C — Classes
    - [ ] R — Resource management ← especially important for your OpenGL project
    - [ ] E — Error handling
    - [ ] SF — Source files
    - [ ] A — Architectural ideas