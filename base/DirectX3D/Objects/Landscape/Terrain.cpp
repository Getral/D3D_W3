#include "Framework.h"

Terrain::Terrain(Vector2 size)
    : GameObject(L"Landscape/Terrain.hlsl"), width(size.x), height(size.y)
{    
    material->SetDiffuseMap(L"Textures/Landscape/tamriel.4.0.0.dds");
    //material->SetSpecularMap(L"Textures/Landscape/Fieldstone_SM.tga");
    //material->SetNormalMap(L"Textures/Landscape/Fieldstone_NM.tga");
      
    //heightMap = Texture::Add(L"Textures/HeightMaps/TamrielBeta_10_2016_01_01.gif");
    heightMap = Texture::Add(L"Textures/Color/Black.png"); // <- 가장 낮은 평지(높이 0)
    //alphaMap = Texture::Add(L"Textures/AlphaMaps/TestAlphaMap.png");
    //secondMap = Texture::Add(L"Textures/Landscape/Dirt.png");
    //thirdMap = Texture::Add(L"Textures/Landscape/Dirt3.png");

    computeShader = Shader::AddCS(L"Compute/ComputePicking.hlsl");

    structuredBuffer = new StructuredBuffer(
        inputs.data(), sizeof(InputDesc), triangleSize,
        sizeof(OutputDesc), triangleSize);
    rayBuffer = new RayBuffer();

    mesh = new Mesh<VertexType>();
    MakeMesh();
    MakeNormal();
    MakeTangent();
    MakeComputeData();
    mesh->CreateMesh();


}

Terrain::Terrain(wstring mapFile, wstring heightmap)
    : GameObject(L"Landscape/Terrain.hlsl")
{
    material->SetDiffuseMap(mapFile);
    //material->SetSpecularMap(L"Textures/Landscape/Fieldstone_SM.tga");
    material->SetNormalMap(L"Textures/Landscape/tamriel.4.0.0_n.dds");

    heightMap = Texture::Add(heightmap);
    //heightMap = Texture::Add(L"Textures/HeightMaps/TestMap.png");
    //heightMap = Texture::Add(L"Textures/HeightMaps/TamrielBeta_10_2016_01_prepped.png");
    //alphaMap = Texture::Add(L"Textures/AlphaMaps/TestAlphaMap.png");
    //secondMap = Texture::Add(L"Textures/Landscape/Dirt.png");
    //thirdMap = Texture::Add(L"Textures/Landscape/Dirt3.png");

    mesh = new Mesh<VertexType>();
    MakeMesh();
    MakeNormal();
    MakeTangent();
    MakeComputeData();
    mesh->CreateMesh();

    computeShader = Shader::AddCS(L"Compute/ComputePicking.hlsl");

    structuredBuffer = new StructuredBuffer(
        inputs.data(), sizeof(InputDesc), triangleSize,
        sizeof(OutputDesc), triangleSize);
    rayBuffer = new RayBuffer();

}

Terrain::~Terrain()
{    
    delete mesh;    
}

void Terrain::Render()
{
    //alphaMap->PSSet(10);
    //secondMap->PSSet(11);
    //thirdMap->PSSet(12);
    GameObject::SetRender();
    mesh->Draw();
    
}

float Terrain::GetHeight(const Vector3& pos, Vector3* normal)
{
    int x = (int)pos.x;
    int z = (int)(height - pos.z - 1);

    if (x < 0 || x >= width - 1) return 0.0f;
    if (z < 0 || z >= height - 1) return 0.0f;

    UINT index[4];
    index[0] = width * z + x;
    index[1] = width * (z + 1) + x;
    index[2] = width * z + x + 1;
    index[3] = width * (z + 1) + x + 1;

    vector<VertexType> vertices = mesh->GetVertices();

    Vector3 p[4];
    for (UINT i = 0; i < 4; i++)
        p[i] = vertices[index[i]].pos;

    float u = pos.x - p[0].x;
    float v = pos.z - p[0].z;

    Vector3 result;

    if (u + v <= 1.0)
    {
        result = ((p[2] - p[0]) * u + (p[1] - p[0]) * v) + p[0];

        if (normal)
        {
            (*normal) = GetNormalFromPolygon(p[0], p[1], p[2]);
        }
        return result.y;
    }
    else
    {
        u = 1.0f - u;
        v = 1.0f - v;

        result = ((p[1] - p[3]) * u + (p[2] - p[3]) * v) + p[3];

        if (normal)
        {
            (*normal) = GetNormalFromPolygon(p[2], p[1], p[3]);
        }
        return result.y;
    }
}

Vector3 Terrain::Picking()
{
    Ray ray = CAM->ScreenPointToRay(mousePos);

    for (UINT z = 0; z < height - 1; z++)
    {
        for (UINT x = 0; x < width - 1; x++)
        {
            UINT index[4];
            index[0] = width * z + x;
            index[1] = width * z + x + 1;
            index[2] = width * (z + 1) + x;
            index[3] = width * (z + 1) + x + 1;

            vector<VertexType> vertices = mesh->GetVertices();

            Vector3 p[4];
            for (UINT i = 0; i < 4; i++)
                p[i] = vertices[index[i]].pos;

            float distance = 0.0f;
            if (Intersects(ray.pos, ray.dir, p[0], p[1], p[2], distance))
            {
                return ray.pos + ray.dir * distance;
            }

            if (Intersects(ray.pos, ray.dir, p[3], p[1], p[2], distance))
            {
                return ray.pos + ray.dir * distance;
            }
        }
    }

    return Vector3();
}

bool Terrain::ComputePicking(Vector3& pos, Transform* transform)
{
    Vector3 pickingPos = transform->Pos();
    pickingPos.y += 3.0f;

    rayBuffer->Get().pos = pickingPos;
    rayBuffer->Get().dir = transform->Down();
    rayBuffer->Get().triangleSize = triangleSize;

    rayBuffer->SetCS(0);

    DC->CSSetShaderResources(0, 1, &structuredBuffer->GetSRV());
    DC->CSSetUnorderedAccessViews(0, 1, &structuredBuffer->GetUAV(), nullptr);

    computeShader->Set();

    UINT x = ceil((float)triangleSize / 64.0f);

    DC->Dispatch(x, 1, 1);

    structuredBuffer->Copy(outputs.data(), sizeof(OutputDesc) * triangleSize);

    float minDistance = FLT_MAX;
    int minIndex = -1;

    UINT index = 0;
    for (OutputDesc output : outputs)
    {
        if (output.picked)
        {
            if (minDistance > output.distance)
            {
                minDistance = output.distance;
                minIndex = index;
            }
        }
        index++;
    }

    if (minIndex >= 0)
    {
        if (transform->GetTag() != "Enemy")
            pos = pickingPos + transform->Down() * minDistance;
        return true;
    }

    return false;
}

void Terrain::MakeNormal()
{
    vector<VertexType>& vertices = mesh->GetVertices();
    vector<UINT> indices = mesh->GetIndices();

    for (UINT i = 0; i < indices.size() / 3; i++)
    {
        UINT index0 = indices[i * 3 + 0];
        UINT index1 = indices[i * 3 + 1];
        UINT index2 = indices[i * 3 + 2];

        Vector3 v0 = vertices[index0].pos;
        Vector3 v1 = vertices[index1].pos;
        Vector3 v2 = vertices[index2].pos;

        Vector3 e0 = v1 - v0;
        Vector3 e1 = v2 - v0;

        Vector3 normal = Cross(e0, e1).GetNormalized();

        vertices[index0].normal = normal + vertices[index0].normal;
        vertices[index1].normal = normal + vertices[index1].normal;
        vertices[index2].normal = normal + vertices[index2].normal;
    }
}

void Terrain::MakeTangent()
{
    vector<VertexType>& vertices = mesh->GetVertices();
    vector<UINT> indices = mesh->GetIndices();

    for (UINT i = 0; i < indices.size() / 3; i++)
    {
        UINT index0 = indices[i * 3 + 0];
        UINT index1 = indices[i * 3 + 1];
        UINT index2 = indices[i * 3 + 2];

        Vector3 p0 = vertices[index0].pos;
        Vector3 p1 = vertices[index1].pos;
        Vector3 p2 = vertices[index2].pos;

        Vector2 uv0 = vertices[index0].uv;
        Vector2 uv1 = vertices[index1].uv;
        Vector2 uv2 = vertices[index2].uv;

        Vector3 e0 = p1 - p0;
        Vector3 e1 = p2 - p0;

        float u1 = uv1.x - uv0.x;
        float v1 = uv1.y - uv0.y;
        float u2 = uv2.x - uv0.x;
        float v2 = uv2.y - uv0.y;

        float d = 1.0f / (u1 * v2 - v1 * u2);
        Vector3 tangent = d * (e0 * v2 - e1 * v1);

        vertices[index0].tangent += tangent;
        vertices[index1].tangent += tangent;
        vertices[index2].tangent += tangent;
    }
}

void Terrain::MakeMesh()
{
    width = (UINT)heightMap->GetSize().x;
    height = (UINT)heightMap->GetSize().y;

    vector<Float4> pixels;
    heightMap->ReadPixels(pixels);

    //Vertices
    vector<VertexType>& vertices = mesh->GetVertices();

    vertices.reserve(width * height);
    for (UINT z = 0; z < height; z++)
    {
        for (UINT x = 0; x < width; x++)
        {
            VertexType vertex;
            vertex.pos = { (float)x, 0.0f, (float)(height - z - 1)};
            vertex.uv.x = x / (float)(width - 1);
            vertex.uv.y = z / (float)(height - 1);

            UINT index = width * z + x;
            vertex.pos.y = pixels[index].x * MAX_HEIGHT;

            vertices.push_back(vertex);
        }
    }

    //Indices
    vector<UINT>& indices = mesh->GetIndices();

    indices.reserve((width - 1) * (height - 1) * 6);

    for (UINT z = 0; z < height - 1; z++)
    {
        for (UINT x = 0; x < width - 1; x++)
        {
            indices.push_back(width * z + x);//0
            indices.push_back(width * z + x + 1);//1
            indices.push_back(width * (z + 1) + x);//2            

            indices.push_back(width * (z + 1) + x);//2
            indices.push_back(width * z + x + 1);//1            
            indices.push_back(width * (z + 1) + x + 1);//3
        }
    }    
}

void Terrain::MakeComputeData()
{
    vector<VertexType> vertices = mesh->GetVertices();
    vector<UINT> indices = mesh->GetIndices();

    triangleSize = indices.size() / 3;

    inputs.resize(triangleSize);
    outputs.resize(triangleSize);

    for (UINT i = 0; i < triangleSize; i++)
    {
        UINT index0 = indices[i * 3 + 0];
        UINT index1 = indices[i * 3 + 1];
        UINT index2 = indices[i * 3 + 2];

        inputs[i].v0 = vertices[index0].pos;
        inputs[i].v1 = vertices[index1].pos;
        inputs[i].v2 = vertices[index2].pos;
    }
}
