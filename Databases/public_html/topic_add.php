<?php include("connect.php"); ?>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Data Submit</title>
<link href='https://fonts.googleapis.com/css?family=Comfortaa' rel='stylesheet'>
<style type="text/css">

body {
font-family: 'Comfortaa';font-size: 22px;
background-color: #38095e
}

</style>
</head>
<body>
  <div align="center">
    <p><BR>
      <?php 
    
    $title = $_POST['game'];
    $user = $_POST['user'];
    $topic = $_POST['topic'];
  
  
	$query1 = "select Game_id from GAME where Title='$title'";
	
		//execute query 
	$result1 = mysqli_query($con, $query1); 

	if (!$result1){
		echo("Error description1: " . mysqli_error($con));
	}
	
	//query database 
	while($rows1 = mysqli_fetch_array($result1)){
	$gameid = $rows1['Game_id'];
	}
	
	$query = "INSERT INTO DISCUSSION (Disc_id, Topic, Publish_date, Disc_starter, Game_about) VALUES (NULL, '$topic', NULL, '$user', '$gameid')";
 
    $queryexe = mysqli_query($con, $query);
    
    if (!$queryexe){
        echo("Error description2: " . mysqli_error($con));
    }else{
        echo("Topic Inserted!");
    }

?>
      </p>
    <a href="Discussion1.php">Πίσω</a><BR>
</div>
</body>
</html>