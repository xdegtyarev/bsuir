function Task(key){
		this.name = "unnamed";
		this.description = "no desc";
		this.startDate = new Date();
		this.endDate = new Date();
		this.subtasks = [];
		console.log("Task .ctor()" + key);
}

Task.prototype.toString = function(){
	return this.name + " " + this.description + " started: " + this.startDate + " ends: " + this.endDate + " has " + this.subtasks.length + " subtasks";
};


function ActiveTask(key){
		Task.apply(this, arguments);
		console.log("ATask .ctor()" + key);
		this.progress = 0;
		this.isDone = false;
}

ActiveTask.prototype = new Task();
ActiveTask.prototype.constructor = ActiveTask;

ActiveTask.prototype.getProgress = function(){
	return this.progress;
}

ActiveTask.prototype.toString = function(){
	var s = Task.prototype.toString.call(this);
	return s + " progress: " + this.progress;
}

function ipr2_4_2(){
	var t1 = new Task("t1");
	var at1 = new ActiveTask("at1");
	console.log("t1:" + t1 + "is Task:" + (t1 instanceof Task) + "is ActiveTask:" + (t1 instanceof ActiveTask));
	console.log("at1:" + at1 + "is Task:" + (at1 instanceof Task) + "is ActiveTask:" + (t1 instanceof ActiveTask));
}

