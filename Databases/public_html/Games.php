<!DOCTYPE html>
<html>
<head>
<title> game platform </title>
<link href='https://fonts.googleapis.com/css?family=Comfortaa' rel='stylesheet'>

<style>

body {
font-family: 'Comfortaa';font-size: 22px;
background-color: #38095e
}

ul { 
list-style-type: none;
margin: 0;
padding: 0;
overflow: hidden;
background-color: #70089f;
position: sticky;
width:100%;
top:0;
}


li {
float:left;
border-right: 1px solid #000;
}

li:last-child {
  border-right: none;
}

li a{
display:block;
color: white;
text-align:center;
padding:14px 16px;
text-decoration: none;
}

.active{
background-color: #28c4fc
}

li a:hover:not(.active) {
color : white;
background-color:#1b75cb;
}


header h1{
height: 150px;
text-align : center;
font-size: 80px;
}

header h1:hover{
	text-shadow:1px 1px 2px #28c4fc, 0 0 25px #ff0028, 0 0 5px black;
}
	

.games_data{
color:white;
}




/* Create three equal columns that floats next to each other */
.column.title_photo {
  float: left;
  width: 45%;
  padding: 5px;
  position:relative;
}

.column.info{
	
	  float: left;
  width: 25%;
  padding: 5px;
  position:relative;
	
}

.column.price_downloads{
	  float: left;
  width: 25%;
  padding: 5px;
  position:relative;
	
}


/* Clear floats after the columns */
.row:after {
  content: "";
  display: table;
  clear: both;
}


.row{
	box-shadow: 0 4px 8px 0 rgba(0, 0, 0, 0.7), 0 6px 20px 0 rgba(0, 0, 0, 0.7);
	margin:18px;
	
}

.row:hover{
	box-shadow:0 4px 8px 0 rgb(40, 196, 252), 0 6px 20px 0 rgb(63, 255, 210);
	
}

.Games_separator{
width:70%;
margin:auto;
}

.title_photo_container{
	margin: auto;
	width: max-content;
}

.info_container{
	position:absolute;

}




</style>
</head>
<body>

<header> <h1>Game Platform</h1></header>

<nav> 
<ul>
<li > <a href='index.html' > Home </a> </li>
<li class='active'> <a href='#Gamespage' > Games </a> </li>
<li> <a href='Discussion1.php' > Discussion </a> </li>
<li> <a href='GAMEusers.php' > Users </a> </li>
<li style='float:right'> <a href='#moreinfo' > More info </a> </li>
</ul>
</nav>


<div class='games_container' style="padding-top: 28px;">
<div class='games_separator'>

<?php
    include('connect.php');

    //SQL query 
    $query = "Select Game_id,Title, Compatibility, Type, Price, Legal_name, count(Downloader) as sum from (GAME JOIN DEVELOPER ON Dev=Tax_number) left outer JOIN DOWNLOAD ON Game_id = Game_downloaded group by Game_id,Title, Compatibility, Type, Price, Legal_name";
     
    //execute query 
    $result = mysqli_query($con, $query); 

    if (!$result){
        echo("Error description: " . mysqli_error($con));
    }
	
    //query database 
    while($rows = mysqli_fetch_array($result)){
		$gameid = $rows['Game_id'];
        $gametitle = $rows['Title'];
        $comp = $rows['Compatibility'];
        $type = $rows['Type'];
        $price = $rows['Price'];
        $legalname = $rows['Legal_name'];
        $sum = $rows['sum'];
    //format results 
		
		
		//checking if price is NULL meaning 0
		if ($price==''){
			$price = 'FREE';
		}else{
			$price = $price.' $';
		}
		
		
		
	$query2 = "select photo from PHOTOS where Game_about = '$gameid'";
     
    //execute query 
    $result2 = mysqli_query($con, $query2); 
	
	    if (!$result2){
        echo("Error description: " . mysqli_error($con));
    }
	
	
    while($rows = mysqli_fetch_array($result2)){
		$photosrc = $rows['photo'];
    }  	
		
        echo ("<div class = 'row' >


	<div class = 'column title_photo'>
		<div class= 'title_photo_container games_data'>
			<div class = 'game_title' style='text-align:center;'>
					$gametitle	
			</div>
			<div class = 'game_photo'>
			<img src = 'photos/game_photos/$photosrc' alt = 'Game Image' style='width:inherit;' >
			</div>
		</div>
	</div>
	<div class = 'column info' >
		<div class = 'info_container games_data'>
			<div class = 'combatibility'>
			Combatibility: $comp
			</div>		
			<div class = 'type'>
			Type: $type
			</div>
			<div class = 'company'>
			Company: $legalname
			</div>
		</div>
	</div>
	<div class = 'column price_downloads '> 
		<div class = 'price_container games_data' >
			<div class = 'downloads'>
			Downloads : $sum
			</div>
			<div class = 'price'>
			Price : $price 
			</div>
		</div>
	</div>


</div>");      
	
    }  

    ?> 

</div>
</div>
</body>
</html>