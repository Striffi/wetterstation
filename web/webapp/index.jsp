<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>Weatherstation</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="">
    <meta name="author" content="">

    <!-- Le styles -->
    <link href="css/bootstrap.css" rel="stylesheet">
    <style>
      body {
        padding-top: 60px; /* 60px to make the container go all the way to the bottom of the topbar */
      }
    </style>
    <link href="css/bootstrap-responsive.css" rel="stylesheet">

    <!-- HTML5 shim, for IE6-8 support of HTML5 elements -->
    <!--[if lt IE 9]>
      <script src="js/html5shiv.js"></script>
    <![endif]-->
    
  </head>

  <body>

    <div class="navbar navbar-inverse navbar-fixed-top">
      <div class="navbar-inner">
        <div class="container">
          <button type="button" class="btn btn-navbar" data-toggle="collapse" data-target=".nav-collapse">
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
          </button>
          <a class="brand" href="#">Weatherstation</a>
        </div>
      </div>
    </div>

    <div class="container" style="float:right;">

		<div id="temperature" style="width: 500px; height: 250px;"></div>
		<div id="humidity" style="width: 500px; height: 250px;"></div>
		<div id="pressure" style="width: 500px; height: 250px;"></div>

    </div> <!-- /container -->

    <!-- Le javascript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->
    <script src="js/jquery-min.js"></script>
    <script type="text/javascript" src="https://www.google.com/jsapi"></script>
    <script src="js/bootstrap-min.js"></script>
    <script src="js/weatherstation.js"></script>
    <script type="text/javascript">
	    google.load("visualization", "1", {packages:["corechart"]});
	    window.weatherstation = new Weatherstation();
		google.setOnLoadCallback(weatherstation.drawChart);
		weatherstation.startRefresh();
	</script>
  </body>
</html>
