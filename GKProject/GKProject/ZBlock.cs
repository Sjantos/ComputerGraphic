using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GKProject
{
    class ZBlock : BaseBlock
    {
        Position actualPosition;

        public ZBlock(int startWidthIndex, ref GameBoardCell[,] gameBoard, int bW, MyCubeTexture texture) : base(startWidthIndex - 2, ref gameBoard, bW, texture)
        {
            actualPosition = Position.Horizontal;
            usedCells.Add(0, new Tuple<int, int>(startWidthIndex - 1, boardHeight - 1));//Block indexes layout
            usedCells.Add(1, new Tuple<int, int>(startWidthIndex    , boardHeight - 1));//       0 1 
            usedCells.Add(2, new Tuple<int, int>(startWidthIndex    , boardHeight - 2));//         2 3
            usedCells.Add(3, new Tuple<int, int>(startWidthIndex + 1, boardHeight - 2));

            TakeCells();
        }

        private enum Position
        {
            Horizontal,     //   Horizontal    Vertical
            Vertical        //                     0
                            //      0 1          1 2
                            //        2 3        3
        }

        public override void Rotate()
        {
            int i = -1, j = -1;
            switch (actualPosition)
            {
                case Position.Horizontal:         // GOTO -> Position.Vertical
                    i = usedCells[1].Item1;
                    j = usedCells[1].Item2;
                    if (i + 1 == boardWidth - 1)
                        i--;

                    if (j - 1 >= 0)
                    {
                        if ((gameBoard[i + 1, j + 1].CellStatus != CellStatus.Filled)
                             && (gameBoard[i + 1, j].CellStatus != CellStatus.Filled)
                             && (gameBoard[i, j - 1].CellStatus != CellStatus.Filled))
                        {
                            FreeCells();
                            usedCells[0] = new Tuple<int, int>(i + 1, j + 1);
                            usedCells[1] = new Tuple<int, int>(i, j);
                            usedCells[2] = new Tuple<int, int>(i + 1, j);
                            usedCells[3] = new Tuple<int, int>(i, j - 1);
                            actualPosition = Position.Vertical;
                            TakeCells();
                        }
                    }
                    break;
                case Position.Vertical:           // GOTO -> Position.Horizontal
                    i = usedCells[1].Item1;
                    j = usedCells[1].Item2;
                    if (i - 1 < 1)
                        i++;
                    else if (i + 1 == boardWidth - 1)
                        i--;

                    if (j - 1 >= 0)
                    {
                        if ((gameBoard[i - 1, j].CellStatus != CellStatus.Filled)
                             && (gameBoard[i, j - 1].CellStatus != CellStatus.Filled)
                             && (gameBoard[i + 1, j - 1].CellStatus != CellStatus.Filled))
                        {
                            FreeCells();
                            usedCells[0] = new Tuple<int, int>(i - 1, j);
                            usedCells[1] = new Tuple<int, int>(i, j);
                            usedCells[2] = new Tuple<int, int>(i, j - 1);
                            usedCells[3] = new Tuple<int, int>(i + 1, j - 1);
                            actualPosition = Position.Horizontal;
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
