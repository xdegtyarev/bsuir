

function Task(){
	if (this instanceof Task) {
		this.name = "unnamed";
		this.description = "no desc";
		this.startDate = new Date();
		this.endDate = new Date();
		this.subtasks = [];

	}else{
		return new Task();
	}
}

function ActiveTask(progress){
	if (this instanceof ActiveTask) {
		this.progress = 0;
		this.isDone = false;
		ActiveTask.prototype = new Task();
		ActiveTask.prototype.constructor = ActiveTask;
		ActiveTask.prototype.GetProgress = function(){
			return progress;
		}
	}else{
		return new ActiveTask()
	}
}

function ipr2_4_2(){
	var t1 = new Task();
	var at1 = new ActiveTask();
	alert("t1:" + t1);
	alert("at1:" + at1);
}

