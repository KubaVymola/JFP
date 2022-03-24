add_subdirectory(renderables)

target_sources(JFP
    PRIVATE
        Camera.cpp
        Camera.h
        CraftVisualizer.cpp
        CraftVisualizer.h
        # Ground.cpp
        # Ground.h
        ICameraProvider.h
        # IRenderable.h
        # IRenderTree.h
        # Line3D.cpp
        # Line3D.h
        # Mesh.cpp
        # Mesh.h
        # Model3D.cpp
        # Model3D.h
        Renderer.cpp
        Renderer.h
        Shader.cpp
        Shader.h
        ShaderBank.cpp
        ShaderBank.h
        # stb_image.h
        # Texture.cpp
        # Texture.h
        # Vertex.h
        Visualizer3D.cpp
        Visualizer3D.h
)