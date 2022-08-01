
namespace Maze3D
{
    class IMeshComponent
    {
    public:

        virtual ~IMeshComponent() {}

        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;
        std::vector<VertexBoneData> bones;

        GLuint VAO, EBO;
        GLuint VBO_vertices;
        GLuint VBO_bones;

        virtual void Setup() = 0;

    };
}
