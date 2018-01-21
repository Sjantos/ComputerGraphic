using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenTK;
using OpenTK.Graphics.OpenGL;

namespace GKProject
{
    class GameBoard
    {
        GameBoardCell[,] board;
        int boardWidth;
        int boardHeight;
        int boardAbsoluteWidth;
        int boardAbsoluteHeight;

        float cubeSize;

        BaseBlock actualBlock;
        Random random;

        public GameBoard(int width = 10)
        {
            random = new Random();
            cubeSize = 1.1f;
            boardWidth = width;
            boardHeight = 2 * width;
            boardAbsoluteWidth = boardWidth + 2;
            boardAbsoluteHeight = boardHeight + 5;
            board = new GameBoardCell[boardAbsoluteWidth, boardAbsoluteHeight];

            for (int i = 0; i < boardAbsoluteWidth; i++)
                for (int j = 0; j < boardAbsoluteHeight; j++)
                    board[i, j] = new GameBoardCell();

            for (int j = 0; j < boardAbsoluteHeight; j++)
            {
                board[0, j].CellStatus = CellStatus.Filled;
                board[boardAbsoluteWidth - 1, j].CellStatus = CellStatus.Filled;
            }
            for (int i = 0; i < boardAbsoluteWidth - 1; i++)
            {
                board[i, 0].CellStatus = CellStatus.Filled;
            }
        }

        public void Render()
        {
            float widthHalf = (boardAbsoluteWidth / 2) * cubeSize;
            float heightHalf = (boardHeight / 2) * cubeSize;

            for (int i = 0; i < boardAbsoluteWidth; i++)
            {
                for (int j = 0; j < boardHeight + 2; j++)
                {
                    if (board[i, j].CellStatus == CellStatus.Filled || board[i, j].CellStatus == CellStatus.TemporaryFilled)
                    {
                        GL.Translate(i * cubeSize - widthHalf, j * cubeSize - heightHalf, 0);
                        board[i, j].CellCube.Render();
                        GL.Translate(-i * cubeSize + widthHalf, -j * cubeSize + heightHalf, 0); 
                    }
                }
            }
        }

        public void MakeNewBlock()
        {
            int randomBlock = random.Next(7);
            switch (randomBlock)
            {
                case 0:
                    actualBlock = new IBlock(boardAbsoluteWidth / 2, ref board, boardAbsoluteWidth, MyCubeTexture.Diamond);
                    break;
                case 1:
                    actualBlock = new JBlock(boardAbsoluteWidth / 2, ref board, boardAbsoluteWidth, MyCubeTexture.Dirt);
                    break;
                case 2:
                    actualBlock = new LBlock(boardAbsoluteWidth / 2, ref board, boardAbsoluteWidth, MyCubeTexture.Wool);
                    break;
                case 3:
                    actualBlock = new OBlock(boardAbsoluteWidth / 2, ref board, boardAbsoluteWidth, MyCubeTexture.Leaves);
                    break;
                case 4:
                    actualBlock = new SBlock(boardAbsoluteWidth / 2, ref board, boardAbsoluteWidth, MyCubeTexture.Redstone);
                    break;
                case 5:
                    actualBlock = new TBlock(boardAbsoluteWidth / 2, ref board, boardAbsoluteWidth, MyCubeTexture.Water);
                    break;
                default:
                    actualBlock = new ZBlock(boardAbsoluteWidth / 2, ref board, boardAbsoluteWidth, MyCubeTexture.Gold);
                    break;
            }

            if (!actualBlock.CheckForStartCollision())
                LockBlock();
        }

        public void LockBlock()
        {
            Dictionary<int, Tuple<int, int>> cells = actualBlock.UsedCells();
            int i = -1, j = -1;
            for (int k = 0; k < 4; k++)
            {
                i = cells[k].Item1;
                j = cells[k].Item2;
                board[i, j].CellStatus = CellStatus.Filled;
            }
        }

        public bool DeleteFullLine()
        {
            bool lineDeleted = false;
            for (int j = 1; j < boardHeight + 1; j++)
            {
                bool rowFilled = true;
                for (int i = 1; i < boardAbsoluteWidth - 1; i++)
                {
                    if(board[i, j].CellStatus == CellStatus.Free)
                    {
                        rowFilled = false;
                        break;
                    }
                }
                if(rowFilled)
                {
                    lineDeleted = true;
                    int k = j;
                    for (k = j; k < boardHeight - 1; k++)
                    {
                        for (int l = 1; l < boardAbsoluteWidth - 1; l++)
                        {
                            board[l, k] = board[l, k + 1];
                        }
                    }
                    for (int l = 1; l < boardAbsoluteWidth - 1; l++)
                    {
                        board[l, k] = new GameBoardCell();
                    }
                    j--;
                }
            }
            return lineDeleted;
        }

        public BaseBlock ActualBlock { get { return actualBlock; } }
    }
}
