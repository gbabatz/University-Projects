<!DOCTYPE html>
<html>
<head>
<title> Users </title>
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

li a:hover:not(.active) {
color : white;
background-color:#1b75cb;
}

.active{
background-color: #28c4fc
}

header h1{
display:box;
height: 150px;
text-align : center;
font-size: 80px;
}

header h1:hover{
	text-shadow:1px 1px 2px #28c4fc, 0 0 25px #ff0028, 0 0 5px black;
}


.userscontainer{
color:white;}



table{
	border-collapse: collapse;
	width: 100%;
}

.users_table, .users_header, .user_data {
	border: 1px solid black;
	border-top: none;
}


th {
	height: 50px;
}

th, td {
  padding: 15px;
  text-align: left;
}



input {
width : 60%;
float:right;
}

input:focus {
  border: 3px solid #555;
}

label{
float:left;
}


span{
display: block;
padding-left: 6px;
margin-left: 30px;
}

.row.radio{
border: 1px solid #000;
}


.dropbtn {
  background-color: #3498DB;
  color: white;
  padding: 16px;
  font-size: 16px;
  border: none;
  cursor: pointer;
}


.dropbtn:hover, .dropbtn:focus {
  background-color: #2980B9;
}


.dropdown {
  position: relative;
  display: inline-block;
}


.new_user_form {
  display: none;
  position: absolute;
  width: 40%;
  border-top: 1px solid black;
}

.show {display:block;}
</style>
<script type='text/JavaScript' src='files/scw.js'></script>
<script language="JavaScript" src="files/gen_validatorv31.js" type="text/javascript"></script>
</head>
<body>

<header> <h1>Game Platform</h1></header>

<nav> 
<ul>
<li> <a href='index.html' > Home </a> </li>
<li> <a href='Games.php' > Games </a> </li>
<li> <a href='Discussion1.php' > Discussion </a> </li>
<li class='active'> <a href='#Users' > Users </a> </li>
<li style='float:right'> <a href='#moreinfo' > More info </a> </li>
</ul>
</nav>

<div class = 'userscontainer'>

<table class="users_table" style="color:white;">
<thead>
 <tr>
  <th class="users_header">
   Username
  </th>
  <th class="users_header">
   Age
  </th>
  <th class="users_header">
   Sex
  </th>  
 </tr>
</thead>
<tbody>
<?php
    include('connect.php');

    //SQL query 
    $query = "Select Username,Age,Sex FROM USER"; 
     
    //execute query 
    $result = mysqli_query($con, $query); 

    if (!$result){
        echo("Error description: " . mysqli_error($con));
    }
    
    //query database 
    while($rows = mysqli_fetch_array($result)){
        $username = $rows['Username'];
        $age = $rows['Age'];
        $sex = $rows['Sex'];
    //format results 

        echo ("<tr><td class=\"user_data\"><div > $username </div></td> <td class=\"user_data\"><div > $age </div></td> <td class=\"user_data\"><div > $sex </div></td></tr>");      

    }  

    ?> 

</tbody>
	</table>
</div>



<div class='new_user'>
<button onclick="myFunction()" class="dropbtn">Add User</button>
<button class= "dropbtn" onclick="window.open('GAME_users_delete.php', '_blank');" >Delete User</button>
<br> <br>
<div id="myDropdown" class = "new_user_form">
<form method="POST" action="GAME_users_insert.php"  name="myform">
	<span class = "row" >
    <label for="username">Username</label>
    <input type="text" id="username" name="username" placeholder="Username" maxlength="25" required> </span><br>
	<span class = "row" >
	<label for="password">Password</label>
    <input type="password" id="password" name="password" placeholder="password" maxlength="25" required> </span><br>
	<span class = "row" >
	<label for="age">age</label>
    <input type="number" id="age" name="age" placeholder="age" min='1' max='99'> </span><br>
	<span class = "row radio" >
	<label for="sex">sex</label> <br><br>
	
	<input type="radio" name="sex" value="M"> Male<br>
	<input type="radio" name="sex" value="F"> Female<br>
	<input type="radio" name="sex" value="O"> Other </span>
	<span class = "row" >
	<label for="email">Email</label>
    <input type="text" id="email" name="email" placeholder="example@domain.com" maxlength="25" required> </span><br>
	<span class = "row" >
	<label for="card">Card Number</label>
    <input type="number" id="card" name="card" placeholder="card number" min="1000000000000000" max="9999999999999999"> </span><br>
	<span class = "row" >
	<label for="exp_date">Expiration date</label>
    <input type="date" id="date" name="exp_date"> </span><br>
	<span class = "row" >
	<label for="cvv">CVV</label>
	<input type="number" id="cvv" name="cvv" min="100" max="999"> </span><br>
	<span class = "row" >
	
        <table style="float:right;" width="200" border="0">
          <tr>
            <td width="100">
              <input type="submit" value="Submit" >
            </td>
            <td width="100">
              <input type="reset" value="Clear" >
            </td>
          </tr>
        </table>
</form>
</div>

</div>

<script>

function myFunction() {
  document.getElementById("myDropdown").classList.toggle("show");
}

</script>

<script language="JavaScript" type="text/javascript">
//You should create the validator only after the definition of the HTML form
  var frmvalidator  = new Validator("myform");
  frmvalidator.EnableMsgsTogether();
  frmvalidator.addValidation("username","req","Fill the field 'Username' !");
  frmvalidator.addValidation("password","req","Fill the field 'Password' !");
  frmvalidator.addValidation("email","req","Fill the field 'Email' !");
  frmvalidator.addValidation("age","num","'Age' is only numeric data !");
  
      </script>

</body>
</html>