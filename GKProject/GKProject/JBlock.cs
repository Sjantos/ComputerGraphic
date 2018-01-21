using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GKProject
{
    class JBlock : BaseBlock
    {
        Position actualPosition;

        public JBlock(int startWidthIndex, ref GameBoardCell[,] gameBoard, int bW, MyCubeTexture texture) : base(startWidthIndex - 2, ref gameBoard, bW, texture)
        {
            actualPosition = Position.Top;
            usedCells.Add(0, new Tuple<int, int>(startWidthIndex    , boardHeight - 1   ));//Block indexes layout
            usedCells.Add(1, new Tuple<int, int>(startWidthIndex    , boardHeight - 2   ));//       0
            usedCells.Add(2, new Tuple<int, int>(startWidthIndex + 1, boardHeight - 2   ));//       1 2 3
            usedCells.Add(3, new Tuple<int, int>(startWidthIndex + 2, boardHeight - 2   ));

            TakeCells();
        }

        private enum Position
        {
            Top,                //      Top     Right       Bottom      Left        Order must be kept
            Right,              //     0         0 1        0 1 2          1
            Bottom,             //     1 2 3     2              3          2
            Left                //               3                       0 3
        }

        public override void Rotate()
        {
            int i = -1, j = -1;
            switch (actualPosition)
            {
                case Position.Top:                  // GOTO -> Position.Right
                    #region case Top
                    i = usedCells[2].Item1;
                    j = usedCells[2].Item2;
                    if (i + 1 == boardWidth - 1)
                        i--;

                    if(j - 1 >= 0)
                    {
                        if((gameBoard[i, j + 1].CellStatus != CellStatus.Filled)
                            && (gameBoard[i + 1, j + 1].CellStatus != CellStatus.Filled)
                                && (gameBoard[i, j - 1].CellStatus != CellStatus.Filled))
                        {
                            FreeCells();
                            usedCells[0] = new Tuple<int, int>(i, j + 1);
                            usedCells[1] = new Tuple<int, int>(i + 1, j + 1);
                            usedCells[2] = new Tuple<int, int>(i, j);
                            usedCells[3] = new Tuple<int, int>(i, j - 1);
                            TakeCells();
                            actualPosition = Position.Right;
                        }
                    }
                    break;
                #endregion
                case Position.Right:                // GOTO -> Position.Bottom
                    #region case Right
                    i = usedCells[2].Item1;
                    j = usedCells[2].Item2;
                    if (i - 1 < 1)
                        i++;
                    else if (i + 1 == boardWidth - 1)
                        i--;

                    if(j - 1 >= 0)
                    {
                        if((gameBoard[i - 1, j].CellStatus != CellStatus.Filled)
                            && (gameBoard[i + 1, j].CellStatus != CellStatus.Filled)
                            && (gameBoard[i + 1, j - 1].CellStatus != CellStatus.Filled))
                        {
                            FreeCells();
                            usedCells[0] = new Tuple<int, int>(i - 1, j);
                            usedCells[1] = new Tuple<int, int>(i, j);
                            usedCells[2] = new Tuple<int, int>(i + 1, j);
                            usedCells[3] = new Tuple<int, int>(i + 1, j - 1);
                            TakeCells();
                            actualPosition = Position.Bottom;
                        }
                    }
                    break;
                #endregion
                case Position.Bottom:               // GOTO -> Position.Left
                    #region case Bottom
                    i = usedCells[1].Item1;
                    j = usedCells[1].Item2;
                    if (i - 1 < 1)
                        i++;
                    else if (i + 1 == boardWidth - 1)
                        i--;

                    if(j - 1 >= 0)
                    {
                        if((gameBoard[i - 1, j - 1].CellStatus != CellStatus.Filled)
                            && (gameBoard[i - 1, j].CellStatus != CellStatus.Filled)
                            && (gameBoard[i + 1, j].CellStatus != CellStatus.Filled))
                        {
                            FreeCells();
                            usedCells[0] = new Tuple<int, int>(i - 1, j - 1);
                            usedCells[1] = new Tuple<int, int>(i, j + 1);
                            usedCells[2] = new Tuple<int, int>(i, j);
                            usedCells[3] = new Tuple<int, int>(i, j - 1);
                            TakeCells();
                            actualPosition = Position.Left;
                        }
                    }
                    break;
                #endregion
                case Position.Left:                 // GOTO -> Position.Top
                    #region case Left
                    i = usedCells[2].Item1;
                    j = usedCells[2].Item2;
                    if (i - 1 < 1)
                        i++;
                    else if (i + 1 == boardWidth - 1)
                        i--;

                    if ((gameBoard[i -1, j + 1].CellStatus != CellStatus.Filled)
                        && (gameBoard[i - 1, j].CellStatus != CellStatus.Filled)
                        && (gameBoard[i + 1, j].CellStatus != CellStatus.Filled))
                    {
                        FreeCells();
                        usedCells[0] = new Tuple<int, int>(i - 1, j + 1);
                        usedCells[1] = new Tuple<int, int>(i - 1, j);
                        usedCells[2] = new Tuple<int, int>(i, j);
                        usedCells[3] = new Tuple<int, int>(i + 1, j);
                        TakeCells();
                        actualPosition = Position.Top;
                    }
                    break;
                #endregion
                default:
                    break;
            }
        }
    }
}
