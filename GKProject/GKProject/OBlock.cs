using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections;
using OpenTK;
using OpenTK.Graphics.OpenGL;

namespace GKProject
{
    class OBlock : BaseBlock
    {
        /// <summary>
        /// creates water O-block at top of gameboard
        /// </summary>
        /// <param name="startWidthIndex">index for left top corner in game board</param>
        /// <param name="bW">game board width</param>
        /// <param name="bH">game board height</param>
        public OBlock(int startWidthIndex, ref GameBoardCell[,] gameBoard, int bW, MyCubeTexture texture) : base(startWidthIndex, ref gameBoard, bW, texture)
        {
            base.usedCells.Add(0, new Tuple<int, int>(startWidthIndex    , boardHeight - 1));//Block indexes layout
            base.usedCells.Add(1, new Tuple<int, int>(startWidthIndex + 1, boardHeight - 1));//        0 1
            base.usedCells.Add(2, new Tuple<int, int>(startWidthIndex    , boardHeight - 2));//        2 3
            base.usedCells.Add(3, new Tuple<int, int>(startWidthIndex + 1, boardHeight - 2));

            TakeCells();
        }

        public override void Rotate()
        {

        }
    }
}
