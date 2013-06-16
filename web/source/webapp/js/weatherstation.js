Weatherstation = function() {
	this.drawChart = function() {
		$.get('weatherstation?type=humidity', function(msg) {
			draw(build(msg), 'humidity');
		});
		$.get('weatherstation?type=temperature', function(msg) {
			draw(build(msg), 'temperature');
		});
		$.get('weatherstation?type=pressure', function(msg) {
			draw(build(msg), 'pressure');
		});
	 }
	 
	 this.startRefresh = function(){
	 	refresh();
	 }
	 
	 function refresh() {
		setTimeout(refresh,1000);
        weatherstation.drawChart();
	 }
	 
	 function draw(array, divId) {
	 	data = google.visualization.arrayToDataTable(array);
	 	var options = {
	      	  title: divId
	    };
	    
	    var chart = new google.visualization.LineChart(document.getElementById(divId));
	    chart.draw(data, options);
	 }
	 
	 function build(msg) {
		var row = msg.split(";");
		var value = [];
		column = row[0].split(",");
		value[0] = [];
		value[0][0] = column[0];
		value[0][1] = column[1];
		for (var i=1;i<row.length;i++) {
			column = row[i].split(",");
			value[i] = [];
			value[i][0] = column[0];
			value[i][1] = parseFloat(column[1]);
		}
		return value;
	}
 }
