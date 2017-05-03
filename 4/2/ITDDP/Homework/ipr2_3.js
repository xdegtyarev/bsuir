function ipr2_3_1(){
	alert("-3,5,1:   " + ipr2_3_range(-3,5,1));
	alert("-3,-10:   " + ipr2_3_range(-3,-10));
	alert("0,20,-2.5:   " + ipr2_3_range(0,20,-2.5));
}

function ipr2_3_range(bound1,bound2, step){
	if (step === undefined) {
		console.log("setting step to default val");
		step = 1;
	}
	var res = [];
	var val = 0;
	if(step>0){
		var lowerBound = Math.min(bound1,bound2);
		var upperBound = Math.max(bound1,bound2);
		val = lowerBound;
		while(val<upperBound){
			res.push(val);
			val = val + step;
		}
	}else{
		var lowerBound = Math.min(bound1,bound2);
		var upperBound = Math.max(bound1,bound2);
		val = upperBound;
		while(val>lowerBound){
			res.push(val);
			val = val + step;
		}
	}
	return res;
}

function ipr2_3_2(){
	array = [3,5,6,2,8,0,1,6,8]
	alert("array " + array + " sorted as: " + qs(array,(a,b)=>{return a-b;}));
}

function qs(array,comp){
	qsc(array,0,array.length-1,comp);
	return array;
}

function qsc(array, lo, hi, comp){
	if(lo<hi){
		console.log(array);
		p = partition(array,lo,hi,comp);
		qsc(array,lo,p-1,comp);
		qsc(array,p+1,hi,comp);
	}
}

function partition(array, lo, hi, comp){
	var pivot = array[hi];
	var i = lo-1;
	var temp = 0;
	for(j=lo;j<hi;j++){
		if(comp(array[j],pivot) <= 0){
			i++;
			temp = array[i];
			array[i] = array[j];
			array[j] = temp;
		}
	}
	temp = array[i+1];
	array[i+1]=array[hi]
	array[hi] = temp;
	return i+1;
}

function ipr2_3_3(){
	m1 = ipr2_3_3_createRandomMatrix(3,3);
	alert(m1.cells);
	m2 = ipr2_3_3_createRandomMatrix(3,3);
	alert(m2.cells);
	sumM = ipr2_3_3_sum(m1,m2);
	alert(sumM.cells);
}

function ipr2_3_3_randomInRange(min, max) {
  return Math.floor(Math.random() * (max - min)) + min;
}

function ipr2_3_3_createRandomMatrix(w,h){
	var matrix = {cells:new Array(w*h),width:w,height:h};
	for (var i = matrix.cells.length - 1; i >= 0; i--) {
		matrix.cells[i] = ipr2_3_3_randomInRange(0,100);
	}
	return matrix;
}

function ipr2_3_3_sum(a,b){
	if(a.height!=b.height||a.width!=b.width){
		alert("matrices are not compatible");
	}
	var matrix = {cells:new Array(a.cells.length),width:a.width,height:a.height};
	for (var i = matrix.cells.length - 1; i >= 0; i--) {
		matrix.cells[i] = a.cells[i]+b.cells[i];
	}
	return matrix;
}
