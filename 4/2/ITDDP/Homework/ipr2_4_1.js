

function Vector(x,y,z){
	if (this instanceof Vector) {
		this.x = x===undefined ? 0 : x;
		this.y = y===undefined ? 0 : y;
		this.z = z===undefined ? 0 : z;
	}else{
		return new Vector(0,0,0);
	}

	Vector.prototype.length = function(){
		return Math.sqrt(this.x*this.x+this.y*this.y+this.z*this.z);
	}

	Vector.prototype.add = function(vector){
		var sum = new Vector();
		sum.x = this.x+vector.x;
		sum.y = this.y+vector.y;
		sum.z = this.z+vector.z;
		return sum;
	};

	Vector.prototype.dot = function(vector){
		return this.x*vector.x+this.y*vector.y+this.z*vector.z;
	};

	Vector.prototype.toString = function(){
		return "(" + this.x + "," + this.y + "," + this.z + ")";
	};

	Vector.prototype.valueOf = function(){
		return [this.x,this.y,this.z];
	};
}

function ipr2_4_1(){
	var v1 = new Vector(3,4,5);
	var v2 = new Vector(-1,2);
	var v3 = new Vector(11,0.4,3);
	alert("v1:" + v1);
	alert("v2:" + v2);
	alert("v3:" + v3);
	alert("Length of v1 is: " + v1.length());
	alert("Sum of v1 and v2 is: " + v1.add(v2));
	alert("Dot prod of v2 and v3 is: " + v2.dot(v3));
	alert("Using valueof: " + v3.valueOf());
}

