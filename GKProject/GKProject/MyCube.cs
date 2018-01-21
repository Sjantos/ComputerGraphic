using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenTK;
using OpenTK.Graphics.OpenGL;
using D = System.Drawing;

namespace GKProject
{
    public enum MyCubeTexture
    {
        Diamond,
        Dirt,
        Gold,
        Leaves,
        Redstone,
        Water,
        Wool,
        Neutral
    }

    class MyCube
    {
        static List<int> texturesCollection;

        float[] leftBottomBack;
        float[] rightBottomBack;
        float[] leftTopBack;
        float[] rightTopBack;

        float[] leftBottomFront;
        float[] rightBottomFront;
        float[] leftTopFront;
        float[] rightTopFront;
        int textureID;
        float width;

        static MyCube()
        {
            texturesCollection = new List<int>();
            for (int i = 0; i < 7; i++)
            {
                texturesCollection.Add(ContentPipe.LoadTexture("diamond.bmp"));
                texturesCollection.Add(ContentPipe.LoadTexture("dirt.bmp"));
                texturesCollection.Add(ContentPipe.LoadTexture("gold.bmp"));
                texturesCollection.Add(ContentPipe.LoadTexture("leaves.bmp"));
                texturesCollection.Add(ContentPipe.LoadTexture("redstone.bmp"));
                texturesCollection.Add(ContentPipe.LoadTexture("water.bmp"));
                texturesCollection.Add(ContentPipe.LoadTexture("wool.bmp"));
                texturesCollection.Add(ContentPipe.LoadTexture("bedrock.bmp"));
            }
        }

        public MyCube(float size, MyCubeTexture tex)
        {
            textureID = texturesCollection[(int)tex];
            leftBottomBack = new float[] { -1.0f, -1.0f, -1.0f };
            rightBottomBack = new float[] { 1.0f, -1.0f, -1.0f };
            leftTopBack = new float[] { -1.0f, 1.0f, -1.0f };
            rightTopBack = new float[] { 1.0f, 1.0f, -1.0f };

            leftBottomFront = new float[] { -1.0f, -1.0f, 1.0f };
            rightBottomFront = new float[] { 1.0f, -1.0f, 1.0f };
            leftTopFront = new float[] { -1.0f, 1.0f, 1.0f };
            rightTopFront = new float[] { 1.0f, 1.0f, 1.0f };

            width = size / 2;
            for (int i = 0; i < 3; i++)
            {
                leftBottomBack[i] *= width;
                rightBottomBack[i] *= width;
                leftTopBack[i] *= width;
                rightTopBack[i] *= width;

                leftBottomFront[i] *= width;
                rightBottomFront[i] *= width;
                leftTopFront[i] *= width;
                rightTopFront[i] *= width;
            }

        }

        public void Render()
        {
            //GL.PushMatrix();
            GL.BindTexture(TextureTarget.Texture2D, textureID);
            GL.Begin(BeginMode.Quads);
            GL.Color3(1.0, 0.0, 0.0);

            //left
            GL.Normal3(-1.0, 0.0, 0.0);
            GL.TexCoord2(0, 0);
            GL.Vertex3(leftTopFront);
            GL.TexCoord2(1, 0);
            GL.Vertex3(leftTopBack);
            GL.TexCoord2(1, 1);
            GL.Vertex3(leftBottomBack);
            GL.TexCoord2(0, 1);
            GL.Vertex3(leftBottomFront);

            //right
            GL.Normal3(1.0, 0.0, 0.0);
            GL.TexCoord2(0, 0);
            GL.Vertex3(rightTopFront);
            GL.TexCoord2(1, 0);
            GL.Vertex3(rightTopBack);
            GL.TexCoord2(1, 1);
            GL.Vertex3(rightBottomBack);
            GL.TexCoord2(0, 1);
            GL.Vertex3(rightBottomFront);

            //bottom
            GL.Normal3(0.0, -1.0, 0.0);
            GL.TexCoord2(0, 0);
            GL.Vertex3(rightBottomFront);
            GL.TexCoord2(1, 0);
            GL.Vertex3(rightBottomBack);
            GL.TexCoord2(1, 1);
            GL.Vertex3(leftBottomBack);
            GL.TexCoord2(0, 1);
            GL.Vertex3(leftBottomFront);

            //top
            GL.Normal3(0.0, 1.0, 0.0);
            GL.TexCoord2(0, 0);
            GL.Vertex3(rightTopFront);
            GL.TexCoord2(1, 0);
            GL.Vertex3(rightTopBack);
            GL.TexCoord2(1, 1);
            GL.Vertex3(leftTopBack);
            GL.TexCoord2(0, 1);
            GL.Vertex3(leftTopFront);

            //back
            GL.Normal3(0.0, 0.0, -1.0);
            GL.TexCoord2(0, 0);
            GL.Vertex3(rightTopBack);
            GL.TexCoord2(1, 0);
            GL.Vertex3(rightBottomBack);
            GL.TexCoord2(1, 1);
            GL.Vertex3(leftBottomBack);
            GL.TexCoord2(0, 1);
            GL.Vertex3(leftTopBack);

            //front
            GL.Normal3(1.0, 0.0, 1.0);
            GL.TexCoord2(0, 0);
            GL.Vertex3(rightTopFront);
            GL.TexCoord2(1, 0);
            GL.Vertex3(rightBottomFront);
            GL.TexCoord2(1, 1);
            GL.Vertex3(leftBottomFront);
            GL.TexCoord2(0, 1);
            GL.Vertex3(leftTopFront);

            GL.End();
            //GL.PopMatrix();
        }

        public MyCubeTexture Texture { get { return (MyCubeTexture)(textureID - 1); } set { textureID = texturesCollection[(int)value]; } }
    }
}
