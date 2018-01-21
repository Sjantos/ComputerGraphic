using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GKProject
{
    class TBlock : BaseBlock
    {
        Position actualPosition;

        public TBlock(int startWidthIndex, ref GameBoardCell[,] gameBoard, int bW, MyCubeTexture texture) : base(startWidthIndex - 2, ref gameBoard, bW, texture)
        {
            actualPosition = Position.Bottom;
            usedCells.Add(0, new Tuple<int, int>(startWidthIndex - 1, boardHeight - 2));//Block indexes layout
            usedCells.Add(1, new Tuple<int, int>(startWidthIndex    , boardHeight - 1));//         1
            usedCells.Add(2, new Tuple<int, int>(startWidthIndex    , boardHeight - 2));//       0 2 3
            usedCells.Add(3, new Tuple<int, int>(startWidthIndex + 1, boardHeight - 2));

            TakeCells();
        }

        private enum Position
        {
            Bottom,     //      Bottom      Right        Top       Left     Order must be kept
            Left,       //                    0                     0
            Right,      //        1           1 2       0 1 3     2 1
            Top         //      0 2 3         3           2         3
        }

        public override void Rotate()
        {
            int i = -1, j = -1;
            switch (actualPosition)
            {
                case Position.Bottom:           // GOTO -> Position.Right
                    i = usedCells[1].Item1;
                    j = usedCells[1].Item2;

                    if(     (i + 1 < boardWidth)
                         && (j - 1 >= 0))
                    {
                        if (    (gameBoard[i, j + 1].CellStatus != CellStatus.Filled)
                             && (gameBoard[i + 1, j].CellStatus != CellStatus.Filled)
                             && (gameBoard[i, j - 1].CellStatus != CellStatus.Filled))
                        {
                            FreeCells();
                            usedCells[0] = new Tuple<int, int>(i, j + 1);
                            usedCells[1] = new Tuple<int, int>(i, j);
                            usedCells[2] = new Tuple<int, int>(i + 1, j);
                            usedCells[3] = new Tuple<int, int>(i, j - 1);
                            actualPosition = Position.Right;
                            TakeCells();
                        }
                    }
                    break;
                case Position.Left:           // GOTO -> Position.Bottom
                    i = usedCells[1].Item1;
                    j = usedCells[1].Item2;
                    if (i - 1 < 1)
                        i++;
                    else if (i + 1 == boardWidth - 1)
                        i--;

                    if (j - 1 >= 0)
                    {
                        if (    (gameBoard[i - 1, j - 1].CellStatus != CellStatus.Filled)
                             && (gameBoard[i, j - 1].CellStatus != CellStatus.Filled)
                             && (gameBoard[i + 1, j - 1].CellStatus != CellStatus.Filled))
                        {
                            FreeCells();
                            usedCells[0] = new Tuple<int, int>(i - 1, j - 1);
                            usedCells[1] = new Tuple<int, int>(i, j);
                            usedCells[2] = new Tuple<int, int>(i, j - 1);
                            usedCells[3] = new Tuple<int, int>(i + 1, j - 1);
                            actualPosition = Position.Bottom;
                            TakeCells();
                        }
                    }
                    break;
                case Position.Right:           // GOTO -> Position.Top
                    i = usedCells[1].Item1;
                    j = usedCells[1].Item2;
                    if (i - 1 < 1)
                        i++;
                    else if (i + 1 == boardWidth - 1)
                        i--;

                    if (j - 1 >= 0)
                    {
                        if (    (gameBoard[i - 1, j].CellStatus != CellStatus.Filled)
                             && (gameBoard[i, j - 1].CellStatus != CellStatus.Filled)
                             && (gameBoard[i + 1, j].CellStatus != CellStatus.Filled))
                        {
                            FreeCells();
                            usedCells[0] = new Tuple<int, int>(i - 1, j);
                            usedCells[1] = new Tuple<int, int>(i, j);
                            usedCells[2] = new Tuple<int, int>(i, j - 1);
                            usedCells[3] = new Tuple<int, int>(i + 1, j);
                            actualPosition = Position.Top;
                            TakeCells();
                        }
                    }
                    break;
                case Position.Top:           // GOTO -> Position.Left
                    i = usedCells[1].Item1;
                    j = usedCells[1].Item2;

                    if(     (i - 1 >= 0)
                         && (j - 1 >= 0))
                    {
                        if (    (gameBoard[i, j + 1].CellStatus != CellStatus.Filled)
                             && (gameBoard[i - 1, j].CellStatus != CellStatus.Filled)
                             && (gameBoard[i, j - 1].CellStatus != CellStatus.Filled))
                        {
                            FreeCells();
                            usedCells[0] = new Tuple<int, int>(i, j + 1);
                            usedCells[1] = new Tuple<int, int>(i, j);
                            usedCells[2] = new Tuple<int, int>(i - 1, j);
                            usedCells[3] = new Tuple<int, int>(i, j - 1);
                            actualPosition = Position.Left;
                            TakeCells();
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
}
