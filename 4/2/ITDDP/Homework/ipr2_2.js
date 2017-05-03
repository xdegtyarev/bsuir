//ipr2_2

//2.2.1

function ipr2_2_1(){
	var rect = new Array(4);
	for (var i = 0; i < rect.length; i++) {
		rect[i] = {x:0, y:0};
		x = window.prompt("Input point " + i + "X:");
		if(x){
			rect[i].x=Number.parseFloat(x);
		}else{
			return;
		}
		y = window.prompt("Input point " + i + "Y:");
		if(y){
			rect[i].y=Number.parseFloat(y);
		}else{
			return;
		}
	}
	var isRect = ipr2_2_1_IsRect(rect);
	if(isRect){
		alert("Is Rect");
		var point = {x:0,y:0};
		x = window.prompt("Input point " + i + "X:");
		if(x){
			point.x=Number.parseFloat(x);
		}else{
			return;
		}
		y = window.prompt("Input point " + i + "Y:");
		if(y){
			point.y=Number.parseFloat(y);
		}else{
			return;
		}
		alert("Point is " + (ipr2_2_1_IsInBounds(point,rect) ? "in Rect":"out of Rect"));
	}else{
		alert("Not Rect");
	}
}

function ipr2_2_1_IsRect(rect){
	for(var i=1; i<=rect.length; i++){
		if(!((rect[i-1].x == rect[i%rect.length].x || rect[i%rect.length].x == rect[(i+1)%rect.length].x) &&
		(rect[i-1].y == rect[i%rect.length].y || rect[i%rect.length].y == rect[(i+1)%rect.length].y))){
			return false;
		}
	}
	return true;
}

function lowerLeft(rect){
	llIndex = 0;
	for (var i = 0; i < rect.length; i++) {
		if(rect[llIndex].x > rect[i].x || rect[llIndex].y > rect[i].y){
			llIndex = i;
		}
	}
	console.log("ll: " + llIndex);
	return rect[llIndex];
}

function upperRight(rect){
	urIndex = 0;
	for (var i = 0; i < rect.length; i++) {
		if(rect[urIndex].x < rect[i].x || rect[urIndex].y < rect[i].y){
			urIndex = i;
		}
	}
	console.log("ur: " + urIndex);
	return rect[urIndex];
}

function ipr2_2_1_IsInBounds(point, rect){
	var ll = lowerLeft(rect);
	var ur = upperRight(rect);
	return ll.x < point.x && point.x < ur.x && ll.y < point.y && point.y < ur.y;
}


//2.2.2

//2.2.2.1
function ipr2_2_2_1_numcheck_args(f){
	return function(){
		for (var i = arguments.length - 1; i >= 0; i--) {
			if(typeof arguments[i] != 'number'){
				alert("Not a Number!");
				return;
			}
		}
		return f.apply(this, arguments);
	}
}

function ipr2_2_2_1_func(param){
	return param*param;
}

function ipr2_2_2_1(){
	//decorating
	ipr2_2_2_1_func = ipr2_2_2_1_numcheck_args(ipr2_2_2_1_func);

	n = window.prompt("Input number to test");
	if(n){
		alert(ipr2_2_2_1_func(Number.parseFloat(n)));
	}
}

//2.2.2.2
function ipr2_2_2_2_numcheck_args(f){
	return function(){
		console.log("decorator:" + arguments.length);
		for (var i = arguments.length - 1; i >= 0; i--) {
			if(typeof arguments[i] != 'number'){
				alert("Not a Number!");
				return;
			}
		}
		return f.apply(this, arguments);
	}
}

function ipr2_2_2_2_sum(){
	console.log(arguments.length);
	var sum = 0;
	for (var i = arguments.length - 1; i >= 0; i--) {
		sum = sum + arguments[i];
	}
	return sum;
}

function ipr2_2_2_2(){
	ipr2_2_2_2_sum = ipr2_2_2_2_numcheck_args(ipr2_2_2_2_sum);
	alert("1,3,12,125,1,5,1 will work: " + ipr2_2_2_2_sum(1,3,12,125,1,5,1));
	alert("a,3,c,125,1,5,1 will fail: " + ipr2_2_2_2_sum('a',3,'c',125,1,5,1));
}
