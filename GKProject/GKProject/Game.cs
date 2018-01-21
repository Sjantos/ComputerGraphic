using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenTK;
using OpenTK.Graphics.OpenGL;

namespace GKProject
{
    class Game : GameWindow
    {
        View view;
        GameBoard gameBoard;
        int gameBoardSize;
        Vector3 modelPosition;

        int frameCounter;
        bool newBlockNeeded;
        int level;

        float near;
        public float Near { get { return near; } set { near = value; } }
        float far;
        public float Far { get { return far; } set { far = value; } }

        public Game(int width, int height) : base(width, height)
        {
            gameBoardSize = 10;
            level = 0;
            frameCounter = 0;
            newBlockNeeded = true;
            near = 1.0f;
            far = 100.0f;
            gameBoard = new GameBoard(gameBoardSize);

            modelPosition = new Vector3(0.0f, 1.0f, 0.0f);

            GL.Enable(EnableCap.Texture2D);
            Input.Initialize(this);
        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);

            GL.ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            GL.Enable(EnableCap.DepthTest);

            //Light
            GL.Enable(EnableCap.Lighting);

            float[] light_position = new float[] { 0.0f, 0.0f, 1.5f };
            GL.Light(LightName.Light0, LightParameter.Position, light_position);
            GL.Enable(EnableCap.Light0);
        }

        protected override void OnUpdateFrame(FrameEventArgs e)
        {
            base.OnUpdateFrame(e);
            frameCounter++;

            if(newBlockNeeded)
            {
                gameBoard.MakeNewBlock();
                newBlockNeeded = false;
                gameBoard.ActualBlock.Move(MoveDirection.Down);
            }

            if (Input.KeyPress(OpenTK.Input.Key.Left))
                gameBoard.ActualBlock.Move(MoveDirection.Left);
            if (Input.KeyPress(OpenTK.Input.Key.Right))
                gameBoard.ActualBlock.Move(MoveDirection.Right);
            if (Input.KeyPress(OpenTK.Input.Key.Up))
                gameBoard.ActualBlock.Rotate();

            if (Input.KeyDown(OpenTK.Input.Key.Down))
                gameBoard.ActualBlock.Move(MoveDirection.Down);

            if (frameCounter >= 60)
            {
                newBlockNeeded = !gameBoard.ActualBlock.Move(MoveDirection.Down);
                frameCounter = level;
            }

            if (newBlockNeeded)
            {
                gameBoard.LockBlock();
                if(gameBoard.DeleteFullLine())
                {
                    if (level < 45)
                        level++;
                    gameBoard.ActualBlock.Move(MoveDirection.Down);
                }
                    
            }

            Input.Update();
        }

        protected override void OnRenderFrame(FrameEventArgs e)
        {
            base.OnRenderFrame(e);

            GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);

            Matrix4 perspective = Matrix4.CreatePerspectiveFieldOfView(0.7f, this.Width / this.Height, near, far);
            
            Matrix4 lookAt = Matrix4.LookAt(new Vector3(0, 2*gameBoardSize, 3*gameBoardSize), modelPosition, new Vector3(0, 1, 0));
            GL.MatrixMode(MatrixMode.Projection);
            GL.LoadIdentity();
            GL.LoadMatrix(ref perspective);

            
            GL.MatrixMode(MatrixMode.Modelview);
            GL.LoadIdentity();
            GL.LoadMatrix(ref lookAt);

            GL.Viewport(0, 0, this.Width, this.Height);

            gameBoard.Render();

            this.SwapBuffers();
        }
    }
}
