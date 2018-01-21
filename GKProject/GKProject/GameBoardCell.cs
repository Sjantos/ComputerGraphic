using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GKProject
{
    public enum CellStatus
    {
        Free,
        TemporaryFilled,
        Filled
    }

    class GameBoardCell
    {
        CellStatus cellStatus;
        MyCube cellCube;

        public GameBoardCell(float cellSize = 1.0f)
        {
            cellStatus = CellStatus.Free;
            cellCube = new MyCube(cellSize, MyCubeTexture.Neutral);
        }

        public CellStatus CellStatus { get { return cellStatus; } set { cellStatus = value; } }
        public MyCube CellCube { get { return cellCube; } set { cellCube = value; } }
    }
}
