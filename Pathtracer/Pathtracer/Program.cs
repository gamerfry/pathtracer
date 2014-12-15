using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System

namespace Pathtracer
{
    class Program
    {
         class Vec {       
            double x, y, z;            
            Vec(double x_=0, double y_=0, double z_=0){ x=x_; y=y_; z=z_; } 
            Vec operator+(Vec b) { return new Vec(x+b.x,y+b.y,z+b.z); } 
            Vec operator-(Vec b) { return new Vec(x-b.x,y-b.y,z-b.z); } 
            Vec operator*(Vec a, double b) { return new Vec(x*b,y*b,z*b); } 
            Vec mult(Vec b) { return new Vec(x*b.x,y*b.y,z*b.z); } 
            Vec norm(){ return this * (1/Math.Sqrt(x*x+y*y+z*z)); } 
            double dot(Vec b) { return x*b.x+y*b.y+z*b.z; } // cross: 
            Vec operator%(Vec a, Vec b){return new Vec(y*b.z-z*b.y,z*b.x-x*b.z,x*b.y-y*b.x);} 
            }; 

        static void Main(string[] args)
        {
        }
    }
}
