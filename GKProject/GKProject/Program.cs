using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenTK;

namespace GKProject
{
    class Program
    {
        static void Main(string[] args)
        {
            //MyGameWindow game = new MyGameWindow(new GameWindow());
            Game game = new Game(800, 800);
            game.Run(1.0 / 60.0);
        }
    }
}
