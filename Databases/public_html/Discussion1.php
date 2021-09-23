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



.container{
color:white;
}

/* Create three equal columns that floats next to each other */
.column.user1col{
	float: left;
	width: 25%;
	padding: 5px;
	position:relative;
}
.column.topiccol{
	float: left;
	width: 47%;
	padding: 5px;
	position:relative;
}
.column.datecol{
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


.row.game_row{
	box-shadow: 0 4px 8px 0 rgba(0, 0, 0, 0.7), 0 6px 20px 0 rgba(0, 0, 0, 0.7);
	margin:18px;
	height:50px;
	
}

.topiccontainer{margin:0 35px 0 35px;}

<!-- .row:hover{ -->
	<!-- box-shadow:0 4px 8px 0 rgb(40, 196, 252), 0 6px 20px 0 rgb(63, 255, 210); -->
	
<!-- } -->

.topic_row{
border-bottom:1px solid black;
height:41px;
}


.gamecol{
	float: left;
	width: 25%;
	padding: 5px;
	position:relative;
}
.topicpick{
	float: left;
	width: 47%;
	padding: 5px;
	position:relative;
}
.usercol1{
	float: left;
	width: 15%;
	padding: 5px;
	position:relative;
}
.submit{
	float: left;
	width: 10%;
	padding: 5px;
	position:relative;
}


input.topicadd{width:100%}



</style>
</head>
<body>

<header> <h1>Game Platform</h1></header>

<nav> 
<ul>
<li > <a href='index.html' > Home </a> </li>
<li > <a href='Games.php' > Games </a> </li>
<li class='active'> <a href='#Discussionpage' > Discussion </a> </li>
<li> <a href='GAMEusers.php' > Users </a> </li>
<li style='float:right'> <a href='#moreinfo' > More info </a> </li>
</ul>
</nav>


<div class='container' style="padding-top: 28px;">
<div class='games_separator'>

<?php
    include('connect.php');

    //SQL query 
    $query = "select Game_id, Title from GAME";
     
    //execute query 
    $result = mysqli_query($con, $query); 

    if (!$result){
        echo("Error description: " . mysqli_error($con));
    }
	
    //query database 
    while($rows = mysqli_fetch_array($result)){
		
		
		$gameid = $rows['Game_id'];
        $title = $rows['Title'];	


		$query2 = "select Topic,Publish_date,Disc_starter from DISCUSSION where Game_about='$gameid'"; 
		$result2 = mysqli_query($con, $query2); 

		if (!$result2){
        echo("Error description: " . mysqli_error($con));
		}
		
		if (mysqli_num_rows($result2) == 0)
		{
			continue;
		} 
		
		echo("<div class = 'row game_row' >
			<div class = 'game_about'>
				<h3 style='padding:13px;'>$title</h3>
			</div>
		</div>
		<div class = 'topiccontainer'>");
	
		while($rows2 = mysqli_fetch_array($result2)){
			$topic = $rows2['Topic'];
			$date = $rows2['Publish_date'];
			$user1 = $rows2['Disc_starter'];

			echo ("<div class = 'row topic_row' >
					<div class = 'column user1col'>
						<div class= 'user1'>
							$user1
						</div>
					</div>
					<div class = 'column topiccol' >
						<div class = 'topic'>
							$topic
						</div>
					</div>
					<div class = 'column datecol'> 
						<div class = 'date' >
							$date
						</div>
					</div>
				</div>");      
		}
		
			echo("</div>");
    }  
	
				echo("<div class = 'topiccontainer'>
				<form method='POST' action='topic_add.php'  name='myform'>
				<div class = 'row add_data' >
					<div class = 'column gamecol'>
						<div class= 'gamepick'>
							<select class='select' name='game'>
							<option value=''>Game about:</option>");
							
							
							
							$query3 = "SELECT Title from GAME"; 
     
							//execute query 
							$result3 = mysqli_query($con, $query3); 

							if (!$result3){
								echo("Error description: " . mysqli_error($con));
							}
							
							//query database 
							while($rows3 = mysqli_fetch_array($result3)){
								$title3 = $rows3['Title'];

							//format results
								echo ("<option value=\"$title3\">$title3</option>");   
							
							}  
							
							
							echo("</select>
						</div>
					</div>
					<div class = 'column topicpick' >
						<div >
							<input class = 'topicadd' type='text' name='topic' placeholder='Topic'> 
						</div>
					</div>
					<div class = 'column usercol1'> 
						<div class = 'userpick' >
							<select class='select' name='user'>
							<option value=''>User:</option>");
							
														

														
							$query4 = "SELECT Username from USER"; 
     
							//execute query 
							$result4 = mysqli_query($con, $query4); 

							if (!$result4){
								echo("Error description: " . mysqli_error($con));
							}
							
							//query database 
							while($rows4 = mysqli_fetch_array($result4)){
								$username4 = $rows4['Username'];

							//format results
								echo ("<option value=\"$username4\">$username4</option>");   
							
							}  
							
							
							
							
							
							
							echo("</select>
						</div>
					</div>
				
					<div class = 'column submit'> 
						<input type='submit' value='add new' />
					</div>
				</div>
			</form></div>");
	
	

    ?> 

</div>
</div>
</body>
</html>