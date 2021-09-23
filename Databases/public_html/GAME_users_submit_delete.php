<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>User delete</title>
<link href='https://fonts.googleapis.com/css?family=Comfortaa' rel='stylesheet'>

<style type="text/css">
body {
font-family: 'Comfortaa';font-size: 22px;
background-color: #38095e
}

.style1{
font-size:40px;
}


</style>
</head>
<body>
  <div align="center">
    <p><BR>
      <?php 

    include('connect.php');
	
	$user = $_POST['username'];
	
    $query = "DELETE FROM USER WHERE Username = '$user'";
	// echo $query;
	
    $queryexe = mysqli_query($con, $query);
    //execute query 
    if ($queryexe) { 
        echo("<p><font size=\"+1\">deletion succesful!</font></p>");
	} 
    else { 
        echo("<p><font size=\"+1\">deletion not succesful..!</font></p>");
		echo("Error description: " . mysqli_error($con));
	} 

?>
      </p>
    <a href="GAMEusers.php">back</a><br>
</div>
</body>
</html>