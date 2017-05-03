//number comparsion
function ipr2_1_1(){
	while(true){
		var n1 = window.prompt("Number 1");
		if(!n1){break;}
		if(Number.isNaN(+n1)){
			alert("N1 is NaN");
			break;
		}
		var n2 = window.prompt("Number 2");
		if(!n2){break;}
		if(Number.isNaN(+n2)){
			alert("N2 is NaN");
			break;
		}

		if(n1>n2){
			alert("n1 is greater than n2");
		}else if(n1<n2){
			alert("n1 is less than n2");
		}else{
			alert("they are equal");
		}
	}
}

function ipr2_1_2_input_validation(numberToValidate){
	return Number.isInteger(numberToValidate) && numberToValidate>0;
}

// flat block/floor counter
function ipr2_1_2(){
	while(true){
		var floorCount = window.prompt("Floor count");
		if(!floorCount){break;}
		floorCount = Number.parseInt(floorCount);
		var blockCount = window.prompt("Block count");
		if(!blockCount){break;}
		blockCount = Number.parseInt(blockCount);
		var flatCount = window.prompt("Flat count");
		if(!flatCount){break;}
		flatCount = Number.parseInt(flatCount);
		var flatNumber = window.prompt("Flat Number");
		if(!flatNumber){break;}
		flatNumber = Number.parseInt(flatNumber);
		if(ipr2_1_2_input_validation(floorCount) && ipr2_1_2_input_validation(blockCount) && ipr2_1_2_input_validation(flatCount) && ipr2_1_2_input_validation(flatNumber) && flatNumber<=(floorCount*blockCount*flatCount)){
			var block = Number.parseInt(flatNumber/(flatCount*floorCount));
			var floor = flatNumber%(flatCount*floorCount);
			alert("Block: " + block + " Floor: " + floor);
		}else{
			alert("Wrong input");
			throw "Wrong input";
			break;
		}
	}
}


// fibonacci
function ipr2_1_3(){
	while(true){
		var n = window.prompt("n: ");
		if(!n){break;}
		n = Number.parseInt(n);
		var prev = 0;
		var cur = 1;
		var temp = 0;
		if(Number.isInteger(n) && n>0){
			while(n>0){
				temp = cur;
				cur = cur+prev;
				prev = temp;
				n--;
			}
			alert(n + "'s number is: " + cur );
		}else{
			alert("Wrong input");
			throw "Wrong input";
			break;
		}
	}
}

// day finder
function ipr2_1_4(){
	while(true){
		var month = window.prompt("Month(1-12)");
		if(!month){break;}
		month = Number.parseInt(month);
		if(!(Number.isInteger(month) && month > 0 && month < 12)){
			alert("Wrong input");
			throw "Wrong input";
		}
		var day = window.prompt("Day(1-31)")
		if(!day){break;}
			day = Number.parseInt(day);
		if(!(Number.isInteger(day) && day > 0 && day < 31)){
			alert("Wrong input");
			throw "Wrong input";
		}
		var year = 2017;
		var d = new Date();
		var days = ["Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"];
		d.setFullYear(2017,month-1,day-1);
		alert(days[d.getDay()]);
	}
}

