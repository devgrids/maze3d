#ifndef MESH_A_HEADER
#define MESH_A_HEADER

namespace Maze3D {

#define NUM_BONES_PER_VERTEX 4

    struct VertexA
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 text_coords;
    };

    struct BoneMatrix
    {
        aiMatrix4x4 offsetMatrix;
        aiMatrix4x4 finalWorldTransform;
    };

    struct VertexBoneData
    {
        GLuint ids[NUM_BONES_PER_VERTEX];  
        float weights[NUM_BONES_PER_VERTEX];

        VertexBoneData()
        {
            memset(ids, 0, sizeof(ids));
            memset(weights, 0, sizeof(weights));
        }

        void addBoneData(GLuint bone_id, float weight)
        {
            for (GLuint i = 0; i < NUM_BONES_PER_VERTEX; i++)
            {
                if (weights[i] == 0.0)
                {
                    ids[i] = bone_id;
                    weights[i] = weight;
                    return;
                }
            }
        }
    };

    class MeshA
    {
    public:

        vector<VertexA> vertices;
        vector<GLuint> indices;
        vector<Texture> textures;
        vector<VertexBoneData> bones;

        GLuint VAO;
        GLuint VBO_vertices;
        GLuint VBO_bones;
        GLuint EBO_indices;

        Util* Util;

        MeshA() {};

        MeshA(vector<VertexA> vertices, vector<unsigned int> indices, vector<Maze3D::Texture> textures, vector<VertexBoneData> bones)
        {
            this->vertices = vertices;
            this->indices = indices;
            this->textures = textures;
            this->bones = bones;

            Util = Util::Instance();

            setupMesh();
        }

        void draw(Shader& shader)
        {
            for (int i = 0; i < textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i);
                TextureType type = textures[i].type;              
                glUniform1i(glGetUniformLocation(shader.ID, ("material." + Util->typeTexture[type]).c_str()), i);
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
            }

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
            glActiveTexture(GL_TEXTURE0);
        }

        void drawInstance(Shader& shader, int amount)
        {
            for (unsigned int i = 0; i < textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i);
                TextureType type = textures[i].type;
                glUniform1i(glGetUniformLocation(shader.ID, ("material." + Util->typeTexture[type]).c_str()), i);
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
            }
            glBindVertexArray(VAO);
            glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, amount);
            glBindVertexArray(0);
        }


    private:

        void setupMesh()
        {
            //vertices data
            glGenBuffers(1, &VBO_vertices);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            //bones data
            glGenBuffers(1, &VBO_bones);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_bones);
            glBufferData(GL_ARRAY_BUFFER, bones.size() * sizeof(bones[0]), &bones[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            //numbers for sequence indices
            glGenBuffers(1, &EBO_indices);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_indices);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            // create VAO and binding data from buffers to shaders
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
            //vertex position
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexA), (GLvoid*)0);
            glEnableVertexAttribArray(1); // offsetof(Vertex, normal) = returns the byte offset of that variable from the start of the struct
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexA), (GLvoid*)offsetof(VertexA, normal));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexA), (GLvoid*)offsetof(VertexA, text_coords));
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            //bones
            glBindBuffer(GL_ARRAY_BUFFER, VBO_bones);
            glEnableVertexAttribArray(3);
            glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (GLvoid*)0); // for INT Ipointer
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (GLvoid*)offsetof(VertexBoneData, weights));
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            //indices
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_indices);
            glBindVertexArray(0);
        }


    };
}
#endif