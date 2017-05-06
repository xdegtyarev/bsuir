function ipr2_6_sendRequest(){
    $.ajax({
    	type: 'GET',
    	url: 'http://api.openweathermap.org/data/2.5/weather',
    	data: {
    		'q':'Minsk,BY',
    		'APPID':'3864d1826ae74be7350e786af03a94f1'}
    })
	.done(function(result) {
	    console.log( "success" );
	    console.log(result);
      var generalData = $("<p><b>" + result.name + "</b>: <i>" + result.weather[0].main + "</i> :" + result.weather[0].description + "</p>");
      var visibilityData = $("<p>Visibility: "+result.visibility  + "m </p>");
      var windData = $("<p>Wind: " + result.wind.speed + "m/s in a direction: " + result.wind.deg + " degrees </p>");
      var tempData = $("<p>Temperature: " + result.main.temp + " F</p>");
      var moreData = $("<p>Humidity: " + result.main.humidity + "<br/> Pressure: " + result.main.pressure + "<br/> </p>");
      $("#list").append(generalData).append(visibilityData).append(windData).append(moreData);
	  })
  	.fail(function() {
    	console.log( "error " + result);
  	})
  	.always(function() {
	    console.log( "complete" );
  	});
}
