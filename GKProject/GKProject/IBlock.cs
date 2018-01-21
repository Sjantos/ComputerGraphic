using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GKProject
{
    class IBlock : BaseBlock
    {
        Position actualPosition;

        public IBlock(int startWidthIndex, ref GameBoardCell[,] gameBoard, int bW, MyCubeTexture texture) : base(startWidthIndex - 2, ref gameBoard, bW, texture)
        {
            actualPosition = Position.Horizontal;
            usedCells.Add(0, new Tuple<int, int>(startWidthIndex - 1, boardHeight - 1));//Block indexes layout
            usedCells.Add(1, new Tuple<int, int>(startWidthIndex + 0, boardHeight - 1));//        0 1 2 3
            usedCells.Add(2, new Tuple<int, int>(startWidthIndex + 1, boardHeight - 1));//        
            usedCells.Add(3, new Tuple<int, int>(startWidthIndex + 2, boardHeight - 1));

            TakeCells();
        }

        private enum Position
        {
            Vertical,
            Horizontal
        }

        public override void Rotate()
        {
            
            int i = -1, j = -1;
            switch (actualPosition)
            {
                case Position.Vertical:
                    i = usedCells[2].Item1;
                    j = usedCells[2].Item2;
                    while (i - 2 < 1)
                        i++;
                    while (i + 1 == boardWidth - 1)
                        i--;
                        
                    if((gameBoard[i - 2, j].CellStatus != CellStatus.Filled)
                        && (gameBoard[i - 1, j].CellStatus != CellStatus.Filled)
                        && (gameBoard[i + 1, j].CellStatus != CellStatus.Filled))
                    {
                        FreeCells();
                        usedCells[0] = new Tuple<int, int>(i - 2, j);
                        usedCells[1] = new Tuple<int, int>(i - 1, j);
                        usedCells[2] = new Tuple<int, int>(i, j);
                        usedCells[3] = new Tuple<int, int>(i + 1, j);
                        actualPosition = Position.Horizontal;
                        TakeCells();
                    }

                    
                    break;
                case Position.Horizontal:
                    i = usedCells[2].Item1;
                    j = usedCells[2].Item2;

                    if(j - 1 >= 1)
                    {
                        if((gameBoard[i, j + 2].CellStatus != CellStatus.Filled)
                            && (gameBoard[i, j + 1].CellStatus != CellStatus.Filled)
                                && (gameBoard[i, j - 1].CellStatus != CellStatus.Filled))
                        {
                            FreeCells();
                            usedCells[0] = new Tuple<int, int>(i, j + 2);
                            usedCells[1] = new Tuple<int, int>(i, j + 1);
                            usedCells[2] = new Tuple<int, int>(i, j);
                            usedCells[3] = new Tuple<int, int>(i, j - 1);
                            actualPosition = Position.Vertical;
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
