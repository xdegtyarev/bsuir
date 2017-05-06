//DOM demo
function ipr2_5_addButton(){
	var list = document.getElementById('list');
	var newItem = document.createElement('li');
	newItem.innerHTML = '<input type="button" style="background-color:' + getRandomColor() + ';" value="Random Button" onclick="ipr2_5_addButton()">';
	list.appendChild(newItem);
}

function ipr2_5_addField(){
	var list = document.getElementById('list');
	var newItem = document.createElement('li');
	newItem.innerHTML = '<input type="text" style="background-color:' + getRandomColor() + ';" value="Random Text">';
	list.appendChild(newItem);
}

function getRandomColor() {
    var letters = '0123456789ABCDEF';
    var color = '#';
    for (var i = 0; i < 6; i++ ) {
        color += letters[Math.floor(Math.random() * 16)];
    }
    return color;
}