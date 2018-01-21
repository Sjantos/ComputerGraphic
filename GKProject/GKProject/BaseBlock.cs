using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenTK;
using OpenTK.Graphics.OpenGL;

namespace GKProject
{
    public enum MoveDirection
    {
        Left,
        Right,
        Down
    }

    abstract class BaseBlock
    {
        protected MyCubeTexture actualTexture;
        protected Dictionary<int, Tuple<int, int>> usedCells;
        protected int boardWidth;
        protected int boardHeight;
        protected GameBoardCell[,] gameBoard;

        protected BaseBlock(int startWidthIndex, ref GameBoardCell[,] gameBoard, int bW, MyCubeTexture texture)
        {
            actualTexture = texture;
            this.gameBoard = gameBoard;
            boardWidth = bW;
            boardHeight = 2 * bW-1;
            usedCells = new Dictionary<int, Tuple<int, int>>();
        }

        public abstract void Rotate();

        //------------------------------------------------------------------------

        public bool Move(MoveDirection direction)
        {
            int i = -1, j = -1;
            switch (direction)
            {
                case MoveDirection.Left:
                    bool canGoLeft = true;
                    for (int k = 0; k < 4; k++)
                    {
                        i = usedCells[k].Item1;
                        j = usedCells[k].Item2;
                        if ((i - 1) >= 0)
                        {
                            if ((gameBoard[i - 1, j].CellStatus == CellStatus.Filled))
                                canGoLeft = false;
                        }
                        else
                            canGoLeft = false;
                    }

                    if (canGoLeft)
                    {
                        FreeCells();

                        //Move block left
                        for (int k = 0; k < 4; k++)
                        {
                            i = usedCells[k].Item1;
                            j = usedCells[k].Item2;
                            usedCells[k] = new Tuple<int, int>(i - 1, j);
                        }

                        TakeCells();
                    }
                    return true;
                    break;
                case MoveDirection.Right:
                    bool canGoRight = true;
                    for (int k = 0; k < 4; k++)
                    {
                        i = usedCells[k].Item1;
                        j = usedCells[k].Item2;
                        if ((i + 1) < boardWidth)
                        {
                            if ((gameBoard[i + 1, j].CellStatus == CellStatus.Filled))
                                canGoRight = false;
                        }
                        else
                            canGoRight = false;
                    }

                    if (canGoRight)
                    {
                        FreeCells();

                        //Move block right
                        for (int k = 0; k < 4; k++)
                        {
                            i = usedCells[k].Item1;
                            j = usedCells[k].Item2;
                            usedCells[k] = new Tuple<int, int>(i + 1, j);
                        }

                        TakeCells();
                    }
                    return true;
                    break;
                case MoveDirection.Down:
                    bool canGoDown = true;
                    for (int k = 0; k < 4; k++)
                    {
                        i = usedCells[k].Item1;
                        j = usedCells[k].Item2;
                        if ((j - 1) >= 0)//boardHeight)
                        {
                            if ((gameBoard[i, j - 1].CellStatus == CellStatus.Filled))
                                canGoDown = false;
                        }
                        else
                            canGoDown = false;
                    }

                    if (canGoDown)
                    {
                        FreeCells();

                        //Move block down
                        for (int k = 0; k < 4; k++)
                        {
                            i = usedCells[k].Item1;
                            j = usedCells[k].Item2;
                            usedCells[k] = new Tuple<int, int>(i, j - 1);
                        }

                        TakeCells();
                    }
                    else
                    {
                        return false;
                    }
                    return true;
                    break;
                default:
                    return true;
                    break;
            }
        }

        public Dictionary<int, Tuple<int, int>> UsedCells()
        {
            return usedCells;
        }

        public void Render()
        {
            float cubeSize = 1.05f;
            float widthHalf = (boardWidth / 2) * cubeSize;
            float heightHalf = (boardHeight / 2) * cubeSize;

            for (int k = 0; k < 4; k++)
            {
                int i = usedCells[k].Item1, j = usedCells[k].Item2;
                GL.Translate(i * cubeSize - widthHalf, j * cubeSize - heightHalf, 0);
                new MyCube(1.0f, MyCubeTexture.Water);
                GL.Translate(-i * cubeSize + widthHalf, -j * cubeSize + heightHalf, 0);
            }
        }

        public void FreeCells()
        {
            int i = -1, j = -1;
            for (int k = 0; k < 4; k++)
            {
                i = usedCells[k].Item1;
                j = usedCells[k].Item2;
                //gameBoard[i, j].CellCube.Texture = MyCubeTexture.Amethyst;
                gameBoard[i, j].CellStatus = CellStatus.Free;
            }
        }

        public void TakeCells()
        {
            int i = -1, j = -1;
            for (int k = 0; k < 4; k++)
            {
                i = usedCells[k].Item1;
                j = usedCells[k].Item2;
                gameBoard[i, j].CellCube.Texture = actualTexture;
                gameBoard[i, j].CellStatus = CellStatus.TemporaryFilled;
            }
        }

        public bool CheckForStartCollision()
        {
            int i = -1, j = -1;
            bool canGoDown = true;
            for (int k = 0; k < 4; k++)
            {
                i = usedCells[k].Item1;
                j = usedCells[k].Item2;
                if ((j - 1) >= 0)//boardHeight)
                {
                    if ((gameBoard[i, j - 1].CellStatus == CellStatus.Filled))
                        canGoDown = false;
                }
                else
                    canGoDown = false;
            }
            return canGoDown;
        }
    }
}
