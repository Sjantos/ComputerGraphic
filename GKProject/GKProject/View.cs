using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenTK;
using OpenTK.Graphics.OpenGL;
using System.Drawing;

namespace GKProject
{
    class View
    {
        public Vector3 position;
        /// <summary>
        /// in radians, + = clockwise
        /// </summary>
        public Vector3 rotation;
        /// <summary>
        /// 1.0 = no zoom
        /// </summary>
        public double zoom;

        public View(Vector3 startPosition, Vector3 startRotation, double startZoom = 1.0)
        { 
            position = startPosition;
            zoom = startZoom;
            rotation = startRotation;
        }

        public void Update()
        {

        }

        public void ApplyTransform()
        {
            Matrix4 transform = Matrix4.Identity;

            transform = Matrix4.Mult(transform, Matrix4.CreateTranslation(-position.X, -position.Y, -position.Z));
            transform = Matrix4.Mult(transform, Matrix4.CreateRotationX(-(float)rotation.X));
            transform = Matrix4.Mult(transform, Matrix4.CreateRotationY(-(float)rotation.Y));
            transform = Matrix4.Mult(transform, Matrix4.CreateRotationZ(-(float)rotation.Z));
            transform = Matrix4.Mult(transform, Matrix4.CreateScale((float)zoom));

            GL.MultMatrix(ref transform);
        }
    }
}
