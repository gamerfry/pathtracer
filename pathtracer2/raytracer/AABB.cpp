#include "AABB.h"
	AABB::AABB(std::vector<double> min,std::vector<double> max)
	{


            
			int i;
            int dimensions=3;
            this->min = min;
            this->max = max;
			if(min.size > 1) {
				if(min.size != max.size) {
				}
				dimensions = min.size;
			}

			this->_numDimensions = dimensions;

			if(this->min.size != 0) {
				for (i = 0; i < this->_numDimensions; i++) {
					this->min.push_back(0);
				}
			}
			if(this->max.size.Equals(0)) {
				for (i = 0; i < this->_numDimensions; i++) {
					this->max.push_back(this->min[i]);
				}
			}
		}

			int num_dimesnions = 3;

			// returns the length of this in a specific dimension
			public double getLength( int axis ){
				if(axis >= this->_numDimensions) return 0;
				return this->max[axis] - this->min[axis];
			}

			// returns an array of the same dimensionality as this with
			// each entry having the length of this in that dimension
			public List<double> getLengths(){
		        List<double> l = new List<double>;
                int i;
				for(i = 0; i < this->_numDimensions; i++) {
					l.push_back(this->getLength( i ));
				}
				return l;
			}

			// expand this to contain otherAABB
			public void expandByAABB(AABB otherAABB ) {
				int i;
				for (i = 0; i < this->_numDimensions; i++) {
					this->min[i] = Math.Min( this->min[i], otherAABB.min[i] - EPSILON );
					this->max[i] = Math.Max( this->max[i], otherAABB.max[i] + EPSILON );
				}
			}

			// expand this to contain a set of elements
			// (each elements must implement an aabb() function)
			public void expandToContainElements( Element[] elements, int startAt ) {
				var i = elements.Length;

				if (i < 1) return;

				while(i-->startAt) {
					this->expandByAABB( elements[i].aabb() );
				}
			}

			// helper function to make a minimal bounding volume from an array
			// of elements (each elements must implement an aabb() function)
			public AABB makeToContainElements(Element[] elements ) {
				AABB aabb =  new AABB(null,null);

				if(elements.Length > 0)
					aabb = elements[0].aabb().clone();
				else
					return null;

				aabb.expandToContainElements( elements, 1 );
				return aabb;
			}

			// returns true if this overlaps or touches otherAABB
			public bool overlaps( AABB otherAABB ) {
				int i,
				    D = 0;

				for(i = 0; i < this->_numDimensions; i++) {
					if(this->min[i] <= otherAABB.max[i] && this->max[i] >= otherAABB.min[i]) D++;
				}
				if(D  == this->_numDimensions) return true;
				return false;
			}

			// returns true if this completely contains otherAABB
			public bool contains( AABB otherAABB ) {
				int i,
				    D = 0;

				for(i = 0; i < this->_numDimensions; i++) {
					if(this->min[i] <=  otherAABB.min[i] && this->max[i] >= otherAABB.max[i]) D++;
				}
				if(D  == this->_numDimensions) return true;
				return false;
			}

			// returns true if this is completely contained by otherAABB
			public bool contained(AABB otherAABB ) {
				int i,
				    D = 0;

				for(i = 0; i < this->_numDimensions; i++) {
					if(otherAABB.min[i] <=  this->min[i] && otherAABB.max[i] >= this->max[i]) D++;
				}
				if(D  == this->_numDimensions) return true;
				return false;
			}

			// Dimension agnogstic surface area (ie. perimeter for a 2D AABB)
			public double getSurfaceArea() {
				double area = 0;
					double facearea = 1;
					int j = this->_numDimensions - 1;
					int k = 0;
					List<double> lengths = this->getLengths();
					int i;

				for(i = 0; i < this->_numDimensions; i++) {
					for(k = i; k < i + j; k++) {
						facearea *= lengths[k % this->_numDimensions];
					}
					area += facearea;
					facearea = 1;
				}
				return 2 * area;
			}

			// Dimension agnogstic volume (ie. area for a 2D AABB)
			public double getVolume() {
				double volume = 1;
			    List<double> lengths = this->getLengths();
				int i;

				for(i = 0; i < this->_numDimensions; i++) {
					volume *= lengths[i];
				}
				return volume;
			}

			public AABB clone() {
				return new AABB(
					this->min,
					this->max);
			}

			// returns the line segment within this defined by a ray or false if
			// the ray lies completely outside of this
			public Tuple<double,double>[] intersectWithRay(Ray ray ) {
				int i,
				    j;
                j=0;
				double[][] parameters = new double[][]{};
				int[] inv_direction = new int[] {};
				int[] sign = new int[]{};
				double  omin,
				    omax,
				    tmin,
				    tmax;
				Tuple<double,double>[] rs;
                
				// Initialize values
				for (i = 0; i < this->_numDimensions; i++) {
					parameters[0][i] = this->min[i];
					parameters[1][i] = this->max[i];
					j = 1 / ray[i].b;
					inv_direction[i] = j;
					sign[i] = (j <= 0) ? 1 : 0;
				}
                omin = 0;
                omax = 0;
                tmin = 0;
                tmax = 0;
				omin = (parameters[sign[0]][0] - ray[0].a) * inv_direction[0];
				omax = (parameters[1 - sign[0]][0] - ray[0].a) * inv_direction[0];

				for (i = 1; i < this->_numDimensions; i++) {
					tmin = (parameters[sign[i]][i] - ray[i].a) * inv_direction[i];
					tmax = (parameters[1 - sign[i]][i] - ray[i].a) * inv_direction[i];

					if ((omin > tmax) || (tmin > omax)) return null;

					if (tmin > omin) omin = tmin;

					if (tmax < omax) omax = tmax;
				}

				if (omin < 0 && omax < 0) return null;

				if (omin < 0) omin = 0;
				if (omax > 1) omax = 1;
                Tuple<double,double>[] new_rs= new Tuple<double,double>[] {};
				for (i = 0; i < this->_numDimensions; i++) {
					new_rs[i].Item1=ray[i].a + ray[i].b * omin;
					new_rs[i].Item2=ray[i].a + ray[i].b * omax;
		
				
                }
				return new_rs;
			}

			// returns the portion of a line segment that lies within this or false if
			// the line segment lies completely outside of this
            public Tuple<double, double>[] intersectWithSegment( rs ) {
				var i,
				    j,
				    parameters = [
				    	[],
				    	[]
				    ],
				    inv_direction = [],
				    sign = [],
				    omin,
				    omax,
				    tmin,
				    tmax,
				    new_rs = [];

				// Initialize values
				for (i = 0; i < this->_numDimensions; i++) {
					parameters[0][i] = this->min[i];
					parameters[1][i] = this->max[i];
					j = 1 / (rs[i].b - rs[i].a);
					inv_direction[i] = j;
					sign[i] = (j <= 0) ? 1 : 0;
				}


				omin = (parameters[sign[0]][0] - rs[0].a) * inv_direction[0];
				omax = (parameters[1 - sign[0]][0] - rs[0].a) * inv_direction[0];

				for (i = 1; i < this->_numDimensions; i++) {
					tmin = (parameters[sign[i]][i] - rs[i].a) * inv_direction[i];
					tmax = (parameters[1 - sign[i]][i] - rs[i].a) * inv_direction[i];

					if ((omin > tmax) || (tmin > omax)) return false;

					if (tmin > omin) omin = tmin;

					if (tmax < omax) omax = tmax;
				}

				if (omin >= Infinity || omax <= -Infinity) return false;

				if (omin < 0 && omax < 0) return false;

				if (omin < 0) omin = 0;
				if (omax > 1) omax = 1;

				for (i = 0; i < this->_numDimensions; i++) {
					new_rs[i] = {
						a : rs[i].a + (rs[i].b - rs[i].a) * omin,
						b : rs[i].a + (rs[i].b - rs[i].a) * omax
					};
				}

				return new_rs;
			}
		};

		return AABB;
}));
	}
}
